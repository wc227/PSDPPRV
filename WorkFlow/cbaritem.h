#ifndef CBarItem_H
#define CBarItem_H

#include <QtGui>
#include "CGraphicsObjectItem.h"
#include <QPropertyAnimation>
#include "citempropertydialog.h"
//#include "cgraphicsscene.h"

/**
  *@brief 用于捕获事件产生
  */
class CBarItem : public CGraphicsObjectItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF sizeChange READ getAnmationSize WRITE setAnmationSize)
//    Q_PROPERTY(QPointF geometry READ m_Geometry WRITE m_SetGeometry)
public:
    enum {Type = UserType + 1}; //根据Type判断是否是CBarItem1
    CBarItem(bool isEditState);

    QRectF getAnmationSize();
    void setAnmationSize(QRectF s);
    virtual void setSize(int width, int height);

    void setBackGroundColor(const QColor &color);       //设置背景颜色
	//2015年3月2日 16:02:03 添加;
	void setEventNumber(QString evts);
	//QString m_EventNumbers;
	QString getEventNumbers();
	////////////////////////////
    int getEventNumber();
    int type() const { return Type;}    //返回判断类型

    QPointF m_Geometry() const;
    void m_SetGeometry(QPointF p);

    void startAnimation();  //运行动画
    QRectF updateRect();    //返回scene刷新的矩形,起点是Item在Scene中的Pos

    int getShowTime();
    void setShowTime(int time);

    void setCaptainName(const QString&);
    QString getCaptainName();

//    void SetRotate(double r); //暂定270 垂直;
private:

    QPropertyAnimation *p;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int width;
    int m_ShowTime;
//    double m_Angle;

protected:
    /**
      *@brief 右键菜单事件
      */
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    /**
      *@brief 鼠标双击事件
      */
//    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CBarItem_H
