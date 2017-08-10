#ifndef MYPOLYLINEITEM_H
#define MYPOLYLINEITEM_H

#include "QGraphicsItem"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

//自定义的折线类，可以绘制一条折线
class CMyPolylineItem : public QAbstractGraphicsShapeItem
{
public:
    enum { Type = UserType + 2 };
    int type() const {return Type;};

    CMyPolylineItem(QGraphicsItem * parent = 0);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

public:
    //设置计算机信息
    void SetPolygon(const QPolygonF &ref)
    {
        myPolygon = ref;
    }

    QPolygonF Polygon() const
    {
        return myPolygon;
    }

private:
    QPolygonF myPolygon;
};

#endif // MYPOLYLINEITEM_H
