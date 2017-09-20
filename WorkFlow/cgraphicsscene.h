#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "cbaritem.h"
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include "coutitem.h"
#include "cmypathitem.h"

/************************************************************************/
/* 用于添加item，处理item的事件传递                                     */
/************************************************************************/
class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene(QObject *parent = 0);
    CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0);

    /**
      **@brief 鼠标是否在Item之内按下的标识
      */
    bool b_MousePressInItem;
    bool b_IsEdit;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QMenu *contextMenu;
    QAction *m_ActionAddIn;
    QAction *m_ActionAddOut;
    QAction *m_ActionDel;
	QAction *m_ActionPathLine;

    QPointF m_RightButtonPos;

    void createContextMenu();

	//2015年5月14日 14:07:16 添加 animation line
	CMyPathItem *m_pCurrentLinePath;  //line
	bool m_startAddLine; //当选择action- addLine时=true,鼠标右键按下时，变成false；

private slots:
    void slotAddgraphicsitem();
    void slotAddCRedItem();
    void slotDelItem();
	void slotADDPathLine();
    void slotEVTFileChange(int);
};

#endif // CGRAPHICSSCENE_H
