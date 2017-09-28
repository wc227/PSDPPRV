#ifndef XXWDOCKWIDGET_H
#define XXWDOCKWIDGET_H

#include <QDockWidget>

//自定义的可停靠窗口类，可以显示或隐藏标题栏
class CXxwDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit CXxwDockWidget(const QString &title, QWidget *parent = Q_NULLPTR,
                         Qt::WindowFlags flags = Qt::WindowFlags())
        :QDockWidget(title,parent,flags)
    {
        titleBarOld = titleBarWidget();
        titleBarEmpty = new QWidget();//默认构造函数的widget
        m_bShowTitleBar = true;
    }

    explicit CXxwDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags())
        :QDockWidget(parent,flags)
    {
        titleBarOld = titleBarWidget();
        titleBarEmpty = new QWidget();//默认构造函数的widget
        m_bShowTitleBar = true;
    }

    virtual ~CXxwDockWidget()
    {
        setTitleBarWidget(titleBarOld);
        if(titleBarEmpty)
            delete titleBarEmpty;//删除
    }

    //显示或隐藏标题栏
    void showTitleBar(bool bShow = true)
    {
        if(m_bShowTitleBar == bShow)
            return;

        /*可通过下面几句代码来隐藏标题栏（
        It is not possible to remove a title bar from a dock widget.
        However, a similar effect can be achieved by setting a default constructed QWidget as the title bar widget.）
        */
        if(bShow)
            setTitleBarWidget(titleBarOld);//显示原有的标题栏
        else
            setTitleBarWidget(titleBarEmpty);//显示一个空的widget实现隐藏标题栏的效果
        m_bShowTitleBar = bShow;
    }

private:
    QWidget *titleBarOld;//原有的标题栏窗口

    QWidget *titleBarEmpty;//用来达到隐藏标题栏的窗口

    bool m_bShowTitleBar;//是否显示标题栏
};

#endif // XXWDOCKWIDGET_H
