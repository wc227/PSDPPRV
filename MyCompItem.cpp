#include "MyCompItem.h"
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

bool CMyCompItem::bShowFlag = false;
int CMyCompItem::s_nLen = 100;//每个计算机矩形的边长（正方形）

CMyCompItem::CMyCompItem(QGraphicsItem * parent)
    :QGraphicsRectItem(parent)
{
    setFlags(ItemIsSelectable);
    setRect(0,0,s_nLen,s_nLen);

    m_lblGif = new QLabel();
    m_Movie = new QMovie(":/img/loading1");
    m_Movie->setScaledSize(QSize(s_nLen,s_nLen/4));//设置大小
    m_lblGif->setMovie(m_Movie);
//    connect(mMovie, SIGNAL(finished()),this, SLOT(slot_movieFinish()));
    m_Movie->start();
    bGifShowed = false;
}


CMyCompItem::~CMyCompItem()
{
    if(m_Movie)
    {
        m_Movie->stop();
        delete m_Movie;
        m_Movie = NULL;
    }

    if(m_lblGif)
    {
        delete m_lblGif;
        m_lblGif = NULL;
    }
}


void CMyCompItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QRectF rect = boundingRect();
    QColor clrFont = Qt::white;//字体颜色

    //1 根据是否在线来充填正方形
    //在线——绿色，离线——灰色
    QColor clrOnline;//是否在线的标识颜色
    if(m_ComInfo.IsOnline())
        clrOnline = QColor::fromRgb(128,255,128);//在线——绿色
    else
        clrOnline = Qt::lightGray;//离线——灰色
    painter->fillRect(rect,clrOnline);

    //2 计算机类型区域：管理节点 或 计算节点 或 终端
    QRectF rectName(rect.x(),rect.y(),rect.width(),rect.height()/4);
    QString sType("");
    QColor clrType;//计算机类型的标识颜色
    QPixmap pixmap(":/img/server");
    QPixmap pixmap2(":/img/calculate");
    QPixmap pixmap3(":/img/terminal");
    QRectF rectFlag(rect.x(),rect.y(),rect.width()/4,rect.height()/4);
    switch (m_ComInfo.CompType())
    {
    case CMyCompInfo::Comp_Server:
        sType = QStringLiteral("服务器");
        clrType = QColor::fromRgb(237,28,36);
        painter->fillRect(rectName,clrType);
        painter->drawPixmap(rectFlag, pixmap, pixmap.rect());
        break;
    case CMyCompInfo::Comp_Calculate:
        sType = QStringLiteral("计算节点");
        clrType = QColor::fromRgb(163,73,164);
        painter->fillRect(rectName,clrType);
        painter->drawPixmap(rectFlag, pixmap2, pixmap2.rect());
        break;
    case CMyCompInfo::Comp_Terminal:
        sType = QStringLiteral("终端");
        clrType = QColor::fromRgb(63,72,204);
        painter->fillRect(rectName,clrType);
        painter->drawPixmap(rectFlag, pixmap3, pixmap3.rect());
        break;
    default:
        break;
    }
    painter->setPen(clrFont);//字体
//    painter->drawText(QRectF(rect.x() + rect.width()/4,rect.y(),rect.width()*3/4,rect.height()/4),Qt::AlignCenter,sType);
    painter->drawText(rectName,Qt::AlignCenter,sType);

    //3 IP
    clrFont = Qt::black;
    QRectF rectIP(rect.x(),rect.y()+rect.height()/4,rect.width(),rect.height()/4);
    painter->setPen(clrFont);
    painter->drawText(rectIP,Qt::AlignCenter,QStringLiteral("IP:") +m_ComInfo.IP());

    //4 任务数
    QRectF rect3(rect.x(),rect.y()+rect.height()/2,rect.width(),rect.height()/4);
    painter->setPen(clrFont);
    painter->drawText(rect3,Qt::AlignCenter, QStringLiteral("任务数:") + QString("%1").arg(m_ComInfo.TaskCount()));

    //5 动画
    //如果没有添加动画，那就添加动画，反之就不用添加，以免重复添加
    if(!bGifShowed && m_ComInfo.IsOnline() && m_ComInfo.TaskCount() > 0)
    {
        //场景上添加控件
        QGraphicsProxyWidget *proxy = scene()->addWidget(m_lblGif);
        if(proxy)
        {
            bGifShowed = true;
            QPointF pt(rect.x(),rect.y()+rect.height()*3/4.0f);
            QPointF pt2 = mapToScene(pt);
            proxy->setPos((int)pt2.x(),(int)pt2.y());
        }
    }

    QPen myPen = pen();//绘制外边框的画笔
    myPen.setColor(clrType);
    myPen.setWidthF(0.1);
    setPen(myPen);

    painter->restore();

    QGraphicsRectItem::paint(painter,option,widget);    
}
