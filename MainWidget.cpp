﻿#include "MainWidget.h"
#include "DlgSetting.h"
#include <QInputDialog>
#include <QTextStream>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QTabWidget>
#include <QAction>
#include <QTimer>

#include "FormLanView.h"
#include "FormWebMap.h"

MainWidget::MainWidget(QWidget *parent)
    :BorderlessWidget(parent)
{
    setObjectName(QStringLiteral("MainWidget"));
    resize(1000, 720);

    setWindowState(Qt::WindowMaximized);//最大化显示
    m_bWndMaxmized = true;

    QString sFileCfg = QApplication::applicationDirPath() + "/PSDPPRV.xml";
    m_cfgMgr.setFileName(sFileCfg);

    for(int i = 0 ; i < 6; ++i)
        m_arrTabInit[i] = false;

    InitActions();
    InitUI();

//    QTimer::singleShot(0, m_wndLanView, SLOT(UpdateLan()));//界面显示后立即更新一次

//    QTimer::singleShot(0, this, SLOT(StartHttpServer()));
    //    connect(&m_proHttpServer,SIGNAL(started()),this,SLOT(loadUrl()));
//    QTimer::singleShot(1000, this, SLOT(loadUrl()));
}

//初始化所有的动作
void MainWidget::InitActions()
{
    m_pActionRefresh = new QAction(this);
    m_pActionRefresh->setShortcut(QKeySequence::Refresh);
    connect(m_pActionRefresh, &QAction::triggered, this, &MainWidget::Refresh);
    addAction(m_pActionRefresh);
}

void MainWidget::InitUI()
{
    m_lblTitle = new QLabel();
    m_lblTitle->setObjectName(QStringLiteral("m_lblTitle"));
    //    m_lblTitle->setText(QStringLiteral("局域网状态监视图"));//设置标题

    m_btnMin = new QToolButton();
    m_btnMin->setObjectName(QStringLiteral("m_btnMin"));
    m_btnMin->setToolTip(QStringLiteral("最小化"));
    m_btnMin->setMinimumSize(QSize(27, 22));
    m_btnMin->setMaximumSize(QSize(27, 22));
    m_btnMin->setStyleSheet(QLatin1String("#m_btnMin{\n"
"border-image: url(:/sysButton/min);\n"
"}\n"
"#m_btnMin:hover:!pressed{\n"
"border-image: url(:/sysButton/min_hover);\n"
"}\n"
"#m_btnMin:pressed{\n"
"border-image: url(:/sysButton/min_pressed);\n"
"}"));


    m_btnMax = new QToolButton();
    m_btnMax->setObjectName(QStringLiteral("m_btnMax"));
    m_btnMax->setToolTip(QStringLiteral("还原"));
    m_btnMax->setMinimumSize(QSize(27, 22));
    m_btnMax->setMaximumSize(QSize(27, 22));
    m_btnMax->setStyleSheet(QLatin1String("#m_btnMax{\n"
"border-image: url(:/sysButton/max);\n"
"}\n"
"#m_btnMax:hover:!pressed{\n"
"border-image: url(:/sysButton/max_hover);\n"
"}\n"
"#m_btnMax:pressed{\n"
"border-image: url(:/sysButton/max_pressed);\n"
"}"));


    m_btnClose = new QToolButton();
    m_btnClose->setObjectName(QStringLiteral("m_btnClose"));
    m_btnClose->setToolTip(QStringLiteral("关闭"));
    m_btnClose->setMinimumSize(QSize(27, 22));
    m_btnClose->setMaximumSize(QSize(27, 22));
    m_btnClose->setStyleSheet(QLatin1String("#m_btnClose{\n"
"border-image: url(:/sysButton/close);\n"
"}\n"
"#m_btnClose:hover:!pressed{\n"
"border-image: url(:/sysButton/close_hover);\n"
"}\n"
"#m_btnClose:pressed{\n"
"border-image: url(:/sysButton/close_pressed);\n"
"}"));

    //最大化，最小化和关闭按钮
    connect(m_btnMin, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(m_btnMax, SIGNAL(clicked()), this, SLOT(showMaxOrNormal()));
    connect(m_btnClose, SIGNAL(clicked()), this, SLOT(close()));

    m_titleBarLayout = new QHBoxLayout();
    m_titleBarLayout->setSpacing(1);
    m_titleBarLayout->setObjectName(QStringLiteral("m_titleBarLayout"));
    m_titleBarLayout->addWidget(m_lblTitle);
    m_titleBarLayout->addWidget(m_btnMin);
    m_titleBarLayout->addWidget(m_btnMax);
    m_titleBarLayout->addWidget(m_btnClose);

    m_tabWidget = new QTabWidget();
    m_tabWidget->setObjectName(QStringLiteral("m_tabWidget"));
    m_tabWidget->setIconSize(QSize(24,24));
    m_tabWidget->setStyleSheet("QTabWidget::tab-bar{left: 530px;}");//距离左侧530px,正好给m_pAfterTabLabel留有余地

    m_wndLanView = new FormLanView;
    m_tabWidget->addTab(m_wndLanView/*,QIcon(":/toolWidget/tiJian")*/,QStringLiteral("局域网工作状态"));

    m_wndWebMap1 = new FormWebMap();
    m_tabWidget->addTab(m_wndWebMap1/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("预想故障分布图"));
    m_wndWebMap2 = new FormWebMap();
    m_tabWidget->addTab(m_wndWebMap2/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("正常潮流安全分析图"));
    m_wndWebMap3 = new FormWebMap();
    m_tabWidget->addTab(m_wndWebMap3/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("匹配项全景感知分析关联图"));
    m_wndWebMap4 = new FormWebMap();
    m_tabWidget->addTab(m_wndWebMap4/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("录波站点分布图"));
    m_wndWebMap5 = new FormWebMap();
    m_tabWidget->addTab(m_wndWebMap5/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("潮流状态比较图"));

    connect(m_tabWidget,SIGNAL(currentChanged(int)), this, SLOT(activeTab(int)));
    m_tabWidget->setCurrentIndex(0);

//    m_arrTabInit[0] = true;

    m_mainLayout = new QGridLayout();
    m_mainLayout->setSpacing(0);
    m_mainLayout->setObjectName(QStringLiteral("m_mainLayout"));
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addLayout(m_titleBarLayout, 0, 0, 1, 1);
    m_mainLayout->addWidget(m_tabWidget, 1, 0, 1, 1);

    setLayout(m_mainLayout);

    m_lblTitleZone = new QLabel(/*"/h2>",*/this);
    m_lblTitleZone->move(10,0);//移动到界面左上角（10，10）的位置，更好看一些
    m_lblTitleZone->raise();//移动到界面的上层，以免被其他东西遮挡
    m_lblTitleZone->setObjectName("m_lblTitleZone");
    m_lblTitleZone->resize(500,50);//该大小与实际的图title保持同样的长宽比，否则会变形

    QString sImgTitle("");
    m_cfgMgr.getValue("img_title",sImgTitle);
    sImgTitle = QApplication::applicationDirPath() + "/" + sImgTitle;
    if(QFile::exists(sImgTitle))
        m_lblTitleZone->setStyleSheet("border-image: url("+ sImgTitle + ");");//有背景图就使用背景图代替标题
    else
        m_lblTitleZone->setText(QStringLiteral("<h2><font size=16 color=white >BPA-MAP</font>"));//没有背景图就使用字来表示
}


//激活标签页窗口
void MainWidget::activeTab(int nTab)
{
    if(nTab < 0 || nTab >= m_tabWidget->count())
        return;

    //如果已经初始化，就不用再初始化
    if(m_arrTabInit[nTab])
        return;
    else //否则，就刷新
    {
        Refresh();
        m_arrTabInit[nTab] = true;
    }
}

//刷新
void MainWidget::Refresh()
{
    if(0 == m_tabWidget->currentIndex() && m_wndLanView)
    {
        QString sFile("");
        m_cfgMgr.getValue("file_lan",sFile);
        m_wndLanView->SetFileCom(sFile);
        m_wndLanView->UpdateLan();
    }
    else if(1 == m_tabWidget->currentIndex() && m_wndWebMap1)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map",sUrl);
		//实际上，此处sUrl还应该带上后缀参数，用来控制web页面展示不同的内容，目前郅治没有实现，等待下一步实现。
        m_wndWebMap1->loadUrl(sUrl);
    }
    else if(2 == m_tabWidget->currentIndex() && m_wndWebMap2)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map",sUrl);
		//实际上，此处sUrl还应该带上后缀参数，用来控制web页面展示不同的内容，目前郅治没有实现，等待下一步实现。
        m_wndWebMap2->loadUrl(sUrl);
    }
    else if(3 == m_tabWidget->currentIndex() && m_wndWebMap3)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map",sUrl);
		//实际上，此处sUrl还应该带上后缀参数，用来控制web页面展示不同的内容，目前郅治没有实现，等待下一步实现。
        m_wndWebMap3->loadUrl(sUrl);
    }
    else if(4 == m_tabWidget->currentIndex() && m_wndWebMap4)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map",sUrl);
		//实际上，此处sUrl还应该带上后缀参数，用来控制web页面展示不同的内容，目前郅治没有实现，等待下一步实现。
        m_wndWebMap4->loadUrl(sUrl);
    }
    else if(5 == m_tabWidget->currentIndex() && m_wndWebMap5)
    {
        QString sUrl("");
        m_cfgMgr.getValue("url_map",sUrl);
		//实际上，此处sUrl还应该带上后缀参数，用来控制web页面展示不同的内容，目前郅治没有实现，等待下一步实现。
        m_wndWebMap5->loadUrl(sUrl);
    }
}


//最大化或还原
void MainWidget::showMaxOrNormal()
{
    if(m_bWndMaxmized)
    {
        showNormal();
//        m_btnMax->setStyleSheet("border-image: url(:/sysButton/max);"
//                                          "border-image: url(:/sysButton/max_hover);"
//                                          "border-image: url(:/sysButton/max_pressed);}");
        m_btnMax->setToolTip(QStringLiteral("最大化"));
    }
    else
    {
        showMaximized();
//        m_btnMax->setStyleSheet("border-image: url(:/sysButton/normal);"
//                                          "border-image: url(:/sysButton/normal_hover);"
//                                          "border-image: url(:/sysButton/normal_pressed);}");
        m_btnMax->setToolTip(QStringLiteral("还原"));
    }
    m_bWndMaxmized = !m_bWndMaxmized;
}


//启动HttpServer
void MainWidget::StartHttpServer()
{
//    QString sExe = QApplication::applicationDirPath() + "/HttpServer.exe";
    QString sExe = "D:/DKY/04_OtherPeople/ZhiZhi/Release/HttpServer.exe";

    m_proHttpServer.setProgram("cmd.exe");
    m_proHttpServer.setArguments(QStringList() << sExe);
    m_proHttpServer.start();
    //    QProcess::startDetached(sExe);
}


//停止HttpServer
void MainWidget::StopHttpServer()
{
    m_proHttpServer.close();
}


void MainWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QString skin_name("");
    m_cfgMgr.getValue("img_bg",skin_name);
    skin_name = QApplication::applicationDirPath() + "/" + skin_name;
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), QPixmap(skin_name));
//    int height = 65;
//    painter.drawRect(QRect(0, height, this->width(), this->height()-height));
    painter.restore();
}

void MainWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    showMaxOrNormal();//双击可以最大化或还原大小
}
