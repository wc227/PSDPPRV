#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T11:53:43
#
#-------------------------------------------------

QT       += core gui widgets axcontainer

RC_ICONS = earth.ico

#DESTDIR += $$PWD/Bin

TARGET = PSDPPRV

TEMPLATE = app

RESOURCES += \
    Resources/360safe.qrc

FORMS += \
#    FrmMain.ui \
    DlgSetting.ui \
    WorkFlow/citempropertydialog.ui

HEADERS += \
    XxwQtPub.h \
#    PubHead.h \
    MyComDatMgr.h \
    MyCompInfo.h \
    MyCompItem.h \
    MyPolylineItem.h \
#    PushButtonEx.h \
    webaxwidget.h \
    BorderlessWidget.h \
#    FrmMain.h \
    DlgSetting.h \
    MainWidget.h \
#    MapWidget.h \
    CfgMgr.h \
    FileMgrBase.h \
    FormLanView.h \
    FormWeb.h \
    WorkFlow/cbaritem.h \
    WorkFlow/CGraphicsObjectItem.h \
    WorkFlow/cgraphicsscene.h \
    WorkFlow/citempropertydialog.h \
    WorkFlow/cmypathitem.h \
    WorkFlow/coutitem.h \
    WorkFlow/cwidget.h \
    WorkFlow/dlgworkflow.h \
    WorkFlow/tpsdevts.h

SOURCES += \
    main.cpp \
    XxwQtPub.cpp \
    MyComDatMgr.cpp \
    MyCompInfo.cpp \
    MyCompItem.cpp \
    MyPolylineItem.cpp \
#    PushButtonEx.cpp \
    BorderlessWidget.cpp \
#    FrmMain.cpp \
    DlgSetting.cpp \
    MainWidget.cpp \
#    MapWidget.cpp \
    CfgMgr.cpp \
    FileMgrBase.cpp \
    FormLanView.cpp \
    FormWeb.cpp \
    WorkFlow/cbaritem.cpp \
    WorkFlow/cgraphicsobjectitem.cpp \
    WorkFlow/cgraphicsscene.cpp \
    WorkFlow/citempropertydialog.cpp \
    WorkFlow/cmypathitem.cpp \
    WorkFlow/coutitem.cpp \
    WorkFlow/cwidget.cpp \
    WorkFlow/dlgworkflow.cpp \
    WorkFlow/tpsdevts.cpp



CONFIG(debug, debug|release) {
  #设置debug配置下编译生成文件的路径
  TARGET = $$join(TARGET,,,d)   #为debug版本生成的文件增加d的后缀
  contains(TEMPLATE, "lib") {
    DESTDIR = $$PWD/Bin/        #将库放在当前工程的Bin/文件夹下
    DLLDESTDIR = $$PWD/Bin/    #将动态库放在当前工程的Bin/文件夹下
  } else {
    DESTDIR = $$PWD/Bin//        #将应用程序放在当前工程的Bin/文件夹下
  }
  OBJECTS_DIR = ./debug/obj     #将生成的对象文件放在专门的obj文件夹下
  MOC_DIR = ./debug/moc         #将QT自动生成的对象放在moc文件夹下
} else {
  #设置release配置下编译生成文件的路径
  contains(TEMPLATE, "lib") {
    DESTDIR = $$PWD/Bin/       #将库放在当前工程的Bin/文件夹下
    DLLDESTDIR = $$PWD/Bin/    #将动态库放在当前工程的Bin/文件夹下
  } else {
    DESTDIR = $$PWD/Bin/       #将应用程序放在当前工程的Bin/文件夹下
  }
  OBJECTS_DIR = ./release/obj   #将生成的对象文件放在专门的obj文件夹下
  MOC_DIR = ./release/moc       #将QT自动生成的对象放在moc文件夹下
}
