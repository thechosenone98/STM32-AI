from PyQt5 import QtCore, QtWidgets, QtGui
from PyQt5.QtCore import pyqtSignal, pyqtSlot, Qt, QThread, QTimer, QCoreApplication, QModelIndex
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QApplication, QWidget, QFileDialog, QAction, QMessageBox, QComboBox, QLabel
import ApplicationWindow

import serial.tools.list_ports
from serial import Serial, serialutil
from time import sleep


def toggle(btn, *args):
    if btn.text() == args[0]:
        btn.setText(args[1])
    else:
        btn.setText(args[0])


class ThreadedUART(QThread):
    received_data_signal = pyqtSignal(bytes)

    def __init__(self, sp: Serial):
        super().__init__()
        self.run_flag = True
        self.sp = sp

    def run(self):
        data = bytearray()
        while self.run_flag:
            while not self.sp.inWaiting():
                self.msleep(50)
            # Verify we got all the data (the end character may be contained within the data and trigger the read_until
            # before it is actually done reading
            while len(data) < 8:
                data.extend([int(b) for b in self.sp.read_until(expected=serialutil.to_bytes([0xAA, 0x55]))])
            self.received_data_signal.emit(bytes(data))
            # Clear the data
            data = bytearray()


class FormMain(ApplicationWindow.Ui_MainWindow, QtWidgets.QMainWindow):
    def __init__(self, path=None, load="normal"):
        super(FormMain, self).__init__()
        self.setupUi(self)

        self.command_dict = {"start_reading": bytearray([0x02, 0xAB, 0xCD, 0xDC, 0xBA, 0x04]),
                             "stop_reading": bytearray([0x02, 0xDC, 0xBA, 0xAB, 0xCD, 0x04]),
                             "handshake": bytearray([0x02, 0xAA, 0x55, 0xAA, 0x55, 0x04])}

        self.sp = None
        self.reading = False
        self.UART_thread = None

        # Scan available COM Ports
        for port in serial.tools.list_ports.comports():
            self.cmb_comPort.addItem(port.name)

        # Connect controls to their respective functions
        self.cmb_comPort.currentIndexChanged.connect(self.changePort)
        self.btn_StartStop.clicked.connect(self.toggleRead)

        # Try to create the UART thread (will fail if no port are available)
        try:
            self.sp = Serial(self.cmb_comPort.currentText(), 115200)
            self.sp.write(self.command_dict["handshake"])
            self.createAndStartThread()
        except Exception:
            pass

    def changePort(self):
        if self.sp is not None:
            self.sp.close()
            self.UART_thread.run_flag = False
            # TODO : find a way to wait for the thread to join (it's not called join for QThread)
        self.sp = Serial(port=self.cmb_comPort.currentText(), baudrate=115200)
        self.sp.write(self.command_dict["handshake"])
        self.createAndStartThread()

    def createAndStartThread(self):
        self.UART_thread = ThreadedUART(self.sp)
        self.UART_thread.received_data_signal.connect(self.updateTerminal)
        self.UART_thread.start()

    def toggleRead(self):
        if self.reading:
            self.reading = False
            self.btn_StartStop.setText("Start")
            self.sp.write(self.command_dict["stop_reading"])
        else:
            self.reading = True
            self.btn_StartStop.setText("Stop")
            self.sp.write(self.command_dict["start_reading"])

    def updateTerminal(self, data):
        x = int.from_bytes(bytes([data[0], data[1]]), byteorder='big', signed=True) * 3.9 / 1000.0
        y = int.from_bytes(bytes([data[2], data[3]]), byteorder='big', signed=True) * 3.9 / 1000.0
        z = int.from_bytes(bytes([data[4], data[5]]), byteorder='big', signed=True) * 3.9 / 1000.0
        self.lsw_Terminal.addItem(f"X:{x:.2f} Y:{y:.2f} Z:{z:.2f}")
        if self.lsw_Terminal.count() >= 50:
            self.lsw_Terminal.takeItem(0)
        self.lsw_Terminal.scrollToBottom()


if __name__ == '__main__':
    app = QApplication([])
    window = FormMain()
    window.show()
    app.exec()
