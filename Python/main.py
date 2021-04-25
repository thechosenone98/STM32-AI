from PyQt5 import QtCore, QtWidgets, QtGui, QtWebEngineWidgets
from PyQt5.QtCore import pyqtSignal, pyqtSlot, Qt, QThread, QTimer, QCoreApplication, QModelIndex
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QApplication, QWidget, QFileDialog, QAction, QMessageBox, QComboBox, QLabel
import ApplicationWindow

import matplotlib

matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

import numpy as np

import serial.tools.list_ports
from serial import Serial, serialutil
from time import sleep

import time
from queue import Queue
from pathlib import Path
import struct

from collections import deque


def sizeof_fmt(num, suffix='B'):
    for unit in ['', 'Ki', 'Mi', 'Gi', 'Ti', 'Pi', 'Ei', 'Zi']:
        if abs(num) < 1024.0:
            return "%3.1f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f%s%s" % (num, 'Yi', suffix)


def current_milli_time():
    return round(time.time() * 1000)


def arrow3d(ax, length=1, width=0.05, head=0.2, headwidth=2.0,
            theta_x=0, theta_z=0, offset=(0, 0, 0), **kw):
    w = width
    h = head
    hw = headwidth
    theta_x = np.deg2rad(theta_x)
    theta_z = np.deg2rad(theta_z)

    a = [[0, 0], [w, 0], [w, (1 - h) * length], [hw * w, (1 - h) * length], [0, length]]
    a = np.array(a)

    r, theta = np.meshgrid(a[:, 0], np.linspace(0, 2 * np.pi, 30))
    z = np.tile(a[:, 1], r.shape[0]).reshape(r.shape)
    x = r * np.sin(theta)
    y = r * np.cos(theta)

    rot_x = np.array([[1, 0, 0], [0, np.cos(theta_x), -np.sin(theta_x)],
                      [0, np.sin(theta_x), np.cos(theta_x)]])
    rot_z = np.array([[np.cos(theta_z), -np.sin(theta_z), 0],
                      [np.sin(theta_z), np.cos(theta_z), 0], [0, 0, 1]])

    b1 = np.dot(rot_x, np.c_[x.flatten(), y.flatten(), z.flatten()].T)
    b2 = np.dot(rot_z, b1)
    b2 = b2.T + np.array(offset)
    x = b2[:, 0].reshape(r.shape)
    y = b2[:, 1].reshape(r.shape)
    z = b2[:, 2].reshape(r.shape)
    ax.plot_surface(x, y, z, **kw)


def getData(data):
    x = int.from_bytes(bytes([data[0], data[1]]), byteorder='big', signed=True) * 3.9 / 1000.0
    y = int.from_bytes(bytes([data[2], data[3]]), byteorder='big', signed=True) * 3.9 / 1000.0
    z = int.from_bytes(bytes([data[4], data[5]]), byteorder='big', signed=True) * 3.9 / 1000.0
    t = int.from_bytes(bytes(data[6:-2]), byteorder='big', signed=False)
    return x, y, z, t


class ThreadedUART(QThread):
    received_data_signal = pyqtSignal(bytes)

    def __init__(self, sp: Serial):
        super().__init__()
        self.__run_flag = True
        self.__sp = sp

    def run(self):
        data = bytearray()
        while self.__run_flag:
            while self.__sp.inWaiting():
                # Verify we got all the data (the end character may be contained within the data and trigger the read_until
                # before it is actually done reading
                while len(data) < 16:
                    data.extend([int(b) for b in self.__sp.read_until(expected=serialutil.to_bytes([0xAA, 0x55]))])
                self.received_data_signal.emit(bytes(data))
                # Clear the data
                data = bytearray()
            self.msleep(10)

    def stop(self):
        self.__run_flag = False


class ThreadedFileWrite(QThread):
    remaining_writes_signal = pyqtSignal(int)

    def __init__(self, path=None):
        super().__init__()
        self.__run_flag = True
        self.__file_obj = open(path, "wb")
        self.__nb_writes = 0
        self.queue = Queue()

    def run(self):
        while self.__run_flag:
            while not self.queue.empty():
                data = self.queue.get()
                # Get acceleration and timestamp
                x, y, z, t = getData(data)
                self.__file_obj.write(struct.pack("dddQ", x, y, z, t))
                self.queue.task_done()
                self.__nb_writes += 1
                self.remaining_writes_signal.emit(self.__nb_writes)
            self.msleep(50)

    def setPath(self, path):
        """Automatically closes previous file and switches to the new file path"""
        self.queue.join()
        self.__file_obj.close()
        self.__nb_writes = 0
        self.remaining_writes_signal.emit(self.__nb_writes)
        self.__file_obj = open(path, "wb")

    def finishWriting(self):
        self.queue.join()

    def stop(self):
        self.finishWriting()
        self.__file_obj.close()
        self.__run_flag = False


class MplCanvas(FigureCanvas):

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        # Used to hold data for all axes
        self.data_x = deque([0.0 for i in range(5000)], maxlen=5000)
        self.data_y = deque([0.0 for i in range(5000)], maxlen=5000)
        self.data_z = deque([0.0 for i in range(5000)], maxlen=5000)
        self.timestamps = deque([i for i in range(5000)], maxlen=5000)
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = self.fig.add_subplot(1, 1, 1)
        self.axes.set_ylim(-16, 16)
        self.__plot_ref_x = self.axes.plot(self.timestamps, self.data_x)[0]
        self.__plot_ref_y = self.axes.plot(self.timestamps, self.data_y)[0]
        self.__plot_ref_z = self.axes.plot(self.timestamps, self.data_z)[0]
        self.fig.legend(["Acc. X", "Acc. Y", "Acc. Z"])
        super(MplCanvas, self).__init__(self.fig)

    def updateData(self, data):
        x, y, z, t = getData(data)
        self.data_x.appendleft(x)
        self.data_y.appendleft(y)
        self.data_z.appendleft(z)

    def updateCanvas(self):
        # X Axis
        self.__plot_ref_x.set_xdata(self.timestamps)
        self.__plot_ref_x.set_ydata(self.data_x)
        # Y axis
        self.__plot_ref_y.set_xdata(self.timestamps)
        self.__plot_ref_y.set_ydata(self.data_y)
        # Z axis
        self.__plot_ref_z.set_xdata(self.timestamps)
        self.__plot_ref_z.set_ydata(self.data_z)
        # Draw all 3 new plots
        self.draw()


class FormMain(ApplicationWindow.Ui_MainWindow, QtWidgets.QMainWindow):
    def __init__(self, path=None, load="normal"):
        super(FormMain, self).__init__()
        self.setupUi(self)

        self.sp = None
        self.reading = False
        self.UART_thread = None
        self.writer_thread = None
        # Acceleration received from the STM32
        self.acceleration = {"x": 0.0, "y": 0.0, "z": 1.0}
        # Timestamp received from the STM32
        self.timestamp = 0
        # Folder o save our data to
        self.saveFolder: Path = Path()

        self.canvas = MplCanvas(parent=self.gb_graphViewer, width=5, height=4, dpi=100)
        self.verticalLayout_3.addWidget(self.canvas)
        self._plot_ref = None
        # Timer that controls the refresh rate of the 3D plot
        self.canvas_timer = QTimer()
        self.canvas_timer.setInterval(100)
        self.canvas_timer.timeout.connect(self.canvas.updateCanvas)
        self.canvas_timer.start()
        # Timer that controls the refresh rate of the terminal
        self.terminalTimer = QTimer()
        self.terminalTimer.setInterval(100)
        self.terminalTimer.timeout.connect(self.updateTerminal)

        # arrow3d(self.plot, length=np.sqrt(self.acceleration["x"]**2 +
        #                                   self.acceleration["y"]**2 +
        #                                   self.acceleration["z"]**2),
        #         theta_x=np.arctan(np.sqrt(self.acceleration["x"]**2 + self.acceleration["y"]**2) / self.acceleration["z"]),
        #         theta_z=np.arctan(self.acceleration["y"] / self.acceleration["x"]),
        #         width=0.02, head=0.1, headwidth=1.5, offset=[1, 1, 0], color="limegreen")

        self.command_dict = {"start_reading": bytearray([0x02, 0x00, 0x00, 0x00, 0x00, 0xAB, 0xCD, 0xDC, 0xBA, 0x04]),
                             "stop_reading": bytearray([0x02, 0x00, 0x00, 0x00, 0x00, 0xDC, 0xBA, 0xAB, 0xCD, 0x04]),
                             "handshake": bytearray([0x02, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x55, 0xAA, 0x55, 0x04]),
                             "settime": bytearray([0x02, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xEE, 0x11, 0x04])}

        # Scan available COM Ports
        for port in serial.tools.list_ports.comports():
            self.cmb_comPort.addItem(port.name)

        # Connect controls to their respective functions
        self.cmb_comPort.currentIndexChanged.connect(self.changePort)
        self.btn_StartStop.clicked.connect(self.toggleRead)
        self.btn_browseFolder.clicked.connect(self.setSaveFolder)
        self.btn_togglePort.clicked.connect(self.togglePort)
        self.btn_setName.clicked.connect(self.setFileName)

    def togglePort(self):
        if self.sp is None:
            self.sp = Serial(port=self.cmb_comPort.currentText(), baudrate=1500000)
            self.sp.write(self.command_dict["handshake"])
            self.UART_thread = ThreadedUART(self.sp)
            if self.txb_filename.text() != "":
                self.writer_thread = ThreadedFileWrite(self.saveFolder.joinpath(self.txb_filename.text()))
                self.writer_thread.remaining_writes_signal.connect(self.updateFileSizeLabel)
                self.UART_thread.received_data_signal.connect(self.writer_thread.queue.put)
            else:
                QMessageBox.about(self, "Error",
                                  "Please specify a name for the file and a folder in which you want to save the file.")
                self.sp.close()
                self.sp = None
                return
            self.UART_thread.received_data_signal.connect(self.updateAccelerations)
            self.UART_thread.received_data_signal.connect(self.canvas.updateData)
            self.UART_thread.start()
            self.writer_thread.start()
            self.btn_togglePort.setText("Close")
        elif self.sp.is_open:
            # Make sure we stop reading data in
            self.sp.write(self.command_dict["stop_reading"])
            self.reading = False
            self.UART_thread.stop()
            self.writer_thread.stop()
            self.terminalTimer.stop()
            self.UART_thread.wait()
            self.writer_thread.wait()
            self.sp.close()
            self.sp = None
            self.btn_togglePort.setText("Open")
            self.btn_StartStop.setText("Start")

    def setSaveFolder(self):
        self.saveFolder = Path(str(QFileDialog.getExistingDirectory(self, "Select Directory")))
        self.txb_saveFolder.setText(str(self.saveFolder))

    def setFileName(self):
        if self.writer_thread is not None:
            self.writer_thread.setPath(self.saveFolder.joinpath(self.txb_filename.text()))

    def changePort(self):
        if self.sp is not None:
            self.sp.close()
            self.UART_thread.stop()
            self.writer_thread.stop()
            self.terminalTimer.stop()
            self.UART_thread.wait()
            self.writer_thread.wait()
        self.btn_togglePort.setText("Open")

    def createAndStartThread(self):
        self.UART_thread = ThreadedUART(self.sp)
        if self.txb_filename.text() != "":
            self.writer_thread = ThreadedFileWrite(self.saveFolder.joinpath(self.txb_filename.text()))
            self.UART_thread.received_data_signal.connect(self.writer_thread.queue.put)
        else:
            QMessageBox.about(self, "Error",
                              "Please specify a name for the file and a folder in which to save the file.")
            return
        self.UART_thread.received_data_signal.connect(self.updateAccelerations)
        self.UART_thread.start()

    def toggleRead(self):
        if self.reading:
            self.reading = False
            self.btn_StartStop.setText("Start")
            self.sp.write(self.command_dict["stop_reading"])
            # Stop updating the terminal
            self.terminalTimer.stop()
        elif self.sp is not None:
            self.reading = True
            self.btn_StartStop.setText("Stop")
            # Send the command to set the UTC time on the STM32
            self.sp.write(self.command_dict["settime"])
            # Create timestamp command (START + TIME + END)
            timestamp = ((0x02 << 72) + (current_milli_time() << 8) + 0x04).to_bytes(10, 'big', signed=False)
            self.sp.write(timestamp)
            self.sp.write(self.command_dict["start_reading"])
            # Start updating the terminal
            self.terminalTimer.start()
        else:
            QMessageBox.about(self, "Error", "Please open the port first")

    def updateAccelerations(self, data):
        directions = ["x", "y", "z"]
        x, y, z, t = getData(data)
        self.acceleration["x"] = x
        self.acceleration["y"] = y
        self.acceleration["z"] = z
        self.timestamp = t

    def updateFileSizeLabel(self, num):
        self.lbl_nbWritesLeft.setText(f"Total file size : {sizeof_fmt(struct.calcsize('dddQ') * num)}")

    def updateTerminal(self):
        self.lsw_Terminal.addItem(f"T:{self.timestamp} "
                                  f"X:{self.acceleration['x']:.2f} "
                                  f"Y:{self.acceleration['y']:.2f} "
                                  f"Z:{self.acceleration['z']:.2f}")
        if self.lsw_Terminal.count() >= 50:
            self.lsw_Terminal.takeItem(0)
        self.lsw_Terminal.scrollToBottom()

    def updateGraph(self):
        if self._plot_ref is None:
            # First time we have no plot reference, so do a normal plot.
            # .plot returns a list of line <reference>s, as we're
            # only getting one we can take the first element.
            plot_refs = self.canvas.axes.plot3D([0, self.acceleration["x"]], [0, self.acceleration["y"]],
                                                [0, self.acceleration["z"]], 'red')
            self._plot_ref = plot_refs[0]
        else:
            # We have a reference, we can use it to update the data for that line.
            self._plot_ref.set_xdata(np.asarray([0, self.acceleration["x"]]))
            self._plot_ref.set_ydata(np.asarray([0, self.acceleration["y"]]))
            self._plot_ref.set_3d_properties(np.asarray([0, self.acceleration["z"]]))

        # Trigger the canvas to update and redraw.
        self.canvas.draw()


if __name__ == '__main__':
    app = QApplication([])
    window = FormMain()
    window.show()
    app.exec()
