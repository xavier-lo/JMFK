# -*- coding: utf-8 -*-
import sys
import re
sys.path.append('./')
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication,QMainWindow,QDialog,QMessageBox,QErrorMessage,QTableWidgetItem,QGraphicsScene,QGraphicsPixmapItem,QHeaderView,QAbstractItemView
from PyQt5.QtGui import QPixmap,QColor,QBrush
from PyQt5.QtCore import pyqtSlot


import threading
import serial
from Timer_Base import BaseTimer
from operate_serialport import *

import serial.tools.list_ports
import datetime
from UI.UI_Main import Ui_MainWindow
from UI.UI_Settinginterface import Ui_DialogSettinginterface
from UI.UI_ManageCard import Ui_Management
from UI.UI_User import Ui_User

from  time import sleep

#全局变量
COM_PORT =  None #串口
SECTOR  =   None #扇区
PASS    =   None #密码

IC_NUM = []#IC序列号
USERCARD_DATA = []#用户卡数据
MANAGECARD_DATA = []#管理卡数据



path = sys.path[0].__str__()
#gfp =open(path + '\\config', 'r')
gfp = open('config', 'r')
red = gfp.readlines()
COM_PORT = red[0][4:int(len(red[0])) - 1]
SECTOR = red[1][7:int(len(red[1]) - 1)]
PASS = red[2][9:int(len(red[2]))]
gfp.close()

seri = serial.Serial()
seri.port = COM_PORT
seri.baudrate = 19200
seek_card_date = [0xbe, 0x04, 0x00 ,0x00, 0x00, 0xe0]
read_card_data_1 = [0xbe, 0x04, 0x01, 0x00, 0x00, 0xe0]#读一块数据
read_card_data_2 = [0xbe, 0x04, 0x02, 0x00 ,0x00, 0xe0]#读俩块数据





class TimerFunc(BaseTimer):
    """可传递任何函数的定时任务类"""
    def __init__(self,func,howtime=1.0,enduring=True):
        BaseTimer.__init__(self,howtime,enduring)
        self.func = func
    def exec(self):
        self.func() #调用函数



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
    #def closeEvent(self, a0: QtGui.QCloseEvent):


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

    @pyqtSlot()
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

        #tablewidget尺寸设置
        self.tableWidget.setColumnWidth(0,100)
        self.tableWidget.setColumnWidth(1,100)
        self.tableWidget.setColumnWidth(2,200)
        self.tableWidget.setColumnWidth(3,200)
        self.tableWidget.setColumnWidth(4,100)
        self.tableWidget.setColumnWidth(5,100)
        # 下面代码让表格100填满窗口
        #self.tableWidget.horizontalHeader().setStretchLastSection(True)
        #self.tableWidget.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        #tablewidget选中函数槽
        #self.tableWidget.setSelectionMode(QAbstractItemView.ExtendedSelection)  # 设置为可以选中多个目标
        self.isn_chose_row = -1
        #self.tableWidget.selectRow.connect(self.isn_chose)
        self.tableWidget.clicked.connect(self.isn_chose)
        #挂失函数槽
        self.pushButton_6.clicked.connect(self.lose_isn)
        #加载搜索图标

        self.image = QPixmap()
        path = sys.path[0].__str__()
        #self.image.load(path+ '\\search1.png')
        self.image.load('search1.png')

        #self.graphicsView.scene = QGraphicsScene()            # 创建一个图片元素的对象
        #item = QGraphicsPixmapItem(self.image)                # 创建一个变量用于承载加载后的图片
        #self.graphicsView.scene.addItem(item)                 # 将加载后的图片传递给scene对象
        #self.graphicsView.setScene(self.graphicsView.scene)   # 这个我也不知道是做了个啥

        self.label_5.setPixmap(self.image)
        #检索
        self.textEdit.textChanged.connect(self.search_isn)
        #显示序列号
        self.display_isn()
        #新建序列号按钮事件
        self.create_isn_state = 0 #写入状态
        self.pushButton_createSerialNum.clicked.connect(self.create_serialnum)
        self.pushButton_saveisn.clicked.connect(self.save_isn)




    #挂失序列号
    def lose_isn(self):
        if self.isn_chose_row == -1:
            QMessageBox.warning(self, "input error", "请选择序列号", QMessageBox.Close)
        else:
            reply = QMessageBox.information(self, "warning", "确定要挂失吗？", QMessageBox.Yes | QMessageBox.No)
            if reply == QMessageBox.Yes:
                path = sys.path[0].__str__()
                fp = open(path + '\\ISNS.csv', 'r', encoding = 'UTF-8')
                ss = fp.readlines()
                if ss[self.isn_chose_row + 1][-4:-1] == "使用中" or ss[self.isn_chose_row + 1][-4:-1] == ",使用":
                    strr = ''
                    for i in range(len(ss[self.isn_chose_row + 1])):
                        if i == 0:
                            strr += '1'
                        else:
                            strr += ss[self.isn_chose_row + 1][i]
                    strr = strr.replace("使用中","已挂失")
                    ss[self.isn_chose_row + 1] = strr
                    fp.close()
                    ffp = open(path + '\\ISNS.csv', 'w', encoding = 'UTF-8')
                    for iii in range(len(ss)):
                        ss[iii] = ss[iii].replace(ss[iii][-14:-12],'^'+ ss[iii][-14:-12])
                    ss = str(ss)
                    ss = re.sub("[\s + \[ + \] + \']", "", ss)
                    ss = ss.replace('\\n,','\n')
                    ss = ss.replace('^', ' ')
                    ffp.write(ss)
                    ffp.close()
                    self.display_isn()
                    pass
                elif ss[self.isn_chose_row + 1][-4:-1] == "已挂失" or ss[self.isn_chose_row + 1][-4:-1] == ",已挂":
                    QMessageBox.warning(self, "input error", "已挂失", QMessageBox.Close)

    #tablewidget选中函数
    def isn_chose(self):
        self.isn_chose_row =  self.tableWidget.currentIndex().row()
        self.tableWidget.selectRow(self.isn_chose_row)#整行选中
        pass

     #检索用户信息
    def search_isn(self):
        ll = []
        path = sys.path[0].__str__()
        fp = open(path + '\\ISNS.csv', 'r', encoding = 'UTF-8')
        ss = fp.readlines()
        ii = 1
        while ii < len(ss):
            str = re.sub("[\s+\,]","",ss[ii])
            if str.find(self.textEdit.toPlainText(),0,len(str)) != -1:
                ll.append(ss[ii])
            ii += 1

        length = len(ll)
        self.tableWidget.setRowCount(length)

        #以,分割成列表 并显示到tableWidget上
        i =  0
        while i < length :
            sss = ll[i].strip(',').split(',')
            j = 0
            while j < len(sss):
                if j == 0:
                    self.tableWidget.setItem(i, j, QTableWidgetItem('0' + sss[j][1:5]))
                else:
                    self.tableWidget.setItem(i, j, QTableWidgetItem(sss[j]))
                if sss[j] == "已挂失" or sss[j] == "已挂失\n":
                    self.tableWidget.item(i, j).setBackground(QBrush(QColor(255, 0, 0)))
                j = j + 1
            i = i + 1
        pass

    #新建序列号
    def create_serialnum(self):
        if self.create_isn_state == 0:
            path = sys.path[0].__str__()
            fp = open(path + '\\ISNS.csv',  'r', encoding = 'UTF-8')
            ss = fp.readlines()
            self.tableWidget.setRowCount(len(ss))
            sss = ss[len(ss) - 1].strip(',').split(',')
            num = str(int(sss[0]) % 10000 +1)
            if len(num) == 1:
                sss = '0000' + num
            if len(num) == 2:
                sss = '000' + num
            if len(num) == 3:
                sss = '00' + num
            if len(num) == 4:
                sss = '0' + num
            if len(num) >= 5:
                sss = num
            self.tableWidget.setItem(len(ss) - 1, 0, QTableWidgetItem(sss))
            for i in range(1,3):
                self.tableWidget.setItem(len(ss) -1,i,QTableWidgetItem('*'))
            time = str(datetime.datetime.now())
            self.tableWidget.setItem(len(ss) - 1, 3, QTableWidgetItem(time[0:16]))
            self.tableWidget.setItem(len(ss) - 1, 4, QTableWidgetItem("未发卡"))
            self.tableWidget.setItem(len(ss) - 1, 5, QTableWidgetItem("创建中"))
            fp.close()
            self.create_isn_state = 1
            pass
        else:
            QMessageBox.warning(self, "input error", "上次创建未保存", QMessageBox.Close)
            pass

    #保存新建序列号
    def save_isn(self):
        if self.create_isn_state == 1:
            new_data = []
            row = self.tableWidget.rowCount() - 1
            for i in range(0,6):
                da = self.tableWidget.item(row,i).text()
                new_data.append(da)
                pass
            path = sys.path[0].__str__()
            fp = open(path + '\\ISNS.csv','a', encoding = 'UTF-8')
            new_data[3] = new_data[3][0:10] + '_' + new_data[3][11:16]
            new_data[5] ="使用中"
            ss = str(new_data)
            ss = re.sub("[\s+\[+\]+\']","",ss)
            ss = '\n' + ss.replace('_',' ')
            fp.write(ss)
            fp.close()
            self.display_isn()
            self.create_isn_state = 0
            pass
        else:
            QMessageBox.warning(self, "input error", "本次次创建已保存，请重新创建", QMessageBox.Close)
            pass
    #显示序列号到tablewidget
    def display_isn(self):
        path = sys.path[0].__str__()
        fp = open(path + '\\ISNS.csv', 'r', encoding = 'UTF-8')
        cf = fp.readlines()
        self.tableWidget.setRowCount(len(cf) - 1)
        #以,分割成列表 并显示到tableWidget上
        i =  0
        while i < len(cf) - 1:
            ss = cf[i + 1].strip(',').split(',')
            j = 0
            while j < len(ss):
                if j == 0:
                    self.tableWidget.setItem(i, j, QTableWidgetItem('0' + str(ss[j][1:5])))
                else:
                    self.tableWidget.setItem(i, j, QTableWidgetItem( str(ss[j])))
                if ss[j] == "已挂失" or ss[j] == "已挂失\n":
                    self.tableWidget.item(i, j).setBackground(QBrush(QColor(255, 0, 0)))
                j = j + 1
            i = i + 1
        fp.close()
        pass

     #全选checkbox
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



def make_verrify(*da):
    ss = 0x00
    i = 1
    while i < da[0][1]:
        ss ^= da[0][i]
        i += 1
    da[0][i] = ss


def seek_ICCard():
    make_verrify(seek_card_date)
    seri.write(seek_card_date)
    print('sending')
    ss = seri.read(10)


def read_ICcard():
    USERCARD_DATA.clear()
    read_card_data_1[3] =int(SECTOR) * 4 + 0
    make_verrify(read_card_data_1)
    seri.write(read_card_data_1)
    ss = seri.read(22)
    ss = list(ss)
    for i in ss:
        USERCARD_DATA.append(i)
    read_card_data_2[3] = int(SECTOR) * 4 + 1
    make_verrify(read_card_data_2)
    seri.write(read_card_data_2)
    ss = seri.read(38)
    ss = list(ss)
    for i in ss:
        USERCARD_DATA.append(i)
    print(USERCARD_DATA)
    pass



def main():
    seri.port = COM_PORT
    seri.baudrate = 19200
    print(seri)
    seri.open()
    print(seri.isOpen())
    seek_ICCard()
    t = TimerFunc(read_ICcard)
    t.start()
    app =QApplication(sys.argv)
    M   =   Main()
    T   =   Management()
#    S   =   Settings()
    M.show()
    while(True):
        if app.exec() is not True:
            t.stop()
            break

    sys.exit()

if __name__ == '__main__':
    main()


