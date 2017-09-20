#ifndef CITEMPROPERTYDIALOG_H
#define CITEMPROPERTYDIALOG_H

#include <QDialog>
#include "cbaritem.h"

class CBarItem;

namespace Ui {
    class CItemPropertyDialog;
}
/**
  ** @brief 设置CBarItem属性性对话框
  **/

class CItemPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CItemPropertyDialog(QWidget *parent = 0);
    ~CItemPropertyDialog();
    /**
      ** @brief 设置ItemX坐标、返回ItemX坐标
      **/
    void setPosXSpinBox(double);
    double getPosX();

    /**
      ** @brief 设置ItemY坐标、返回ItemY坐标
      **/
    void setPoxYSpinBox(double);
    double getPosY();

    /**
      ** @brief 设置Item宽度、返回Item宽度
      **/
    void setWidthSpinBox(double);
    double getWidth();

    /**
      ** @brief 设置Item高度、返回Item高度
      **/
    void setHeightSpinBox(double);
    double getHeight();

    /**
      ** @brief 设置Item背景颜色
      **/
    void setBackGroundColorLabel(QColor);
    QColor getBackgroundColor();

    /**
      ** @brief 设置Item事件编号
      **/
    void setEventNumberLineEdit(QString);
    double getEventNumber();

    /**
      ** @brief 获取产生Dlg的item
      **/
    void getItemPointer(QGraphicsItem *item);

    void setCaptatinName(QString &name);

    void setVerticalCheckBox(bool);

    void setShowTimeLineEdit(int showTime);

private:
    Ui::CItemPropertyDialog *ui;
    void createConnect();

    double m_PosX;              //Item的X坐标
    double m_PosY;              //Item的Y坐标
    double m_Width;             //Item的宽
    double m_Height;            //Item的高
	
    int m_EventNumber;          //Item的事件编号
	//2015年3月2日 15:59:32 修改
	QString m_EventNumbers;     //Items事件编号

    QColor m_BackgroundColor;   //Item的背景颜色
    QString m_CaptainName;      //Item的标识

    QPalette background;
    CBarItem *currentItem;

signals:

    //通过槽函数直接修改CBarItem中的数据
private slots:
    /**
      ** @brief 当spinbox改变时，修改Item的X坐标
      **/
    void SLOT_PosXSpinBoxChanged(int);
    /**
      ** @brief 当spinbox改变时，修改Item的Y坐标
      **/
    void SLOT_PosYSpinBoxChanged(int);
    /**
      ** @brief 当spinbox改变时，修改Item的宽度
      **/
    void SLOT_WidthSpinBoxChanged(int);
    /**
      ** @brief 当spinbox改变时，修改Item的高度
      **/
    void SLOT_HeightSpixBoxChanged(int);
    /**
      ** @brief 设置Item事件编号
      **/
    void SLOT_EventNumberEditFinished();
    /**
      ** @brief 设置Item事件编号
      **/
    void SLOT_SetBackgroundColor();
    /**
      ** @brief 设置属性ui->showLabelcolor的背景颜色
      **/
    void SLOT_SetLabelShowColor();
    /**
      ** @brief 设置属性ui->Name 用于标识Item的名称
      **/
    void SLOT_SetCaptainName();
    /**
      ** @brief 设置属性ui->showTime 用于标识Item的名称
      **/
    void SLOT_SetShowTime();
    /**
      ** @brief 设置属性ui->VerticalCheckBox用于标识Item是否垂直显示
      ** 参数： 用于获得checkState(int)中参数;
      **/
    void SLOT_SetVertical(int);
};

#endif // CITEMPROPERTYDIALOG_H
