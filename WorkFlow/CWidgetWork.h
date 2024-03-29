﻿#ifndef CWIDGETWORK_H
#define CWIDGETWORK_H

#include "cwidget.h"
#include <QWidget>
#include <QSpacerItem>
#include <QGridLayout>

//运行状况窗口，可以显示系统运行的流程和状况
class CWidgetWork : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetWork(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    //初始化界面
    void initUI();

    //视图居中处理
    void centerView();

    //设置文件配置路径
    void setFileCfg(QString sFile);

protected:
    virtual void resizeEvent(QResizeEvent *event);

signals:
    void sendCmd(QString sCmd);//发送命令

private:
    CXGraphicsView *View;//

};

#endif // CWIDGETWORK_H
