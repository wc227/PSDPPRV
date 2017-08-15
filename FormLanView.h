#ifndef FORMLANVIEW_H
#define FORMLANVIEW_H

#include <QWidget>
#include <QTimer>

#include "MyCompItem.h"
#include "MyComDatMgr.h"

class QTimer;
class QGraphicsScene;
class QGraphicsView;

class FormLanView : public QWidget
{
    Q_OBJECT

public:
    explicit FormLanView(QWidget *parent = 0);
    ~FormLanView();

    //初始化
    void Init();

    //删除图形，并释放指针，防止内存泄露
    void RemoveItem(QGraphicsItem *item);

    //link 2 computers by polyline
    //server is server
    //client is client
    bool Link2Comps(CMyCompItem *server, CMyCompItem *client);

    //设置计算机信息文件
    void SetFileCom(QString sFile);

public slots:
    //clear all computers
    void ClearCompData();

    //update all computers data
    void UpdateCompData();

    //update the lan
    void UpdateLan();

    //更新图
    void UpdateGraph();

    //set the timer step
    void SetTimeStep();

    //显示flag
    void ShowFlag(bool);

    //更新设置
    void UpdateSetting();

private:
    QGraphicsView *m_viewLan;
    QGraphicsScene *m_scene;

    ////////////////////////////////////
    //局域网状态图相关
    bool m_bUpdateRun;//局域网状态图更新中
    QTimer m_timerUpdate;//更新定时器
    CMyComDatMgr m_myComDat;

    //静态变量
    static int s_nGap;//边距
    static int s_nMax;//每行的最大个数
    static int s_nTimeUpdate;//刷新时间间隔,单位：秒
};

#endif // FORMLANVIEW_H
