#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "BorderlessWidget.h"
#include <ActiveQt/QAxWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <webaxwidget.h>

#include "CfgMgr.h"
#include "MyCompInfo.h"
#include "MyCompItem.h"
#include "MyPolylineItem.h"
#include "MyComDatMgr.h"
#include <QProcess>
#include <QAction>
#include <QLabel>
#include <QVector>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QTimer;
QT_END_NAMESPACE


class MainWidget : public BorderlessWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

    void InitUI();


protected:
    void paintEvent(QPaintEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    //Initialize mainwindow including m_view and m_scene
    void InitMain();

    //初始化所有的动作
    void InitActions();

    //删除图形，并释放指针，防止内存泄露
    void RemoveItem(QGraphicsItem *item);

    //link 2 computers by polyline
    //server is server
    //client is client
    bool Link2Comps(CMyCompItem *server, CMyCompItem *client);

private slots:
    //最大化或还原
    void showMaxOrNormal();

    //启动HttpServer
    void StartHttpServer();

    //停止HttpServer
    void StopHttpServer();

    //clear all computers
    void ClearCompData();

    //update all computers data
    void UpdateCompData();

    //update the lan
    void UpdateLan();

    //更新图
    void UpdateGraph();

    //set the timer step
    void SetTimeStep();

    //显示flag
    void ShowFlag(bool);

    //更新设置
    void UpdateSetting();

    //激活标签页窗口
    void activeTab(int nTab);

    //刷新
    void Refresh();

    //打开网址
    void loadUrl(int nTab);

    //打开网址
    void loadUrl();

private:

    QGridLayout *m_mainLayout;
    QHBoxLayout *m_titleBarLayout;//标题栏的布局
    QLabel *m_lblTitle;
    QToolButton *m_btnMin;
    QToolButton *m_btnMax;
    QToolButton *m_btnClose;
    QTabWidget *m_tabWidget;
    QGraphicsView *m_viewLan;
    QGraphicsScene *m_scene;
    WebAxWidget *m_webWidget1;
    WebAxWidget *m_webWidget2;
    WebAxWidget *m_webWidget3;
    WebAxWidget *m_webWidget4;
    WebAxWidget *m_webWidget5;

    QVector<bool> m_vecTabInit;//标签页面是否初始化

    QLabel *m_lblTitleZone;//用来显示标题区域

    bool m_bWndMaxmized;//窗口是否最大化

    QAction *m_pActionRefresh;//刷新动作

    CfgMgr m_cfgMgr;//配置信息

    ////////////////////////////////////
    //局域网状态图相关
    bool m_bUpdateRun;//局域网状态图更新中
    QTimer *m_timerUpdate;//更新定时器
    CMyComDatMgr m_myComDat;

    //
    QProcess m_proHttpServer;//httpServer进程

    //静态变量
    static int s_nGap;//边距
    static int s_nMax;//每行的最大个数
    static int s_nTimeUpdate;//刷新时间间隔,单位：秒
};

#endif // MAINWIDGET_H
