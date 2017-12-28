#include "MainWnd.h"
#include "FormWeb.h"
#include "XxwDockWidget.h"
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QTabWidget>
#include <QAction>
#include <QTextEdit>
#include <QProcess>


MainWnd::MainWnd(QWidget *parent)
    :QMainWindow(parent)
{
    setObjectName(QStringLiteral("MainWnd"));
    setWindowTitle(QStringLiteral("全景感知录波系统"));

    setMinimumSize(1200,900);

    //最大化显示
    setWindowState(Qt::WindowMaximized);

    //全屏显示
//    setWindowState(Qt::WindowFullScreen);

    m_chartView = 0;

    //初始化配置文件
    QString sFileCfg = QApplication::applicationDirPath() + "/PSDPPRV.ini";
    m_mySettings = new QSettings(sFileCfg,QSettings::IniFormat,this);
    m_mySettings->setIniCodec("UTF-8");

    createActions();
    initUI();
}

MainWnd::~MainWnd()
{
    if(m_mySettings)
        delete m_mySettings;
}

//初始化所有的动作
void MainWnd::createActions()
{
    m_actionRefresh = new QAction(this);
    m_actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(m_actionRefresh, &QAction::triggered, this, &MainWnd::refresh);
    addAction(m_actionRefresh);

    m_actionShowWndError = new QAction(this);
    m_actionShowWndError->setShortcut(QKeySequence(Qt::Key_F1));
    connect(m_actionShowWndError, &QAction::triggered, this, &MainWnd::showWndError);
    addAction(m_actionShowWndError);

    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setShortcut(QKeySequence(Qt::Key_F12));
    connect(m_actionFullScreen, &QAction::triggered, this, &MainWnd::fullScreen);
    addAction(m_actionFullScreen);
}

void MainWnd::initUI()
{
    m_tabMain = new QTabWidget(this);
    m_tabMain->setObjectName(QStringLiteral("m_tabMain"));
    m_tabMain->setIconSize(QSize(24,24));
    //距离左侧450px,正好给m_pAfterTabLabel留有余地，并设置背景图片
//    m_tabMain->setStyleSheet(QStringLiteral("#m_tabMain::pane{border-image: url(:/StateGreen/back_main.png);}\n"//背景图
//                                            "#m_tabMain::tab-bar{left: 500;}\n"//标签页左侧距离
//                                            "QTabBar::tab{height:50px;}"));//tab标签页高度
//    m_tabMain->setStyleSheet(QStringLiteral("#m_tabMain::tab-bar{left: 500;}\n"//标签页左侧距离
//                                            "QTabBar::tab{height:50px;}"));//tab标签页高度

    m_wndWorkFlow = new CWidgetWork();
    connect(m_wndWorkFlow,SIGNAL(sendCmd(QString)),this,SLOT(receiveCmd(QString)));
    QString sFileLan = m_mySettings->value("PSDPPRV/file_lan").toString();
    m_wndWorkFlow->setFileCfg(sFileLan);
    m_tabMain->addTab(m_wndWorkFlow/*,QIcon(":/toolWidget/tiJian")*/,QStringLiteral("系统运行状态"));

    m_wndWebMap1 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap1/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("事故潮流分布"));
    m_wndWebMap2 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap2/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("正常潮流分布"));
    m_wndWebMap3 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap3/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("录波装置分布"));

    createBarChart();
    m_tabMain->addTab(m_chartView/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("场站数据时序状态"));

    m_tabMain->setCurrentIndex(0);
    setCentralWidget(m_tabMain);
    connect(m_tabMain,SIGNAL(currentChanged(int)), this, SLOT(activeTab(int)));

    for(int i = 0; i < m_tabMain->count(); ++i)
    {
        m_mapTabInit.insert(i,false);
    }

    m_lblTitleZone = new QLabel(this);
//    m_lblTitleZone->move(5,5);//移动到界面左上角（10，10）的位置，更好看一些
    m_lblTitleZone->raise();//移动到界面的上层，以免被其他东西遮挡
    m_lblTitleZone->setObjectName(QStringLiteral("m_lblTitleZone"));

    m_lblTitleZone->resize(250,50);//该大小与实际的图title保持同样的长宽比，否则会变形
//    m_lblTitleZone->setStyleSheet("#m_lblTitleZone{border-image: url(:/StateGreen/logo.png);}");

    createDockWnd();

    connect(&m_timerErrInfo,&QTimer::timeout,this,&MainWnd::updateErrInfo);
    m_timerErrInfo.start(2000);

    //新建一个QLCDNumber对象
    m_lcdTime = new QLCDNumber(this);
    m_lcdTime->setObjectName("m_lcdTime");
//    m_lcdTime->resize(200,50);
    //设置晶体管控件QLCDNumber能显示的位数
    m_lcdTime->setDigitCount(10);
    //设置显示的模式为十进制
    m_lcdTime->setMode(QLCDNumber::Dec);
    //设置显示方式
    m_lcdTime->setSegmentStyle(QLCDNumber::Flat);

//    m_lcdTime->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    m_lcdTime->display(QTime::currentTime().toString("hh:mm:ss"));

    connect(&m_timerUpdateTime,&QTimer::timeout,this,&MainWnd::updateTime);
    //启动定时器
    m_timerUpdateTime.start(1000);
}


//创建可停靠窗口
void MainWnd::createDockWnd()
{
    m_wndError = new CXxwDockWidget(tr("异常信息"));
    m_wndError->setFeatures(QDockWidget::DockWidgetMovable);//能移动
    m_wndError->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);//停靠在主窗口下方/*
    m_wndError->setToolTip(QStringLiteral("按F1可显示或隐藏\"异常信息\"窗口"));

    m_txtError = new QTextEdit(m_wndError);
    m_txtError->setObjectName(QStringLiteral("Output1"));
    m_txtError->setStyleSheet("#m_txtError{border:0px; }");
    m_txtError->setText("111");
    m_txtError->setMinimumSize(10,10);//设置最小的宽度和高度

    m_wndError->setWidget(m_txtError);
//    m_wndError->showTitleBar(false);//不显示标题栏
    addDockWidget(Qt::BottomDockWidgetArea, m_wndError);//添加dock1

    //此函数的功能是把两个dock进行左右或上下并排布置，做成一个类似QSplit的功能
//    splitDockWidget(m_wndError,dock3,Qt::Horizontal);//在dock1右侧水平添加dock3,和dock1水平并列

    //此函数的功能是把多个dock变成一个tab形式的窗体
//    tabifyDockWidget(m_wndError,dock2);//添加dock2和dock1合并成tab

    QString sFileErr = m_mySettings->value("PSDPPRV/file_err").toString();
    m_fmErr.setFileName(sFileErr);
    if(m_txtError)
        m_txtError->setText(m_fmErr.getData());

    setDockNestingEnabled(true);
}


void MainWnd::createBarChart()
{
    if(m_chartView)
        delete m_chartView;
    m_chartView = new XBarChart();
    m_chartView->setTitle("场站数据时序状态图");
    QString sFileDidx = m_mySettings->value("PSDPPRV/file_bar").toString();
    m_chartView->setFileData(sFileDidx);
}


//激活标签页窗口
void MainWnd::activeTab(int nTab)
{
    if(nTab < 0 || nTab >= m_tabMain->count())
        return;

    //如果已经初始化，就不用再初始化
    bool bInit = m_mapTabInit.value(nTab,false);
    if(!bInit)
    {
        refresh();
        m_mapTabInit[nTab] = true;
    }

    if(m_wndError && nTab == 0)
        m_wndError->show();
    else if(m_wndError && nTab > 0)
        m_wndError->hide();
}

//刷新
void MainWnd::refresh()
{
    int nIndex = m_tabMain->currentIndex();
//    if(0 == nIndex && m_wndWorkFlow)
//    {
//        QString sFile("");
//        m_mySettings->value("file_lan",sFile);
//        m_wndLanView->SetFileCom(sFile);
//        m_wndLanView->UpdateLan();
//    }
//    else
    if(1 == nIndex && m_wndWebMap1)
    {
        QString sUrl = m_mySettings->value("PSDPPRV/url_map1").toString();
        m_wndWebMap1->loadUrl(sUrl);
    }
    else if(2 == nIndex && m_wndWebMap2)
    {
        QString sUrl = m_mySettings->value("PSDPPRV/url_map2").toString();
        m_wndWebMap2->loadUrl(sUrl);
    }
    else if(3 == nIndex && m_wndWebMap3)
    {
        QString sUrl = m_mySettings->value("PSDPPRV/url_map3").toString();
        m_wndWebMap3->loadUrl(sUrl);
    }
//    else if(4 == nIndex && m_chartView)
//    {
//    }
}


//收到命令
void MainWnd::receiveCmd(QString sCmd)
{
    sCmd = sCmd.trimmed();
    if(sCmd.isEmpty())
        return;

    QStringList lst = sCmd.split('=');
    if(2 == lst.count())
    {
        QString sKey = lst.at(0);
        sKey = sKey.trimmed().toLower();
        QString sValue = lst.at(1);
        if(sKey == "openfile")
            QProcess::execute("explorer " + sValue);//使用系统默认程序打开指定文件
        else if(sKey == "activetab")
        {
            //激活对应的tab页
            int iTab = sValue.toInt();
            if(iTab >=0 && iTab < m_tabMain->count()-1)
            {
                m_tabMain->setCurrentIndex(iTab);
            }
        }
    }
}

//读取错误信息
void MainWnd::updateErrInfo()
{
    if(m_fmErr.isDirty())
    {
        m_fmErr.openFile();
        if(m_txtError)
            m_txtError->setText(m_fmErr.getData());
    }
}

//显示或隐藏错误信息窗
void MainWnd::showWndError()
{
    if(m_wndError)
    {
        if(m_wndError->isHidden())
            m_wndError->show();
        else
            m_wndError->hide();
    }
}


//启动或退出全屏显示
void MainWnd::fullScreen()
{
    if(isFullScreen())
        showMaximized();
    else
        showFullScreen();
}


//更新时间
void MainWnd::updateTime()
{
    if(m_lcdTime)
    {
//        //获取系统当前时间
//        //设置晶体管控件QLCDNumber上显示的内容
        m_lcdTime->display(QTime::currentTime().toString("hh:mm:ss"));

//        m_lcdTime->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
}


void MainWnd::keyPressEvent(QKeyEvent *event)
{
    int keyid = event->key();
    switch (keyid)
    {
    case Qt::Key_Escape:
    {
        if(isFullScreen())
            showMaximized();//如果是全屏显示，按esc键退出全屏
    }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}


void MainWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QRect rc = m_lblTitleZone->geometry();
    if(rc.contains(pos))
    {
        m_tabMain->setCurrentIndex(0);//显示第一个标签页面
    }
    else
        QMainWindow::mouseDoubleClickEvent(event);
}

void MainWnd::resizeEvent(QResizeEvent *event)
{
    if(m_lcdTime)
    {
        QPoint pt = this->centralWidget()->geometry().topRight();
        m_lcdTime->setGeometry(pt.x()-150, pt.y(),150,50);
    }
}
