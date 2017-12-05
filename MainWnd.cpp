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
    setStyleSheet(QStringLiteral("#MainWnd{border-image: url(:/skin/newbg);}"));//设置背景图
    resize(1000, 720);

    setWindowState(Qt::WindowMaximized);//最大化显示
    m_bWndMaxmized = true;

    m_chartView = 0;

    for(int i = 0 ; i < 5; ++i)
        m_arrTabInit[i] = false;

    //读取配置文件
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
    m_tabMain = new QTabWidget(this);
    m_tabMain->setObjectName(QStringLiteral("m_tabMain"));
    m_tabMain->setIconSize(QSize(24,24));
    //距离左侧450px,正好给m_pAfterTabLabel留有余地，并设置背景图片
    m_tabMain->setStyleSheet(QStringLiteral("#m_tabMain::pane{border-image: url(:/skin/bg);}\n"//背景图
                                            "#m_tabMain::tab-bar{left: 450px;}\n"//标签页左侧距离
                                            "QTabBar::tab{height:50px;}"));//tab标签页高度

    m_wndWorkFlow = new CWidgetWork();
    connect(m_wndWorkFlow,SIGNAL(sendCmd(QString)),this,SLOT(receiveCmd(QString)));
    QString sFileLan("");
    m_cfgMgr.getValue("file_lan",sFileLan);
    m_wndWorkFlow->setFileCfg(sFileLan);
    m_tabMain->addTab(m_wndWorkFlow/*,QIcon(":/toolWidget/tiJian")*/,QStringLiteral("  系统运行状态图  "));

    m_wndWebMap1 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap1/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("事故潮流分布"));
    m_wndWebMap2 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap2/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("正常潮流分布"));
    m_wndWebMap3 = new FormWebBase();
    m_tabMain->addTab(m_wndWebMap3/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("录波装置分布"));

    createBarChart();
    m_tabMain->addTab(m_chartView/*,QIcon(":/toolWidget/muMa")*/, QStringLiteral("场站数据时序状态图"));

    m_tabMain->setCurrentIndex(0);
    setCentralWidget(m_tabMain);
    connect(m_tabMain,SIGNAL(currentChanged(int)), this, SLOT(activeTab(int)));

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

    connect(&m_timerErrInfo,SIGNAL(timeout()),this,SLOT(updateErrInfo()));
    m_timerErrInfo.start(2000);
}


//创建可停靠窗口
void MainWnd::createDockWnd()
{
    wndErr = new CXxwDockWidget(tr("异常信息"));
    wndErr->setFeatures(QDockWidget::DockWidgetMovable);//不能移动、浮动、关闭
    wndErr->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);//停靠在主窗口下方/*
    txtErr = new QTextEdit(wndErr);
    txtErr->setObjectName(QStringLiteral("Output1"));
    txtErr->setStyleSheet("#txtErr{border:0px; }");
    txtErr->setText("111");
    wndErr->setWidget(txtErr);

    //不显示标题栏
//    wndErr->showTitleBar(false);

    //设置最小的宽度和高度
    txtErr->setMinimumSize(10,10);

    addDockWidget(Qt::BottomDockWidgetArea, wndErr);//添加dock1
//    splitDockWidget(wndErr,dock3,Qt::Horizontal);//在dock1右侧水平添加dock3,和dock1水平并列
//    tabifyDockWidget(wndErr,dock2);//添加dock2和dock1合并成tab

    QString sFileErr("");
    m_cfgMgr.getValue("file_err",sFileErr);
    m_fmErr.setFileName(sFileErr);
    if(txtErr)
        txtErr->setText(m_fmErr.getData());
}


void MainWnd::createBarChart()
{
    if(m_chartView)
        delete m_chartView;
    m_chartView = new XBarChart();
    m_chartView->setTitle("场站数据时序状态图");
    QString sFileColor("");
    m_cfgMgr.getValue("file_color",sFileColor);
    m_chartView->setColorCfgFile(sFileColor);
    QString sFileDidx("");
    m_cfgMgr.getValue("file_bar",sFileDidx);
    m_chartView->setFileData(sFileDidx);
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
        if(wndErr)
            wndErr->showNormal();
    }
    else
    {
        if(wndErr)
            wndErr->hide();
    }
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
        if(txtErr)
            txtErr->setText(m_fmErr.getData());
    }
}
