#ifndef MYCOMPITEM_H
#define MYCOMPITEM_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "MyCompInfo.h"
#include <QMovie>
#include <QLabel>

//计算机图形类，主体是一个矩形框
class CMyCompItem : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 1 };
    int type() const {return Type;};

    CMyCompItem(QGraphicsItem * parent = 0);
    ~CMyCompItem();

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    //设置计算机信息
    void UpdateComp(const CMyCompInfo &ref)
    {
        m_ComInfo = ref;
        if(m_Movie)
        {
            if(m_ComInfo.TaskCount() > 0)
                m_Movie->setSpeed(m_ComInfo.TaskCount()*20);//设置速度
            else
                m_Movie->setPaused(true);
        }
    }

    CMyCompInfo Comp() const
    {
        return m_ComInfo;
    }

private:
    CMyCompInfo m_ComInfo;
    QMovie *m_Movie;//显示简单的无声的动画（进度条），用来表示任务数，任务数越多，进度条速度越快
    QLabel *m_lblGif;
    bool bGifShowed;//是否已经添加了动画

public:
    static bool bShowFlag;//是否显示标志位
    static int s_nLen;//每个计算机矩形的边长（正方形）
};

#endif // MYCOMPITEM_H
