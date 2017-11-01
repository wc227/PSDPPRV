#include "XBarChart.h"
#include <QTextBlockFormat>
#include <QTextCursor>
#include "XxwQtPub.h"
#include <QSettings>
#include <QTextCodec>
#include <QFont>

XBarChart::XBarChart(QWidget *parent)
    : QGraphicsView(parent)
{
    initChart();
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
    m_BarGroupNum = 5;
    m_MaxBarNumInGroup = 0;

    for(int i = 0; i < m_BarGroupNum; ++i)
    {
        QGraphicsTextItem *textItem = new QGraphicsTextItem();
        textItem->setPlainText("");
        textItem->setDefaultTextColor(Qt::red);
        m_TimeItems.append(textItem);
        m_Scene->addItem(textItem);
    }

    m_TimeInterval = 5;
//    connect(&m_TimerCheckFile, SIGNAL(timeout()), this, SLOT(checkFileData()));
    connect(&m_TimerCheckFile,&QTimer::timeout,this,&XBarChart::checkFileData);
    m_TimerCheckFile.start(m_TimeInterval * 1000);//m_timeInterval秒检查一次该文件是否修改
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
    QRectF rectTitle(left+wid,top+hei,m_Margin,m_Margin);
    painter->drawText(rectTitle,Qt::AlignLeft | Qt::AlignVCenter,"时间");

    painter->restore();
}

//绘制网格线
void XBarChart::drawGrid(QPainter *painter)
{
    if(!m_GridVisible)
        return;

    //计算条形图组中条形图数目的最大值
    m_MaxBarNumInGroup = 0;
    foreach(ListBar bars, m_BarGroups)
    {
        int cnt = bars.count();
        if(m_MaxBarNumInGroup < cnt)
            m_MaxBarNumInGroup = cnt;
    }

    painter->save();

    painter->setPen(Qt::lightGray);
    qreal left = m_Margin;
    qreal top = m_Margin;
    qreal wid = this->width() - m_Margin * 2;
    qreal hei = this->height() - m_Margin * 2;

    //水平网格线
    //    qreal diffY = hei/m_MaxBarNumInGroup;
    //    for(int i = 1; i < m_MaxBarNumInGroup; i++)
    //    {
    //        painter->drawLine(left, top + diffY * i, left + wid, top + diffY * i);
    //    }

    //垂直网格线
    qreal diffX = wid/m_BarGroupNum;
    for(int i = 1; i < m_BarGroupNum; i++)
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
}

//更新X轴
void XBarChart::updateAxisX()
{
    qreal wid = this->width() - m_Margin * 2;
    qreal top = this->height() - m_Margin;
    qreal left = m_Margin;
    qreal gridWid = wid/m_BarGroupNum;//网格宽度
    for(int i=0; i < m_Times.count(); ++i)
    {
        left = m_Margin + gridWid * i;
        QString sTxt = m_Times.at(i);
        QGraphicsTextItem *text = m_TimeItems.at(i);
        text->setPlainText(sTxt);
        text->setTextWidth(gridWid);

        QTextBlockFormat format;
        format.setAlignment(Qt::AlignCenter);
        QTextCursor cursor = text->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.mergeBlockFormat(format);
        cursor.clearSelection();
        text->setTextCursor(cursor);

        text->setPos(left, top);
    }
}

//更新条形图位置和大小
void XBarChart::updateBars()
{
    if(0 == m_BarGroups.count())
        return;

    int nFlag = 1;
    if(1 == nFlag)
    {
        //方法1 所有列的条形图高度一致，有些列铺不满
        qreal left = m_Margin;
        qreal top = m_Margin;
        qreal bottom = this->height() - m_Margin;
        qreal wid = this->width() - m_Margin * 2;
        qreal hei = this->height() - m_Margin * 2;

        qreal gridWid = wid/m_BarGroupNum;//网格宽度
        qreal barWid = gridWid/2;//条形图的宽度（等于网格宽度的一半）
        qreal barTop = 0;
        qreal barHei = 0;

        for(int i = 0; i < m_BarGroups.count(); ++i)
        {
            left = m_Margin + gridWid * i;
            bottom = this->height() - m_Margin;
            ListBar barList = m_BarGroups.at(i);

            for(int j = 0; j < barList.count(); ++j)
            {
                XBar* bar = barList.at(j);
                if(!bar)
                    continue;
                BarInfo bf = bar->barInfo();
                barHei = hei / m_MaxBarNumInGroup;
                barTop = bottom - barHei;
                QRectF rect(0,0,barWid,barHei);
                bar->setRect(rect);
                bar->setPos(left + gridWid/4,barTop);

                bottom = barTop;
            }
        }
    }
    else if(2 == nFlag)
    {
        //方法2 每列都铺满，每列的每个条形图高度一致，但是和其它列的不一定相同
        qreal left = m_Margin;
        qreal top = m_Margin;
        qreal bottom = this->height() - m_Margin;
        qreal wid = this->width() - m_Margin * 2;
        qreal hei = this->height() - m_Margin * 2;

        qreal gridWid = wid/m_BarGroupNum;//网格宽度
        qreal barWid = gridWid/2;//条形图的宽度（等于网格宽度的一半）
        qreal barTop = 0;
        qreal barHei = 0;

        for(int i = 0; i < m_BarGroups.count(); ++i)
        {
            bottom = this->height() - m_Margin;
            ListBar barList = m_BarGroups.at(i);
            left = m_Margin + gridWid * i;
            for(int j = 0; j < barList.count(); ++j)
            {
                XBar* bar = barList.at(j);
                if(!bar)
                    continue;
                BarInfo bf = bar->barInfo();
                barHei = hei / barList.count();
                barTop = bottom - barHei;
                QRectF rect(0,0,barWid,barHei);
                bar->setRect(rect);
                bar->setPos(left + gridWid/4,barTop);

                bottom = barTop;
            }
        }
    }
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

//添加一组条形图
void XBarChart::addBars(ListBarInfo bars)
{
    if(0 == bars.count())
        return;

    //添加一组新的条形图
    ListBar lstBar;
    foreach (BarInfo bi, bars)
    {
        XBar *bar = new XBar();
        bar->setBarInfo(bi);
        QColor clr = m_Type2Color.value(bar->barInfo().m_Type,Qt::green);
        bar->setBackColor(clr);
        lstBar.append(bar);
        m_Scene->addItem(bar);//添加图元
    }
    m_BarGroups.append(lstBar);
    m_Times.append(bars.at(0).m_Time);//添加时间

    //如果组数超出最大值，就删除第组列条形图
    if(m_BarGroups.count() > m_BarGroupNum)
    {
        ListBar barsRemove = m_BarGroups.first();
        foreach(XBar* bar, barsRemove)
        {
            if(bar)
            {
                m_Scene->removeItem(bar);//删除图元
            }
        }
        m_BarGroups.removeFirst();
        m_Times.removeFirst();
    }

    repaintChart();

    update();
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
