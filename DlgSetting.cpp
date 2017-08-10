#include "DlgSetting.h"
#include "ui_DlgSetting.h"

DlgSetting::DlgSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetting),
    nLength(100), nGap(40), nMax(8)
{
    ui->setupUi(this);
}

DlgSetting::~DlgSetting()
{
    delete ui;
}


void DlgSetting::updateUI(bool bUI)
{
    if(bUI)
    {
        ui->spinBox_Length->setValue(nLength);
        ui->spinBox_Gap->setValue(nGap);
        ui->spinBox_Max->setValue(nMax);
    }
    else
    {
        nLength = ui->spinBox_Length->value();
        nGap = ui->spinBox_Gap->value();
        nMax = ui->spinBox_Max->value();
    }
}

void DlgSetting::on_btn_OK_clicked()
{
    updateUI(false);
    accept();
}
