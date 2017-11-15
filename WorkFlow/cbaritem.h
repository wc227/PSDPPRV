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

    int type() const { return Type;}    //返回判断类型

    //获取动画显示时间
    int getShowTime();

    //设置动画显示时间
    void setShowTime(int time = 1);

    //设置是否循环执行动画
    void enableLoopAnimation(bool loop = true);

    //是否循环执行动画
    bool isLoopAnimation();

    //设置是否忽略结束事件
    void enableIgnoreEndEvt(bool val = true);

    //是否忽略结束事件
    bool isIgnoreEndEvt();

    //设置启动延时事件
    void setStartDelay(int val = 1);

    //获取启动延时时间
    int getStartDelay();

public Q_SLOTS:
    //运行动画
    void startAnimation();

    //结束动画
    void stopAnimation();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void initAnimation();//初始化动画

    QPropertyAnimation *m_pAnimation;

    int m_widthAni;//动画矩形的宽度
    int m_ShowTime;//动画显示时间(单位:ms)
    bool m_bLoopAnimation;//是否循环显示动画
    bool m_bIgnoreEndEvt;//是否忽略结束事件
    int m_startDelay;//启动延时(单位:ms)
    qint64 m_timeStartAni;//动画启动的时间(单位：秒)
};

#endif // CBarItem_H
