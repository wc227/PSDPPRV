#ifndef XBARCHART_H
#define XBARCHART_H

#include "XBar.h"
#include "FileMgrDIDX.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QPaintEvent>
#include <QMap>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QColor>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QDateTime>

//标准化时间
//原时间time：20170801112503
//格式化后的时间：2017/08/01 11:25:03
QString g_formatTime(const QString &time);

//标准化时间
//原时间time：20170801112503
//格式化后的时间：2017/08/01 11:25:03
QDateTime g_getFormatDateTime(const QString &time);


class XBarChart : public QGraphicsView
{
    Q_OBJECT
public:
    enum LegendPosition{
        LP_TOP = 0,
        LP_BOTTOM,
        LP_LEFT,
        LP_RIGHT
    };

protected:
    QGraphicsScene *m_Scene;//场景

    int m_MarginLeft;//左边距(最小20，最大100)
    int m_MarginRight;//右边距(最小20，最大100)
    int m_MarginTop;//上边距(最小20，最大100)
    int m_MarginBottom;//下边距(最小20，最大100)
    QColor m_BackColor;//背景颜色

    bool m_TitleVisible;//标题栏是否可见
    QGraphicsTextItem *m_Title;//标题栏
    QString m_TitleTxt;//标题内容
    int m_TitleHeight;//标题区域高度(最小20，最大50)
    QRect m_TitleRect;//标题栏矩形区域

    bool m_LegendVisible;//图例是否可见
    LegendPosition m_LegendPosition;//图例位置
    int m_LegendWidth;//图例高度(默认100)
    int m_LegendHeight;//图例宽度(默认30)
    QRect m_LegendRect;//图例矩形区域
    QRect m_MainPlotRect;//主绘图矩形区域

    bool m_GridVisible;//是否显示网格线
    int m_MaxGroupNumInPage;//页面显示的条形图组数目（条形图列数）,也就是垂直网格数，每个网格里容纳一组条形图，以堆积形式显示条形图(最小2，最大10，默认是5)
    int m_MaxBarNumOfGroupInPage;//页面上垂直网格里显示条形图的最大值(列中条形图数目)，最小2，最大10，默认是5
    int m_MaxBarNumOfGroup;//条形图组中条形图的数目的最大值

    QList<QGraphicsTextItem *> m_TimeItems;//用来显示条形图组时间的图元
    QList<ListBarInfo> m_BarGroups;//条形图组
    QList<XBar *> m_BarItems;//所有的条形图
    int m_BarGroupsCapacity;//条形图组的最大容量（也就是最多可容纳多少组条形图）
    QStringList m_Times;//条形图组对应的时间

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
    QLabel *m_LblPageNO2;//当前页号

    QMenu *m_PopMenu;//右键菜单    
    QAction *m_Action_Property;

    QMenu *m_PopMenuBar;//条形图的右键菜单
    QAction *m_Action_BarOpenFile;

    QGraphicsItem *m_CurrentItem;//当前选中的图元

public:
    XBarChart(QWidget *parent=0);

    ~XBarChart();

    //更新各区域大小和位置
    void updatePositionAndSize();

    //更新标题大小和位置
    void updateTitleRect();

    //更新图例大小和位置
    void updateLegendRect();

    //更新主绘图区域大小和位置
    void updateMainPlotRect();

    //获取边距
    int getMarginLeft() const;

    //设置边距
    void setMarginLeft(int val);

    //获取边距
    int getMarginRight() const;

    //设置边距
    void setMarginRight(int val);

    //获取边距
    int getMarginTop() const;

    //设置边距
    void setMarginTop(int val);

    //获取边距
    int getMarginBottom() const;

    //设置边距
    void setMarginBottom(int val);

    //获取背景颜色
    QColor getBackColor() const;

    //设置背景颜色
    void setBackColor(QColor val);

    //设置标题
    void setTitle(QString sTitle);

    //获取标题
    QString getTitle() const;

    //标题是否可见
    bool isTitleVisible() const;

    //设置是否显示标题
    void setTitleVisible(bool vis);

    //获取标题区域高度
    int getTitleHeight() const;

    //设置标题区域高度
    void setTitleHeight(int val);

    //图例是否可见
    bool isLegendVisible() const;

    //设置是否显示图例
    void setLegendVisible(bool val);

    //设置图例位置
    void setLegendPosition(LegendPosition pos);

    //获取图例位置
    LegendPosition getLegendPosition() const;

    //网格线是否可见
    bool isGridVisible() const;

    //设置是否显示网格线
    void setGridVisible(bool val);

    //获取垂直网格数
    int getMaxGroupNumInPage() const;

    //设置垂直网格数
    void setMaxGroupNumInPage(int val);

    //页面上垂直网格里显示条形图的最大值
    int getMaxBarNumOfGroupInPage() const;

    //页面上垂直网格里显示条形图的最大值
    void setMaxBarNumOfGroupInPage(int val);

    //获取条形图组的最大数量值
    int getBarGroupsCapacity() const;

    //设置条形图组的最大数量值
    void setBarGroupsCapacity(int val);

    //设置颜色配置文件
    void setColorCfgFile(QString val);

    //获取检查间隔时间（单位：毫秒）
    int getTimeInterval() const;

    //设置检查间隔时间（单位：毫秒）
    void setTimeInterval(int val);

    //添加一组条形图
    void addBars(ListBarInfo bars);

    //添加一个图元
    void addBarItem(XBar *item);

    //清空所有的条形图
    void clearBarItems();

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

    //绘制图例
    virtual void drawLegend(QPainter *painter);

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

    //初始化右键菜单
    virtual void initPopMenu();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event) ;
    void mouseDoubleClickEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

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

    //属性编辑
    void editProperty();

    //打开条形图的数据文件
    void openFileOfBar();
};

#endif // XBARCHART_H
