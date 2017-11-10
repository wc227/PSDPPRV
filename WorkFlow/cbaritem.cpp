#include "cbaritem.h"
#include <QGraphicsScene>

CBarItem::CBarItem(bool isEditState):
    CGraphicsObjectItem(isEditState),
    m_pAnimation(0),
    m_ShowTime(2000),
    m_bLoopAnimation(false),
    m_widthAni(m_Width)
{
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

void CBarItem::enableLoopAnimation(bool loop)
{
    m_bLoopAnimation = loop;
}

bool CBarItem::isLoopAnimation()
{
    return m_bLoopAnimation;
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

    if(m_bLoopAnimation)
        connect(m_pAnimation, &QPropertyAnimation::finished,this,&CBarItem::startAnimation);//可循环动画

    if(m_pAnimation->state() == QAbstractAnimation::Stopped)//判断动画是否停止
    {
        this->show();//显示该对象

        m_pAnimation->setDuration(m_ShowTime);
        m_pAnimation->setStartValue(QRectF(0,0,0,m_Height));
        m_pAnimation->setEndValue(QRectF(0,0,m_widthAni,m_Height));

        m_pAnimation->start();//运行动画

        m_timeStartAni = QDateTime::currentSecsSinceEpoch();//动画启动的时间
    }
}

//结束动画
void CBarItem::stopAnimation()
{
    if(m_pAnimation)
    {
		//让图元按照最大化刷新一次，否则界面可能仍然留有部分残留
        m_Width = m_widthAni;
        update();

        //1 方法1 ，删除动画，即可停止循环动画
//        m_pAnimation->stop();
//        delete m_pAnimation;
//        m_pAnimation = 0;

        //方法2 不删除动画，只断开动画的循环连接，也可实现停止动画的消化
        if(m_bLoopAnimation)
            disconnect(m_pAnimation, &QPropertyAnimation::finished,this,&CBarItem::startAnimation);//取消循环动画
        m_pAnimation->stop();

        this->hide();//隐藏
    }
}

//重绘
void CBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    CGraphicsObjectItem::paint(painter,option,widget);

    if(!b_IsEditState && m_widthAni == m_Width)
    {
        this->hide();
        return;
    }

    painter->fillPath(shape(),QBrush(Qt::green));

    qint64 timeNow = QDateTime::currentSecsSinceEpoch();//当前时间
    qint64 timeSpan = timeNow - m_timeStartAni;
    if(timeSpan > 3)
        painter->drawText(QRectF(0,0,m_Width,m_Height),Qt::AlignCenter,QString("%1秒").arg(timeNow - m_timeStartAni));
}
