#include "DlgXBarChartProperty.h"
#include "ui_DlgXBarChartProperty.h"
#include <QColorDialog>

DlgXBarChartProperty::DlgXBarChartProperty(XBarChart *chart,QWidget *parent) :
    QDialog(parent),m_myChart(chart),
    ui(new Ui::DlgXBarChartProperty)
{
    ui->setupUi(this);
    updateChart(false);
}

DlgXBarChartProperty::~DlgXBarChartProperty()
{
    delete ui;
}

///
/// \brief updateChart:更新chart
/// \param bUpdate,true:更新chart,false:更新当前界面上的控件
///
void DlgXBarChartProperty::updateChart(bool bUpdate)
{
    if(!m_myChart)
        return;

    if(bUpdate)
    {
        m_myChart->setTitleVisible(ui->checkBox_TitleVisible->isChecked());
        m_myChart->setTitle(ui->lineEdit_Title->text());

        m_myChart->setLegendVisible(ui->checkBox_LegendVisible->isChecked());
        if(ui->radioButton_Top->isChecked())
            m_myChart->setLegendPosition(XBarChart::LP_TOP);
        else if(ui->radioButton_Bottom->isChecked())
            m_myChart->setLegendPosition(XBarChart::LP_BOTTOM);
        else if(ui->radioButton_Left->isChecked())
            m_myChart->setLegendPosition(XBarChart::LP_LEFT);
        else if(ui->radioButton_Right->isChecked())
            m_myChart->setLegendPosition(XBarChart::LP_RIGHT);

        m_myChart->setMarginLeft(ui->spinBox_MarginLeft->value());
        m_myChart->setMarginRight(ui->spinBox_MarginRight->value());
        m_myChart->setMarginTop(ui->spinBox_MarginTop->value());
        m_myChart->setMarginBottom(ui->spinBox_MarginBottom->value());

        QPalette pal = ui->pushButton_BackColor->palette();
        m_myChart->setBackColor(pal.color(QPalette::Button));

        m_myChart->setMaxGroupNumInPage(ui->spinBox_MaxGroupNumInPage->value());
        m_myChart->setMaxBarNumOfGroupInPage(ui->spinBox_MaxBarNumOfGroupInPage->value());

        m_myChart->update();
    }
    else
    {
        ui->checkBox_TitleVisible->setChecked(m_myChart->isTitleVisible());
        ui->lineEdit_Title->setText(m_myChart->getTitle());
        ui->lineEdit_Title->setEnabled(ui->checkBox_TitleVisible->isChecked());

        ui->checkBox_LegendVisible->setChecked(m_myChart->isLegendVisible());
        if(XBarChart::LP_TOP == m_myChart->getLegendPosition())
            ui->radioButton_Top->setChecked(true);
        else if(XBarChart::LP_BOTTOM == m_myChart->getLegendPosition())
            ui->radioButton_Bottom->setChecked(true);
        else if(XBarChart::LP_LEFT == m_myChart->getLegendPosition())
            ui->radioButton_Left->setChecked(true);
        else if(XBarChart::LP_RIGHT == m_myChart->getLegendPosition())
            ui->radioButton_Right->setChecked(true);
        ui->radioButton_Top->setEnabled(ui->checkBox_LegendVisible->isChecked());
        ui->radioButton_Bottom->setEnabled(ui->checkBox_LegendVisible->isChecked());
        ui->radioButton_Left->setEnabled(ui->checkBox_LegendVisible->isChecked());
        ui->radioButton_Right->setEnabled(ui->checkBox_LegendVisible->isChecked());

        ui->spinBox_MarginLeft->setValue(m_myChart->getMarginLeft());
        ui->spinBox_MarginRight->setValue(m_myChart->getMarginRight());
        ui->spinBox_MarginTop->setValue(m_myChart->getMarginTop());
        ui->spinBox_MarginBottom->setValue(m_myChart->getMarginBottom());

        QPalette pal = ui->pushButton_BackColor->palette();
        pal.setColor(QPalette::Button,m_myChart->getBackColor());
        ui->pushButton_BackColor->setPalette(pal);
        ui->pushButton_BackColor->setAutoFillBackground(true);
        ui->pushButton_BackColor->setFlat(true);

        ui->spinBox_MaxGroupNumInPage->setValue(m_myChart->getMaxGroupNumInPage());
        ui->spinBox_MaxBarNumOfGroupInPage->setValue(m_myChart->getMaxBarNumOfGroupInPage());
    }
}

void DlgXBarChartProperty::on_pushButton_OK_clicked()
{
    updateChart(true);
    this->close();
}

void DlgXBarChartProperty::on_pushButton_Cancel_clicked()
{
    this->close();
}

void DlgXBarChartProperty::on_pushButton_BackColor_clicked()
{
    QColor colorDefault = m_myChart->getBackColor();
    QColor color = QColorDialog::getColor(colorDefault, this,"背景颜色");
    if(!color.isValid())
        color = colorDefault;
    QPalette pal = ui->pushButton_BackColor->palette();
    pal.setColor(QPalette::Button,color);
    ui->pushButton_BackColor->setPalette(pal);
    ui->pushButton_BackColor->setAutoFillBackground(true);
    ui->pushButton_BackColor->setFlat(true);
    m_myChart->setBackColor(color);
}

void DlgXBarChartProperty::on_checkBox_TitleVisible_clicked()
{
//    m_myChart->setTitleVisible(ui->checkBox_TitleVisible->isChecked());
    ui->lineEdit_Title->setEnabled(ui->checkBox_TitleVisible->isChecked());
}

//void DlgXBarChartProperty::on_lineEdit_Title_textChanged(const QString &arg1)
//{
//    m_myChart->setTitle(ui->lineEdit_Title->text());
//}

void DlgXBarChartProperty::on_checkBox_LegendVisible_clicked()
{
//    m_myChart->setLegendVisible(ui->checkBox_LegendVisible->isChecked());
    ui->radioButton_Top->setEnabled(ui->checkBox_LegendVisible->isChecked());
    ui->radioButton_Bottom->setEnabled(ui->checkBox_LegendVisible->isChecked());
    ui->radioButton_Left->setEnabled(ui->checkBox_LegendVisible->isChecked());
    ui->radioButton_Right->setEnabled(ui->checkBox_LegendVisible->isChecked());
}

//void DlgXBarChartProperty::on_radioButton_Top_clicked()
//{
//    if(ui->radioButton_Top->isChecked())
//        m_myChart->setLegendPosition(XBarChart::LP_TOP);
//}

//void DlgXBarChartProperty::on_radioButton_Bottom_clicked()
//{
//    if(ui->radioButton_Bottom->isChecked())
//        m_myChart->setLegendPosition(XBarChart::LP_BOTTOM);
//}

//void DlgXBarChartProperty::on_radioButton_Left_clicked()
//{
//    if(ui->radioButton_Left->isChecked())
//        m_myChart->setLegendPosition(XBarChart::LP_LEFT);
//}

//void DlgXBarChartProperty::on_radioButton_Right_clicked()
//{
//    if(ui->radioButton_Bottom->isChecked())
//        m_myChart->setLegendPosition(XBarChart::LP_BOTTOM);
//}

//void DlgXBarChartProperty::on_spinBox_MarginLeft_valueChanged(int arg1)
//{
//    m_myChart->setMarginLeft(ui->spinBox_MarginLeft->value());
//}

//void DlgXBarChartProperty::on_spinBox_MarginRight_valueChanged(int arg1)
//{
//    m_myChart->setMarginRight(ui->spinBox_MarginRight->value());
//}

//void DlgXBarChartProperty::on_spinBox_MarginTop_valueChanged(int arg1)
//{
//    m_myChart->setMarginTop(ui->spinBox_MarginTop->value());
//}

//void DlgXBarChartProperty::on_spinBox_MarginBottom_valueChanged(int arg1)
//{
//    m_myChart->setMarginBottom(ui->spinBox_MarginBottom->value());
//}

//void DlgXBarChartProperty::on_spinBox_MaxGroupNumInPage_valueChanged(int arg1)
//{
//    m_myChart->setMaxGroupNumInPage(ui->spinBox_MaxGroupNumInPage->value());
//}

//void DlgXBarChartProperty::on_spinBox_MaxBarNumOfGroupInPage_valueChanged(int arg1)
//{
//    m_myChart->setMaxBarNumOfGroupInPage(ui->spinBox_MaxBarNumOfGroupInPage->value());
//}
