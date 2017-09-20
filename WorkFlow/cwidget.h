#ifndef CWIDGET_H
#define CWIDGET_H

#include <QtGui>
#include "cgraphicsscene.h"
#include "cbaritem.h"
#include "coutitem.h"
#include "tpsdevts.h"
#include <QGraphicsView>

class CWidget : public QGraphicsView
{
    Q_OBJECT

public:
    CWidget(QWidget *parent = 0);
    ~CWidget();

    QSize getWindowSizie();
	void setIniPath(const QString &path); //设置文件路径

	int m_picWidth;
	int m_picHeight;

private:
    QString m_BackgroundPicName;    //获取打开的图片名
//    QGraphicsView *m_View;          //视图用来承载场景
    CGraphicsScene *m_Scene;        //场景用来显示Item

    void currentBackground();       //更新当前的背景
    void initAllItemsFromIniFile();
    void saveToIniFile();         //保存到Ini文件中

    void stringToItemData(QString data,QString type);

    bool isEdit;
	int showTime;                   //显示时间
    CBarItem *item;

	QString m_IniCFGPath;
	int m_nTimerID;

private:
    QMap<int, int> m_NumberToCounter;//用于存EventNumber和事件counter; 用于对输入事件编号进行是否改变判断
	//2015年3月2日 14:51:29 修改
	//增加事件分类触发
	QMap<QString, int> m_StrEvtsToCounters; //用于存放EvtentNumber和时间counter
	QMap<QString, QList<QGraphicsItem*> > m_StrEvtsToItems; //事件号与对应的Item;
	TPsdEvts m_psdEvts;			//事件管理类，用于获取、触发事件号与计数器;
    QVector<CBarItem*> barItems;

public slots:
    void SLOT_EvtFileChange(int);	//事件号改变

public:
    void timerEvent(QTimerEvent *);  //定时器检测事件号是否发生改变

};

#endif // CWIDGET_H
