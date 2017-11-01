#ifndef XBARCHART_H
#define XBARCHART_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QPaintEvent>
#include <QMap>
#include "XBar.h"
#include <QGraphicsTextItem>
#include <QTimer>
#include "FileMgrDIDX.h"
#include <QColor>

class XBarChart : public QGraphicsView
{    
protected:
    QGraphicsScene *m_Scene;//场景

    int m_Margin;//边距
    QColor m_BackColor;//背景颜色

    QGraphicsTextItem *m_Title;//标题栏
    QString m_TitleTxt;//标题内容
    int m_TitleHeight;//标题区域高度

    bool m_GridVisible;//是否显示网格线
    int m_BarGroupNum;//条形图组数，也是垂直网格数，每个网格里容纳一组条形图，以堆积形式显示条形图
    int m_MaxBarNumInGroup;//组中条形图的数目的最大值

    QList<QGraphicsTextItem *> m_TimeItems;//用来显示条形图组时间的图元
    QList<ListBar> m_BarGroups;//条形图组
    QStringList m_Times;//条形图组对应的时间

    QString m_ColorCfgFile;//颜色配置文件
    QMap<int, QColor> m_Type2Color;//类型和颜色的映射，真实数据应该从配置文件中读取

    FileMgrDIDX m_FileMgrDIDX;//
    QTimer m_TimerCheckFile;//定时器，定时检查标记文件是否更新
    int m_TimeInterval;//检查间隔时间

public:
    XBarChart(QWidget *parent=0);

    ~XBarChart();

    //添加一组条形图
    virtual void addBars(ListBarInfo bars);

    //获取边距
    int getMargin() const
    {
        return m_Margin;
    }

    //设置边距
    void setMargin(int val)
    {
        if(m_Margin != val)
        {
            m_Margin = val;
            this->update();
        }
    }

    //获取背景颜色
    QColor getBackColor() const
    {
        return m_BackColor;
    }

    //设置背景颜色
    void setBackColor(QColor val)
    {
        if(m_BackColor != val)
        {
            m_BackColor = val;
            this->update();
        }
    }

    //设置标题
    void setTitle(QString sTitle)
    {
        if(m_TitleTxt != sTitle)
        {
            m_TitleTxt = sTitle;
            m_Title->setPlainText(m_TitleTxt);
        }
    }

    //获取标题
    QString getTitle() const
    {
        return m_TitleTxt;
    }

    //标题是否可见
    bool isTitleVisible() const
    {
        return m_Title->isVisible();
    }

    //设置是否显示标题
    void setTitleVisible(bool vis)
    {
        m_Title->setVisible(vis);
    }

    //获取标题区域高度
    int getTitleHeight() const
    {
        return m_TitleHeight;
    }

    //设置标题区域高度
    void setTitleHeight(int val)
    {
        if(m_TitleHeight != val)
        {
            m_TitleHeight = val;
            this->update();
        }
    }

    //网格线是否可见
    bool isGridVisible() const
    {
        return m_GridVisible;
    }

    //设置是否显示网格线
    void setGridVisible(bool val)
    {
        if(m_GridVisible != val)
        {
            m_GridVisible = val;
            this->update();
        }
    }

    //获取条形图组数
    int getBarGroupNum() const
    {
        return m_BarGroupNum;
    }

    //设置条形图组数
    void setBarGroupNum(int val)
    {
        if(m_BarGroupNum != val)
        {
            m_BarGroupNum = val;
            this->update();
        }
    }

    //设置颜色配置文件
    void setColorCfgFile(QString val)
    {
        if(m_ColorCfgFile != val)
        {
            m_ColorCfgFile = val;
            loadColorCfgFile();
        }
    }

    //加载颜色配置文件
    void loadColorCfgFile();

    //设置数据文件
    void setFileData(QString val);

    //获取数据文件
    QString getFileData();


protected:
    //初始化图
    virtual void initChart();

    //重新绘制图
    virtual void repaintChart();

    //绘制主区域，包括边框和横轴
    virtual void drawMainArea(QPainter *painter);

    //绘制网格线
    virtual void drawGrid(QPainter *painter);

    //更新标题
    virtual void updateTitle();

    //更新X轴
    virtual void updateAxisX();

    //更新条形图
    virtual void updateBars();

protected:
    void paintEvent(QPaintEvent *event) ;
    void resizeEvent(QResizeEvent *event);

public slots:
    //检查文件是否更新
    void checkFileData();
};

#endif // XBARCHART_H
