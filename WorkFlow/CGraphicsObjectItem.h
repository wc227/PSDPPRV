#ifndef CGRAPHICSOBJECTITEM_H
#define CGRAPHICSOBJECTITEM_H

#include <QtGui>
#include <QGraphicsObject>
#include <QPainterPath>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QColor>
#include <QSizeF>


class CItemPrivate
{
public:
    int     m_EventNumber;
	QString m_EventNumbers;
    QPointF m_ScenePos;
    QPointF m_MousePressPos;
    QPointF m_MouseReleasePos;
    QColor  m_Color;
    QSizeF  m_Size;
    QString m_CaptainName;
    int     m_ShowTime;
};


class CGraphicsObjectItem : public QGraphicsObject, public CItemPrivate
{
//    Q_OBJECT
//    Q_PROPERTY(QRectF sizeChange READ getAnmationSize WRITE setAnmationSize)
//    enum { Type = UserType + 5 }; //UserTyped是系统定义的 65536 根据Type来判断子类的类型
public:
    enum CursorPos{MouseAtTopLeft,
                   MouseAtTopMiddle,
                   MouseAtTopRight,
                   MouseAtMiddleLeft,
                   MouseAtMiddleRight,
                   MouseAtBottomLeft,
                   MouseAtBottomMiddle,
                   MouseAtBottomRight,
                   MouseNo
                  } m_CursorPos;

public:
    explicit CGraphicsObjectItem(bool IsEditState);

    static const int staticWidth = 50;
    static const int staticHeight = 30;
    const double square;
    double m_Width;
    double m_Height;

    bool b_HoverEnter;
    bool b_LeftMouseButtonIsPressed;

    bool b_IsEditState;

    QSizeF m_BorderSquareSize;
    QPointF m_Offset; //用于拖拽放大和缩小产生的偏移量

    //
    bool isLine;

    virtual void setEventNumber(int eventNumber);
//    void setEventNumber(QList<int> eventNumber);
    /**
     * @brief setSize
     * @param width
     * @param height
     */
    virtual void setSize(int width, int height);
public:
    /**
      *@brief ID.唯一的标示与ObjectName一样
      */
//    QString m_Id;
    /**
      *@brief 描绘Item的形状，矩形、圆角矩形……
      */
    virtual QPainterPath m_RectShape();
    /**
      *@brief 得到Item的形状
      */
    virtual QPainterPath shape();

    /** pure virtual**/
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //用于描绘选中时四边用的小方框
    //Top Border
    QRectF m_BorderSquare_TopLeft;
    QRectF m_BorderSquare_TopMiddle;
    QRectF m_BorderSquare_TopRight;
    //Middle Border
    QRectF m_BorderSquare_MiddleLeft;
    QRectF m_BorderSquare_MiddleRight;
    //Bottom Border
    QRectF m_BorderSquare_BottomLeft;
    QRectF m_BorderSquare_BottomMiddle;
    QRectF m_BorderSquare_BottomRight;
    void createBorderSquare();
    void rePosBorderSquare();

    /**
      *@brief 画item周边放大缩小的小正方形
      */
    void drawBorderSquare(QPainter *painter);

private:
    int m_SpeHeight; //用于当鼠标按下时临时存储当前的高

protected:
    /**
      *@brief 右键菜单事件
      */
//    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    /**
      *@brief 鼠标按下事件
      */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /**
      *@brief 鼠标抬起事件
      */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /**
      *@brief 鼠标双击事件
      */
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    /**
      *@brief 鼠标移动事件(用于拖拽放大缩小用)
      */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    /**
      *@brief 鼠标悬浮移动范围事件(用于拖拽放大缩小用)
      */
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    /**
      *@brief 鼠标悬浮进入Item范围事件
      */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /**
      *@brief 鼠标悬浮离开Item范围事件
      */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:

public slots:

};

#endif // CGRAPHICSOBJECTITEM_H
