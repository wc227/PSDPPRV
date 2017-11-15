#include "cgraphicsscene.h"
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
    m_bEditMode = true;//默认是处于编辑模式
    createContextMenu();

    QString sDllName = "Mems.dll";
    //ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }
}

CGraphicsScene::CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene( x, y, width, height, parent)
{
    createContextMenu();
	m_pCurrentLinePath = NULL;
	m_startAddLine = false;
}


//设置是否处于编辑模式
void CGraphicsScene::enableEdit(bool val)
{
    m_bEditMode = val;
}

//是否处于编辑模式
bool CGraphicsScene::isEdit() const
{
    return m_bEditMode;
}

void CGraphicsScene::createContextMenu()
{
    m_MenuEdit = new QMenu;
    m_ActionAddIn = new QAction("In",m_MenuEdit);
    m_ActionAddOut = new QAction("Out",m_MenuEdit);
    m_ActionPathLine = new QAction("Line",m_MenuEdit);
    m_ActionDel = new QAction("Del",m_MenuEdit);

    connect(m_ActionAddIn, SIGNAL(triggered()), this, SLOT(slotAddgraphicsitem()));
    connect(m_ActionAddOut, SIGNAL(triggered()), this, SLOT(slotAddCRedItem()));
    connect(m_ActionDel, SIGNAL(triggered()), this, SLOT(slotDelItem()));
	connect(m_ActionPathLine, SIGNAL(triggered()), this, SLOT(slotADDPathLine()));

    m_MenuEdit->addAction(m_ActionAddIn);
    m_MenuEdit->addAction(m_ActionAddOut);
    m_MenuEdit->addAction(m_ActionPathLine);
    m_MenuEdit->addAction(m_ActionDel);


    m_MenuOutItem = new QMenu;//outitem的右键菜单
    m_Action_Property = new QAction("弹出信息");
    m_MenuOutItem->addAction(m_Action_Property);
    connect(m_Action_Property, SIGNAL(triggered()), this, SLOT(showOutitemMsg()));
}

void CGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
    {
		if(m_startAddLine && event->button() == Qt::LeftButton)
		{
			if(m_pCurrentLinePath == NULL)
				return;
			//Q_ASSERT(currentPathItem);
			QPolygonF polygon = m_pCurrentLinePath->getPolygon();

			polygon << event->scenePos();
			m_pCurrentLinePath->setPolygon(polygon);
			QPainterPath path;
			path.addPolygon(polygon);
			m_pCurrentLinePath->setPath(path);
			m_pCurrentLinePath->editEnd();
			update(m_pCurrentLinePath->boundingRect().adjusted(-1, -1, 1, 1));
		}
		else
		{
			QGraphicsScene::mouseDoubleClickEvent(event);
			return;
		}
    }
	else
	{
		if(m_startAddLine)
			m_startAddLine = false;
	}
    qDebug() << "mouse double click";
}

void CGraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if(event->button() == Qt::RightButton)
    {
        if(m_startAddLine)
            m_startAddLine = false;
        if(m_pCurrentLinePath)
            m_pCurrentLinePath = NULL;
    }
    QGraphicsScene::mousePressEvent(event);
}

//右键菜单
//编辑模式下，在空白处单击右键弹出“编辑”菜单
//运行模式下，在coutitem上单击右键，弹出Outitem的右键菜单
void CGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QTransform transform;
    QGraphicsItem *item = itemAt(event->scenePos(),transform);//当前位置的图元
    if(!item)//没有图元
    {
        if(m_bEditMode)//编辑模式
        {
            m_MenuEdit->exec(event->screenPos());//编辑模式下，在空白处单击右键弹出“编辑”菜单
        }
    }
    else
    {
        if(!m_bEditMode && item->type() == COutItem::Type)//运行模式
        {
            m_MenuOutItem->exec(event->screenPos());//运行模式下，在coutitem上单击右键，弹出Outitem的右键菜单
        }
        else
            QGraphicsScene::contextMenuEvent(event);
    }
}

//--slot
void CGraphicsScene::slotAddgraphicsitem()
{
    CBarItem *item = new CBarItem(true);
    qDebug() << m_RightButtonPos;
    item->setPos(m_RightButtonPos.x() - item->m_Width/2, m_RightButtonPos.y() - item->m_Height/2);
    item->setSelected(true);
    this->addItem(item);
}

void CGraphicsScene::slotAddCRedItem()
{
    COutItem *item = new COutItem(true);
    item->setPos(m_RightButtonPos.x() - item->m_Width/2, m_RightButtonPos.y() - item->m_Height/2);
    item->setSelected(true);
    this->addItem(item); 
}

void CGraphicsScene::slotDelItem()
{
    qDebug() << selectedItems().count();
    if(selectedItems().count())
    {
        for(int i=0; selectedItems().count(); i++)
            delete selectedItems().at(i);
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

void CGraphicsScene::slotADDPathLine()
{
	if(!m_startAddLine)
		m_startAddLine = true;
	CMyPathItem *pItem = new CMyPathItem(this,false);
	m_pCurrentLinePath = pItem;
	this->addItem(pItem);
}


void CGraphicsScene::showOutitemMsg()
{
    QMessageBox::information(0,"信息","测试信息","确定");
}
