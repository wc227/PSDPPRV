#ifndef COUTITEM_H
#define COUTITEM_H

#include <QtGui>
//#include "CGraphicsObjectItem.h"
#include <QGraphicsProxyWidget>
#include <QProgressBar>
#include "citempropertydialog.h"
#include "cgraphicsscene.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#ifndef  EVT_FILE_TYPE
#define  EVT_FILE_TYPE      0       //201402020 文件事件类型
#define  EVT_TASK_TYPE      1       //201402020 任务事件类型
#define  EVT_STATUS_TYPE    2       //201402018 状态类型，主要是记录保持的状态变量，只能是0或1（假或真）
#define  EVT_APP_TYPE       3       //201402018 增加进程状态标记，不与普通事件标记共享区域（一般用于内部通讯）
#endif

/////////////////////////////////////////////////
/// \brief The COutItemPropertyDialog class
/// 用于CoutItem在可编辑状态时，双击使用显示该Item的属性
///

enum ItemShape
{
    Default, //矩形
    Line,    //线
    Circle,  //圆
    Triangle //三角形
};

class COutItem;

/************************************************************************/
/* 用于显示COutItem的属性对话框                                             */
/************************************************************************/
class COutItemPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    COutItemPropertyDialog(QWidget *parent = 0, COutItem *currentItem = NULL);
    ~COutItemPropertyDialog();

    void setName(const QString &name);  //设置item的名字
    void setEvents(const QString &events);	//设置item的的事件号
    void setShape(const int &shape);	//设置item的形状

    ItemShape shapeType;

private:

    QLabel *m_pNameLabel;		//name 标签
    QLineEdit *m_pNameLineEdit; //显示与输入name edit
    QString m_SaveName;
    QString m_ModifiName;

    QLabel *m_pEventLabel;
    QLineEdit *m_pEventLineEdit;
    QString m_SaveEvents;
    QString m_ModifiEvents;

    QLabel *m_pShapeLabel;
    QComboBox *m_pShapeComBox;
    int nShape;

    QPushButton *m_pBtnOK;

    bool bDataIsChanged;        //用于弹出窗口是否修改；
    COutItem *m_CurrentItem;
public slots:
    void SLOT_EventLineEditChanged();	//事件lineEdit发生改变
    void SLOT_NameLineEditChanged();	//名字LineEdit发生改变
    void SLOT_ShapeComboxChanged(int index); //形状选择
};

class COutItem : public CGraphicsObjectItem
{
    Q_OBJECT

public:
    enum {Type = UserType + 2}; //根据Type判断是否是COutItem
    COutItem(bool isEditState);

    int type() const { return Type;} //COutItem自定义type，重新type

    void setEventNumbers(const QString &eventNumbers);//设置事件号

    int getShape(); //获取item形状
    void setShape(int); //设置item形状

signals:
    void EvtFileChange(int number); //文件事件发生改变

private:
//    QList<int> m_TaskNumbers;
    int nShape;            //设置显示形状
    QList<QString> m_TaskNumbersList;

    void SetEvtList(QList<QString>& arList, int val);  //触发事件
    int EventType(const QString pchEvent, int *pnEventNo); //事件类型获取

    QPainterPath currentPath;
//    QRectF boundingRect() const;
    virtual QPainterPath shape();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);	//鼠标双击事件
//    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);	//右键菜单项

protected:
    QString m_Commands;//命令行参数

public:
    void setCommands(QString val)
    {
        if(m_Commands != val)
            m_Commands = val;
    }

    QString getCommands() const
    {
        return m_Commands;
    }
};

#endif // COUTITEM_H
