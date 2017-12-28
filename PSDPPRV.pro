#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T11:53:43
#
#-------------------------------------------------

#QT       += core gui widgets
QT       += axcontainer charts

RC_ICONS = Resources/earth.ico

TARGET = PSDPPRV

TEMPLATE = app

FORMS += \
    WorkFlow/citempropertydialog.ui \
    WorkFlow/CXAnimatePolylinePropterty.ui \
    DlgXBarChartProperty.ui


HEADERS += \
    WorkFlow/cbaritem.h \
    WorkFlow/CGraphicsObjectItem.h \
    WorkFlow/cgraphicsscene.h \
    WorkFlow/citempropertydialog.h \
    WorkFlow/cmypathitem.h \
    WorkFlow/coutitem.h \
    WorkFlow/cwidget.h \
    WorkFlow/tpsdevts.h \
    WorkFlow/CWidgetWork.h \
    WorkFlow/CXAnimatePolyline.h \
    WorkFlow/CXAnimatePolylinePropterty.h \
    XxwQtPub.h \
    webaxwidget.h \
    FileMgrBase.h \
    FormWeb.h \
    MainWnd.h \
    XBar.h \
    XBarChart.h \
    FileMgrDIDX.h \
    DlgXBarChartProperty.h \
    FileMgrErrInfo.h \
    xxwdockwidget.h

SOURCES += \
    main.cpp \
    WorkFlow/cbaritem.cpp \
    WorkFlow/cgraphicsobjectitem.cpp \
    WorkFlow/cgraphicsscene.cpp \
    WorkFlow/citempropertydialog.cpp \
    WorkFlow/cmypathitem.cpp \
    WorkFlow/coutitem.cpp \
    WorkFlow/cwidget.cpp \
    WorkFlow/tpsdevts.cpp \
    WorkFlow/CXAnimatePolyline.cpp \
    WorkFlow/CXAnimatePolylinePropterty.cpp \
    WorkFlow/CWidgetWork.cpp \
    XxwQtPub.cpp \
    FileMgrBase.cpp \
    FormWeb.cpp \
    MainWnd.cpp \
    XBar.cpp \
    XBarChart.cpp \
    FileMgrDIDX.cpp \
    DlgXBarChartProperty.cpp \
    FileMgrErrInfo.cpp


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

RESOURCES += \
    Resources/StateGreen/StateGreen.qrc
