﻿#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QSharedMemory>
#include "MainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*使用共享内存方式只运行一个相同的Qt实例进程
    （1）QSharedMemory
        方式：先创建一个共享内存，然后在每一个应用程序开始运行之前进行检查是否可以创建一个具有相同unique_id的共享内存，
        如果不能，则表示创建了实例正在运行。
    */
    QSharedMemory shared_memory;
    shared_memory.setKey(QString("PSDPPRV"));

    if(shared_memory.attach())
    {
        return 0;
    }

    if(shared_memory.create(1))
    {
        //设置样式
        QFile qss(":/qss/360safe");
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();

        MainWidget wnd;
        wnd.show();

        return a.exec();
    }
}
