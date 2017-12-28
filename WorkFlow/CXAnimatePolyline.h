#ifndef CXAnimatePolyline_H
#define CXAnimatePolyline_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QTimer>
#include <QPen>

//一个动态的折线，显示流水滚动效果（线段沿着折线流动），线段终点可显示箭头表示方向
class CXAnimatePolyline : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1001 };
    int type() const{return Type;}

    explicit CXAnimatePolyline(const QPolygonF &poly = QPolygonF() << QPointF(0,0) << QPointF(20,20),
                            bool edit = true,QGraphicsItem *parent = 0);

    ~CXAnimatePolyline();

    //清空指针
    void releaseAll();

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;
//    virtual bool contains(const QPointF &point) const Q_DECL_OVERRIDE;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);    

protected:
    //初始化
    void init();

public:
    //设置名称
    void setName(QString cap);

    //获取名称
    QString getName() const;

    //设置事件编号
    void setEventNumbers(const QString &evts);

    //获取事件编号
    QString getEventNumbers() const;

    //是否是编辑模式
    bool isEditMode() const;

    //启用编辑模式
    void enableEditMode(bool edit = true);

    //获取点pt所处的句柄（矩形）
    QRectF getHandleRect(QPointF pt) const;

    //移动指定序号的句柄
    void moveHandleTo(int id, QPointF pt);

    //显示或隐藏箭头
    void showArrow(bool show = true);

    //是否显示箭头
    bool isShowArrow() const;

    //是否反向动画
    bool isAnimateReverse() const;

    //启用反向动画
    void enableAnimateReverse(bool val = true);

public slots:

    //开始动画
    void startAnimation();

    //停止动画
    void stopAnimation();

    //暂停动画，其实和停止动画一样
    void pauseAnimation();

    //更新虚线偏离距离
    void updateDashOffset();

    //编辑属性
    void editProperty();

protected:    
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
//    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

protected:    
    QGraphicsPolygonItem *m_pItemArrow;//三角形箭头图元（子图）
    QString m_sName;//名称
    QString m_sEventNumbers;//事件号
    bool m_bIsEditMode;//是否是编辑模式

    bool m_bShiftKeyDown;//shift键是否按下

    QPointF m_ptDown;//鼠标按下时的位置（当前图元坐标）
    QPointF m_ptPrevious;//鼠标上一个位置（当前图元坐标）
    QPointF m_ptNow;//鼠标当前位置（当前图元坐标）
    int m_iHandleID;//选中的句柄编号（从0开始）

    bool m_bAnimateReverse;//动画反向
    QPen m_dashPen;//虚线
    int m_nDashOffset;//虚线上短横线的偏离（其实反向偏离）
    QTimer m_timerUpdate;//刷新定时器
    int m_nTimerInterval;//定时器间隔时间
    int m_nDurationMax;//最大动画显示时间
    int m_nDurationNow;//已经显示的时间

    static int s_handleSize;//句柄大小（正方形边长）
    static int s_blankSizeOfDashline;//虚线上短横线之间的空白长度
};

#endif // CXAnimatePolyline_H
