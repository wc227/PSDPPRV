#ifndef CWIDGET_H
#define CWIDGET_H

#include "cgraphicsscene.h"
#include "tpsdevts.h"
#include <QGraphicsView>
//#include <QMultiMap>

class CXGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CXGraphicsView(QWidget *parent = 0);
    ~CXGraphicsView();

private:
    QSize getWindowSizie();
    void initBack();                //更新当前的背景
    void initAllItems();            //根据从配置文件中提取的数据来显式所有的图元
    void stringToItemData(QString sVal,QString sGroup);//字符串转换成图元数据
    void saveToIniFile();           //保存到Ini文件中

private:
    CGraphicsScene *m_Scene;        //场景用来显示Item
    QString m_sFileCfg;             //配置文件绝对路径
    QString m_sFileBackPic;         //背景图片
    bool m_bEditMode;                 //是否是编辑模式，true-编辑模式，false-运行模式
    int m_nTimerID;                 //定时器id

	//增加事件分类触发
    QMap<QString, int> m_mapEvtNum2Counter; //动画的事件号和事件计数值
    QMultiMap<QString, QGraphicsItem*> m_mapEvtNum2BarsStart; //事件号与对应的多个启动动画Items;
    QMultiMap<QString, QGraphicsItem*> m_mapEvtNum2BarsStop; //事件号与对应的多个结束动画Items;
    TPsdEvts m_psdEvts;			    //事件管理类，用于获取、触发事件号与计数器;

    QSize m_backSize;//背景大小，也就是场景大小
public:
    QSize backSize() const
    {
        return m_backSize;
    }

signals:
    void sendCmd(QString sCmd);//发送命令

public slots:
    virtual void setFileCfg(const QString &path); //设置配置文件路径
    void SLOT_EvtFileChange(int);	//事件号改变
    void btnClickTest();//单击按钮测试
    //切换模式（编辑/运行）
    void switchMode();

protected:
    //定时器检测事件号是否发生改变
    void timerEvent(QTimerEvent *);

    //上/下/左/右键向各个方向移动
    void keyPressEvent(QKeyEvent *event) ;

    //移动选择的图元
    void moveSelectedItems(int x,int y);
};

#endif // CWIDGET_H
