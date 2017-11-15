#include "XBarChart.h"
#include <QTextBlockFormat>
#include <QTextCursor>
#include "XxwQtPub.h"
#include <QSettings>
#include <QTextCodec>
#include <QFont>
#include "DlgXBarChartProperty.h"

//标准化时间
//原时间time：20170801112503
//格式化后的时间：2017/08/01 11:25:03
QString g_formatTime(const QString &sTime)
{
    if(sTime.count() != 14)
        return "";
    return QString("%1/%2/%3 %4:%5:%6")
            .arg(sTime.mid(0,4))
            .arg(sTime.mid(4,2))
            .arg(sTime.mid(6,2))
            .arg(sTime.mid(8,2))
            .arg(sTime.mid(10,2))
            .arg(sTime.mid(12,2));
}

//标准化时间
//原时间time：20170801112503
//格式化后的时间：2017/08/01 11:25:03
QDateTime g_getFormatDateTime(const QString &sTime)
{
    if(sTime.count() != 14)
        return QDateTime::currentDateTime();

    QDate date(sTime.mid(0,4).toInt(), sTime.mid(4,4).toInt(), sTime.mid(6,4).toInt());
    QTime time(sTime.mid(8,2).toInt(), sTime.mid(10,2).toInt(), sTime.mid(12,2).toInt());
    return QDateTime(date,time);
}

XBarChart::XBarChart(QWidget *parent)
    : QGraphicsView(parent)
{
    initPopMenu();
    initChart();
    m_CurrentItem = 0;
}

XBarChart::~XBarChart()
{
    m_TimerCheckFile.stop();
}

void XBarChart::initChart()
{    
    //不显示滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing|
                   QPainter::TextAntialiasing);

    m_Scene = new QGraphicsScene();
    this->setScene(m_Scene);

    m_Margin = 40;
    m_BackColor = QColor(240,240,255,123);//很浅的蓝色

    m_TitleHeight = 30;
    m_TitleTxt = "条形图";
    m_Title = new QGraphicsTextItem();
    m_Title->setPlainText(m_TitleTxt);//设置显示标题
    QFont font;
//    font.setFamily("楷体");
    font.setPointSize(14);
    font.setWeight(QFont::Normal);
    font.setItalic(false);
    font.setUnderline(false);
    m_Title->setFont(font);//设置字体
    m_Title->setDefaultTextColor(Qt::blue);//设置颜色

    //设置居中显示
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = m_Title->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    m_Title->setTextCursor(cursor);

    m_Title->setTextWidth(this->width());//设置宽度
    m_Title->setPos(0,10);
    m_Scene->addItem(m_Title);

    m_GridVisible = true;
    m_MaxGroupNumInPage = 5;
    m_MaxBarNumOfGroupInPage = 5;
    m_MaxBarNumOfGroup = 0;

    for(int i = 0; i < m_MaxGroupNumInPage; ++i)
    {
        QGraphicsTextItem *textItem = new QGraphicsTextItem();
        textItem->setPlainText("");
        textItem->setDefaultTextColor(Qt::red);
        m_TimeItems.append(textItem);
        m_Scene->addItem(textItem);
    }
    m_BarGroupsCapacity = 20;
    m_IsSortByDuriation = true;

    m_HPageCount = 1;
    m_HPageNo = 1;

    m_VPageCount = 1;
    m_VPageNo = 1;

    m_TimeInterval = 5000;
//    connect(&m_TimerCheckFile, SIGNAL(timeout()), this, SLOT(checkFileData()));
    connect(&m_TimerCheckFile,&QTimer::timeout,this,&XBarChart::checkFileData);
    m_TimerCheckFile.start(m_TimeInterval);//m_timeInterval毫秒检查一次该文件是否修改

    m_BtnPageLeft = new QPushButton("←");//上一页
    m_BtnPageLeft->setToolTip("前一页");
    m_BtnPageLeft->setShortcut(Qt::Key_Left);
    m_BtnPageRight = new QPushButton("→");//下一页
    m_BtnPageRight->setToolTip("后一页");
    m_BtnPageRight->setShortcut(Qt::Key_Right);
    m_LblPageNO= new QLabel("");//当前页号
    m_BtnPageLeft->setMinimumSize(30,20);
    m_BtnPageLeft->setMaximumSize(30,20);
    m_BtnPageRight->setMinimumSize(30,20);
    m_BtnPageRight->setMaximumSize(30,20);
    m_LblPageNO->setMinimumSize(30,20);
    m_LblPageNO->setMaximumSize(30,20);
    m_LblPageNO->setText(QString("%1/%2").arg(m_HPageNo).arg(m_HPageCount));
    m_LblPageNO->setAlignment(Qt::AlignCenter);

    m_Scene->addWidget(m_BtnPageLeft);
    m_Scene->addWidget(m_BtnPageRight);
    m_Scene->addWidget(m_LblPageNO);
    int y = this->height() - 20;
    int x = this->width()/2 - m_BtnPageLeft->width() - m_LblPageNO->width()/2;
    m_BtnPageLeft->setGeometry(x,y,m_BtnPageLeft->width(),m_BtnPageLeft->height());
    x = this->width()/2 - m_LblPageNO->width()/2;
    m_LblPageNO->setGeometry(x,y,m_LblPageNO->width(),m_LblPageNO->height());
    x = this->width()/2 + m_LblPageNO->width()/2;
    m_BtnPageRight->setGeometry(x,y,m_BtnPageRight->width(),m_BtnPageRight->height());

    m_BtnPageUp = new QPushButton("↑");//上一页
    m_BtnPageUp->setToolTip("上一页");
    m_BtnPageUp->setShortcut(Qt::Key_Up);
    m_BtnPageDown = new QPushButton("↓");//下一页
    m_BtnPageDown->setToolTip("下一页");
    m_BtnPageDown->setShortcut(Qt::Key_Down);
    m_BtnPageUp->setMinimumSize(30,20);
    m_BtnPageUp->setMaximumSize(30,20);
    m_BtnPageDown->setMinimumSize(30,20);
    m_BtnPageDown->setMaximumSize(30,20);
    m_LblPageNO2 = new QLabel("");//当前页号
    m_LblPageNO2->setMinimumSize(30,20);
    m_LblPageNO2->setMaximumSize(30,20);
    m_LblPageNO2->setText(QString("%1/%2").arg(m_VPageNo).arg(m_VPageCount));
    m_LblPageNO2->setAlignment(Qt::AlignCenter);

    m_Scene->addWidget(m_BtnPageUp);
    m_Scene->addWidget(m_BtnPageDown);
    m_Scene->addWidget(m_LblPageNO2);
    x = this->width() - 30;
    y = this->height() / 2 - m_BtnPageUp->height() - m_LblPageNO2->height()/2;
    m_BtnPageUp->setGeometry(x,y,m_BtnPageUp->width(),m_BtnPageUp->height());    
    y = this->height() / 2 - m_LblPageNO2->height()/2;
    m_LblPageNO2->setGeometry(x,y,m_LblPageNO2->width(),m_LblPageNO2->height());
    y = this->height() / 2 + m_LblPageNO2->height()/2;
    m_BtnPageDown->setGeometry(x,y,m_BtnPageDown->width(),m_BtnPageDown->height());

    connect(m_BtnPageLeft,&QPushButton::clicked,this,&XBarChart::pageLeft);
    connect(m_BtnPageRight,&QPushButton::clicked,this,&XBarChart::pageRight);
    connect(m_BtnPageUp,&QPushButton::clicked,this,&XBarChart::pageUp);
    connect(m_BtnPageDown,&QPushButton::clicked,this,&XBarChart::pageDown);
}

//获取边距
int XBarChart::getMargin() const
{
    return m_Margin;
}

//设置边距
void XBarChart::setMargin(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 100)
        val = 100;

    if(m_Margin != val)
    {
        m_Margin = val;
        this->update();
    }
}

//获取背景颜色
QColor XBarChart::getBackColor() const
{
    return m_BackColor;
}

//设置背景颜色
void XBarChart::setBackColor(QColor val)
{
    if(m_BackColor != val)
    {
        m_BackColor = val;
        this->update();
    }
}

//设置标题
void XBarChart::setTitle(QString sTitle)
{
    if(m_TitleTxt != sTitle)
    {
        m_TitleTxt = sTitle;
        m_Title->setPlainText(m_TitleTxt);
    }
}

//获取标题
QString XBarChart::getTitle() const
{
    return m_TitleTxt;
}

//标题是否可见
bool XBarChart::isTitleVisible() const
{
    return m_Title->isVisible();
}

//设置是否显示标题
void XBarChart::setTitleVisible(bool vis)
{
    m_Title->setVisible(vis);
}

//获取标题区域高度
int XBarChart::getTitleHeight() const
{
    return m_TitleHeight;
}

//设置标题区域高度
void XBarChart::setTitleHeight(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 50)
        val = 50;

    if(m_TitleHeight != val)
    {
        m_TitleHeight = val;
        this->update();
    }
}

//网格线是否可见
bool XBarChart::isGridVisible() const
{
    return m_GridVisible;
}

//设置是否显示网格线
void XBarChart::setGridVisible(bool val)
{
    if(m_GridVisible != val)
    {
        m_GridVisible = val;
        this->update();
    }
}

//获取垂直网格数
int XBarChart::getMaxGroupNumInPage() const
{
    return m_MaxGroupNumInPage;
}

//设置垂直网格数
void XBarChart::setMaxGroupNumInPage(int val)
{
    if(val < 2)
        val = 2;
    else if(val >10)
        val = 10;

    if(m_MaxGroupNumInPage != val)
    {
        m_MaxGroupNumInPage = val;

        if(0 == m_BarGroups.count() % m_MaxGroupNumInPage)
            m_HPageCount = m_BarGroups.count() / m_MaxGroupNumInPage;
        else
            m_HPageCount = m_BarGroups.count() / m_MaxGroupNumInPage + 1;

        if(m_HPageNo > m_HPageCount)
            m_HPageNo = m_HPageCount;

        this->update();
    }
}

//页面上垂直网格里显示条形图的最大值
int XBarChart::getMaxBarNumOfGroupInPage() const
{
    return m_MaxBarNumOfGroupInPage;
}

//页面上垂直网格里显示条形图的最大值
void XBarChart::setMaxBarNumOfGroupInPage(int val)
{
    if(val < 2)
        val = 2;
    else if(val > 10)
        val = 10;

    if(m_MaxBarNumOfGroupInPage != val)
    {
        m_MaxBarNumOfGroupInPage = val;

        if(0 == m_MaxBarNumOfGroup % m_MaxBarNumOfGroupInPage)
            m_VPageCount = m_MaxBarNumOfGroup / m_MaxBarNumOfGroupInPage;
        else
            m_VPageCount = m_MaxBarNumOfGroup / m_MaxBarNumOfGroupInPage + 1;

        if(m_VPageNo > m_VPageCount)
            m_VPageNo = m_VPageCount;

        this->update();
    }
}

//获取条形图组的最大数量值
int XBarChart::getBarGroupsCapacity() const
{
    return m_BarGroupsCapacity;
}

//设置条形图组的最大数量值
void XBarChart::setBarGroupsCapacity(int val)
{
    if(val < m_MaxGroupNumInPage)
        val = m_MaxGroupNumInPage;

    if(m_BarGroupsCapacity != val)
    {
        m_BarGroupsCapacity = val;
    }
}

//设置颜色配置文件
void XBarChart::setColorCfgFile(QString val)
{
    if(m_ColorCfgFile != val)
    {
        m_ColorCfgFile = val;
        loadColorCfgFile();
    }
}

//获取检查间隔时间（单位：毫秒）
int XBarChart::getTimeInterval() const
{
    return m_TimeInterval;
}

//设置检查间隔时间（单位：毫秒）
void XBarChart::setTimeInterval(int val)
{
    if(val < 1)
        val = 1;

    if(m_TimeInterval != val)
    {
        m_TimeInterval = val;
        m_TimerCheckFile.setInterval(m_TimeInterval);
    }
}


//添加一组条形图
void XBarChart::addBars(ListBarInfo bars)
{
    if(0 == bars.count())
        return;

    //按照持续时间降序排序
    if(m_IsSortByDuriation)
    {
        qSort(bars.begin(),bars.end(),[=](BarInfo a,BarInfo b)->bool{
            return a.m_Duration > b.m_Duration;
        });
    }

    //添加一组新的条形图
    m_BarGroups.append(bars);
    m_Times.append(bars.at(0).m_Time);//添加时间

    //如果组数超出最大值，就删除第组列条形图
    if(m_BarGroups.count() > m_BarGroupsCapacity)
    {
        m_BarGroups.removeFirst();
        m_Times.removeFirst();
    }

    //计算条形图组中条形图数目的最大值
    m_MaxBarNumOfGroup = 0;
    foreach(ListBarInfo lbi, m_BarGroups)
    {
        int cnt = lbi.count();
        if(m_MaxBarNumOfGroup < cnt)
            m_MaxBarNumOfGroup = cnt;
    }

    //更新垂直方向页面数目
    if(m_MaxBarNumOfGroup > m_MaxBarNumOfGroupInPage * m_VPageCount)
        m_VPageCount++;

    //更新水平方向页面数目
    if(m_BarGroups.count() > m_MaxGroupNumInPage * m_HPageCount)
         m_HPageCount++;

    m_HPageNo = m_HPageCount;//自动跳转到新的页面
    m_VPageNo = 1;

    repaintChart();

    update();
}

//添加一个图元
void XBarChart::addBarItem(XBar *item)
{
    m_Scene->addItem(item);
    m_BarItems.append(item);
}

//清空所有的条形图
void XBarChart::clearBarItems()
{
    foreach (XBar *bar, m_BarItems)
    {
        m_Scene->removeItem(bar);
    }
    m_BarItems.clear();
}

//重新绘制图标
void XBarChart::repaintChart()
{
    QPainter p(viewport());
    drawMainArea(&p);
    drawGrid(&p);

    updateTitle();
    updateAxisX();
    updateBars();
    updateNavi();
}

//绘制主区域
void XBarChart::drawMainArea(QPainter *painter)
{
    painter->save();
    QPen pen(Qt::black,1);
    painter->setPen(pen);

    qreal left = m_Margin;
    qreal top = m_Margin;
    qreal wid = this->width() - m_Margin * 2;
    qreal hei = this->height() - m_Margin * 2;

    QRectF rect(left,top,wid,hei);
    painter->fillRect(rect,m_BackColor);//浅灰色充填背景
    painter->drawRect(rect);//画边框

    //画箭头
    painter->drawLine(left+wid,top+hei,left+wid+15,top+hei);
    painter->drawLine(left+wid+5,top+hei-3,left+wid+15,top+hei);
    painter->drawLine(left+wid+5,top+hei+3,left+wid+15,top+hei);

    //画横轴标题
    pen.setColor(Qt::red);
    painter->setPen(pen);
    QRectF rectTitle(left+wid,top+hei,40,20);
    painter->drawText(rectTitle,Qt::AlignLeft | Qt::AlignVCenter,"时间");

    painter->restore();
}

//绘制网格线
void XBarChart::drawGrid(QPainter *painter)
{
    if(!m_GridVisible)
        return;

//    //计算条形图组中条形图数目的最大值
//    m_MaxBarNumOfGroup = 0;
//    foreach(ListBarInfo lbi, m_BarGroups)
//    {
//        int cnt = lbi.count();
//        if(m_MaxBarNumOfGroup < cnt)
//            m_MaxBarNumOfGroup = cnt;
//    }

    painter->save();

    painter->setPen(Qt::lightGray);
    qreal left = m_Margin;
    qreal top = m_Margin;
    qreal wid = this->width() - m_Margin * 2;
    qreal hei = this->height() - m_Margin * 2;

    //水平网格线
    //    qreal diffY = hei/m_MaxBarNumOfGroup;
    //    for(int i = 1; i < m_MaxBarNumOfGroup; i++)
    //    {
    //        painter->drawLine(left, top + diffY * i, left + wid, top + diffY * i);
    //    }

    //垂直网格线
    qreal diffX = wid/m_MaxGroupNumInPage;
    for(int i = 1; i < m_MaxGroupNumInPage; i++)
    {
        painter->drawLine(left + diffX * i, top, left + diffX * i, top + hei);
    }

    painter->restore();
}


//更新标题
void XBarChart::updateTitle()
{
    if(!isTitleVisible())
        return;
    m_Title->setTextWidth(this->width());//更新标题的宽度
    //设置居中显示
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = m_Title->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    m_Title->setTextCursor(cursor);
}

//更新X轴
void XBarChart::updateAxisX()
{
    foreach(QGraphicsTextItem * item, m_TimeItems)
        item->setPlainText("");

    qreal wid = this->width() - m_Margin * 2;
    qreal top = this->height() - m_Margin;
    qreal left = m_Margin;
    qreal gridWid = wid/m_MaxGroupNumInPage;//网格宽度
    for(int vgridNo(0),groupID = m_MaxGroupNumInPage * (m_HPageNo - 1);
        groupID < m_BarGroups.count() && vgridNo < m_MaxGroupNumInPage;
        ++groupID, ++vgridNo)
    {
        ListBarInfo lbi = m_BarGroups.at(groupID);
        if(lbi.isEmpty())
            continue;
        left = m_Margin + gridWid * vgridNo;
        long maxDuriation = 0;
        if(m_IsSortByDuriation)
            maxDuriation = lbi.at(0).m_Duration;
        else
        {
            foreach (BarInfo bi, lbi) {
                if(maxDuriation < bi.m_Duration)
                    maxDuriation = bi.m_Duration;
            }
        }

        QString sTime = m_Times.at(groupID);//例如：20170801112503
        sTime = g_formatTime(sTime);//2017/08/01 11:25:03
        QString sTxt = QString("%1(%2秒)").arg(sTime).arg(maxDuriation);
        QGraphicsTextItem *text = m_TimeItems.at(vgridNo);
        if(text)
        {
            text->setPlainText(sTxt);
            text->setPos(left, top);
        }
    }
}

//更新条形图位置和大小
void XBarChart::updateBars()
{
    clearBarItems();

    if(0 == m_BarGroups.count())
        return;

//    //计算条形图组中条形图数目的最大值
//    m_MaxBarNumOfGroup = 0;
//    foreach(ListBarInfo lbi, m_BarGroups)
//    {
//        int cnt = lbi.count();
//        if(m_MaxBarNumOfGroup < cnt)
//            m_MaxBarNumOfGroup = cnt;
//    }

//    int nVal = m_MaxBarNumOfGroup % m_MaxGroupNumInPage;
//    if(0 == nVal)
//        m_VPageCount = m_MaxBarNumOfGroup / m_MaxGroupNumInPage;
//    else
//        m_VPageCount = m_MaxBarNumOfGroup / m_MaxGroupNumInPage + 1;

    qreal left = m_Margin;
//    qreal top = m_Margin;
    qreal bot = this->height() - m_Margin;
    qreal wid = this->width() - m_Margin * 2;
    qreal hei = this->height() - m_Margin * 2;

    //所有列的条形图高度一致，宽度代表持续时间
    qreal gridWid = wid / m_MaxGroupNumInPage;//网格宽度
    qreal barWid = gridWid;//条形图的宽度
    qreal barTop = 0;
    qreal barHei = hei / m_MaxBarNumOfGroupInPage;

    //当前页面 m_HPageNo,m_VPageNo
    for(int groupID = m_MaxGroupNumInPage * (m_HPageNo - 1);
        groupID < m_BarGroups.count() && groupID < m_MaxGroupNumInPage * m_HPageNo;
        ++groupID)
    {
        ListBarInfo lbi = m_BarGroups.at(groupID);

        int gNo = groupID - m_MaxGroupNumInPage * (m_HPageNo - 1);
        left = m_Margin + gridWid * gNo;
        bot = this->height() - m_Margin;
        if(lbi.isEmpty())
            continue;

        //当前组中最大持续时间
        long maxDuriation = 0;
        if(m_IsSortByDuriation)
            maxDuriation = lbi.at(0).m_Duration;//因为之前已经按持续时间从大到小降序排列，所以第一个就是最大的
        else
        {
            foreach (BarInfo bi, lbi) {
                if(maxDuriation < bi.m_Duration)
                    maxDuriation = bi.m_Duration;
            }
        }

        for(int barID = m_MaxBarNumOfGroupInPage * (m_VPageNo - 1);
            barID < lbi.count() && barID < m_MaxBarNumOfGroupInPage * m_VPageNo;
            ++barID)
        {
            BarInfo bi = lbi.at(barID);
            XBar* bar = new XBar();
            bar->setBarInfo(bi);
            QColor clr = m_Type2Color.value(bar->barInfo().m_Type,Qt::green);
            bar->setBackColor(clr);
            barTop = bot - barHei;
            barWid = gridWid * bi.m_Duration / (maxDuriation * 1.0f);
            QRectF rect(0,0,barWid,barHei);
            bar->setRect(rect);
            addBarItem(bar);
            bar->setPos(left,barTop);
            bot = barTop;
        }
    }
}

//更新导航功能相关的部件
void XBarChart::updateNavi()
{
    m_BtnPageLeft->setEnabled(true);
    m_BtnPageRight->setEnabled(true);
    m_LblPageNO->setText(QString("%1/%2").arg(m_HPageNo).arg(m_HPageCount));

    int y = this->height() - 20;
    int x = this->width()/2 - m_BtnPageLeft->width() - m_LblPageNO->width()/2;
    m_BtnPageLeft->setGeometry(x,y,m_BtnPageLeft->width(),m_BtnPageLeft->height());
    x = this->width()/2 - m_LblPageNO->width()/2;
    m_LblPageNO->setGeometry(x,y,m_LblPageNO->width(),m_LblPageNO->height());
    x = this->width()/2 + m_LblPageNO->width()/2;
    m_BtnPageRight->setGeometry(x,y,m_BtnPageRight->width(),m_BtnPageRight->height());

    if(1 == m_HPageNo)
        m_BtnPageLeft->setEnabled(false);
    if(m_HPageNo == m_HPageCount)
        m_BtnPageRight->setEnabled(false);

    m_BtnPageUp->setEnabled(true);
    m_BtnPageDown->setEnabled(true);
    m_LblPageNO2->setText(QString("%1/%2").arg(m_VPageNo).arg(m_VPageCount));
    x = this->width() - 30;
    y = this->height() / 2 - m_BtnPageUp->height() - m_LblPageNO2->height()/2;
    m_BtnPageUp->setGeometry(x,y,m_BtnPageUp->width(),m_BtnPageUp->height());
    y = this->height() / 2 - m_LblPageNO2->height()/2;
    m_LblPageNO2->setGeometry(x,y,m_LblPageNO2->width(),m_LblPageNO2->height());
    y = this->height() / 2 + m_LblPageNO2->height()/2;
    m_BtnPageDown->setGeometry(x,y,m_BtnPageDown->width(),m_BtnPageDown->height());
    if(1 == m_VPageNo)
        m_BtnPageDown->setEnabled(false);
    if(m_VPageNo == m_VPageCount)
        m_BtnPageUp->setEnabled(false);
}


//初始化右键菜单
void XBarChart::initPopMenu()
{
    //创建菜单项
    m_PopMenu = new QMenu(this);
    m_Action_Property = new QAction(this);
    m_Action_Property->setText("属性");
    //    m_Action_Property->setShortcut(Qt::Key_F1);
    //    m_Action_Property->setIcon(icon);
    QObject::connect(m_Action_Property, &QAction::triggered, this,&XBarChart::editProperty);
    m_PopMenu->addAction(m_Action_Property);

    m_PopMenuBar = new QMenu(this);
    m_Action_BarOpenFile = new QAction(this);
    m_Action_BarOpenFile->setText("打开数据文件");
    //    m_Action_BarOpenFile->setShortcut(Qt::Key_F1);
    //    m_Action_BarOpenFile->setIcon(icon);
    QObject::connect(m_Action_BarOpenFile, &QAction::triggered, this,&XBarChart::openFileOfBar);
    m_PopMenuBar->addAction(m_Action_BarOpenFile);
}

void XBarChart::paintEvent(QPaintEvent *event)
{
    repaintChart();
    QGraphicsView::paintEvent(event);
}

void XBarChart::resizeEvent(QResizeEvent *event)
{
    setSceneRect(0,0,this->width(),this->height());//随时更新场景大小，刚好布满整个view
}

void XBarChart::mouseDoubleClickEvent(QMouseEvent *event)
{
    editProperty();
}


//void XBarChart::mousePressEvent(QMouseEvent *event)
//{
//    m_CurrentItem = itemAt(event->pos());
//}

//在此集中处理chart和chart上面的所有的图元的右键菜单
void XBarChart::contextMenuEvent(QContextMenuEvent *event)
{
    m_CurrentItem = itemAt(event->pos());
    if (m_CurrentItem)
    {
        if( m_CurrentItem->type() == XBar::Type)
            m_PopMenuBar->exec(QCursor::pos());//在条形图上弹出右键菜单
        //在item上，就将控制权交还给QGraphicsView来处理，例如显示item的右键菜单
        else
            QGraphicsView::contextMenuEvent(event);
    }
    else
    {
        //不在item上，就调用自定义的右键菜单
        m_PopMenu->exec(QCursor::pos());//菜单出现的位置为当前鼠标的位置
//        m_PopMenu->exec(event->globalPos());
//        event->accept();
    }
}


//加载颜色配置文件
void XBarChart::loadColorCfgFile()
{
    if(!QFile::exists(m_ColorCfgFile))
        return;

    m_Type2Color.clear();//清空

    QSettings setIni(m_ColorCfgFile,QSettings::IniFormat);
//    setIni.setIniCodec("UTF-8");
    setIni.setIniCodec(QTextCodec::codecForName("UTF-8"));
    setIni.beginGroup("ColorCfg");
    QStringList keys = setIni.allKeys();
    foreach (QString key, keys)
    {
        QString val = setIni.value(key).toString();
        if(val.isEmpty())
            continue;
        val = val.mid(1);//删除#
        bool ok;
        long hex = val.toLong(&ok, 16);
        QColor clr(hex);
        m_Type2Color.insert(key.toInt(),clr);
    }

    setIni.endGroup();
}

//设置数据文件
void XBarChart::setFileData(QString val)
{
    if(QFile::exists(val))
    {
        m_FileMgrDIDX.setFileName(val);
        addBars(m_FileMgrDIDX.getData());
    }
}

//获取数据文件
QString XBarChart::getFileData()
{
    return m_FileMgrDIDX.getFileName();
}

//检查文件是否更新
void XBarChart::checkFileData()
{
    //文件更新了，就重新读取文件
    if(m_FileMgrDIDX.isDirty())
    {
        m_FileMgrDIDX.openFile();
        addBars(m_FileMgrDIDX.getData());
    }
}


//左一页
void XBarChart::pageLeft()
{
    m_HPageNo--;
    if(m_HPageNo < 1)
        m_HPageNo = 1;
    update();
}

//右一页
void XBarChart::pageRight()
{
    m_HPageNo++;
    if(m_HPageNo > m_HPageCount)
        m_HPageNo = m_HPageCount;
    update();
}

//上一页
void XBarChart::pageUp()
{
    m_VPageNo++;
    if(m_VPageNo > m_VPageCount)
        m_VPageNo = m_VPageCount;
    update();
}

//下一页
void XBarChart::pageDown()
{
    m_VPageNo--;
    if(m_VPageNo < 1)
        m_VPageNo = 1;
    update();
}


//属性编辑
void XBarChart::editProperty()
{
    DlgXBarChartProperty dlg(this,this);
    dlg.exec();
}


//打开条形图的数据文件
void XBarChart::openFileOfBar()
{
    if(m_CurrentItem && m_CurrentItem->type() == XBar::Type)
    {
        XBar *bar = dynamic_cast<XBar *>(m_CurrentItem);
        bar->openFile();
    }
}
