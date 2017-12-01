#ifndef CXANIMATEPOLYLINEPROPTERTY_H
#define CXANIMATEPOLYLINEPROPTERTY_H

#include <QDialog>
#include "CXAnimatePolyline.h"

namespace Ui {
class CXAnimatePolylinePropterty;
}

class CXAnimatePolylinePropterty : public QDialog
{
    Q_OBJECT

public:
    explicit CXAnimatePolylinePropterty(CXAnimatePolyline *polyline,QWidget *parent = 0);
    ~CXAnimatePolylinePropterty();

    //更新界面
    void updateUI();

private slots:
    void on_lineEdit_Name_textChanged(const QString &arg1);

    void on_lineEdit_Event_Start_textChanged(const QString &arg1);

    void on_lineEdit_Event_Stop_textChanged(const QString &arg1);

    void on_spinBox_X_valueChanged(int arg1);

    void on_spinBox_Y_valueChanged(int arg1);

    void on_btnOK_clicked();

private:
    Ui::CXAnimatePolylinePropterty *ui;

    CXAnimatePolyline *m_currentItem;
};

#endif // CXANIMATEPOLYLINEPROPTERTY_H
