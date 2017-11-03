#ifndef XBAR_H
#define XBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QList>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

//自定义的条形图的基本信息
struct BarInfo
{
    QString m_Time;//时间
    long m_Duration;//持续时间（单位:m）
    QString m_Name;//站点名称
    qreal m_Margin;//裕度——决定颜色透明度
    int m_Type;//类型——决定色系
    QString m_FilePath;//文件地址
};

class XBar:public QGraphicsRectItem
{
public:
    enum {Type = UserType + 1 };

    virtual int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    XBar(QGraphicsItem *parent = Q_NULLPTR);
    XBar(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    XBar(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

    void init();

    void initAnimation();

    void startAnimation();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;

    void updateInfo();

protected:
//    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
//    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
    BarInfo barInfo() const;

    void setBarInfo(const BarInfo &info);

    void setBackColor(QColor clr);

    void setAlpha(qreal a);

protected:
    BarInfo m_BarInfo;//基本信息

    QColor m_BackColor;//背景色

    QGraphicsItemAnimation *animation;

    QTimeLine *timer;
};


typedef QList<BarInfo> ListBarInfo;
typedef QList<XBar *> ListBar;

#endif // XBAR_H
