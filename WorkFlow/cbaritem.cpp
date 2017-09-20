#include "cbaritem.h"

CBarItem::CBarItem(bool isEditState):
    CGraphicsObjectItem(isEditState),
    m_ShowTime(2000)
{
    setZValue(1);
    p = new QPropertyAnimation(this, "sizeChange");
    p->setDuration(m_ShowTime);
//    p->setStartValue(QRectF(0,0,0,m_Height));
//    p->setEndValue(QRectF(0,0,m_Width,m_Height));
//    this->rotate(270);
}

QRectF CBarItem::getAnmationSize()
{
    return QRectF(0,0,m_Width,m_Height);
}

void CBarItem::setAnmationSize(QRectF s)
{
    setSize(s.width(), s.height());
}

void CBarItem::setSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    update();
}

int CBarItem::getEventNumber()
{
    return m_EventNumber;
}

QPointF CBarItem::m_Geometry() const
{
    return this->pos();
}

void CBarItem::m_SetGeometry(QPointF p)
{
    setPos(p);
}

//运行动画
void CBarItem::startAnimation()
{
    if(p->state() == p->Stopped)//判断动画是否停止
    {
        this->show();//显示该对象
        if(p->startValue().toRectF() != QRectF(0,0,0,m_Height))
            p->setStartValue(QRectF(0,0,0,m_Height));

        if(p->endValue().toRectF() != QRectF(0,0,m_Width,m_Height))
            p->setEndValue(QRectF(0,0,m_Width,m_Height));

        width = m_Width;
        p->start();//运行动画
    }
}

//更新大小
QRectF CBarItem::updateRect()
{
    return QRectF(this->scenePos(), QSize(m_Width, m_Height));
}

//获取显示时间
int CBarItem::getShowTime()
{
    return m_ShowTime/1000;
}

//设置显示时间
void CBarItem::setShowTime(int time)
{
    m_ShowTime = time*1000;
    p->setDuration(m_ShowTime);
}

void CBarItem::setCaptainName(const QString &name)
{
    m_CaptainName = name;
}

QString CBarItem::getCaptainName()
{
    return m_CaptainName;
}

//重绘
void CBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(Qt::white);
    painter->setPen(Qt::NoPen);

    if(b_IsEditState)
    {
        if (option->state & QStyle::State_Selected) {  //设置虚线
            QPen dashLinepen(Qt::black);
            dashLinepen.setWidth(1);
            dashLinepen.setStyle(Qt::DashLine);
            painter->setPen(dashLinepen);
        }
    }
//    painter->setOpacity(1);

    QLinearGradient linearGradient(0,0,0,m_Height);
    linearGradient.setColorAt(0.2, Qt::white);
    linearGradient.setColorAt(0.6, Qt::green);
    linearGradient.setColorAt(1.0, Qt::black);
    painter->setBrush(QBrush(linearGradient));

    painter->drawRect(QRectF(0, 0, m_Width, m_Height));

    if(b_IsEditState)
    {
        if (option->state & QStyle::State_Selected) {
            QPen borderSquarePen(Qt::gray);
            painter->setPen(borderSquarePen);
//            painter->setBrush(Qt::gray);
//            painter->setOpacity(0.7);
            createBorderSquare();
            drawBorderSquare(painter);
        }
    }

    if(!b_IsEditState && width == m_Width)
        this->hide();

//    CGraphicsObjectItem::paint(painter, option, widget);
}

void CBarItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    qDebug() << "sss";
}

void CBarItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    CGraphicsObjectItem::mouseMoveEvent(event);
}

void CBarItem::setEventNumber(QString evts)
{
	m_EventNumbers = evts;
}

QString CBarItem::getEventNumbers()
{
	return m_EventNumbers;
}
