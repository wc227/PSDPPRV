#ifndef _MAINWND_H_20170928_
#define _MAINWND_H_20170928_

#include <QMainWindow>
#include "WorkFlow/CWidgetWork.h"
#include "CfgMgr.h"
#include <QProcess>
//#include "NXDockWidget/NXDockWidget.h"

#include <QChartGlobal>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QToolButton;
class QTabWidget;
class QAction;
class FormWebBase;
class QListWidget;
class CXxwDockWidget;
class QTextEdit;
class ThemeWidget;
QT_END_NAMESPACE

class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    MainWnd(QWidget *parent = 0);

    //初始化界面
    void initUI();

    //创建可停靠窗口
    void createDockWnd();

    void createBarChart();

protected:
    //初始化所有的动作
    void initActions();

private slots:
    //激活标签页窗口
    void activeTab(int nTab);

    //刷新
    void refresh();

    //显示隐藏可停靠窗口
    void showDockWnds();

private:    
    QAction *m_actionRefresh;//刷新动作

    QWidget *m_wgtCentral;//中央主窗体
    QGridLayout *m_mainLayout;//主窗体的布局
    QHBoxLayout *m_titleBarLayout;//标题栏的布局
    QLabel *m_lblTitle;
    QLabel *m_lblTitleZone;//用来显示标题区域
    QTabWidget *m_tabMain;

    CWidgetWork *m_wndWorkFlow;//导航页面
    FormWebBase *m_wndWebMap1;
    FormWebBase *m_wndWebMap2;
    FormWebBase *m_wndWebMap3;
//    ThemeWidget *m_wndCharts;
    QChartView *m_chartView;


    CXxwDockWidget *dock1;
    QTextEdit *txt1;

    CXxwDockWidget *dock2;
    QTextEdit *txt2;

    CXxwDockWidget *dock3;
    QTextEdit *txt3;

    CXxwDockWidget *dock4;
    QTextEdit *txt4;

    bool m_arrTabInit[5];//标签页面是否初始化

    bool m_bWndMaxmized;//窗口是否最大化

    CfgMgr m_cfgMgr;//配置信息
};

#endif // _MAINWND_H_20170928_
