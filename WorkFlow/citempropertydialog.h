#ifndef CITEMPROPERTYDIALOG_H
#define CITEMPROPERTYDIALOG_H

#include <QDialog>

class CXAnimateBar;

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
    explicit CItemPropertyDialog(CXAnimateBar *item,QWidget *parent = 0);
    ~CItemPropertyDialog();

    //更新界面
    void updateUI();

private:
    Ui::CItemPropertyDialog *ui;

    CXAnimateBar *m_currentItem;

    //通过槽函数直接修改CBarItem中的数据
private slots:
    void on_btnOK_clicked();
    void on_lineEdit_Name_textChanged(const QString &arg1);
    void on_spinBox_X_valueChanged(int arg1);
    void on_spinBox_Y_valueChanged(int arg1);
    void on_spinBox_Width_valueChanged(int arg1);
    void on_spinBox_Height_valueChanged(int arg1);
    void on_spinBox_Rotation_valueChanged(int arg1);
    void on_lineEdit_Event_Start_textChanged(const QString &arg1);
    void on_lineEdit_Event_Stop_textChanged(const QString &arg1);
    void on_checkBox_IgnoreEndEvent_toggled(bool checked);
    void on_checkBox_Loop_toggled(bool checked);
    void on_spinBox_Duriation_valueChanged(int arg1);
    void on_spinBox_StartDelay_valueChanged(int arg1);
};

#endif // CITEMPROPERTYDIALOG_H
