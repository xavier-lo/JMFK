# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UI_Settinginterface.ui'
#
# Created by: PyQt5 UI code generator 5.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_DialogSettinginterface(object):
    def setupUi(self, DialogSettinginterface):
        DialogSettinginterface.setObjectName("DialogSettinginterface")
        DialogSettinginterface.resize(434, 346)
        self.groupBox = QtWidgets.QGroupBox(DialogSettinginterface)
        self.groupBox.setGeometry(QtCore.QRect(20, 20, 401, 311))
        self.groupBox.setObjectName("groupBox")
        self.label = QtWidgets.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(70, 90, 81, 16))
        self.label.setObjectName("label")
        self.comboBox_comport = QtWidgets.QComboBox(self.groupBox)
        self.comboBox_comport.setGeometry(QtCore.QRect(140, 90, 99, 24))
        self.comboBox_comport.setObjectName("comboBox_comport")
        self.label_2 = QtWidgets.QLabel(self.groupBox)
        self.label_2.setGeometry(QtCore.QRect(70, 140, 101, 18))
        self.label_2.setObjectName("label_2")
        self.comboBox_sector = QtWidgets.QComboBox(self.groupBox)
        self.comboBox_sector.setGeometry(QtCore.QRect(140, 140, 99, 24))
        self.comboBox_sector.setObjectName("comboBox_sector")
        self.label_3 = QtWidgets.QLabel(self.groupBox)
        self.label_3.setGeometry(QtCore.QRect(70, 200, 81, 18))
        self.label_3.setObjectName("label_3")
        self.textEdit_password = QtWidgets.QTextEdit(self.groupBox)
        self.textEdit_password.setGeometry(QtCore.QRect(140, 190, 201, 31))
        self.textEdit_password.setObjectName("textEdit_password")
        self.pushButton = QtWidgets.QPushButton(self.groupBox)
        self.pushButton.setGeometry(QtCore.QRect(140, 250, 112, 34))
        self.pushButton.setObjectName("pushButton")

        self.retranslateUi(DialogSettinginterface)
        QtCore.QMetaObject.connectSlotsByName(DialogSettinginterface)

    def retranslateUi(self, DialogSettinginterface):
        _translate = QtCore.QCoreApplication.translate
        DialogSettinginterface.setWindowTitle(_translate("DialogSettinginterface", "Setting Interface"))
        self.groupBox.setTitle(_translate("DialogSettinginterface", "配置"))
        self.label.setText(_translate("DialogSettinginterface", "串口:"))
        self.label_2.setText(_translate("DialogSettinginterface", "扇区:"))
        self.label_3.setText(_translate("DialogSettinginterface", "密码:"))
        self.pushButton.setText(_translate("DialogSettinginterface", "保存"))

