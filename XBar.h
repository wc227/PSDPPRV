#ifndef XBAR_H
#define XBAR_H

#include <QGraphicsRectItem>
#include <QColor>
#include <QList>

//自定义的条形图的基本信息
struct BarInfo
{
    QString m_Time;//时间
    qreal m_Duration;//持续时间（单位:m）
    QString m_Name;//站点名称
    qreal m_MarginV;//母线电压裕度
    qreal m_MarginI;//出线电流裕度
    int m_Type;//类型——决定色系
    QString m_FilePath;//文件地址
};

//条形图，继承自QGraphicsRectItem，
//继承QObject，用来使用信号和槽
class XBar: public QGraphicsRectItem
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

//    ~XBar();

    //初始化
    void init();

    //更新提示信息
    void updateInfo();

    //绘图
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;

protected:
//    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
//    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
//    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
    BarInfo barInfo() const;

    void setBarInfo(const BarInfo &info);

    void setBackColor(QColor clr);
    void setBackColor2(QColor clr);

    void setAlpha(qreal a);

public slots:
    //打开文件
    void openFile();

protected:
    BarInfo m_BarInfo;//基本信息

    QColor m_BackColor;//背景色
    QColor m_BackColor2;//背景色2
};


typedef QList<BarInfo> BarInfoList;
typedef QList<XBar *> BarItemList;

#endif // XBAR_H
