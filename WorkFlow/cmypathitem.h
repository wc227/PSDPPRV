/**************************************************************************
**
**  文件名称： cmypathitem.h
**  创建日期： 2015/5/13
**  创建作者： xiaotan
**  电子邮箱： 421053694@qq.com
**
**************************************************************************/

#ifndef CMYPATHITEM_H
#define CMYPATHITEM_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QTimer>

class CMyPathItem : public QObject , public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit CMyPathItem(QObject *parent = 0, bool edit = false);

    enum { Type = UserType + 3 }; //Item Type, 用于辨别item的类

    int type() const{
        return Type;
    }

    ///@brief pen property
    struct PENDATA{
        int penWidth;
        QColor penColor;
    };
    ///@brief static
    bool isEdit;

    QPolygonF m_polygon;
    QPolygonF& getPolygon(){
        return m_polygon;
    }

    void setPolygon(const QPolygonF &p){
        m_polygon = p;
    }

    QRectF boundingRect() const;
    QList<QRectF> m_rects;
    void editEnd();

    bool mousePressed;  //用于鼠标按下
    bool enterRect;
    int rectNumber;

    static double getK(QPointF p1, QPointF p2);
    bool timerOut;
    QTimer *timer;
    int var;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void myTimerOut();

};

#endif // CMYPATHITEM_H
