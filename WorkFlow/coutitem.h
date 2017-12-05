#ifndef COUTITEM_H
#define COUTITEM_H

#include <QtGui>
#include "CGraphicsObjectItem.h"
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
    Rect = 0, //矩形
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
    COutItemPropertyDialog(COutItem *item,QWidget *parent = 0);
//    ~COutItemPropertyDialog();

    //初始化界面
    void initUI();

    //更新界面
    void updateUI();

private:
    QLabel *m_labelName;		//name 标签
    QLineEdit *m_lineName; //显示与输入name edit

    QLabel *m_labelEvent;
    QLineEdit *m_lineEvent;//事件号

    QLabel *m_labelCmd;
    QLineEdit *m_lineCmd;//命令参数

    QPushButton *m_btnClose;

    COutItem *m_CurrentItem;

public slots:
    void nameChanged(QString arg1);	//名字LineEdit发生改变
    void eventChanged(QString arg1);	//事件lineEdit发生改变
    void commandChanged(QString arg1); //命令行参数改变
};

class COutItem : public CGraphicsObjectItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 2}; //根据Type判断是否是COutItem
    COutItem(bool isEditState);

    int type() const { return Type;} //COutItem自定义type，重新type

    void setEventNumbers(const QString &eventNumbers);//设置事件号

    ItemShape getShape(); //获取item形状

    void setShape(ItemShape); //设置item形状

    void setCommands(QString val);//设置命令参数

    QString getCommands() const;//获取命令参数

    //编辑属性
    void editProperty();

    void enableEditMode(bool mode = true);

signals:
    void EvtFileChange(int number); //文件事件发生改变

    void sendCmd(QString sCmd);//发送命令

private:
    ItemShape m_shape;//设置显示形状(暂时没有任何作用)
    QList<QString> m_TaskNumbersList;
    QString m_Commands;//命令行参数

private:
    void SetEvtList(QList<QString>& arList, int val);  //触发事件
    int EventType(const QString pchEvent, int *pnEventNo); //事件类型获取

protected:
    virtual QPainterPath shape();
//    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);	//鼠标双击事件
//    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);	//右键菜单项    

};

#endif // COUTITEM_H
