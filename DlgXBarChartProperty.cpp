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
        m_myChart->setMarginLeft(ui->spinBox_MarginLeft->value());
        m_myChart->setMarginRight(ui->spinBox_MarginRight->value());
        m_myChart->setMarginTop(ui->spinBox_MarginTop->value());
        m_myChart->setMarginBotton(ui->spinBox_MarginBotton->value());
        m_myChart->setTitle(ui->lineEdit_Title->text());
        QPalette pal = ui->pushButton_BackColor->palette();
        m_myChart->setBackColor(pal.color(QPalette::Button));
        m_myChart->setMaxGroupNumInPage(ui->spinBox_MaxGroupNumInPage->value());
        m_myChart->setMaxBarNumOfGroupInPage(ui->spinBox_MaxBarNumOfGroupInPage->value());
        m_myChart->update();
    }
    else
    {
        ui->spinBox_MarginLeft->setValue(m_myChart->getMarginLeft());
        ui->spinBox_MarginRight->setValue(m_myChart->getMarginRight());
        ui->spinBox_MarginTop->setValue(m_myChart->getMarginTop());
        ui->spinBox_MarginBotton->setValue(m_myChart->getMarginBotton());
        ui->lineEdit_Title->setText(m_myChart->getTitle());
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
}
