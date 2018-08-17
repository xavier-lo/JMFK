

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

        self.actionSetting_Interface.triggered.connect(self.setting)
        self.actionManagement_Card.triggered.connect(self.managementCard)

    def setting(self):
        self.child1.show()
        pass
    def managementCard(self):
        self.gridLayout_operate.addWidget(self.child2)
        self.child2.show()
        print("laile")
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
    def __init__(self):
        super(Management, self).__init__()
        self.setupUi(self)
        '''
       动态添加Checkbox
       '''
        disp_checkbox = []
        for i in range(0,8):
            for j in range(0,8):
                disp_checkbox.append(QtWidgets.QCheckBox(self.groupBox))
                disp_checkbox[i * 8 + j].setGeometry(QtCore.QRect(20 + 90 * (i), 30 *( j + 1), 71, 16))
                disp_checkbox[i * 8 + j].setObjectName("checkBox{0}" .format(i * 8 + j))
                disp_checkbox[i * 8 + j].setText(str(i * 8 + j + 1) + "楼")

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


def main():
    app =QApplication(sys.argv)
    M   =   Main()
    T   =   Management()
#    S   =   Settings()
    M.show()
    '''
    path = sys.path[0].__str__()
    fp = open(path + '\\config','r')

    cf=fp.readlines()
    fp.close()
    fp = open(path + '\\config','w')
    cf = [ss.strip('\n') for ss in cf]

    print(cf)
    print(type(cf))
    va = ['com1','2','123345']
    rr = dict(zip(cf,va))
    rr1 =str(rr)

    rr2 = re.sub("[\s+\'+\{+\}]","",rr1)
    rr2 = re.sub("[\s+\,]","\n",rr2)
    print(rr2)
    fp.write(rr2)
    fp.close()
#    fp.write(str(rr))
 #   fp.close()

'''
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