#include "BorderlessWidget.h"
#include <QDebug>

BorderlessWidget::BorderlessWidget(QWidget *parent) :
    QWidget(parent)
{
    //设置无边框透明	//Qt::WindowStaysOnTopHint这个很重要，如果没有这句话即使是自定义界面成功了，界面可以拖动，但也还存在问题，那就是界面能够拖动到任务栏之下！
    setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
//    m_areaMovable = geometry();
    m_bPressed = false;
}

void BorderlessWidget::mousePressEvent(QMouseEvent *e)
{
    //鼠标左键
    if(e->button() == Qt::LeftButton)
    {
        m_ptPress = e->pos();
        qDebug() << pos()  << e->pos() << m_ptPress;
//        m_bPressed = m_areaMovable.contains(m_ptPress);
        m_bPressed = true;
    }
}

void BorderlessWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bPressed)
    {
        qDebug() << pos()  << e->pos() << m_ptPress;
        move(pos() + e->pos() - m_ptPress);
    }
}

void BorderlessWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_bPressed = false;
}


void BorderlessWidget::resizeEvent(QResizeEvent *event)
{
    m_areaMovable = geometry();
}

//设置鼠标按下的区域
void BorderlessWidget::setAreaMovable(const QRect rt)
{
    if(m_areaMovable != rt)
    {
        m_areaMovable = rt;
    }
}
