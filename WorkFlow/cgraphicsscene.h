#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>

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
    //创建actions
    void createActions();

public:
    //设置是否处于编辑模式
    void enableEdit(bool val);

    //是否处于编辑模式
    bool isEdit() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
//    QMenu *m_MenuEdit;//编辑模式下的右键弹出菜单
    QAction *m_ActionAddIn;
    QAction *m_ActionAddOut;
    QAction *m_ActionDel;
    QAction *m_ActionPathLine;

    QMenu *m_MenuOutItem;//outitem的右键菜单
    QAction *m_Action_Property;

    bool m_bEditMode;//是否处于编辑模式
    QPointF m_rightButtonPos;

	bool m_startAddLine; //当选择action- addLine时=true,鼠标右键按下时，变成false；

public slots:
    void slotAddgraphicsitem();
    void slotAddCRedItem();
    void slotDelItem();
	void slotADDPathLine();
    void slotEVTFileChange(int);
    void showOutitemMsg();
    void selectAll();//选择所有的图元
};

#endif // CGRAPHICSSCENE_H
