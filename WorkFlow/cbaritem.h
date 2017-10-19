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
    Q_PROPERTY(QRectF anmationSize READ getAnmationSize WRITE setAnmationSize)
public:
    enum {Type = UserType + 1}; //根据Type判断是否是CBarItem1
    CBarItem(bool isEditState);

    QRectF getAnmationSize();
    void setAnmationSize(QRectF s);

    virtual void setSize(int width, int height);

	//2015年3月2日 16:02:03 添加;
	void setEventNumber(QString evts);

	//QString m_EventNumbers;
	QString getEventNumbers();

    int getEventNumber();
    int type() const { return Type;}    //返回判断类型

//    QPointF Geometry() const;
//    void SetGeometry(QPointF p);

//    QRectF updateRect();    //返回scene刷新的矩形,起点是Item在Scene中的Pos

    int getShowTime();
    void setShowTime(int time);

    void setCaptainName(const QString&);
    QString getCaptainName();

    void enableLoopAnimation(bool loop = true);

    bool isLoopAnimation();

public Q_SLOTS:
    void startAnimation();  //运行动画

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    void initAnimation();//初始化动画

    QPropertyAnimation *m_pAnimation;
    int m_widthAni;//动画矩形的宽度
    int m_ShowTime;//动画显示时间
//    double m_Angle;
};

#endif // CBarItem_H
