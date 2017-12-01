#include "CXAnimatePolyline.h"
#include "CXAnimatePolylinePropterty.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>


int CXAnimatePolyline::s_handleSize = 8;
int CXAnimatePolyline::s_blankSizeOfDashline = 4;

//全局静态函数，只能在本文件中使用
//根据画笔来重新计算原有的绘图路径
static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    const qreal penWidthZero = qreal(0.00000001);
    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;

    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

CXAnimatePolyline::CXAnimatePolyline(const QPolygonF &poly,bool edit,QGraphicsItem *parent)
    :QObject()
    ,QGraphicsPolygonItem(poly,parent)
    ,m_bIsEditMode(edit)
    ,m_pItemArrow(0)
{
    if(m_bIsEditMode)
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
    init();
}


CXAnimatePolyline::~CXAnimatePolyline()
{
    releaseAll();
}

//清空指针
void CXAnimatePolyline::releaseAll()
{
}

QPainterPath CXAnimatePolyline::shape() const
{
//    return QGraphicsPolygonItem::shape();
    QPainterPath path;
    QPolygonF myPolygon = polygon();

//    path.addPolygon(myPolygon);
    if(myPolygon.count() > 0)
    {
        path.moveTo(myPolygon.at(0));
        for(int i = 1; i < myPolygon.count(); ++i)
        {
            path.lineTo(myPolygon.at(i));
        }
    }

    foreach (QPointF p, myPolygon)
    {
        path.addRect(getHandleRect(p));
    }

    return qt_graphicsItem_shapeFromPath(path,pen());//考虑到画笔宽
}

QRectF CXAnimatePolyline::boundingRect() const
{
    QRectF rc = shape().controlPointRect();
    rc.adjust(-s_handleSize/2,-s_handleSize/2,s_handleSize/2,s_handleSize/2);//往外扩展（考虑到句柄），否则移动过程会有残影
    return rc;
}

void CXAnimatePolyline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF myPolygon = polygon();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);

    QPen myPen = pen();

    //动画效果
    if(m_timerUpdate.isActive())
    {
        m_dashPen.setDashOffset(m_nDashOffset);
        painter->setPen(m_dashPen);
        painter->drawPolyline(myPolygon);
        myPen.setColor(m_dashPen.color());
    }
    //正常的静态效果
    else
    {
        painter->setBrush(Qt::NoBrush);
        if (isSelected())
        {
            myPen.setColor(Qt::red);
            myPen.setStyle(Qt::DashLine);
        }
        painter->setPen(myPen);
        painter->drawPolyline(myPolygon);
    }

    //绘制两端的控制句柄
    if (m_bIsEditMode && isSelected())
    {
        QPolygonF myPolygon = polygon();
        for(int i = 0; i < myPolygon.count(); ++i)
        {
            painter->fillRect(getHandleRect(myPolygon.at(i)),Qt::blue);
        }
    }
    painter->restore();

    //更新三角箭头的位置和方向
    int nCnt = myPolygon.count();
    if(nCnt >= 2)
    {
        QPointF p1 = myPolygon.at(nCnt-2);
        QPointF p2 = myPolygon.at(nCnt-1);
        QLineF line(p1,p2);
        if(m_pItemArrow)
        {
            qreal sz = myPen.widthF() * 2.5;
            QPolygonF poly;
            poly << QPointF(-2*sz,-sz) << QPointF(0,0) << QPointF(-2*sz,sz)/* << QPointF(-sz*2,0)*/ ;
            m_pItemArrow->setPolygon(poly);
            m_pItemArrow->setPen(Qt::NoPen);
            m_pItemArrow->setBrush(myPen.color());
            m_pItemArrow->setPos(p2);
            m_pItemArrow->setRotation(-line.angle());
        }
    }
}

void CXAnimatePolyline::init()
{    
    m_bShiftKeyDown = false;

    //正常模式的画笔
    QPen myPen = QPen(Qt::darkGray,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    setPen(myPen);

    //虚线画笔（绘制动态是的画笔）
    m_dashPen = QPen(Qt::red,2,Qt::DashLine,Qt::RoundCap,Qt::RoundJoin);
    QVector<qreal> dashes;
    dashes << s_blankSizeOfDashline*2 << s_blankSizeOfDashline;//虚线短横线长度是空白的2倍
    m_dashPen.setDashPattern(dashes);//自定义的虚线

    m_sName = "";
    m_sEventNumbers = "";
    m_bIsEditMode = true;

    m_ptDown = m_ptPrevious = m_ptNow = QPointF();
    m_iHandleID = -1;

    m_nDashOffset = s_blankSizeOfDashline*4;    
    m_nTimerInterval = 400;
    m_nDurationMax = 2000;//2000ms
    m_nDurationNow = 0;
    m_timerUpdate.setInterval(m_nTimerInterval);
    connect(&m_timerUpdate,&QTimer::timeout,this,&CXAnimatePolyline::updateDashOffset);

    m_bAnimateReverse = false;//默认是正向动画

    //构造一个三角形箭头，箭头水平朝右
    qreal sz = myPen.widthF() * 2.5;
    QPolygonF poly;
    poly << QPointF(-2*sz,-sz) << QPointF(0,0) << QPointF(-2*sz,sz)/* << QPointF(-sz*2,0)*/ ;
    m_pItemArrow = new QGraphicsPolygonItem(poly ,this);
    m_pItemArrow->setZValue(-1);
    m_pItemArrow->setPen(Qt::NoPen);
    m_pItemArrow->setBrush(myPen.color());
}

void CXAnimatePolyline::startAnimation()
{
    m_timerUpdate.start();
    m_nDurationNow = 0;
    update();
}

void CXAnimatePolyline::stopAnimation()
{
    m_timerUpdate.stop();
    m_nDurationNow = 0;
    update();
}

void CXAnimatePolyline::pauseAnimation()
{
    m_timerUpdate.stop();
    m_nDurationNow = 0;
    update();
}

void CXAnimatePolyline::setName(QString cap)
{
    m_sName = cap;
}

QString CXAnimatePolyline::getName() const
{
    return m_sName;
}

void CXAnimatePolyline::setEventNumbers(QString evts)
{
    m_sEventNumbers = evts;
}

QString CXAnimatePolyline::getEventNumbers() const
{
    return m_sEventNumbers;
}

bool CXAnimatePolyline::isEditMode() const
{
    return m_bIsEditMode;
}

void CXAnimatePolyline::enableEditMode(bool edit)
{
    if(m_bIsEditMode != edit)
    {
        m_bIsEditMode = edit;        
        if(m_bIsEditMode)
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
        if(m_bIsEditMode)
            stopAnimation();
        update();
    }
}

//获取点pt所处的句柄（矩形）
QRectF CXAnimatePolyline::getHandleRect(QPointF pt) const
{
    return QRectF(pt.x()-s_handleSize/2, pt.y()-s_handleSize/2, s_handleSize, s_handleSize);
}

//移动指定序号的句柄
void CXAnimatePolyline::moveHandleTo(int id, QPointF pt)
{
    QPolygonF myPolygon = polygon();
    if(id >=0 && id < myPolygon.count())
    {
        if(myPolygon.at(id) != pt)
        {
            myPolygon.replace(id,pt);
            setPolygon(myPolygon);
//            updateAnimation();
        }
    }
}

//显示或隐藏箭头
void CXAnimatePolyline::showArrow(bool show)
{
    if(m_pItemArrow)
    {
        m_pItemArrow->setVisible(show);
    }
}

//是否显示箭头
bool CXAnimatePolyline::isShowArrow() const
{
    if(m_pItemArrow)
    {
        return m_pItemArrow->isVisible();
    }
    return false;
}

//是否反向动画
bool CXAnimatePolyline::isAnimateReverse() const
{
    return m_bAnimateReverse;
}

//启用反向动画
void CXAnimatePolyline::enableAnimateReverse(bool val)
{
    if(m_bAnimateReverse != val)
    {
        m_bAnimateReverse = val;
        update();
    }
}


void CXAnimatePolyline::keyPressEvent(QKeyEvent *event)
{
    m_bShiftKeyDown = false;
    if(event->key() == Qt::Key_Shift)
        m_bShiftKeyDown = true;

    QGraphicsPolygonItem::keyPressEvent(event);
}

void CXAnimatePolyline::keyReleaseEvent(QKeyEvent *event)
{
    m_bShiftKeyDown = false;
//    if(event->key() == Qt::Key_Shift)
//        m_bShiftKeyDown = true;

    QGraphicsPolygonItem::keyReleaseEvent(event);
}


//在某条线段上双击，添加点
void CXAnimatePolyline::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bIsEditMode)
        return;

    if(event->button() == Qt::LeftButton) //左键按下
    {
        //shift键按下
        if(m_bShiftKeyDown)
        {
            QPolygonF myPolygon = polygon();
            QPointF pt = event->pos();
            for(int i = 1; i < myPolygon.count(); ++i)
            {
                QPointF p1  = myPolygon.at(i-1);
                QPointF p2 = myPolygon.at(i);
                QRectF r1 = getHandleRect(p1);
                QRectF r2 = getHandleRect(p2);
                QRectF r(p1,p2);
                if(r.width() <= 1)
                    r.adjust(-1,0,1,0);
                if(r.height() <= 1)
                    r.adjust(0,-1,0,1);
                if(!r1.contains(pt) && !r2.contains(pt) //不处于两个句柄上
                        && this->contains(pt) //处于整条线上
                        && r.contains(pt)) //处于两个点之间
                {
                    myPolygon.insert(i,pt);
                    setPolygon(myPolygon);
                    return;
                }
            }
        }
        else
        {
            editProperty();
        }
    }

    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}


void CXAnimatePolyline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bIsEditMode)
        return;

    m_iHandleID = -1;//默认没有选中
    if(event->button() == Qt::LeftButton) //左键按下
    {
        QPointF pt = event->pos();
        QPolygonF myPolygon = polygon();
        for(int i=0; i < myPolygon.count(); ++i)
        {
            QRectF rc = getHandleRect(myPolygon.at(i));
            if(rc.contains(pt))
            {
                m_ptDown = m_ptNow = m_ptPrevious = pt;
                m_iHandleID = i;
            }
        }
    }
    QGraphicsPolygonItem::mousePressEvent(event);
}

void CXAnimatePolyline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bIsEditMode)
        return;

    if(m_iHandleID >= 0)
    {
        m_ptNow = event->pos();
        moveHandleTo(m_iHandleID,m_ptNow);
        m_ptPrevious = m_ptNow;
    }
    else
        QGraphicsPolygonItem::mouseMoveEvent(event);
}

void CXAnimatePolyline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bIsEditMode)
        return;

    if(m_iHandleID >= 0)
    {
        m_ptNow = event->pos();
        moveHandleTo(m_iHandleID,m_ptNow);
//        m_ptPrevious = m_ptNow;
    }
    else
        QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void CXAnimatePolyline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pt = event->pos();
    QPolygonF myPolygon = polygon();
    for(int i=0; i < myPolygon.count(); ++i)
    {
        QRectF rc = getHandleRect(myPolygon.at(i));
        if(rc.contains(pt))
        {
            setCursor(Qt::SizeAllCursor);
            return;
        }
    }
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void CXAnimatePolyline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(isSelected())
    {
        QPointF pt = event->pos();
        QPolygonF myPolygon = polygon();
        for(int i=0; i < myPolygon.count(); ++i)
        {
            QRectF rc = getHandleRect(myPolygon.at(i));
            if(rc.contains(pt))
            {
                setCursor(Qt::SizeAllCursor);
                return;
            }
        }
    }
}

void CXAnimatePolyline::updateDashOffset()
{
    if(m_bAnimateReverse)
    {
        //反向动画
        m_nDashOffset += s_blankSizeOfDashline;
        if(m_nDashOffset == s_blankSizeOfDashline * 4)
            m_nDashOffset = 0;
    }
    else
    {
        //正向动画
        m_nDashOffset -= s_blankSizeOfDashline;
        if(m_nDashOffset == 0)
            m_nDashOffset = s_blankSizeOfDashline * 4;
    }
    update();

    m_nDurationNow += 200;
    if(m_nDurationNow >= m_nDurationMax)
        stopAnimation();//超过最大时间，就停止动画
}


//编辑属性
void CXAnimatePolyline::editProperty()
{
    CXAnimatePolylinePropterty dlg(this);
    dlg.exec();
}
