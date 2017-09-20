#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "BorderlessWidget.h"
#include "WorkFlow/dlgworkflow.h"
#include "CfgMgr.h"
#include <QProcess>

QT_BEGIN_NAMESPACE
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QToolButton;
class QTabWidget;
class QAction;
class FormWebBase;
QT_END_NAMESPACE


class MainWidget : public BorderlessWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

    void InitUI();

protected:
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    //初始化所有的动作
    void InitActions();

private slots:
    //最大化或还原
    void showMaxOrNormal();

    //启动HttpServer
    void StartHttpServer();

    //停止HttpServer
    void StopHttpServer();

    //激活标签页窗口
    void activeTab(int nTab);

    //刷新
    void Refresh();

private:
    QGridLayout *m_mainLayout;
    QHBoxLayout *m_titleBarLayout;//标题栏的布局
    QLabel *m_lblTitle;
    QLabel *m_lblTitleZone;//用来显示标题区域
    QToolButton *m_btnMin;
    QToolButton *m_btnMax;
    QToolButton *m_btnClose;
    QTabWidget *m_tabWidget;

    DlgWorkFlow *m_wndWorkFlow;
    FormWebBase *m_wndWebMap1;
    FormWebBase *m_wndWebMap2;
    FormWebBase *m_wndWebMap3;
    FormWebBase *m_wndWebBar;
    bool m_arrTabInit[5];//标签页面是否初始化

    bool m_bWndMaxmized;//窗口是否最大化

    QAction *m_pActionRefresh;//刷新动作

    QProcess m_proHttpServer;//httpServer进程

    CfgMgr m_cfgMgr;//配置信息
};

#endif // MAINWIDGET_H
