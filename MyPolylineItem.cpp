#include "MyPolylineItem.h"
#include <QPen>

CMyPolylineItem::CMyPolylineItem(QGraphicsItem * parent)
    :QAbstractGraphicsShapeItem(parent)
{
    QPen pen(Qt::darkGreen);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(2);
    setPen(pen);
}

void CMyPolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(pen());
    painter->drawPolyline(myPolygon);
    painter->restore();
}


QRectF CMyPolylineItem::boundingRect() const
{
    return shape().controlPointRect();
}

QPainterPath CMyPolylineItem::shape() const
{
    QPainterPath path;
    path.addPolygon(myPolygon);
    return path;
}
