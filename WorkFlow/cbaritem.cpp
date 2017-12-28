#include "cbaritem.h"
#include "citempropertydialog.h"
#include "cwidget.h"
#include <QGraphicsScene>

CXAnimateBar::CXAnimateBar(bool isEditState):
    CGraphicsObjectItem(isEditState),
    m_pAnimation(0),
    m_ShowTime(2000),
    m_bLoopAnimation(false),
    m_bIgnoreEndEvt(false),
    m_widthAni(m_Width),
    m_startDelay(0),
    m_timeStartAni(0)
{
    if(isEditState)
        setVisible(true);//编辑态显示
    else
        setVisible(false);//运行态不显示，只有执行动画时才显示

    setZValue(1);
    initAnimation();
}

void CXAnimateBar::enableEditMode(bool mode)
{
    if(m_bEditMode != mode)
    {
        m_bEditMode = mode;
        if(m_bEditMode)
        {
            setFlag(QGraphicsItem::ItemIsSelectable, true);
            setFlag(QGraphicsItem::ItemIsFocusable, true);
            setFlag(QGraphicsItem::ItemIsMovable, true);
            setVisible(true);//编辑态显示
        }
        else
        {
            setFlag(QGraphicsItem::ItemIsSelectable, false);
            setFlag(QGraphicsItem::ItemIsFocusable, false);
            setFlag(QGraphicsItem::ItemIsMovable, false);
            setVisible(false);//运行态不显示，只有执行动画时才显示
        }
        update();
    }
}

void CXAnimateBar::setEventNumbers(const QString &evts)
{
    QString evtNew = evts.trimmed();
    if(m_EventNumbers != evtNew)
    {
        QString evtOld = m_EventNumbers;
        m_EventNumbers = evtNew;

        if(0 == scene())
            return;

        if(scene()->views().isEmpty())
            return;

        if(0 == scene()->views().at(0))
            return;

        CXGraphicsView *view = (CXGraphicsView *)(scene()->views().at(0));
        if(view)
            view->updateItemEventMap(this,evtOld);
    }
}

QRectF CXAnimateBar::getAnmationSize()
{
    return QRectF(0,0,m_Width,m_Height);
}

void CXAnimateBar::setAnmationSize(QRectF sz)
{
    m_Width = sz.width();
    m_Height = sz.height();
    update();
}

void CXAnimateBar::setSize(int width, int height)
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

//获取动画显示时间
int CXAnimateBar::getShowTime()
{
    return m_ShowTime;
}

//设置动画显示时间
void CXAnimateBar::setShowTime(int time)
{
    m_ShowTime = time;
}

//设置是否循环执行动画
void CXAnimateBar::enableLoopAnimation(bool loop)
{
    m_bLoopAnimation = loop;
}

//是否循环执行动画
bool CXAnimateBar::isLoopAnimation()
{
    return m_bLoopAnimation;
}

//设置是否忽略结束事件
void CXAnimateBar::enableIgnoreEndEvt(bool val)
{
    m_bIgnoreEndEvt = val;
}

//是否忽略结束事件
bool CXAnimateBar::isIgnoreEndEvt()
{
    return m_bIgnoreEndEvt;
}

//设置启动延时事件
void CXAnimateBar::setStartDelay(int val)
{
    m_startDelay = val;
}

//获取启动延时时间
int CXAnimateBar::getStartDelay()
{
    return m_startDelay;
}

//初始化动画
void CXAnimateBar::initAnimation()
{
    if(!m_pAnimation)
        m_pAnimation = new QPropertyAnimation(this, "anmationSize");

    m_pAnimation->setDuration(m_ShowTime);
    m_pAnimation->setStartValue(QRectF(0,0,0,m_Height));
    m_pAnimation->setEndValue(QRectF(0,0,m_Width,m_Height));
}


//运行动画
void CXAnimateBar::startAnimation()
{
    if(!m_pAnimation)
        initAnimation();

    if(m_bLoopAnimation)
        connect(m_pAnimation, &QPropertyAnimation::finished,this,&CXAnimateBar::startAnimation);//可循环动画

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
void CXAnimateBar::stopAnimation()
{
    if(m_bIgnoreEndEvt)
        return;//如果忽略结束事件，就不用结束动画了

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
            disconnect(m_pAnimation, &QPropertyAnimation::finished,this,&CXAnimateBar::startAnimation);//取消循环动画
        m_pAnimation->stop();

        this->hide();//隐藏
    }
}

//编辑属性
void CXAnimateBar::editProperty()
{
    CItemPropertyDialog dlg(this);
    dlg.exec();
}

//重绘
void CXAnimateBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!m_bEditMode && m_widthAni == m_Width)
    {
        this->hide();
        return;
    }

    painter->fillPath(shape(),QBrush(Qt::green));

    //如果动画执行的时间超过3秒，就绘制出时间
    painter->save();
    painter->setPen(Qt::red);
    if(m_pAnimation && m_pAnimation->state() == QAbstractAnimation::Running)
    {
        qint64 timeNow = QDateTime::currentSecsSinceEpoch();//当前时间
        qint64 timeSpan = timeNow - m_timeStartAni;
        if(timeSpan > 3)
            painter->drawText(QRectF(0,0,m_Width,m_Height),Qt::AlignCenter,QString("%1秒").arg(timeNow - m_timeStartAni));
    }
    painter->restore();

    CGraphicsObjectItem::paint(painter,option,widget);
}

//鼠标双击事件
void CXAnimateBar::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bEditMode)
        return;

    if(event->button() == Qt::LeftButton) //双击左键
    {
        editProperty();
    }
}
