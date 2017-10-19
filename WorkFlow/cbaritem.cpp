#include "cbaritem.h"

CBarItem::CBarItem(bool isEditState):
    CGraphicsObjectItem(isEditState),
    m_pAnimation(0),
    m_ShowTime(2000),
    m_widthAni(m_Width)
{
    m_bLoopAnimation = false;

    if(isEditState)
        setVisible(true);//编辑态显示
    else
        setVisible(false);//运行态不显示，只有执行动画时才显示

    setZValue(1);
    initAnimation();
}

QRectF CBarItem::getAnmationSize()
{
    return QRectF(0,0,m_Width,m_Height);
}

void CBarItem::setAnmationSize(QRectF sz)
{
    m_Width = sz.width();
    m_Height = sz.height();
    update();
}

void CBarItem::setSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    update();

    m_widthAni = width;
    if(m_pAnimation)
    {
        m_pAnimation->setStartValue(QRectF(0,0,0,m_Height));
        m_pAnimation->setEndValue(QRectF(0,0,m_widthAni,m_Height));
    }
}

int CBarItem::getEventNumber()
{
    return m_EventNumber;
}

//QPointF CBarItem::Geometry() const
//{
//    return this->pos();
//}

//void CBarItem::SetGeometry(QPointF p)
//{
//    setPos(p);
//}

//更新大小
//QRectF CBarItem::updateRect()
//{
//    return QRectF(this->scenePos(), QSize(m_Width, m_Height));
//}

//获取显示时间
int CBarItem::getShowTime()
{
    return m_ShowTime/1000;
}

//设置显示时间
void CBarItem::setShowTime(int time)
{
    m_ShowTime = time*1000;
    if(m_pAnimation)
        m_pAnimation->setDuration(m_ShowTime);
}

void CBarItem::setCaptainName(const QString &name)
{
    m_CaptainName = name;
}

QString CBarItem::getCaptainName()
{
    return m_CaptainName;
}

void CBarItem::setEventNumber(QString evts)
{
	m_EventNumbers = evts;
}

QString CBarItem::getEventNumbers()
{
	return m_EventNumbers;
}

void CBarItem::enableLoopAnimation(bool loop)
{
    m_bLoopAnimation = loop;
    if(m_bLoopAnimation && m_pAnimation)
        connect(m_pAnimation, &QPropertyAnimation::finished,this,&CBarItem::startAnimation);//可循环动画
}

bool CBarItem::isLoopAnimation()
{
    return m_bLoopAnimation;
}

//重绘
void CBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //渐变色充填
//    QLinearGradient linearGradient(0,0,0,m_Height);
//    linearGradient.setColorAt(0, Qt::green);
//    linearGradient.setColorAt(0.5, Qt::white);
//    linearGradient.setColorAt(1.0, Qt::green);
//    painter->fillPath(shape(),QBrush(linearGradient));
    painter->fillPath(shape(),QBrush(Qt::green));

    CGraphicsObjectItem::paint(painter,option,widget);

    if(!b_IsEditState && m_widthAni == m_Width)
        this->hide();
}

//初始化动画
void CBarItem::initAnimation()
{
    if(!m_pAnimation)
        m_pAnimation = new QPropertyAnimation(this, "anmationSize");

    m_pAnimation->setDuration(m_ShowTime);
    m_pAnimation->setStartValue(QRectF(0,0,0,m_Height));
    m_pAnimation->setEndValue(QRectF(0,0,m_Width,m_Height));
}

//运行动画
void CBarItem::startAnimation()
{
    if(!m_pAnimation)
        initAnimation();

    if(m_pAnimation->state() == QAbstractAnimation::Stopped)//判断动画是否停止
    {
        show();//显示该对象

        m_pAnimation->setDuration(m_ShowTime);
        m_pAnimation->setStartValue(QRectF(0,0,0,m_Height));
        m_pAnimation->setEndValue(QRectF(0,0,m_widthAni,m_Height));

        m_pAnimation->start();//运行动画
    }
}
