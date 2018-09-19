# -*- mode: python -*-

block_cipher = None


a = Analysis(['main.py'],
             pathex=['F:\\MyDev\\JMFK\\JMFK\\UI_JMFK'],
             binaries=[],
             datas=[('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\UI\*.*','UI'),('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\config',''),('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\ISNS.csv',''),('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\search1.png',''),('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\Timer_Base.py',''),('F:\\MyDev\\JMFK\\JMFK\\UI_JMFK\\operate_serialport.py','')],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          exclude_binaries=True,
          name='main',
          debug=False,
          strip=False,
          upx=True,
          console=True )
coll = COLLECT(exe,
               a.binaries,
               a.zipfiles,
               a.datas,
               strip=False,
               upx=True,
               name='main')
