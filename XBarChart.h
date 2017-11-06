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
#include <QPushButton>
#include <QLabel>

class XBarChart : public QGraphicsView
{    
protected:
    QGraphicsScene *m_Scene;//场景

    int m_Margin;//边距(最小20，最大50)
    QColor m_BackColor;//背景颜色

    QGraphicsTextItem *m_Title;//标题栏
    QString m_TitleTxt;//标题内容
    int m_TitleHeight;//标题区域高度(最小20，最大50)

    bool m_GridVisible;//是否显示网格线
    int m_VGridNum;//垂直网格数(也就是页面上显示条形图组的最大值)，每个网格里容纳一组条形图，以堆积形式显示条形图(最小2，最大6，默认是5)
    int m_MaxBarNumOfGroupInPage;//页面上垂直网格里显示条形图的最大值(最小2，最大10，默认是5)
    int m_MaxBarNumOfGroup;//条形图组中条形图的数目的最大值

    QList<QGraphicsTextItem *> m_TimeItems;//用来显示条形图组时间的图元
    QList<ListBarInfo> m_BarGroups;//条形图组
    QList<XBar *> m_BarItems;//所有的条形图
    int m_BarGroupsCapacity;//条形图组的最大容量（也就是最多可容纳多少组条形图）
    QStringList m_Times;//条形图组对应的时间
    bool m_IsSortByDuriation;//是否根据持续时间排序

    QString m_ColorCfgFile;//颜色配置文件
    QMap<int, QColor> m_Type2Color;//类型和颜色的映射，真实数据应该从配置文件中读取

    FileMgrDIDX m_FileMgrDIDX;//
    QTimer m_TimerCheckFile;//定时器，定时检查标记文件是否更新
    int m_TimeInterval;//检查间隔时间（单位：毫秒）

    int m_HPageCount;//水平方向页面总数
    int m_HPageNo;//水平方向页面序号（从1开始）

    int m_VPageCount;//垂直方向页面总数
    int m_VPageNo;//垂直方向页面序号（从1开始）

    QPushButton *m_BtnPageLeft;//左一页
    QPushButton *m_BtnPageRight;//右一页
    QLabel *m_LblPageNO;//当前页号

    QPushButton *m_BtnPageUp;//上一页
    QPushButton *m_BtnPageDown;//下一页

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
        if(val < 20)
            val = 20;
        else if(val > 50)
            val = 50;

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
        if(val < 20)
            val = 20;
        else if(val > 50)
            val = 50;

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

    //获取垂直网格数
    int getVGridNum() const
    {
        return m_VGridNum;
    }

    //设置垂直网格数
    void setVGridNum(int val)
    {
        if(val < 2)
            val = 2;
        else if(val >6)
            val = 6;

        if(m_VGridNum != val)
        {
            m_VGridNum = val;
            this->update();
        }
    }

    //页面上垂直网格里显示条形图的最大值
    int getMaxBarNumOfGroupInPage() const
    {
        return m_MaxBarNumOfGroupInPage;
    }

    //页面上垂直网格里显示条形图的最大值
    void setMaxBarNumOfGroupInPage(int val)
    {
        if(val < 2)
            val = 2;
        else if(val > 10)
            val = 10;

        if(m_MaxBarNumOfGroupInPage != val)
        {
            m_MaxBarNumOfGroupInPage = val;
            this->update();
        }
    }

    //获取条形图组的最大数量值
    int getBarGroupsCapacity() const
    {
        return m_BarGroupsCapacity;
    }

    //设置条形图组的最大数量值
    void setBarGroupsCapacity(int val)
    {
        if(val < m_VGridNum)
            val = m_VGridNum;

        if(m_BarGroupsCapacity != val)
        {
            m_BarGroupsCapacity = val;
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

    //获取检查间隔时间（单位：毫秒）
    int getTimeInterval() const
    {
        return m_TimeInterval;
    }

    //设置检查间隔时间（单位：毫秒）
    void setTimeInterval(int val)
    {
        if(val < 1)
            val = 1;

        if(m_TimeInterval != val)
        {
            m_TimeInterval = val;
            m_TimerCheckFile.setInterval(m_TimeInterval);
        }
    }

    //添加一个图元
    void addBarItem(XBar *item)
    {
        m_Scene->addItem(item);
        m_BarItems.append(item);
    }

    //清空所有的条形图
    void clearBarItems()
    {
        foreach (XBar *bar, m_BarItems)
        {
            m_Scene->removeItem(bar);
        }
        m_BarItems.clear();
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

    //更新导航功能相关的部件
    virtual void updateNavi();

protected:
    void paintEvent(QPaintEvent *event) ;
    void resizeEvent(QResizeEvent *event);

public slots:
    //检查文件是否更新
    void checkFileData();

    //左一页
    void pageLeft();

    //右一页
    void pageRight();

    //上一页
    void pageUp();

    //下一页
    void pageDown();
};

#endif // XBARCHART_H
