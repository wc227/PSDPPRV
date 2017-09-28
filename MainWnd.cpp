#include "MainWnd.h"
#include "FormWeb.h"
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QTabWidget>
#include <QAction>
#include <QTimer>
#include <QTextEdit>
#include "XxwDockWidget.h"

MainWnd::MainWnd(QWidget *parent)
    :QMainWindow(parent)
{
    setObjectName(QStringLiteral("MainWnd"));
    setStyleSheet(QStringLiteral("#MainWnd{border-image: url(:/skin/newbg);}"));//设置背景图
    resize(1000, 720);

    setWindowState(Qt::WindowMaximized);//最大化显示
    m_bWndMaxmized = true;

    for(int i = 0 ; i < 5; ++i)
        m_arrTabInit[i] = false;

    QString sFileCfg = QApplication::applicationDirPath() + "/PSDPPRV.xml";
    m_cfgMgr.setFileName(sFileCfg);

    initActions();
    initUI();
}

//初始化所有的动作
void MainWnd::initActions()
{
    m_actionRefresh = new QAction(this);
    m_actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(m_actionRefresh, &QAction::triggered, this, &MainWnd::refresh);
    addAction(m_actionRefresh);
}

void MainWnd::initUI()
{
    m_wgtCentral = new QWidget(this);
    m_wgtCentral->setObjectName(QStringLiteral("m_wgtCentral"));
    //主窗口已经设置了背景，此处就不用重复设置了
//    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/newbg);}"));
    setCentralWidget(m_wgtCentral);

    m_lblTitle = new QLabel(m_wgtCentral);
    m_lblTitle->setObjectName(QStringLiteral("m_lblTitle"));
    m_lblTitle->setMinimumHeight(20);
    m_lblTitle->setMaximumHeight(25);

    m_titleBarLayout = new QHBoxLayout();
    m_titleBarLayout->setSpacing(1);
    m_titleBarLayout->setObjectName(QStringLiteral("m_titleBarLayout"));
    m_titleBarLayout->addWidget(m_lblTitle);

    m_tabMain = new QTabWidget(m_wgtCentral);
    m_tabMain->setObjectName(QStringLiteral("m_tabMain"));
    m_tabMain->setIconSize(QSize(24,24));
    //距离左侧450px,正好给m_pAfterTabLabel留有余地，并设置背景图片
    m_tabMain->setStyleSheet(QStringLiteral("#m_tabMain::tab-bar{left: 450px;}\n"
                                            "#m_tabMain::pane{border-image: url(:/skin/bg);}"));

    m_wndWorkFlow = new CNaviWidget();
    QString sFileLan("");
    m_cfgMgr.getValue("file_lan",sFileLan);
    m_wndWorkFlow->setFileCfg(sFileLan);
    m_tabMain->addTab(m_wndWorkFlow/*,QIcon(":/toolWidget/tiJian")*/,QStringLiteral("导航图"));

    m_wndWebMap1 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap1/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("预想故障分布图"));
    m_wndWebMap2 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap2/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("正常潮流安全分析图"));
    m_wndWebMap3 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap3/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("匹配项全景感知分析关联图"));
    m_tabMain->setCurrentIndex(0);

    m_mainLayout = new QGridLayout(m_wgtCentral);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setObjectName(QStringLiteral("m_mainLayout"));
    m_mainLayout->setContentsMargins(1, 1, 1, 1);
    m_mainLayout->addLayout(m_titleBarLayout, 0, 0, 1, 1);
    m_mainLayout->addWidget(m_tabMain, 1, 0, 1, 1);

//    m_wgtCentral->setLayout(m_mainLayout);
//    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/bg);}"));
//    setCentralWidget(m_wgtCentral);

    m_lblTitleZone = new QLabel(this);
    m_lblTitleZone->move(10,0);//移动到界面左上角（10，10）的位置，更好看一些
    m_lblTitleZone->raise();//移动到界面的上层，以免被其他东西遮挡
    m_lblTitleZone->setObjectName(QStringLiteral("m_lblTitleZone"));
    m_lblTitleZone->resize(425,50);//该大小与实际的图title保持同样的长宽比，否则会变形
    QString sImgTitle("");
    m_cfgMgr.getValue("img_title",sImgTitle);
    sImgTitle = QApplication::applicationDirPath() + "/" + sImgTitle;
    if(QFile::exists(sImgTitle))
        m_lblTitleZone->setStyleSheet("#m_lblTitleZone{border-image: url("+ sImgTitle + ");}");//有背景图就使用背景图代替标题
    else
        m_lblTitleZone->setText(QStringLiteral("<h2><font size=16 color=white >BPA-MAP</font>"));//没有背景图就使用字来表示

    createDockWnd();

    connect(m_tabMain,SIGNAL(currentChanged(int)), this, SLOT(activeTab(int)));
}


//创建可停靠窗口
void MainWnd::createDockWnd()
{
    dock1 = new CXxwDockWidget(tr("窗口1"), this);
    dock1->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock1->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方/*
    txt1 = new QTextEdit(dock1);
    txt1->setObjectName(QStringLiteral("txt1"));
    txt1->setStyleSheet("#txt1{border:0px; }");
    txt1->setText("111");
    dock1->setWidget(txt1);

    dock2 = new CXxwDockWidget(tr("窗口2"), this);
    dock2->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock2->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt2 = new QTextEdit(dock2);
    txt2->setObjectName(QStringLiteral("txt2"));
    txt2->setStyleSheet("#txt2{border:0px; }");
    txt2->setText("222");
    dock2->setWidget(txt2);

    dock3 = new CXxwDockWidget(tr("窗口3"), this);
    dock3->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock3->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt3 = new QTextEdit(dock3);
    txt3->setObjectName(QStringLiteral("txt3"));
    txt3->setStyleSheet("#txt3{border:0px; }");
    txt3->setText("333");
    dock3->setWidget(txt3);

    dock4 = new CXxwDockWidget(tr("窗口4"), this);
    dock4->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock4->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt4 = new QTextEdit(dock4);
    txt4->setObjectName(QStringLiteral("txt4"));
    txt4->setStyleSheet("#txt4{border:0px; }");
    txt4->setText("444");
    dock4->setWidget(txt4);

//    dock1->showTitleBar(false);
//    dock2->showTitleBar(false);
//    dock3->showTitleBar(false);
//    dock4->showTitleBar(false);

    dock1->setMinimumWidth(500);
    dock2->setMinimumWidth(500);
    dock3->setMinimumWidth(500);
    dock4->setMinimumWidth(500);

    addDockWidget(Qt::BottomDockWidgetArea, dock1);//在底部添加dock1
//    splitDockWidget(dock1,dock3,Qt::Horizontal);//在dock1右侧水平添加dock3,和dock1水平并列
    tabifyDockWidget(dock1,dock2);//添加dock2和dock1合并成tab

//    addDockWidget(Qt::BottomDockWidgetArea, dock3);//在右侧添加dock3
    tabifyDockWidget(dock2,dock3);//添加dock4和dock4合并成tab
    tabifyDockWidget(dock3,dock4);//添加dock4和dock4合并成tab
    dock1->raise();
}

//激活标签页窗口
void MainWnd::activeTab(int nTab)
{
    if(nTab < 0 || nTab >= m_tabMain->count())
        return;

    //显示或隐藏子窗口
    showDockWnds();

    //如果已经初始化，就不用再初始化
    if(m_arrTabInit[nTab])
        return;
    else //否则，就刷新
    {
        refresh();
        m_arrTabInit[nTab] = true;
    }
}

//刷新
void MainWnd::refresh()
{
    if(0 == m_tabMain->currentIndex() && m_wndWorkFlow)
    {
//        QString sFile("");
//        m_cfgMgr.getValue("file_lan",sFile);
//        m_wndLanView->SetFileCom(sFile);
//        m_wndLanView->UpdateLan();
    }
    else if(1 == m_tabMain->currentIndex() && m_wndWebMap1)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map1",sUrl);
        m_wndWebMap1->loadUrl(sUrl);
    }
    else if(2 == m_tabMain->currentIndex() && m_wndWebMap2)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map2",sUrl);
        m_wndWebMap2->loadUrl(sUrl);
    }
    else if(3 == m_tabMain->currentIndex() && m_wndWebMap3)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map3",sUrl);
        m_wndWebMap3->loadUrl(sUrl);
    }
    else if(4 == m_tabMain->currentIndex() && m_wndWebBar)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_bar",sUrl);
        m_wndWebBar->loadUrl(sUrl);
    }
}


//显示隐藏可停靠窗口
void MainWnd::showDockWnds()
{
    if(0 == m_tabMain->currentIndex() && m_wndWorkFlow)
    {
        dock1->show();
        dock2->show();
        dock3->show();
        dock4->show();
    }
    else
    {
        dock1->hide();
        dock2->hide();
        dock3->hide();
        dock4->hide();
    }
}
