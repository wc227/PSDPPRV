﻿#include "XBarChart.h"
#include <QTextBlockFormat>
#include <QTextCursor>
#include "XxwQtPub.h"
#include <QSettings>
#include <QTextCodec>
#include <QFont>
#include "DlgXBarChartProperty.h"

//标准化时间
//原时间time：22/06/2017,15:05:30.000250
//格式化后的时间：2017/06/22 15:05:30.000250
QString g_formatTime(const QString &sTime)
{
    if(sTime.count() < 20)
        return "";
    return QString("%1/%2/%3,%4")
            .arg(sTime.mid(6,4))
            .arg(sTime.mid(3,2))
            .arg(sTime.mid(0,2))
            .arg(sTime.mid(11));
}

//标准化时间
//原时间time：22/06/2017,15:05:30.000250
//格式化后的时间：2017/06/22 15:05:30.000250
QDateTime g_getFormatDateTime(const QString &sTime)
{
    if(sTime.count() < 20)
        return QDateTime::currentDateTime();

    QDate date(sTime.mid(6,4).toInt(), sTime.mid(3,2).toInt(), sTime.mid(0,2).toInt());
    QTime time(sTime.mid(11,2).toInt(), sTime.mid(14,2).toInt(), sTime.mid(17,2).toInt(),sTime.mid(20).toInt());
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

    m_MarginLeft = 40;
    m_MarginRight = 40;
    m_MarginTop = 20;
    m_MarginBottom = 40;
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
    m_Title->setPos(0,m_MarginTop);
    m_Scene->addItem(m_Title);
    m_TitleRect = QRect(m_MarginLeft,
                        m_MarginTop,
                        this->width() - m_MarginLeft - m_MarginRight,
                        m_TitleHeight);

    m_LegendVisible = true;
    m_LegendPosition = LP_LEFT;
    m_LegendWidth = 100;
    m_LegendHeight = 30;
    m_LegendRect = QRect(m_MarginLeft,
                         m_MarginTop + m_TitleHeight,
                         this->width() - m_MarginLeft - m_MarginRight,
                         m_LegendHeight);

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


//更新各区域大小和位置
void XBarChart::updatePositionAndSize()
{
    updateTitleRect();
    updateLegendRect();
    updateMainPlotRect();
}

//更新标题大小和位置
void  XBarChart::updateTitleRect()
{
    //更新标题区域大小
    if(isTitleVisible())
        m_TitleRect.setRect(m_MarginLeft,m_MarginTop,this->width() - m_MarginLeft - m_MarginRight, m_TitleHeight);
    else
        m_TitleRect.setRect(0,0,0,0);
}

//更新图例大小和位置
void  XBarChart::updateLegendRect()
{
    //更新图例区域大小
    if(m_LegendVisible)
    {
        int x,y,w,h;
        x = m_MarginLeft;
        y = m_MarginTop;
        w = this->width() - m_MarginLeft - m_MarginRight;
        h = m_LegendHeight;
        if(isTitleVisible())
            y += m_TitleHeight;

        if(LP_BOTTOM == m_LegendPosition)
        {
            y = this->height() - m_MarginBottom - m_LegendHeight;
        }
        else if(LP_LEFT == m_LegendPosition)
        {
            w = m_LegendWidth;
            h = this->height() - m_MarginTop - m_MarginBottom;
            if(isTitleVisible())
                h -= m_TitleHeight;
        }
        else if(LP_RIGHT == m_LegendPosition)
        {
            x = this->width() - m_MarginRight - m_LegendWidth;
            w = m_LegendWidth;
            h = this->height() - m_MarginTop - m_MarginBottom;
            if(isTitleVisible())
                h -= m_TitleHeight;
        }
        m_LegendRect.setRect(x,y,w,h);
    }
    else
    {
        m_LegendRect.setRect(0,0,0,0);
    }
}

//更新主绘图区域大小和位置
void  XBarChart::updateMainPlotRect()
{
    //计算主绘图区域大小
    int x = m_MarginLeft;
    int y = m_MarginTop + m_TitleRect.height();
    int wid = this->width() - m_MarginLeft - m_MarginRight;
    int hei = this->height() - m_MarginTop - m_MarginBottom - m_TitleRect.height();
    if(LP_TOP == m_LegendPosition)
    {
        y += m_LegendRect.height();
        hei -= m_LegendRect.height();
    }
    else if(LP_BOTTOM == m_LegendPosition)
    {
        hei -= m_LegendRect.height();
    }
    else if(LP_LEFT == m_LegendPosition)
    {
        x += m_LegendRect.width();
        wid -= m_LegendRect.width();
    }
    else if(LP_RIGHT == m_LegendPosition)
    {
        wid -= m_LegendRect.width();
    }
    m_MainPlotRect.setRect(x,y,wid,hei);
}

//获取边距
int XBarChart::getMarginLeft() const
{
    return m_MarginLeft;
}

//设置边距
void XBarChart::setMarginLeft(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 100)
        val = 100;

    if(m_MarginLeft != val)
    {
        m_MarginLeft = val;
        this->update();
    }
}


//获取边距
int XBarChart::getMarginRight() const
{
    return m_MarginRight;
}

//设置边距
void XBarChart::setMarginRight(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 100)
        val = 100;

    if(m_MarginRight != val)
    {
        m_MarginRight = val;
        this->update();
    }
}


//获取边距
int XBarChart::getMarginTop() const
{
    return m_MarginTop;
}

//设置边距
void XBarChart::setMarginTop(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 100)
        val = 100;

    if(m_MarginTop != val)
    {
        m_MarginTop = val;
        this->update();
    }
}


//获取边距
int XBarChart::getMarginBottom() const
{
    return m_MarginBottom;
}

//设置边距
void XBarChart::setMarginBottom(int val)
{
    if(val < 20)
        val = 20;
    else if(val > 100)
        val = 100;

    if(m_MarginBottom != val)
    {
        m_MarginBottom = val;
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
    if(m_Title->isVisible() != vis)
    {
        m_Title->setVisible(vis);
        this->update();
    }
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
        if(isTitleVisible())
            this->update();
    }
}

//图例是否可见
bool XBarChart::isLegendVisible() const
{
    return m_LegendVisible;
}

//设置是否显示图例
void XBarChart::setLegendVisible(bool val)
{
    if(m_LegendVisible != val)
    {
        m_LegendVisible = val;
        this->update();
    }
}

//设置图例位置
void XBarChart::setLegendPosition(LegendPosition pos)
{
    if(m_LegendPosition != pos)
    {
        m_LegendPosition = pos;
        this->update();
    }
}

//获取图例位置
XBarChart::LegendPosition XBarChart::getLegendPosition() const
{
    return m_LegendPosition;
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
//    if(m_IsSortByDuriation)
//    {
//        qSort(bars.begin(),bars.end(),[=](BarInfo a,BarInfo b)->bool{
//            return a.m_Duration > b.m_Duration;
//        });
//    }

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
    if(m_MaxBarNumOfGroup % m_MaxBarNumOfGroupInPage == 0)
        m_VPageCount = m_MaxBarNumOfGroup / m_MaxBarNumOfGroupInPage;
    else
        m_VPageCount = m_MaxBarNumOfGroup / m_MaxBarNumOfGroupInPage + 1;

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
    updatePositionAndSize();

    QPainter p(viewport());
    updateTitle();
    drawLegend(&p);
    drawMainArea(&p);
    drawGrid(&p);

    updateAxisX();
    updateBars();
    updateNavi();
}


//绘制图例
void XBarChart::drawLegend(QPainter *painter)
{
    if(!m_LegendVisible)
        return;

    painter->save();

    qreal x = 0;
    qreal y = 0;
    qreal w = 0;
    qreal h = 0;
    qreal legendLeft = m_LegendRect.left();
    qreal legendTop = m_LegendRect.top();
    qreal legendWid = m_LegendRect.width();
    qreal legendHei = m_LegendRect.height();
    qreal squareSize = 10;

    if(LP_TOP == m_LegendPosition || LP_BOTTOM == m_LegendPosition)
    {
        int nMax = m_Type2Color.count();//配色数目
        if(m_LegendWidth * nMax <= legendWid)
        {
            legendLeft += (legendWid - m_LegendWidth * nMax)/2;
        }

        int i = 0;//计数器
        foreach (int type, m_Type2Color.keys())
        {
            x = legendLeft + m_LegendWidth * i;
            y = legendTop;
            w = m_LegendWidth;
            h = m_LegendHeight;

            //画方块
            QColor clr = m_Type2Color.value(type);
            painter->setPen(clr);
            QRectF rectSquare(x,y + (h-squareSize)/2,squareSize,squareSize);
            painter->fillRect(rectSquare,QBrush(clr));

            //画类型
            painter->setPen(Qt::black);
            QRectF rectType(x + squareSize + 5,y,w - squareSize - 5,h);
            painter->drawText(rectType,Qt::AlignLeft | Qt::AlignVCenter,QString("类型:%1").arg(type));

            i++;

            if(m_LegendWidth * (i+1) > legendWid)
                break;//超出了宽度，就不用画了
        }
    }
    else if(LP_LEFT == m_LegendPosition || LP_RIGHT == m_LegendPosition)
    {
        int nMax = m_Type2Color.count();//配色数目
        if(m_LegendHeight * nMax <= legendHei)
        {
            legendTop += (legendHei - m_LegendHeight * nMax)/2;
        }
        legendLeft += 5;

        int i = 0;//计数器
        foreach (int type, m_Type2Color.keys())
        {
            x = legendLeft;
            y = legendTop + m_LegendHeight * i;
            w = m_LegendWidth;
            h = m_LegendHeight;

            //画方块
            QColor clr = m_Type2Color.value(type);
            painter->setPen(clr);
            QRectF rectSquare(x,y + (h-squareSize)/2,squareSize,squareSize);
            painter->fillRect(rectSquare,QBrush(clr));

            //画类型
            painter->setPen(Qt::black);
            QRectF rectType(x + squareSize + 5,y,w - squareSize - 5,h);
            painter->drawText(rectType,Qt::AlignLeft | Qt::AlignVCenter,QString("类型:%1").arg(type));

            i++;

            if(m_LegendHeight * (i+1) > legendHei)
                break;//超出了高度，就不用画了
        }
    }
    painter->restore();
}

//绘制主区域
void XBarChart::drawMainArea(QPainter *painter)
{
    painter->save();
    QPen pen(Qt::black,1);
    painter->setPen(pen);

    int x = m_MainPlotRect.x();
    int y = m_MainPlotRect.y();
    int wid = m_MainPlotRect.width();
    int hei = m_MainPlotRect.height();

    painter->fillRect(m_MainPlotRect,m_BackColor);//浅灰色充填背景
    painter->drawRect(m_MainPlotRect);//画边框

    //画箭头
    painter->drawLine(x+wid,y+hei,x+wid+15,y+hei);
    painter->drawLine(x+wid+5,y+hei-3,x+wid+15,y+hei);
    painter->drawLine(x+wid+5,y+hei+3,x+wid+15,y+hei);

    //画横轴标题
    pen.setColor(Qt::red);
    painter->setPen(pen);
    QRectF rectTitle(x+wid,y+hei,40,20);
    painter->drawText(rectTitle,Qt::AlignLeft | Qt::AlignVCenter,"时间");

    painter->restore();
}

//绘制网格线
void XBarChart::drawGrid(QPainter *painter)
{
    if(!m_GridVisible)
        return;

    painter->save();

    painter->setPen(Qt::lightGray);

    qreal x = m_MainPlotRect.x();
    qreal y = m_MainPlotRect.y();
    qreal wid = m_MainPlotRect.width();
    qreal hei = m_MainPlotRect.height();

    //水平网格线
    //    qreal diffY = hei/m_MaxBarNumOfGroup;
    //    for(int i = 1; i < m_MaxBarNumOfGroup; i++)
    //    {
    //        painter->drawLine(x, y + diffY * i, x + wid, y + diffY * i);
    //    }

    //垂直网格线
    qreal diffX = wid/m_MaxGroupNumInPage;
    for(int i = 1; i < m_MaxGroupNumInPage; i++)
    {
        painter->drawLine(x + diffX * i, y, x + diffX * i, y + hei);
    }

    painter->restore();
}


//更新标题
void XBarChart::updateTitle()
{
    if(!isTitleVisible())
        return;

    //设置居中显示
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = m_Title->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    m_Title->setTextCursor(cursor);

    m_Title->setPos(m_TitleRect.x(),m_TitleRect.y());
    m_Title->setTextWidth(m_TitleRect.width());//设置宽度
}

//更新X轴
void XBarChart::updateAxisX()
{
    foreach(QGraphicsTextItem * item, m_TimeItems)
        item->setPlainText("");

    qreal x = m_MainPlotRect.x();
    qreal wid = m_MainPlotRect.width();

    qreal gridWid = wid/m_MaxGroupNumInPage;//网格宽度
    for(int vgridNo(0),groupID = m_MaxGroupNumInPage * (m_HPageNo - 1);
        groupID < m_BarGroups.count() && vgridNo < m_MaxGroupNumInPage;
        ++groupID, ++vgridNo)
    {
        ListBarInfo lbi = m_BarGroups.at(groupID);
        if(lbi.isEmpty())
            continue;

        x = m_MainPlotRect.x() + gridWid * vgridNo;

        QDateTime oldDT = g_getFormatDateTime(lbi.at(0).m_Time);
        foreach (BarInfo bi, lbi)
        {
            QDateTime tempDT = g_getFormatDateTime(bi.m_Time);
            if(tempDT < oldDT)
                oldDT = tempDT;
        }

        long maxDuriation = 0;//最大持续时间（单位：秒）
        long oldTime = oldDT.toSecsSinceEpoch();
        foreach (BarInfo bi, lbi)
        {
            QDateTime tempDT = g_getFormatDateTime(bi.m_Time);
            long tempTime = tempDT.toSecsSinceEpoch();
            long tempDuriation = tempTime - oldTime + bi.m_Duration;
            if(maxDuriation < tempDuriation)
                maxDuriation = tempDuriation;
        }

        QString sTime = m_Times.at(groupID);//例如：20170801112503
        sTime = g_formatTime(sTime);//2017/08/01 11:25:03
        QString sTxt = QString("%1(%2秒)").arg(sTime).arg(maxDuriation);
        QGraphicsTextItem *text = m_TimeItems.at(vgridNo);
        if(text)
        {
            text->setPlainText(sTxt);
            text->setPos(x, m_MainPlotRect.y()+m_MainPlotRect.height());
        }
    }
}

//更新条形图位置和大小
void XBarChart::updateBars()
{
    clearBarItems();

    if(0 == m_BarGroups.count())
        return;

    qreal x = m_MainPlotRect.x();
    qreal y = m_MainPlotRect.y();
    qreal bot = m_MainPlotRect.bottom();
    qreal wid = m_MainPlotRect.width();
    qreal hei = m_MainPlotRect.height();

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
        x = m_MainPlotRect.x() + gridWid * gNo;
        bot = m_MainPlotRect.bottom();
        if(lbi.isEmpty())
            continue;

        //当前组中最大持续时间
        QDateTime oldDT = g_getFormatDateTime(lbi.at(0).m_Time);
        foreach (BarInfo bi, lbi)
        {
            QDateTime tempDT = g_getFormatDateTime(bi.m_Time);
            if(tempDT < oldDT)
                oldDT = tempDT;
        }

        long maxDuriation = 0;//最大持续时间（单位：秒）
        long oldTime = oldDT.toSecsSinceEpoch();
        foreach (BarInfo bi, lbi)
        {
            QDateTime tempDT = g_getFormatDateTime(bi.m_Time);
            long tempTime = tempDT.toSecsSinceEpoch();
            long tempDuriation = tempTime - oldTime + bi.m_Duration;
            if(maxDuriation < tempDuriation)
                maxDuriation = tempDuriation;
        }

        for(int barID = m_MaxBarNumOfGroupInPage * (m_VPageNo - 1);
            barID < lbi.count() && barID < m_MaxBarNumOfGroupInPage * m_VPageNo;
            ++barID)
        {
            BarInfo bi = lbi.at(barID);
            XBar* bar = new XBar();
            bar->setBarInfo(bi);

            //提取颜色（文件类型跟颜色一一对应）
//            QColor clr = m_Type2Color.value(bar->barInfo().m_Type,Qt::green);
            //但是，此处的文件类型基本都是.dat，类型一致，颜色也是一致，
            //所以为了区分开来，此处采用分配不同颜色的方式
            int nc = m_Type2Color.count();
            int nt = barID % nc + 1;
            QColor clr = m_Type2Color.value(nt,Qt::darkGreen);

            bar->setBackColor(clr);
            barTop = bot - barHei;
            barWid = gridWid * bi.m_Duration / (maxDuriation * 1.0f);
            QRectF rect(0,0,barWid,barHei);
            bar->setRect(rect);
            addBarItem(bar);

            QDateTime tempDT = g_getFormatDateTime(bi.m_Time);
            long tempTime = tempDT.toSecsSinceEpoch();
            qreal offLeft = gridWid * (tempTime - oldTime) / (maxDuriation * 1.0f);
            bar->setPos(x + offLeft, barTop);
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
