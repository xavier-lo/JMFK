# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UI_ManageCard.ui'
#
# Created by: PyQt5 UI code generator 5.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Management(object):
    def setupUi(self, Management):
        Management.setObjectName("Management")
        Management.resize(760, 490)
        self.centralwidget = QtWidgets.QWidget(Management)
        self.centralwidget.setObjectName("centralwidget")
        self.tabWidget_ManagementCard = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget_ManagementCard.setGeometry(QtCore.QRect(4, 5, 750, 440))
        self.tabWidget_ManagementCard.setObjectName("tabWidget_ManagementCard")
        self.tab = QtWidgets.QWidget()
        self.tab.setObjectName("tab")
        self.groupBox_SetPara = QtWidgets.QGroupBox(self.tab)
        self.groupBox_SetPara.setGeometry(QtCore.QRect(10, 20, 521, 80))
        self.groupBox_SetPara.setObjectName("groupBox_SetPara")
        self.comboBox_SetSec = QtWidgets.QComboBox(self.groupBox_SetPara)
        self.comboBox_SetSec.setGeometry(QtCore.QRect(420, 30, 69, 22))
        self.comboBox_SetSec.setObjectName("comboBox_SetSec")
        self.label_3 = QtWidgets.QLabel(self.groupBox_SetPara)
        self.label_3.setGeometry(QtCore.QRect(350, 35, 71, 16))
        self.label_3.setObjectName("label_3")
        self.comboBox_SetGroup = QtWidgets.QComboBox(self.groupBox_SetPara)
        self.comboBox_SetGroup.setGeometry(QtCore.QRect(90, 30, 69, 22))
        self.comboBox_SetGroup.setObjectName("comboBox_SetGroup")
        self.label = QtWidgets.QLabel(self.groupBox_SetPara)
        self.label.setGeometry(QtCore.QRect(30, 35, 54, 12))
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(self.groupBox_SetPara)
        self.label_2.setGeometry(QtCore.QRect(190, 35, 54, 12))
        self.label_2.setObjectName("label_2")
        self.comboBox_SetElv = QtWidgets.QComboBox(self.groupBox_SetPara)
        self.comboBox_SetElv.setGeometry(QtCore.QRect(250, 30, 69, 22))
        self.comboBox_SetElv.setObjectName("comboBox_SetElv")
        self.groupBox = QtWidgets.QGroupBox(self.tab)
        self.groupBox.setGeometry(QtCore.QRect(10, 110, 731, 301))
        self.groupBox.setObjectName("groupBox")
        self.checkBox_65 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_65.setGeometry(QtCore.QRect(20, 270, 71, 16))
        self.checkBox_65.setObjectName("checkBox_65")
        self.checkBox_66 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_66.setGeometry(QtCore.QRect(110, 270, 71, 16))
        self.checkBox_66.setObjectName("checkBox_66")
        self.checkBox_67 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_67.setGeometry(QtCore.QRect(200, 270, 71, 16))
        self.checkBox_67.setObjectName("checkBox_67")
        self.checkBox_68 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_68.setGeometry(QtCore.QRect(290, 270, 71, 16))
        self.checkBox_68.setObjectName("checkBox_68")
        self.checkBox_69 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_69.setGeometry(QtCore.QRect(380, 270, 71, 16))
        self.checkBox_69.setObjectName("checkBox_69")
        self.checkBox_70 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_70.setGeometry(QtCore.QRect(470, 270, 71, 16))
        self.checkBox_70.setObjectName("checkBox_70")
        self.checkBox_71 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_71.setGeometry(QtCore.QRect(560, 270, 71, 16))
        self.checkBox_71.setObjectName("checkBox_71")
        self.checkBox_72 = QtWidgets.QCheckBox(self.groupBox)
        self.checkBox_72.setGeometry(QtCore.QRect(650, 270, 71, 16))
        self.checkBox_72.setObjectName("checkBox_72")
        self.groupBox_2 = QtWidgets.QGroupBox(self.tab)
        self.groupBox_2.setGeometry(QtCore.QRect(550, 20, 191, 80))
        self.groupBox_2.setObjectName("groupBox_2")
        self.pushButton = QtWidgets.QPushButton(self.groupBox_2)
        self.pushButton.setGeometry(QtCore.QRect(50, 30, 91, 31))
        self.pushButton.setObjectName("pushButton")
        self.tabWidget_ManagementCard.addTab(self.tab, "")
        self.tab_2 = QtWidgets.QWidget()
        self.tab_2.setObjectName("tab_2")
        self.groupBox_4 = QtWidgets.QGroupBox(self.tab_2)
        self.groupBox_4.setGeometry(QtCore.QRect(10, 10, 721, 80))
        self.groupBox_4.setTitle("")
        self.groupBox_4.setObjectName("groupBox_4")
        self.pushButton_createSerialNum = QtWidgets.QPushButton(self.groupBox_4)
        self.pushButton_createSerialNum.setGeometry(QtCore.QRect(30, 30, 75, 23))
        self.pushButton_createSerialNum.setObjectName("pushButton_createSerialNum")
        self.pushButton_5 = QtWidgets.QPushButton(self.groupBox_4)
        self.pushButton_5.setGeometry(QtCore.QRect(250, 30, 75, 23))
        self.pushButton_5.setObjectName("pushButton_5")
        self.pushButton_6 = QtWidgets.QPushButton(self.groupBox_4)
        self.pushButton_6.setGeometry(QtCore.QRect(140, 30, 75, 23))
        self.pushButton_6.setObjectName("pushButton_6")
        self.pushButton_4 = QtWidgets.QPushButton(self.groupBox_4)
        self.pushButton_4.setGeometry(QtCore.QRect(360, 30, 111, 23))
        self.pushButton_4.setObjectName("pushButton_4")
        self.treeWidget = QtWidgets.QTreeWidget(self.tab_2)
        self.treeWidget.setGeometry(QtCore.QRect(10, 100, 721, 311))
        self.treeWidget.setObjectName("treeWidget")
        self.tabWidget_ManagementCard.addTab(self.tab_2, "")
        self.tab_3 = QtWidgets.QWidget()
        self.tab_3.setObjectName("tab_3")
        self.groupBox_3 = QtWidgets.QGroupBox(self.tab_3)
        self.groupBox_3.setGeometry(QtCore.QRect(20, 20, 661, 91))
        self.groupBox_3.setObjectName("groupBox_3")
        self.label_4 = QtWidgets.QLabel(self.groupBox_3)
        self.label_4.setGeometry(QtCore.QRect(60, 40, 81, 16))
        self.label_4.setObjectName("label_4")
        self.dateTimeEdit = QtWidgets.QDateTimeEdit(self.groupBox_3)
        self.dateTimeEdit.setGeometry(QtCore.QRect(150, 37, 194, 22))
        self.dateTimeEdit.setObjectName("dateTimeEdit")
        self.pushButton_2 = QtWidgets.QPushButton(self.groupBox_3)
        self.pushButton_2.setGeometry(QtCore.QRect(410, 37, 75, 23))
        self.pushButton_2.setObjectName("pushButton_2")
        self.pushButton_3 = QtWidgets.QPushButton(self.groupBox_3)
        self.pushButton_3.setGeometry(QtCore.QRect(520, 37, 75, 23))
        self.pushButton_3.setObjectName("pushButton_3")
        self.tabWidget_ManagementCard.addTab(self.tab_3, "")
        Management.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(Management)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 760, 23))
        self.menubar.setObjectName("menubar")
        Management.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(Management)
        self.statusbar.setObjectName("statusbar")
        Management.setStatusBar(self.statusbar)

        self.retranslateUi(Management)
        self.tabWidget_ManagementCard.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(Management)

    def retranslateUi(self, Management):
        _translate = QtCore.QCoreApplication.translate
        Management.setWindowTitle(_translate("Management", "management"))
        self.groupBox_SetPara.setTitle(_translate("Management", "设置（拨码1拨ON）"))
        self.label_3.setText(_translate("Management", "扇区号："))
        self.label.setText(_translate("Management", "群号："))
        self.label_2.setText(_translate("Management", "梯号："))
        self.groupBox.setTitle(_translate("Management", "写入短路数据"))
        self.checkBox_65.setText(_translate("Management", "列选"))
        self.checkBox_66.setText(_translate("Management", "列选"))
        self.checkBox_67.setText(_translate("Management", "列选"))
        self.checkBox_68.setText(_translate("Management", "列选"))
        self.checkBox_69.setText(_translate("Management", "列选"))
        self.checkBox_70.setText(_translate("Management", "列选"))
        self.checkBox_71.setText(_translate("Management", "列选"))
        self.checkBox_72.setText(_translate("Management", "列选"))
        self.groupBox_2.setTitle(_translate("Management", "写卡操作"))
        self.pushButton.setText(_translate("Management", "写卡"))
        self.tabWidget_ManagementCard.setTabText(self.tabWidget_ManagementCard.indexOf(self.tab), _translate("Management", "写数据"))
        self.pushButton_createSerialNum.setText(_translate("Management", "新建序列号"))
        self.pushButton_5.setText(_translate("Management", "写入管理卡"))
        self.pushButton_6.setText(_translate("Management", "挂失序列号"))
        self.pushButton_4.setText(_translate("Management", "初始新序列用户卡"))
        self.treeWidget.headerItem().setText(0, _translate("Management", "序列号"))
        self.treeWidget.headerItem().setText(1, _translate("Management", "姓名"))
        self.treeWidget.headerItem().setText(2, _translate("Management", "公司"))
        self.treeWidget.headerItem().setText(3, _translate("Management", "创建时间"))
        self.treeWidget.headerItem().setText(4, _translate("Management", "发卡状态"))
        self.treeWidget.headerItem().setText(5, _translate("Management", "卡片状态"))
        self.tabWidget_ManagementCard.setTabText(self.tabWidget_ManagementCard.indexOf(self.tab_2), _translate("Management", "写序列号"))
        self.groupBox_3.setTitle(_translate("Management", "时间"))
        self.label_4.setText(_translate("Management", "刷卡器时间："))
        self.pushButton_2.setText(_translate("Management", "读时间"))
        self.pushButton_3.setText(_translate("Management", "写时间"))
        self.tabWidget_ManagementCard.setTabText(self.tabWidget_ManagementCard.indexOf(self.tab_3), _translate("Management", "读写刷卡器时间"))
