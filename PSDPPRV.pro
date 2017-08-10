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
    DlgSetting.ui

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
    CfgMgr.h

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
    CfgMgr.cpp
