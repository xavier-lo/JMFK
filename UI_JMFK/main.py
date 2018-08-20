

import sys
import re
sys.path.append('./UI/UI_Main.py')
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication,QMainWindow,QDialog,QMessageBox,QErrorMessage
import serial

import serial.tools.list_ports
import datetime
from UI.UI_Main import Ui_MainWindow
from UI.UI_Settinginterface import Ui_DialogSettinginterface
from UI.UI_ManageCard import Ui_Management
from UI.UI_User import Ui_User

#全局变量
COM_PORT =  None #串口
SECTOR  =   None #扇区
PASS    =   None #密码

class Main(QMainWindow,Ui_MainWindow):
    def __init__(self):
        super(Main, self).__init__()
        self.setupUi(self)
        self.child1 = Settings()
        self.child2 = Management()
        self.child3 = User()

        self.actionSetting_Interface.triggered.connect(self.setting)
        self.actionManagement_Card.triggered.connect(self.managementCard)
        self.actionUser_Card.triggered.connect(self.user)

    def setting(self):
        self.child1.show()
        pass
    def managementCard(self):
        self.gridLayout_operate.addWidget(self.child2)
        self.child3.close()
        self.child2.show()
        pass
    def user(self):
        self.gridLayout_operate.addWidget(self.child3)
        self.child2.close()
        self.child3.show()
        pass


class Settings(QDialog,Ui_DialogSettinginterface):
    def __init__(self):
        super(Settings, self).__init__()
        self.setupUi(self)

        # 检测所有存在的串口，将信息存储在字典中
        self.Com_Dict = {}
        port_list = list(serial.tools.list_ports.comports())
        self.comboBox_comport.clear()
        for port in port_list:
            self.Com_Dict["%s" % port[0]] = "%s" % port[1]
            self.comboBox_comport.addItem(port[0])
        #添加扇区
        for s in range(0,64):
            self.comboBox_sector.addItem(s.__str__())
        #添加事件
        self.pushButton.clicked.connect(self.ok_setting)
    '''
    单击保存事件
    '''
    def ok_setting(self):
        conf = ['None','None','None']
        #获取数据
        conf[0] = self.comboBox_comport.currentText()
        conf[1] = self.comboBox_sector.currentText()
        conf[2] = self.textEdit_password.toPlainText()
        #判断不为空
        if conf[0] == '':
            QMessageBox.warning(self,"input error","COM not null",QMessageBox.Close)
            return
        if conf[2] == '':
            QMessageBox.warning(self,"input error","Password not null",QMessageBox.Close)
            return
        try:#将数据写入到配置文件
            path = sys.path[0]
            fp = open(path + '\\config','w')
            na = ['COM','Sector','Password']
            print(conf)
            rt = dict(zip(na,conf))
            rt = str(rt)
            rt = re.sub("[\s+\{+\}+\']","",rt)
            rt = re.sub("[\s+\,]","\n",rt)
            fp.write(rt)
            fp.close()
            #数据更新到变量
            COM_PORT = conf[0]
            SECTOR  =  conf[1]
            PASS    =  conf[2]
            self.close()
        except Exception as e:#错误报警
            QMessageBox.warning(self,"WriteConfigError",e,QMessageBox.Yes|QMessageBox.Cancel)



class Management(QMainWindow,Ui_Management):
    disp_checkbox = []
    def __init__(self):
        super(Management, self).__init__()
        self.setupUi(self)
        '''
       动态添加Checkbox
       '''
        for i in range(0,8):
            for j in range(0,8):
                self.disp_checkbox.append(QtWidgets.QCheckBox(self.groupBox))
                self.disp_checkbox[i * 8 + j].setGeometry(QtCore.QRect(20 + 90 * (i), 30 *( j + 1), 71, 21))
                self.disp_checkbox[i * 8 + j].setObjectName("checkBox{0}" .format(i * 8 + j))
                self.disp_checkbox[i * 8 + j].setText(str(i * 8 + j + 1) + "楼")

        #初始化设置数据
        for i in range(0,8):
            self.comboBox_SetGroup.addItem(str(i))
        for i in range(0,32):
            self.comboBox_SetElv.addItem(str(i))
        for i in range(0,16):
            self.comboBox_SetSec.addItem(str(i))
        #初始时间控件
        nowtime = datetime.datetime.now()
        self.dateTimeEdit.setDateTime(nowtime)

        #列选事件
        self.checkBox_65.stateChanged.connect(self.allselect1)
        self.checkBox_66.stateChanged.connect(self.allselect2)
        self.checkBox_67.stateChanged.connect(self.allselect3)
        self.checkBox_68.stateChanged.connect(self.allselect4)
        self.checkBox_69.stateChanged.connect(self.allselect5)
        self.checkBox_70.stateChanged.connect(self.allselect6)
        self.checkBox_71.stateChanged.connect(self.allselect7)
        self.checkBox_72.stateChanged.connect(self.allselect8)
        #显示序列号
        path = sys.path[0].__str__()
        fp = open(path + '\\ISNS.csv', 'r')

        cf = fp.readlines()
        s1 ='['+ cf[1] + ']'
        s1 = eval(s1)
        print(s1)
    def allselect1(self):
        if self.checkBox_65.checkState() == QtCore.Qt.Checked:
            for i in range(0,8):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(0,8):
                self.disp_checkbox[i].setChecked(False)
    def allselect2(self):
        if self.checkBox_66.checkState() == QtCore.Qt.Checked:
            for i in range(8,16):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(8,16):
                self.disp_checkbox[i].setChecked(False)
    def allselect3(self):
        if self.checkBox_67.checkState() == QtCore.Qt.Checked:
            for i in range(16,24):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(16,24):
                self.disp_checkbox[i].setChecked(False)
    def allselect4(self):
        if self.checkBox_68.checkState() == QtCore.Qt.Checked:
            for i in range(24,32):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(24,32):
                self.disp_checkbox[i].setChecked(False)
    def allselect5(self):
        if self.checkBox_69.checkState() == QtCore.Qt.Checked:
            for i in range(32,40):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(32,40):
                self.disp_checkbox[i].setChecked(False)
    def allselect6(self):
        if self.checkBox_70.checkState() == QtCore.Qt.Checked:
            for i in range(40,48):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(40,48):
                self.disp_checkbox[i].setChecked(False)
    def allselect7(self):
        if self.checkBox_71.checkState() == QtCore.Qt.Checked:
            for i in range(48,56):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(48,56):
                self.disp_checkbox[i].setChecked(False)
    def allselect8(self):
        if self.checkBox_72.checkState() == QtCore.Qt.Checked:
            for i in range(56,64):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(56,64):
                self.disp_checkbox[i].setChecked(False)


class User(QMainWindow,Ui_User):
    disp_checkbox = []
    def __init__(self):
        super(User,self).__init__()
        self.setupUi(self)
        '''
       动态添加Checkbox
       '''
        for i in range(0,8):
            for j in range(0,8):
                self.disp_checkbox.append(QtWidgets.QCheckBox(self.groupBox_2))
                self.disp_checkbox[i * 8 + j].setGeometry(QtCore.QRect(20 + 90 * (i), 30 *( j + 1), 71, 21))
                self.disp_checkbox[i * 8 + j].setObjectName("checkBox{0}" .format(i * 8 + j))
                self.disp_checkbox[i * 8 + j].setText(str(i * 8 + j + 1) + "楼")
        #日期初始化
        self.dateEdit.setDate(datetime.datetime.now())
        self.dateEdit_2.setDate(datetime.datetime.now())
        #群号梯号初始化
        for i in range(0,8):
            self.comboBox.addItem(str(i))
        for i in range(0,32):
            self.comboBox_2.addItem(str(i))
        #列选事件
        self.checkBox_65.stateChanged.connect(self.allselect1)
        self.checkBox_66.stateChanged.connect(self.allselect2)
        self.checkBox_67.stateChanged.connect(self.allselect3)
        self.checkBox_68.stateChanged.connect(self.allselect4)
        self.checkBox_69.stateChanged.connect(self.allselect5)
        self.checkBox_70.stateChanged.connect(self.allselect6)
        self.checkBox_71.stateChanged.connect(self.allselect7)
        self.checkBox_72.stateChanged.connect(self.allselect8)

    def allselect1(self):
        if self.checkBox_65.checkState() == QtCore.Qt.Checked:
            for i in range(0,8):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(0,8):
                self.disp_checkbox[i].setChecked(False)
    def allselect2(self):
        if self.checkBox_66.checkState() == QtCore.Qt.Checked:
            for i in range(8,16):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(8,16):
                self.disp_checkbox[i].setChecked(False)
    def allselect3(self):
        if self.checkBox_67.checkState() == QtCore.Qt.Checked:
            for i in range(16,24):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(16,24):
                self.disp_checkbox[i].setChecked(False)
    def allselect4(self):
        if self.checkBox_68.checkState() == QtCore.Qt.Checked:
            for i in range(24,32):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(24,32):
                self.disp_checkbox[i].setChecked(False)
    def allselect5(self):
        if self.checkBox_69.checkState() == QtCore.Qt.Checked:
            for i in range(32,40):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(32,40):
                self.disp_checkbox[i].setChecked(False)
    def allselect6(self):
        if self.checkBox_70.checkState() == QtCore.Qt.Checked:
            for i in range(40,48):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(40,48):
                self.disp_checkbox[i].setChecked(False)
    def allselect7(self):
        if self.checkBox_71.checkState() == QtCore.Qt.Checked:
            for i in range(48,56):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(48,56):
                self.disp_checkbox[i].setChecked(False)
    def allselect8(self):
        if self.checkBox_72.checkState() == QtCore.Qt.Checked:
            for i in range(56,64):
                self.disp_checkbox[i].setChecked(True)
        else:
            for i in range(56,64):
                self.disp_checkbox[i].setChecked(False)









def main():
    app =QApplication(sys.argv)
    M   =   Main()
    T   =   Management()
#    S   =   Settings()
    M.show()






    sys.exit(app.exec())


if __name__ == '__main__':
    main()


'''
    def test(self):
        dgwindow = QDialog
        dg = Ui_DialogSettinginterface()
        dg.setupUi(dgwindow)
        dgwindow.show()
'''