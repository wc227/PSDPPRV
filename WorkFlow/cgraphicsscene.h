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

protected:
    void createContextMenu();

public:
    //设置是否处于编辑模式
    void enableEdit(bool val);

    //是否处于编辑模式
    bool isEdit() const;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QMenu *m_MenuEdit;//编辑模式下的右键弹出菜单
    QAction *m_ActionAddIn;
    QAction *m_ActionAddOut;
    QAction *m_ActionDel;
	QAction *m_ActionPathLine;

    QMenu *m_MenuOutItem;//outitem的右键菜单
    QAction *m_Action_Property;

    bool m_bEditMode;//是否处于编辑模式
    QPointF m_RightButtonPos;

	//2015年5月14日 14:07:16 添加 animation line
	CMyPathItem *m_pCurrentLinePath;  //line

	bool m_startAddLine; //当选择action- addLine时=true,鼠标右键按下时，变成false；


private slots:
    void slotAddgraphicsitem();
    void slotAddCRedItem();
    void slotDelItem();
	void slotADDPathLine();
    void slotEVTFileChange(int);

    void showOutitemMsg();
};

#endif // CGRAPHICSSCENE_H
