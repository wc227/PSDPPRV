#ifndef DLGXBARCHARTPROPERTY_H
#define DLGXBARCHARTPROPERTY_H

#include <QDialog>
#include <XBarChart.h>

namespace Ui {
class DlgXBarChartProperty;
}

class DlgXBarChartProperty : public QDialog
{
    Q_OBJECT

public:
    explicit DlgXBarChartProperty(XBarChart *chart,QWidget *parent = 0);
    ~DlgXBarChartProperty();

protected:

    ///
    /// \brief updateChart:更新chart
    /// \param bUpdate,true:更新chart,false:更新当前界面上的控件
    ///
    void updateChart(bool bUpdate);

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_BackColor_clicked();

    void on_checkBox_TitleVisible_clicked();

//    void on_lineEdit_Title_textChanged(const QString &arg1);

    void on_checkBox_LegendVisible_clicked();

//    void on_radioButton_Top_clicked();

//    void on_radioButton_Bottom_clicked();

//    void on_radioButton_Left_clicked();

//    void on_radioButton_Right_clicked();

//    void on_spinBox_MarginLeft_valueChanged(int arg1);

//    void on_spinBox_MarginRight_valueChanged(int arg1);

//    void on_spinBox_MarginTop_valueChanged(int arg1);

//    void on_spinBox_MarginBottom_valueChanged(int arg1);

//    void on_spinBox_MaxGroupNumInPage_valueChanged(int arg1);

//    void on_spinBox_MaxBarNumOfGroupInPage_valueChanged(int arg1);

private:
    Ui::DlgXBarChartProperty *ui;

    XBarChart *m_myChart;
};

#endif // DLGXBARCHARTPROPERTY_H
