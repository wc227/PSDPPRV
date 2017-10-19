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

//#include <QTreeWidget>
//#include <QListView>
//#include <QFileSystemModel>
//#include <QPushButton>
//#include <QRadioButton>
//#include <QCheckBox>
//#include <QFormLayout>
//#include <QSpinBox>
//#include <QLineEdit>

#include <QtCharts/QBarSet>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "CXBarChart.h"

MainWnd::MainWnd(QWidget *parent)
    :QMainWindow(parent)
{
    setObjectName(QStringLiteral("MainWnd"));
    setStyleSheet(QStringLiteral("#MainWnd{border-image: url(:/skin/newbg);}"));//设置背景图
    resize(1000, 720);

    setWindowState(Qt::WindowMaximized);//最大化显示
    m_bWndMaxmized = true;

    m_chartView = 0;

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
//    m_wgtCentral = new QWidget(this);
//    m_wgtCentral->setObjectName(QStringLiteral("m_wgtCentral"));
//    //主窗口已经设置了背景，此处就不用重复设置了
////    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/newbg);}"));
//    setCentralWidget(m_wgtCentral);

//    m_lblTitle = new QLabel(m_wgtCentral);
//    m_lblTitle->setObjectName(QStringLiteral("m_lblTitle"));
//    m_lblTitle->setMinimumHeight(20);
//    m_lblTitle->setMaximumHeight(25);

//    m_titleBarLayout = new QHBoxLayout();
//    m_titleBarLayout->setSpacing(1);
//    m_titleBarLayout->setObjectName(QStringLiteral("m_titleBarLayout"));
//    m_titleBarLayout->addWidget(m_lblTitle);

//    m_tabMain = new QTabWidget(m_wgtCentral);
    m_tabMain = new QTabWidget(this);
    m_tabMain->setObjectName(QStringLiteral("m_tabMain"));
    m_tabMain->setIconSize(QSize(24,24));
    //距离左侧450px,正好给m_pAfterTabLabel留有余地，并设置背景图片
    m_tabMain->setStyleSheet(QStringLiteral("#m_tabMain::pane{border-image: url(:/skin/bg);}\n"//背景图
                                            "#m_tabMain::tab-bar{left: 450px;}\n"//标签页左侧距离
                                            "QTabBar::tab{height:50px;}"));//tab标签页高度

    m_wndWorkFlow = new CWidgetWork();
    QString sFileLan("");
    m_cfgMgr.getValue("file_lan",sFileLan);
    m_wndWorkFlow->setFileCfg(sFileLan);
    m_tabMain->addTab(m_wndWorkFlow/*,QIcon(":/toolWidget/tiJian")*/,QStringLiteral("系统运行图"));

    m_wndWebMap1 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap1/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("预想故障分布图"));
    m_wndWebMap2 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap2/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("正常潮流安全分析图"));
    m_wndWebMap3 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap3/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("匹配项全景感知分析关联图"));

//    m_wndCharts = new ThemeWidget();
//    m_tabMain->addTab(m_wndCharts/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("QtCharts示例"));

    createBarChart();
    if(m_chartView)
        m_tabMain->addTab(m_chartView/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("动态堆积柱状图"));

    m_tabMain->setCurrentIndex(0);

//    m_mainLayout = new QGridLayout(m_wgtCentral);
//    m_mainLayout->setSpacing(0);
//    m_mainLayout->setObjectName(QStringLiteral("m_mainLayout"));
//    m_mainLayout->setContentsMargins(1, 1, 1, 1);
//    m_mainLayout->addLayout(m_titleBarLayout, 0, 0, 1, 1);
//    m_mainLayout->addWidget(m_tabMain, 1, 0, 1, 1);

//    m_wgtCentral->setLayout(m_mainLayout);
//    m_wgtCentral->setStyleSheet(QStringLiteral("#m_wgtCentral{border-image: url(:/skin/bg);}"));
//    setCentralWidget(m_wgtCentral);
    setCentralWidget(m_tabMain);

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
    dock1 = new CXxwDockWidget(tr("窗口1"));
    dock1->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock1->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方/*
    txt1 = new QTextEdit(dock1);
    txt1->setObjectName(QStringLiteral("txt1"));
    txt1->setStyleSheet("#txt1{border:0px; }");
    txt1->setText("111");
    dock1->setWidget(txt1);

    dock2 = new CXxwDockWidget(tr("窗口2"));
    dock2->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock2->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt2 = new QTextEdit(dock2);
    txt2->setObjectName(QStringLiteral("txt2"));
    txt2->setStyleSheet("#txt2{border:0px; }");
    txt2->setText("222");
    dock2->setWidget(txt2);

    dock3 = new CXxwDockWidget(tr("窗口3"));
    dock3->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock3->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt3 = new QTextEdit(dock3);
    txt3->setObjectName(QStringLiteral("txt3"));
    txt3->setStyleSheet("#txt3{border:0px; }");
    txt3->setText("333");
    dock3->setWidget(txt3);

    dock4 = new CXxwDockWidget(tr("窗口4"));
    dock4->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    dock4->setAllowedAreas(Qt::BottomDockWidgetArea);//停靠在主窗口下方
    txt4 = new QTextEdit(dock4);
    txt4->setObjectName(QStringLiteral("txt4"));
    txt4->setStyleSheet("#txt4{border:0px; }");
    txt4->setText("444");
    dock4->setWidget(txt4);

    //不显示标题栏
//    dock1->showTitleBar(false);
//    dock2->showTitleBar(false);
//    dock3->showTitleBar(false);
//    dock4->showTitleBar(false);

    //设置最小的宽度和高度
    dock1->setMinimumWidth(500);
    dock2->setMinimumWidth(500);
    dock3->setMinimumWidth(500);
    dock4->setMinimumWidth(500);
    dock1->setMinimumHeight(5);
    dock2->setMinimumHeight(5);
    dock3->setMinimumHeight(5);
    dock4->setMinimumHeight(5);

    addDockWidget(Qt::BottomDockWidgetArea, dock1);//添加dock1
//    splitDockWidget(dock1,dock3,Qt::Horizontal);//在dock1右侧水平添加dock3,和dock1水平并列
    tabifyDockWidget(dock1,dock2);//添加dock2和dock1合并成tab
    dock1->raise();

    addDockWidget(Qt::BottomDockWidgetArea, dock3);//添加dock3
//    tabifyDockWidget(dock2,dock3);//添加dock3和dock2合并成tab
    tabifyDockWidget(dock3,dock4);//添加dock4和dock3合并成tab
    dock3->raise();


//！使用NXDockWidget实现可停靠窗口，失败？
//    NXDockWidget* fileDockWidget = new NXDockWidget("File Explorer");
//    addDockWidget(Qt::LeftDockWidgetArea, fileDockWidget);

//    QFileSystemModel* fsModel = new QFileSystemModel;
//    fsModel->setRootPath(QDir::currentPath());

//    QTreeView* fileTreeView = new QTreeView();
//    fileTreeView->setModel(fsModel);
//    fileTreeView->setRootIndex(fsModel->index(QDir::currentPath()));

//    fileDockWidget->setWidget(fileTreeView);

//    //------------------------------------------------------------------------
//    // Create the dockwidget that display list names of some Qt classes
//    //------------------------------------------------------------------------

//    NXDockWidget* strListDockWidget = new NXDockWidget("Class View");
//    addDockWidget(Qt::RightDockWidgetArea, strListDockWidget);

//    QStringListModel* slModel = new QStringListModel();
//    QStringList classList;
//    classList << "QWidget" << "QPushButton" << "QImage" << "QCheckBox" << "QWindow" << "QTextEdit" << "QScrollBar" << "QPoint";
//    slModel->setStringList(classList);

//    QListView* strView = new QListView();
//    strView->setModel(slModel);

//    strListDockWidget->setWidget(strView);

//    //------------------------------------------------------------------------
//    // Create the dockwidget that display list of some standard widgets
//    //------------------------------------------------------------------------

//    NXDockWidget* toolBoxDockWidget = new NXDockWidget("Tool Box");

//    QWidget* toolBox = new QWidget();
//    QFormLayout* formLayout = new QFormLayout();
//    formLayout->setLabelAlignment(Qt::AlignRight);
//    toolBox->setLayout(formLayout);

//    formLayout->addRow("Push Button", new QPushButton("OK"));
//    formLayout->addRow("Tool Button", new QToolButton());
//    formLayout->addRow("Radio Button", new QRadioButton());
//    formLayout->addRow("Check Box", new QCheckBox());
//    formLayout->addRow("Spin Box", new QSpinBox());
//    formLayout->addRow("Line Edit", new QLineEdit());
//    formLayout->addRow("Combo Box", new QComboBox());

//    toolBoxDockWidget->setWidget(toolBox);
//    addDockWidget(Qt::RightDockWidgetArea, toolBoxDockWidget);

//    //------------------------------------------------------------------------
//    // Create the dockwidget that display a text edit widget
//    //------------------------------------------------------------------------

//    NXDockWidget* outputDockWidget = new NXDockWidget("Output");
//    addDockWidget(Qt::BottomDockWidgetArea, outputDockWidget);

//    QTextEdit* textEdit = new QTextEdit();
//    textEdit->setReadOnly(true);
//    textEdit->setText(R"(Output text)");

//    outputDockWidget->setWidget(textEdit);
}


void MainWnd::createBarChart()
{
    if(m_chartView)
        delete m_chartView;

    CXBarChart *chart = new CXBarChart();

    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
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
    else if(4 == m_tabMain->currentIndex() && m_chartView)
    {
    }
}


//显示隐藏可停靠窗口
void MainWnd::showDockWnds()
{
    if(0 == m_tabMain->currentIndex() && m_wndWorkFlow)
    {
        if(dock1)
            dock1->showNormal();
        if(dock2)
            dock2->showNormal();
        if(dock3)
            dock3->showNormal();
        if(dock4)
            dock4->showNormal();
    }
    else
    {
        if(dock1)
            dock1->hide();
        if(dock2)
            dock2->hide();
        if(dock3)
            dock3->hide();
        if(dock4)
            dock4->hide();
    }
}
