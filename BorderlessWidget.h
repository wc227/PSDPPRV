/**************************************************************************
**
**  文件名称： BorderlessWidget.h
**  创建日期： 2017/04/12
**  创建作者： xiongxuanwen
**  电子邮箱： xiongxuanwen@163.com
**
**************************************************************************/

#ifndef BORDERLESSWIDGET_H
#define BORDERLESSWIDGET_H

#include <QWidget>
#include <QMouseEvent>

//继承自QWidget的自定义窗口，单击（左右键皆可）窗口并移动鼠标可拖动窗口
class BorderlessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BorderlessWidget(QWidget *parent = 0);

    //设置鼠标按下可移动窗口的区域，在子窗口中必须设置该区域
    void setAreaMovable(const QRect rt);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void resizeEvent(QResizeEvent *event);


private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
};

#endif // BORDERLESSWIDGET_H
