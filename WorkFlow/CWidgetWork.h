#ifndef CWIDGETWORK_H
#define CWIDGETWORK_H

#include "cwidget.h"
#include <QWidget>

//运行状况窗口，可以显示系统运行的流程和状况
class CWidgetWork : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetWork(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    //初始化界面
    void initUI();

    //设置文件配置路径
    void setFileCfg(QString sFile);

private:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    CWidget *View;//
};

#endif // CWIDGETWORK_H
