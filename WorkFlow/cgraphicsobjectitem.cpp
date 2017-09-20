#include "CGraphicsObjectItem.h"
#include <QDebug>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QTimer>
#include <QTextCodec>

#include "citempropertydialog.h"

CGraphicsObjectItem::CGraphicsObjectItem(bool IsEditState) :
    square(4),
    b_IsEditState(IsEditState)
{
    setAcceptHoverEvents(true);

    if(b_IsEditState)
    {
        setFlags(QGraphicsItem::ItemIsMovable |
                 QGraphicsItem::ItemIsSelectable);
    }
    else
    {
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

    m_Width = staticWidth;
    m_Height = staticHeight;
    b_HoverEnter = false;
    m_CursorPos = MouseNo;

    b_LeftMouseButtonIsPressed = false;
    isLine = false;

//    m_BorderSquareSize = QSizeF(square, square);
//    createContextMenu();
//    createBorderSquare();
    //    startTimer(500);
}

/********************************
void CGraphicsObjectItem::setInterval(int msec)
{
    if(m_TimerId == -1)
    {
        startTimer(msec);
        bar->show();
    }
}
********************************/

//void CGraphicsObjectItem::setProgressMaxValue(int maxValue)
//{
//    bar->setMaximum(maxValue);
//}

void CGraphicsObjectItem::setEventNumber(int eventNumber)
{
    m_EventNumber = eventNumber;
}

void CGraphicsObjectItem::setSize(int width, int height)
{
    m_Height = height;
    m_Width = width;
}

/*********************************************
void CGraphicsObjectItem::setSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    if(m_Width < 2)
        bar->setMaximumWidth(0);
    else
        bar->setMaximumWidth(width - 2);

    if(m_Height < 2)
        bar->setMaximumHeight(0);
    else
        bar->setMaximumHeight(m_Height - 2);
    bar->setGeometry(0 + 1, 0 + 1, m_Width - 2, m_Height - 2);
    update();
}
**********************************************/

/**********************************************************************************
Function Name: boundingRect 是从基类里继承来的
Description  : 得到当前的边框矩形大小

Parameters	:  无
Return Value	:  QRectF 得到当前的大小
***********************************************************************************/
QRectF CGraphicsObjectItem::boundingRect() const
{
    return QRectF(0-square/2,0-square/2,m_Width+square+1,m_Height+square+1);
}

void CGraphicsObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & QStyle::State_Selected) {  //设置虚线
        QPen dashLinepen(Qt::black);
        dashLinepen.setWidth(1);
        dashLinepen.setStyle(Qt::DashLine);
        painter->setPen(dashLinepen);
    }
    painter->setOpacity(1);

    painter->drawPath(m_RectShape());

    if (option->state & QStyle::State_Selected) {
        QPen borderSquarePen(Qt::gray);
        painter->setPen(borderSquarePen);
        painter->setBrush(Qt::gray);
        painter->setOpacity(0.3);
        createBorderSquare();
        drawBorderSquare(painter);
    }
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


// menu
//void CGraphicsObjectItem::createContextMenu()
//{
//    contextMenu = new QMenu;
//    active1 = new QAction("SetEventNumber",0);
//    active2 = new QAction("属性",0);

//    connect(active1, SIGNAL(triggered()), this, SLOT(slotSeteventnumber()));

//    contextMenu->addAction(active1);
//    contextMenu->addAction(active2);
//}

//创建可拖拽时用的小正方形
void CGraphicsObjectItem::createBorderSquare()
{
    m_BorderSquareSize = QSizeF(square, square);
    //左顶角border square
    QPointF topLeft = QPointF(-square/2,-square/2);
    //TopLeft、TopMiddle、TopRight
    m_BorderSquare_TopLeft = QRectF(topLeft, m_BorderSquareSize);
    m_BorderSquare_TopMiddle = QRectF(topLeft + QPointF(m_Width/2,0), m_BorderSquareSize);
    m_BorderSquare_TopRight = QRectF(topLeft + QPointF(m_Width, 0), m_BorderSquareSize);

    QPointF middleLeft = QPointF(0,m_Height/2) + QPointF(-square/2,-square/2);
    //MiddleLeft、MiddleRight
    m_BorderSquare_MiddleLeft = QRectF(middleLeft, m_BorderSquareSize);
    m_BorderSquare_MiddleRight = QRectF(middleLeft + QPointF(m_Width,0), m_BorderSquareSize);

    QPointF bottomLeft = topLeft + QPointF(0,m_Height);
    //BottomLeft、BottomMiddle、BottomRight
    m_BorderSquare_BottomLeft = QRectF(bottomLeft, m_BorderSquareSize);
    m_BorderSquare_BottomMiddle = QRectF(bottomLeft + QPointF(m_Width/2,0), m_BorderSquareSize);
    m_BorderSquare_BottomRight = QRectF(bottomLeft + QPointF(m_Width,0), m_BorderSquareSize);
}

//绘画8个小正方形，进行选中拖拽时使用
void CGraphicsObjectItem::drawBorderSquare(QPainter *painter)
{
    painter->drawRect(m_BorderSquare_TopLeft);
    painter->drawRect(m_BorderSquare_TopMiddle);
    painter->drawRect(m_BorderSquare_TopRight);
    painter->drawRect(m_BorderSquare_MiddleLeft);
    painter->drawRect(m_BorderSquare_MiddleRight);
    painter->drawRect(m_BorderSquare_BottomLeft);
    painter->drawRect(m_BorderSquare_BottomMiddle);
    painter->drawRect(m_BorderSquare_BottomRight);
}

//void CGraphicsObjectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
//{
//    contextMenu->exec(event->screenPos());
//}

//鼠标按下事件
void CGraphicsObjectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(b_IsEditState)
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
//            dynamic_cast<CGraphicsScene*>(this->scene())->b_MousePressInItem = true;
        }
        QGraphicsObject::mousePressEvent(event);
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

//鼠标双击事件
void CGraphicsObjectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(b_IsEditState)
    {
//        CItemPropertyDialog *dlg = new CItemPropertyDialog(static_cast<QWidget*> (this->scene()->views().at(0)));
        CItemPropertyDialog *dlg = new CItemPropertyDialog;

        dlg->getItemPointer(this);
        dlg->setPosXSpinBox(this->scenePos().x());
        dlg->setPoxYSpinBox(this->scenePos().y());
        dlg->setWidthSpinBox(this->m_Width);
        dlg->setHeightSpinBox(this->m_Height);
        dlg->setEventNumberLineEdit(this->m_EventNumbers);
        //    dlg->setBackGroundColorLabel(this->m_CapatinName);
        dlg->setCaptatinName(this->m_CaptainName);

        //新增、showTime、vertical;
        if(this->rotation() == 270)
            dlg->setVerticalCheckBox(true);
        else
            dlg->setVerticalCheckBox(false);

        dlg->setShowTimeLineEdit(dynamic_cast<CBarItem*> (this)->getShowTime());

        dlg->exec();
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
        case MouseAtMiddleRight:        //鼠标在右中
            break;
        case MouseAtBottomLeft:         //鼠标在左下角
            break;
        case MouseAtBottomMiddle:       //鼠标在下中
            break;
        case MouseAtBottomRight:        //鼠标在右下角
            this->moveBy(0.000001,0);   //移动0.000001的距离防止刷新时有影子

            //计算偏移量
            m_Offset = event->scenePos() - m_MousePressPos; //event->scenePos()是记录产生事件的坐标
            //计算移动后的宽
            m_Width = event->scenePos().x() - this->scenePos().x();

            //计算高度，min height's : 10
            if(m_Height < 10)
            {
                m_Height = 10;
            }
            else if(m_Height == 10)
            {
                m_Height += 1;
            }
            else
            {
                m_Height = event->scenePos().y() - this->scenePos().y();
            }

//            if(m_Height < 0)
//                m_Height = 0;
//            else

            update();
            /** setMaximumWidth **/
//            bar->setMaximumWidth(m_Width - 2);
//            bar->setMaximumHeight(m_Height - 2);
//            bar->setMinimumHeight(m_Height - 2);
//            bar->setGeometry(0 + 2, 0 + 2, m_Width - 3, m_Height - 3);
            break;
        default:
            QGraphicsObject::mouseMoveEvent(event); //将事件返回给基类，调用基类的mouseMoveEvent;
            break;
        }
    }

}

//鼠标悬浮事件,判断鼠标的位置进行拖拽大小
void CGraphicsObjectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!shape().contains(event->pos()) && !b_IsEditState
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
        if(m_BorderSquare_TopLeft.contains(event->pos()))
        {
            setCursor(Qt::SizeFDiagCursor);
            m_CursorPos = MouseAtTopLeft;
        }
        else if(m_BorderSquare_TopMiddle.contains(event->pos()))
        {
            setCursor(Qt::SizeVerCursor);
            m_CursorPos = MouseAtTopMiddle;
        }
        else if(m_BorderSquare_TopRight.contains(event->pos()))
        {
            setCursor(Qt::SizeBDiagCursor);
            m_CursorPos = MouseAtTopRight;
        }
        else if(m_BorderSquare_MiddleLeft.contains(event->pos()))
        {
            setCursor(Qt::SizeHorCursor);
            m_CursorPos = MouseAtMiddleLeft;
        }
        else if(m_BorderSquare_MiddleRight.contains(event->pos()))
        {
            setCursor(Qt::SizeHorCursor);
            m_CursorPos = MouseAtMiddleRight;
        }
        else if(m_BorderSquare_BottomLeft.contains(event->pos()))
        {
            setCursor(Qt::SizeBDiagCursor);
            m_CursorPos = MouseAtBottomLeft;
        }
        else if(m_BorderSquare_BottomMiddle.contains(event->pos()))
        {
            setCursor(Qt::SizeVerCursor);
            m_CursorPos = MouseAtBottomMiddle;
        }
        else if(m_BorderSquare_BottomRight.contains(event->pos()))
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

//鼠标悬浮进入事件
void CGraphicsObjectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(!shape().contains(event->pos()))
        return;
    b_HoverEnter = true;
    update();
    qDebug()<< "enter";
}

//鼠标悬浮离开事件
void CGraphicsObjectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    m_CursorPos = MouseNo;
    b_HoverEnter = false;
    update();
//    if(dynamic_cast<CGraphicsScene*>(this->scene())->b_MousePressInItem)
//    {
//        dynamic_cast<CGraphicsScene*>(this->scene())->b_MousePressInItem = false;
//    }
}

/* void CGraphicsObjectItem::timerEvent(QTimerEvent *e)
{
    if(m_TimerId == -1)
    {
        m_TimerId = e->timerId();
    }
    upValue++;
    if(upValue >= bar->maximum())
    {
        upValue = 0;
        bar->hide(); 
        this->killTimer(m_TimerId);
        m_TimerId = -1;
    }
    bar->setValue(upValue);
}*/
