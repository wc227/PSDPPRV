#include "cgraphicsscene.h"
#include "cbaritem.h"
#include "coutitem.h"
#include "CXAnimatePolyline.h"
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

//long ShareMemoryBuild();
typedef long(*FUN1)();

//int  EVTFileChange(int nFileNo,int nIncrement=1);
//int  EVTTaskChange(int nTaskNo,int nIncrement=1);
//int   PSDAPPChange(int nAppNo = APP_PSDCLOUD_NO, int nCountOffset = 0);
typedef int(*FUN2)(int,int);

//bool  EVTStatus(int nStatusNo);
typedef bool(*FUN3)(int);

//bool  EVTStatusSet(int nStatusNo,bool bTrue);
typedef bool(*FUN4)(int,bool);

//int  EVTTaskCounter(int nTaskNo);
//int  EVTFileCounter(int nFileNo);
typedef int(*FUN5)(int);


CGraphicsScene::CGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    QString sDllName = "Mems.dll";
    //ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }
    m_bEditMode = true;//默认是处于编辑模式
    createActions();
}

CGraphicsScene::CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene( x, y, width, height, parent)
{
    QString sDllName = "Mems.dll";
    //ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }
    m_bEditMode = true;//默认是处于编辑模式
    createActions();
}


//设置是否处于编辑模式
void CGraphicsScene::enableEdit(bool val)
{
    if(m_bEditMode != val)
    {
        m_bEditMode = val;
        foreach (QGraphicsItem *item, items())
        {
            if(CXAnimateBar::Type == item->type())
            {
                CXAnimateBar *bar = qgraphicsitem_cast<CXAnimateBar *>(item);
                bar->enableEditMode(m_bEditMode);
            }
            else if(CXAnimatePolyline::Type == item->type())
            {
                CXAnimatePolyline *polyline = qgraphicsitem_cast<CXAnimatePolyline *>(item);
                polyline->enableEditMode(m_bEditMode);
            }
            else if(COutItem::Type == item->type())
            {
                COutItem *outitem = qgraphicsitem_cast<COutItem *>(item);
                outitem->enableEditMode(m_bEditMode);
            }
        }
        update();
    }
}

//是否处于编辑模式
bool CGraphicsScene::isEdit() const
{
    return m_bEditMode;
}

void CGraphicsScene::createActions()
{
    m_ActionAddIn = new QAction("输入(条形图)");
    m_ActionPathLine = new QAction("连接线(折线)");
    m_ActionAddOut = new QAction("输出(矩形框)");
    m_ActionDel = new QAction("删除");
    m_ActionDel->setShortcut(QKeySequence::Delete);

    connect(m_ActionAddIn, SIGNAL(triggered()), this, SLOT(slotAddgraphicsitem()));
    connect(m_ActionPathLine, SIGNAL(triggered()), this, SLOT(slotADDPathLine()));
    connect(m_ActionAddOut, SIGNAL(triggered()), this, SLOT(slotAddCRedItem()));
    connect(m_ActionDel, SIGNAL(triggered()), this, SLOT(slotDelItem()));

    m_Action_Property = new QAction("弹出信息");
    connect(m_Action_Property, SIGNAL(triggered()), this, SLOT(showOutitemMsg()));
}


//右键菜单
//编辑模式下，在空白处单击右键弹出“编辑”菜单
//运行模式下，在coutitem上单击右键，弹出Outitem的右键菜单
void CGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QTransform transform;
    QGraphicsItem *item = itemAt(event->scenePos(),transform);//当前位置的图元
    if(m_bEditMode)
    {
        QMenu menuMain;
        menuMain.addAction(m_ActionAddIn);
        menuMain.addAction(m_ActionPathLine);
        menuMain.addAction(m_ActionAddOut);
        if(!item)
        {
            menuMain.exec(QCursor::pos());
        }
        else
        {
            menuMain.addSeparator();
            menuMain.addAction(m_ActionDel);//添加删除选项
            menuMain.exec(QCursor::pos());
//            QGraphicsScene::contextMenuEvent(event);
        }
    }
    else if(item && item->type() == COutItem::Type)
    {
        QMenu menuOutItem;
        menuOutItem.addAction(m_Action_Property);
        menuOutItem.exec(QCursor::pos());
    }
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(event->button() == Qt::RightButton)
    {
        m_rightButtonPos = event->scenePos();
        QGraphicsItem *item = itemAt(m_rightButtonPos,QTransform());
        if(item)
            item->setSelected(true);
    }
}

//--slot
void CGraphicsScene::slotAddgraphicsitem()
{
    CXAnimateBar *item = new CXAnimateBar(m_bEditMode);
    qDebug() << m_rightButtonPos;
    item->setPos(m_rightButtonPos);
    item->setSelected(true);
    addItem(item);
}

void CGraphicsScene::slotADDPathLine()
{
    CXAnimatePolyline *item = new CXAnimatePolyline();
    item->enableEditMode(m_bEditMode);
    qDebug() << m_rightButtonPos;
    item->setPos(m_rightButtonPos);
    item->setSelected(true);
    addItem(item);
}

void CGraphicsScene::slotAddCRedItem()
{
    COutItem *item = new COutItem(m_bEditMode);
    qDebug() << m_rightButtonPos;
    item->setPos(m_rightButtonPos);
    item->setSelected(true);
    addItem(item);
}

void CGraphicsScene::slotDelItem()
{
    QList<QGraphicsItem *> items = selectedItems();
    qDebug() << items.count();
    for(int i = items.count()-1; i >= 0; --i)
    {
        QGraphicsItem* item = items.at(i);
        removeItem(item);
    }
}

void CGraphicsScene::slotEVTFileChange(int n)
{
    qDebug() << "change" << n;

    QString sDllName = "Mems.dll";
//     EVTFileChange(n,1);
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
        if (EVTFileChange)
            EVTFileChange(n,1);
    }
}

void CGraphicsScene::showOutitemMsg()
{
    QMessageBox::information(0,"信息","测试信息","确定");
}
