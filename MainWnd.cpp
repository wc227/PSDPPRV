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
#include <QDockWidget>
//#include <QTextEdit>
#include <QListWidget>

MainWnd::MainWnd(QWidget *parent)
    :BorderlessMainWnd(parent)
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

    InitActions();
    InitUI();
}

//初始化所有的动作
void MainWnd::InitActions()
{
    m_pActionRefresh = new QAction(this);
    m_pActionRefresh->setShortcut(QKeySequence::Refresh);
    connect(m_pActionRefresh, &QAction::triggered, this, &MainWnd::Refresh);
    addAction(m_pActionRefresh);
}

void MainWnd::InitUI()
{
    m_wgtCentral = new QWidget(this);
    m_wgtCentral->setObjectName(QStringLiteral("m_wgtCentral"));
    //主窗口已经设置了背景，此处就不用重复设置了
//    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/newbg);}"));

    m_lblTitle = new QLabel(m_wgtCentral);
    m_lblTitle->setObjectName(QStringLiteral("m_lblTitle"));

    m_btnMin = new QToolButton(m_wgtCentral);
    m_btnMin->setObjectName(QStringLiteral("m_btnMin"));
    m_btnMin->setToolTip(QStringLiteral("最小化"));
    m_btnMin->setMinimumSize(QSize(27, 22));
    m_btnMin->setMaximumSize(QSize(27, 22));
    m_btnMin->setStyleSheet(QStringLiteral("#m_btnMin{border-image: url(:/sysButton/min);}\n"
                                          "#m_btnMin:hover:!pressed{border-image: url(:/sysButton/min_hover);}\n"
                                          "#m_btnMin:pressed{border-image: url(:/sysButton/min_pressed);}"));

    m_btnMax = new QToolButton(m_wgtCentral);
    m_btnMax->setObjectName(QStringLiteral("m_btnMax"));
    m_btnMax->setToolTip(QStringLiteral("还原"));
    m_btnMax->setMinimumSize(QSize(27, 22));
    m_btnMax->setMaximumSize(QSize(27, 22));
    m_btnMax->setStyleSheet(QStringLiteral("#m_btnMax{border-image: url(:/sysButton/max);}\n"
                                          "#m_btnMax:hover:!pressed{border-image: url(:/sysButton/max_hover);}\n"
                                          "#m_btnMax:pressed{border-image: url(:/sysButton/max_pressed);}"));

    m_btnClose = new QToolButton(m_wgtCentral);
    m_btnClose->setObjectName(QStringLiteral("m_btnClose"));
    m_btnClose->setToolTip(QStringLiteral("关闭"));
    m_btnClose->setMinimumSize(QSize(27, 22));
    m_btnClose->setMaximumSize(QSize(27, 22));
    m_btnClose->setStyleSheet(QStringLiteral("#m_btnClose{border-image: url(:/sysButton/close);}\n"
                                            "#m_btnClose:hover:!pressed{border-image: url(:/sysButton/close_hover);}\n"
                                            "#m_btnClose:pressed{border-image: url(:/sysButton/close_pressed);}"));

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

    connect(m_tabMain,SIGNAL(currentChanged(int)), this, SLOT(activeTab(int)));
    m_tabMain->setCurrentIndex(0);

    m_mainLayout = new QGridLayout(m_wgtCentral);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setObjectName(QStringLiteral("m_mainLayout"));
    m_mainLayout->setContentsMargins(1, 1, 1, 1);
    m_mainLayout->addLayout(m_titleBarLayout, 0, 0, 1, 1);
    m_mainLayout->addWidget(m_tabMain, 1, 0, 1, 1);

    m_wgtCentral->setLayout(m_mainLayout);
//    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/bg);}"));
    setCentralWidget(m_wgtCentral);

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

    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
            << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
            << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
            << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
            << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
            << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Paragraphs"), this);
    paragraphsList = new QListWidget(dock);
    paragraphsList->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "We have dispatched those items that were in stock. The "
               "rest of your order will be dispatched once all the "
               "remaining items have arrived at our warehouse. No "
               "additional shipping charges will be made."
            << "You made a small overpayment (less than $5) which we "
               "will keep on account for you, or return at your request."
            << "You made a small underpayment (less than $1), but we have "
               "sent your order anyway. We'll add this underpayment to "
               "your next bill."
            << "Unfortunately you did not send enough money. Please remit "
               "an additional $. Your order will be dispatched as soon as "
               "the complete amount has been received."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    dock->setWidget(paragraphsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}


//激活标签页窗口
void MainWnd::activeTab(int nTab)
{
    if(nTab < 0 || nTab >= m_tabMain->count())
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
void MainWnd::Refresh()
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


//最大化或还原
void MainWnd::showMaxOrNormal()
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

void MainWnd::resizeEvent(QResizeEvent *event)
{
    setAreaMovable(m_titleBarLayout->geometry());
}

void MainWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);

    if(m_titleBarLayout->geometry().contains(event->pos()))
        showMaxOrNormal();//双击可以最大化或还原大小
}
