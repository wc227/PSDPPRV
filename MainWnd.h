#ifndef _MAINWND_H_20170928_
#define _MAINWND_H_20170928_

#include <QMainWindow>
#include "WorkFlow/CWidgetWork.h"
#include "XBarChart.h"
#include "FileMgrErrInfo.h"
#include <QTimer>
#include <QSettings>

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

    ~MainWnd();

    //初始化界面
    void initUI();

    //创建可停靠窗口
    void createDockWnd();

    void createBarChart();

protected:
    //初始化所有的动作
    void createActions();

    virtual void keyPressEvent(QKeyEvent *event) ;

    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    //激活标签页窗口
    void activeTab(int nTab);

    //刷新
    void refresh();

    //收到命令
    void receiveCmd(QString sCmd);

    //读取和刷新错误信息
    void updateErrInfo();

    //显示或隐藏错误信息窗
    void showWndError();

    //启动或退出全屏显示
    void fullScreen();

private:    
    QAction *m_actionRefresh;//刷新动作
    QAction *m_actionShowWndError;//显示或隐藏错误信息窗口
    QAction *m_actionFullScreen;//全屏显示

    QWidget *m_wgtCentral;//中央主窗体
    QGridLayout *m_mainLayout;//主窗体的布局
    QHBoxLayout *m_titleBarLayout;//标题栏的布局

    QLabel *m_lblTitleZone;//用来显示标题区域
    QTabWidget *m_tabMain;

    CWidgetWork *m_wndWorkFlow;//导航页面
    FormWebBase *m_wndWebMap1;
    FormWebBase *m_wndWebMap2;
    FormWebBase *m_wndWebMap3;
    XBarChart *m_chartView;

    CXxwDockWidget *m_wndError;//错误信息停靠窗口
    QTextEdit *m_txtError;//显示错误信息的可编辑文本框

    QMap<int,bool> m_mapTabInit;//标签页面是否初始化

    QSettings *m_mySettings;//配置信息

    FileMgrErrInfo m_fmErr;//错误信息管理
    QTimer m_timerErrInfo;//检测错误预警文件的定时器
};

#endif // _MAINWND_H_20170928_
