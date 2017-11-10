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

private:
    Ui::DlgXBarChartProperty *ui;

    XBarChart *m_myChart;
};

#endif // DLGXBARCHARTPROPERTY_H
