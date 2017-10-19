#ifndef CWIDGET_H
#define CWIDGET_H

#include <QtGui>
#include "cgraphicsscene.h"
#include "cbaritem.h"
#include "coutitem.h"
#include "tpsdevts.h"
#include <QGraphicsView>
#include <QWidget>
#include <QGridLayout>
#include <QSpacerItem>

class CWidget : public QGraphicsView
{
    Q_OBJECT

public:
    CWidget(QWidget *parent = 0);
    ~CWidget();

private:
    QSize getWindowSizie();
    void initBack();                //更新当前的背景
    void initAllItems();            //根据从配置文件中提取的数据来显式所有的图元
    void stringToItemData(QString data,QString type);//字符串转换成图元数据
    void saveToIniFile();           //保存到Ini文件中

private:
    CGraphicsScene *m_Scene;        //场景用来显示Item
    QString m_sFileCfg;             //配置文件绝对路径
    QString m_sFileBackPic;         //背景图片
    bool m_bIsEdit;                 //是否是编辑模式，true-编辑模式，false-运行模式
    int m_nTimeShow;                //滚动条显示时间
    int m_nTimerID;                 //定时器id
//    QMap<int, int> m_NumberToCounter;//用于存EventNumber和事件counter; 用于对输入事件编号进行是否改变判断
	//2015年3月2日 14:51:29 修改
	//增加事件分类触发
    QMap<QString, int> m_mapEvtNum2Counter; //事件号和事件计数器
    QMap<QString, QList<QGraphicsItem*> > m_mapEvtNum2Items; //事件号与对应的多个Items;
    TPsdEvts m_psdEvts;			    //事件管理类，用于获取、触发事件号与计数器;

public slots:
    virtual void setFileCfg(const QString &path); //设置配置文件路径
    void SLOT_EvtFileChange(int);	//事件号改变

public:
    void timerEvent(QTimerEvent *);  //定时器检测事件号是否发生改变
};

#endif // CWIDGET_H
