#ifndef _MAINWND_H_20170927_
#define _MAINWND_H_20170927_

#include "BorderlessMainWnd.h"
#include "WorkFlow/cwidget.h"
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
class QListWidget;
QT_END_NAMESPACE


class MainWnd : public BorderlessMainWnd
{
    Q_OBJECT

public:
    MainWnd(QWidget *parent = 0);

    void InitUI();

protected:
    void resizeEvent(QResizeEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    //初始化所有的动作
    void InitActions();

private slots:
    //最大化或还原
    void showMaxOrNormal();

    //激活标签页窗口
    void activeTab(int nTab);

    //刷新
    void Refresh();

private:
    QWidget *m_wgtCentral;//中央主窗体
    QGridLayout *m_mainLayout;//主窗体的布局
    QHBoxLayout *m_titleBarLayout;//标题栏的布局
    QLabel *m_lblTitle;
    QLabel *m_lblTitleZone;//用来显示标题区域
    QToolButton *m_btnMin;
    QToolButton *m_btnMax;
    QToolButton *m_btnClose;
    QTabWidget *m_tabMain;

    CNaviWidget *m_wndWorkFlow;//导航页面
    FormWebBase *m_wndWebMap1;
    FormWebBase *m_wndWebMap2;
    FormWebBase *m_wndWebMap3;
    FormWebBase *m_wndWebBar;

    QListWidget *customerList;
    QListWidget *paragraphsList;

    bool m_arrTabInit[5];//标签页面是否初始化

    bool m_bWndMaxmized;//窗口是否最大化

    QAction *m_pActionRefresh;//刷新动作

    QProcess m_proHttpServer;//httpServer进程

    CfgMgr m_cfgMgr;//配置信息
};

#endif // _MAINWND_H_20170927_
