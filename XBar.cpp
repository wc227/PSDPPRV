#include "XBar.h"
#include <QPainter>
#include <QMessageBox>
#include <QProcess>

XBar::XBar(QGraphicsItem* parent)
    :QGraphicsRectItem(parent)
{
    init();
}

XBar::XBar(const QRectF &rect, QGraphicsItem *parent)
    :QGraphicsRectItem(rect,parent)
{
    init();
    setRect(rect);
}

XBar::XBar(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    :QGraphicsRectItem(x,y,w,h,parent)
{
    init();
    setRect(x,y,w,h);
}

//XBar::~XBar()
//{

//}

void XBar::init()
{
//    setFlags(ItemIsSelectable|ItemIsFocusable);
//    setAcceptHoverEvents(true);

    setRect(0,0,100,100);
    updateInfo();

    m_BackColor = Qt::red;
    m_BackColor.setAlphaF(1);
}

BarInfo XBar::barInfo() const
{
    return m_BarInfo;
}

void XBar::setBarInfo(const BarInfo &info)
{
    m_BarInfo = info;
    updateInfo();
}

void XBar::setBackColor(QColor clr)
{
    m_BackColor = clr;
}

void XBar::setBackColor2(QColor clr)
{
    m_BackColor2 = clr;
}

void XBar::setAlpha(qreal a)
{
    m_BackColor.setAlphaF(a);
}

void XBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QGraphicsRectItem::paint(painter, option, widget);
    painter->save();

    //设置透明度(计算出来的)
//    if(m_BarInfo.m_MarginV > 0 && m_BarInfo.m_MarginI > 0 )
//        m_BackColor.setAlphaF(0.5 + (m_BarInfo.m_MarginV + m_BarInfo.m_MarginI) * 0.5);
//    else if(m_BarInfo.m_MarginV > 0)
//        m_BackColor.setAlphaF(0.5 + m_BarInfo.m_MarginV);
//    else if(m_BarInfo.m_MarginI > 0 )
//        m_BackColor.setAlphaF(0.5 + m_BarInfo.m_MarginI);

    QLinearGradient gradient;
    gradient.setStart(rect().topLeft().toPoint());
    gradient.setFinalStop(rect().topRight().toPoint());
    gradient.setColorAt(0, m_BackColor);
    gradient.setColorAt(1, m_BackColor2);
    QBrush brush(gradient);
    painter->fillRect(rect(),brush);

    painter->setPen(Qt::white);
    painter->drawRect(rect());

//    QColor clrTxt(255-m_BackColor.red(),255-m_BackColor.green(),255-m_BackColor.blue());//颜色取反
    QColor clrTxt = Qt::white;
    painter->setPen(clrTxt);
    painter->setFont(QFont(QStringLiteral("黑体"),12));
    painter->drawText(rect(),Qt::AlignCenter,m_BarInfo.m_Name);
    painter->restore();
}

void XBar::updateInfo()
{
    QString sTxt = QString("时间：%1\n持续时长：%2\n名称：%3\n母线电压裕度：%4\n出线电流裕度：%5\n文件路径：%6")
            .arg(m_BarInfo.m_Time)
            .arg(m_BarInfo.m_Duration)
            .arg(m_BarInfo.m_Name)
            .arg(m_BarInfo.m_MarginV)
            .arg(m_BarInfo.m_MarginI)
            .arg(m_BarInfo.m_FilePath);
    this->setToolTip(sTxt);//设置提示内容
}

//双击提示
void XBar::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    openFile();
}

//打开文件
void XBar::openFile()
{
    if(QFile::exists(m_BarInfo.m_FilePath))
    {
        //调用系统自带程序打开文本文件（只适合windows系统）
        QProcess::execute("explorer " + m_BarInfo.m_FilePath);
    }
    else
    {
        //提示文件不存在
        QMessageBox::warning(0
                             ,"提示"
                             ,QString("文件：\'%1\' 不存在").arg(m_BarInfo.m_FilePath)
                             ,QMessageBox::Ok);
    }
}
