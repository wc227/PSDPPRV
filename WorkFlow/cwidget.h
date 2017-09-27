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


//含有导航图view的widget，其中view无边框背景透明且居中显示
class CNaviWidget:public QWidget
{
public:
    explicit CNaviWidget(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        :QWidget(parent, f)
    {
        gridLayout = new QGridLayout(this);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

        View = new CWidget;
        View->setObjectName(QStringLiteral("View"));
        gridLayout->addWidget(View, 1, 1, 1, 1);//居中显示
//        setStyleSheet(QStringLiteral("border-image: url(:/skin/bg);"));//显示背景图
//        View->setStyleSheet(QStringLiteral("CWidget#View{border-image:none;border:0px; background:transparent;}"));//不显示背景图，无边框，背景透明
    }

    //设置文件配置路径
    void setFileCfg(QString sFile)
    {
//        View->setFileCfg("D:/DKY/20170721_PSDPPRV/Doc/cfg.ini");
        View->setFileCfg(sFile);
    }

private:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    CWidget *View;//
};

#endif // CWIDGET_H
