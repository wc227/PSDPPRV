#include "CGraphicsObjectItem.h"
#include <QDebug>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QTimer>
#include <QTextCodec>
#include <QGraphicsView>
#include "citempropertydialog.h"

CGraphicsObjectItem::CGraphicsObjectItem(bool IsEditState) :
    c_squareLength(8),
    m_bEditMode(IsEditState)
{
    setAcceptHoverEvents(true);

    //编辑态，可选，可移动
    if(m_bEditMode)
    {
        setFlags(QGraphicsItem::ItemIsMovable |
                 QGraphicsItem::ItemIsSelectable);
    }
    else
    {
        //运行态，不可选，不可移动编辑
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

    m_Width = staticWidth;
    m_Height = staticHeight;
    b_HoverEnter = false;
    m_CursorPos = MouseNo;

    b_LeftMouseButtonIsPressed = false;
    isLine = false;
}

void CGraphicsObjectItem::enabelEditMode(bool mode)
{
    if(m_bEditMode != mode)
    {
        m_bEditMode = mode;
        if(m_bEditMode)
        {
            setFlag(QGraphicsItem::ItemIsSelectable, true);
            setFlag(QGraphicsItem::ItemIsFocusable, true);
            setFlag(QGraphicsItem::ItemIsMovable, true);
        }
        else
        {
            setFlag(QGraphicsItem::ItemIsSelectable, false);
            setFlag(QGraphicsItem::ItemIsFocusable, false);
            setFlag(QGraphicsItem::ItemIsMovable, false);
        }
    }
}

void CGraphicsObjectItem::setSize(int width, int height)
{
    m_Height = height;
    m_Width = width;
    update();
}


/**********************************************************************************
Function Name: boundingRect 是从基类里继承来的
Description  : 得到当前的边框矩形大小
Parameters	:  无
Return Value	:  QRectF 得到当前的大小
***********************************************************************************/
QRectF CGraphicsObjectItem::boundingRect() const
{
    return QRectF(0-c_squareLength/2,0-c_squareLength/2,m_Width+c_squareLength+1,m_Height+c_squareLength+1);
}

void CGraphicsObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);

    //编辑态，绘制虚线边框
    if(m_bEditMode && isSelected())
    {
        QPen dashLinepen(Qt::red);
        dashLinepen.setWidth(1);
        dashLinepen.setStyle(Qt::DashLine);
        painter->setPen(dashLinepen);
    }
    painter->drawRect(QRectF(0, 0, m_Width, m_Height));

    if(m_bEditMode && isSelected())
    {
        createBorderSquare();
        drawBorderSquare(painter);
    }

    painter->restore();
}

QPainterPath CGraphicsObjectItem::shape()
{
    return m_RectShape();
}

QPainterPath CGraphicsObjectItem::m_RectShape()
{
    QPainterPath rectPath;
    rectPath.addRect(QRect(0,0,m_Width,m_Height));
    return rectPath;
}

//创建可拖拽时用的小正方形
void CGraphicsObjectItem::createBorderSquare()
{    
    QSizeF sizeHandle;//句柄矩形的大小
    sizeHandle = QSizeF(c_squareLength, c_squareLength);
//    //左顶角border c_squareLength
    QPointF topLeft = QPointF(-c_squareLength/2,-c_squareLength/2);
//    //TopLeft、TopMiddle、TopRight
//    m_BorderSquare_TopLeft = QRectF(topLeft, sizeHandle);
//    m_BorderSquare_TopMiddle = QRectF(topLeft + QPointF(m_Width/2,0), sizeHandle);
//    m_BorderSquare_TopRight = QRectF(topLeft + QPointF(m_Width, 0), sizeHandle);

//    QPointF middleLeft = QPointF(0,m_Height/2) + QPointF(-c_squareLength/2,-c_squareLength/2);
//    //MiddleLeft、MiddleRight
//    m_BorderSquare_MiddleLeft = QRectF(middleLeft, sizeHandle);
//    m_BorderSquare_MiddleRight = QRectF(middleLeft + QPointF(m_Width,0), sizeHandle);

    QPointF bottomLeft = topLeft + QPointF(0,m_Height);
//    //BottomLeft、BottomMiddle、BottomRight
//    m_BorderSquare_BottomLeft = QRectF(bottomLeft, sizeHandle);
//    m_BorderSquare_BottomMiddle = QRectF(bottomLeft + QPointF(m_Width/2,0), sizeHandle);
    m_BorderSquare_BottomRight = QRectF(bottomLeft + QPointF(m_Width,0), sizeHandle);
}

//绘画8个小正方形，进行选中拖拽时使用
void CGraphicsObjectItem::drawBorderSquare(QPainter *painter)
{
//    painter->fillRect(m_BorderSquare_TopLeft);
//    painter->fillRect(m_BorderSquare_TopMiddle);
//    painter->fillRect(m_BorderSquare_TopRight);
//    painter->fillRect(m_BorderSquare_MiddleLeft);
//    painter->fillRect(m_BorderSquare_MiddleRight);
//    painter->fillRect(m_BorderSquare_BottomLeft);
//    painter->fillRect(m_BorderSquare_BottomMiddle);
    painter->fillRect(m_BorderSquare_BottomRight,Qt::blue);
}

//鼠标按下事件
void CGraphicsObjectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_bEditMode)
    {
        if(event->button() == Qt::LeftButton)
        {
            if(m_CursorPos != MouseNo)
                m_MousePressPos = event->scenePos();
            b_LeftMouseButtonIsPressed = true;

            m_SpeHeight = m_Height;
        }

        if(event->button() == Qt::RightButton)
        {
            if(this->isSelected())
                setSelected(false);
        }
        QGraphicsObject::mousePressEvent(event);
    }
}

//鼠标移动事件
void CGraphicsObjectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(b_LeftMouseButtonIsPressed)      //判断左键是否按下
    {
        m_ScenePos = this->scenePos();  //获取鼠标按下时在scene中的坐标

        switch (m_CursorPos)            //判断鼠标的位置
        {
        case MouseAtTopLeft:            //鼠标在左上角
            break;
        case MouseAtTopMiddle:          //鼠标在上中
            break;
        case MouseAtTopRight:           //鼠标在右上角
            break;
        case MouseAtMiddleLeft:         //鼠标在左中
            break;
        case MouseAtBottomLeft:         //鼠标在左下角
            break;
        case MouseAtMiddleRight:        //鼠标在右中
            break;
        case MouseAtBottomMiddle:       //鼠标在下中
            break;
        case MouseAtBottomRight:        //鼠标在右下角
        {
            this->moveBy(0.000001,0);   //移动0.000001的距离防止刷新时有影子
            //计算偏移量
            m_Offset = event->scenePos() - m_MousePressPos; //event->scenePos()是记录产生事件的坐标
            //计算移动后的宽
            m_Width = event->scenePos().x() - m_ScenePos.x();
            m_Height = event->scenePos().y() - m_ScenePos.y();
            if(m_Width < 10)
                m_Width = 10;
            if(m_Height < 10)
                m_Height = 10;

            update();
        }
            break;
        default:
            QGraphicsObject::mouseMoveEvent(event); //将事件返回给基类，调用基类的mouseMoveEvent;
            break;
        }
    }
}

//鼠标释放事件
void CGraphicsObjectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseReleasePos = event->scenePos();
        b_LeftMouseButtonIsPressed = false;
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

//鼠标悬浮进入事件
void CGraphicsObjectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(!shape().contains(event->pos()))
        return;
    b_HoverEnter = true;
    update();
    qDebug()<< "enter";
}

//鼠标悬浮事件,判断鼠标的位置进行拖拽大小
void CGraphicsObjectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!shape().contains(event->pos()) && !m_bEditMode
            && !isLine)
    {
        if(b_HoverEnter)
        {
            b_HoverEnter = false;
            update();
        }
        return;
    }

    if(this->isSelected())
    {
//        if(m_BorderSquare_TopLeft.contains(event->pos()))
//        {
//            setCursor(Qt::SizeFDiagCursor);
//            m_CursorPos = MouseAtTopLeft;
//        }
//        else if(m_BorderSquare_TopMiddle.contains(event->pos()))
//        {
//            setCursor(Qt::SizeVerCursor);
//            m_CursorPos = MouseAtTopMiddle;
//        }
//        else if(m_BorderSquare_TopRight.contains(event->pos()))
//        {
//            setCursor(Qt::SizeBDiagCursor);
//            m_CursorPos = MouseAtTopRight;
//        }
//        else if(m_BorderSquare_MiddleLeft.contains(event->pos()))
//        {
//            setCursor(Qt::SizeHorCursor);
//            m_CursorPos = MouseAtMiddleLeft;
//        }
//        else if(m_BorderSquare_MiddleRight.contains(event->pos()))
//        {
//            setCursor(Qt::SizeHorCursor);
//            m_CursorPos = MouseAtMiddleRight;
//        }
//        else if(m_BorderSquare_BottomLeft.contains(event->pos()))
//        {
//            setCursor(Qt::SizeBDiagCursor);
//            m_CursorPos = MouseAtBottomLeft;
//        }
//        else if(m_BorderSquare_BottomMiddle.contains(event->pos()))
//        {
//            setCursor(Qt::SizeVerCursor);
//            m_CursorPos = MouseAtBottomMiddle;
//        }
//        else
        if(m_BorderSquare_BottomRight.contains(event->pos()))
        {
            setCursor(Qt::SizeFDiagCursor);
            m_CursorPos = MouseAtBottomRight;
        }
        else
        {
            setCursor(Qt::ArrowCursor);
            m_CursorPos = MouseNo;
        }
    }
    else
    {
        b_HoverEnter = true;
        update();
    }
}

//鼠标悬浮离开事件
void CGraphicsObjectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    m_CursorPos = MouseNo;
    b_HoverEnter = false;
    update();
}
