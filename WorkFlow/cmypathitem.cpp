/**************************************************************************
**
**  文件名称： cmypathitem.cpp
**  创建日期： 2015/5/13
**  创建作者： xiaotan
**  电子邮箱： 421053694@qq.com
**
**************************************************************************/

#include "cmypathitem.h"
#include <QDebug>

CMyPathItem::CMyPathItem(QObject *parent, bool edit) :
    QObject(parent),
    isEdit(edit)
{
    if(isEdit)
    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    //setAcceptsHoverEvents(true);
        setAcceptHoverEvents(true);
    }
    var = 0;
    timer = new QTimer(this);
    timer->setInterval(200);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(myTimerOut()));
}

void CMyPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isEdit)
    {
        painter->save();
        painter->setPen(QPen(Qt::black,2));
        painter->setRenderHint(QPainter::Antialiasing);
        QGraphicsPathItem::paint(painter,option,widget);
        painter->restore();
    }
    painter->save();
    QPen pen = QPen(Qt::black,2,Qt::DashLine,Qt::RoundCap,Qt::RoundJoin);
    pen.setDashOffset(var);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPolyline(m_polygon);
    painter->restore();

    if(isEdit)
    {
        for(int i=0; i<m_rects.size(); i++)
        {
            painter->drawRect(m_rects.at(i));
        }
    }
}

void CMyPathItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(this->isSelected())
    {
        for(int i=0; i<m_rects.size(); i++)
        {
            if(m_rects.at(i).contains(event->pos()))
            {
                qDebug() << "The Rect Number:" << i << "contains mouse pos";
                enterRect = true;
                rectNumber = i;
            }
        }
    }
    else
    {
        if(rectNumber >=0)
            rectNumber = -1;
    }
}

void CMyPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!mousePressed)
        mousePressed = true;
    QGraphicsPathItem::mousePressEvent(event);
}

void CMyPathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mousePressed)
        mousePressed = false;
    if(enterRect)
        enterRect = false;
    qDebug() << "enterRect:" << enterRect;
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void CMyPathItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << enterRect;
    if(enterRect)
    {
        if(m_polygon.size() > rectNumber && rectNumber >=0)
        {
            m_polygon[rectNumber] = event->pos();
            qDebug() << m_polygon[rectNumber];
            ///////////////////////////////
            int x = event->pos().x()-3;
            int y = event->pos().y()-3;
            QRectF rectf(x,y,6,6);
            ///////////////////////////////
            m_rects[rectNumber] = rectf;
            QPainterPath path;
            path.addPolygon(m_polygon);
            this->setPath(path);
            //this->update(boundingRect().adjusted(-1,-1,1,1));
        }
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}

void CMyPathItem::myTimerOut()
{
    if(var > 11)
        var = 0;
    var += 2 ;
    update();
}

QRectF CMyPathItem::boundingRect() const
{
    QRectF rectf = QGraphicsPathItem::boundingRect();
    return rectf.adjusted(-3,-3,3,3);
}

void CMyPathItem::editEnd()
{
    m_rects.clear();
    for(int i=0; i<m_polygon.count(); i++)
    {
        int x = m_polygon.at(i).x()-3;
        int y = m_polygon.at(i).y()-3;
        QRectF rectf(x,y,6,6);
        m_rects << rectf;
    }
}

double CMyPathItem::getK(QPointF p1, QPointF p2)
{
    return (p2.y() - p1.y())/(p2.x() - p1.x());
}
