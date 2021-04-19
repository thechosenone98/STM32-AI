# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ApplicationWindow.ui'
#
# Created by: PyQt5 UI code generator 5.15.4
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1068, 712)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout_5 = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.gridLayout = QtWidgets.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.gb_graphViewer = QtWidgets.QGroupBox(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gb_graphViewer.sizePolicy().hasHeightForWidth())
        self.gb_graphViewer.setSizePolicy(sizePolicy)
        self.gb_graphViewer.setObjectName("gb_graphViewer")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.gb_graphViewer)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.gridLayout.addWidget(self.gb_graphViewer, 0, 1, 1, 1)
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.verticalLayout.setObjectName("verticalLayout")
        self.lsw_Terminal = QtWidgets.QListWidget(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.lsw_Terminal.sizePolicy().hasHeightForWidth())
        self.lsw_Terminal.setSizePolicy(sizePolicy)
        self.lsw_Terminal.setMinimumSize(QtCore.QSize(200, 300))
        self.lsw_Terminal.setMaximumSize(QtCore.QSize(200, 300))
        self.lsw_Terminal.setObjectName("lsw_Terminal")
        self.verticalLayout.addWidget(self.lsw_Terminal)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.btn_StartStop = QtWidgets.QPushButton(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.btn_StartStop.sizePolicy().hasHeightForWidth())
        self.btn_StartStop.setSizePolicy(sizePolicy)
        self.btn_StartStop.setMinimumSize(QtCore.QSize(200, 0))
        self.btn_StartStop.setMaximumSize(QtCore.QSize(200, 16777215))
        self.btn_StartStop.setObjectName("btn_StartStop")
        self.horizontalLayout.addWidget(self.btn_StartStop)
        self.cmb_comPort = QtWidgets.QComboBox(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.cmb_comPort.sizePolicy().hasHeightForWidth())
        self.cmb_comPort.setSizePolicy(sizePolicy)
        self.cmb_comPort.setMinimumSize(QtCore.QSize(75, 0))
        self.cmb_comPort.setMaximumSize(QtCore.QSize(100, 16777215))
        self.cmb_comPort.setObjectName("cmb_comPort")
        self.horizontalLayout.addWidget(self.cmb_comPort)
        self.verticalLayout.addLayout(self.horizontalLayout)
        spacerItem = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.gridLayout.addLayout(self.verticalLayout, 0, 0, 1, 1)
        self.horizontalLayout_5.addLayout(self.gridLayout)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1068, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.gb_graphViewer.setTitle(_translate("MainWindow", "3D Viewer"))
        self.btn_StartStop.setText(_translate("MainWindow", "Start"))
