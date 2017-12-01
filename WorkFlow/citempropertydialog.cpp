#include "citempropertydialog.h"
#include "ui_citempropertydialog.h"
#include "cbaritem.h"
#include <QPalette>
#include <QColorDialog>
#include <QCheckBox>

CItemPropertyDialog::CItemPropertyDialog(CXAnimateBar *item,QWidget *parent) :
    QDialog(parent),m_currentItem(item),
    ui(new Ui::CItemPropertyDialog)
{
    ui->setupUi(this);
    updateUI();
}

CItemPropertyDialog::~CItemPropertyDialog()
{
    delete ui;
}


//初始化UI界面
void CItemPropertyDialog::updateUI()
{
    if(!m_currentItem)
        return;

    ui->lineEdit_Name->setText(m_currentItem->getCaptainName());
    ui->spinBox_X->setValue((int)(m_currentItem->pos().x()));
    ui->spinBox_Y->setValue((int)(m_currentItem->pos().y()));
    ui->spinBox_Width->setValue((int)(m_currentItem->m_Width));
    ui->spinBox_Height->setValue((int)(m_currentItem->m_Height));
    ui->spinBox_Rotation->setValue((int)(m_currentItem->rotation()));

    QString sEvt = m_currentItem->getEventNumbers();
    QStringList lstEvt = sEvt.split('-');
    if(lstEvt.count()>0)
    {
        ui->lineEdit_Event_Start->setText(lstEvt.at(0));
        if(lstEvt.count()>1)
            ui->lineEdit_Event_Stop->setText(lstEvt.at(1));
    }
    ui->lineEdit_Duration->setText(QString("%1").arg(m_currentItem->getShowTime()));
    ui->lineEdit_StartDelay->setText(QString("%1").arg(m_currentItem->getStartDelay()));

    ui->checkBox_IgnoreEndEvent->setChecked(m_currentItem->isIgnoreEndEvt());
    ui->checkBox_Loop->setChecked(m_currentItem->isLoopAnimation());
}

void CItemPropertyDialog::on_btnOK_clicked()
{
    close();
}

void CItemPropertyDialog::on_lineEdit_Name_textChanged(const QString &arg1)
{
    if(m_currentItem)
        m_currentItem->setCaptainName(arg1);
}

void CItemPropertyDialog::on_spinBox_X_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        qreal x = arg1;
        qreal y = m_currentItem->pos().y();
        m_currentItem->setPos(x,y);
    }
}

void CItemPropertyDialog::on_spinBox_Y_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        qreal x = m_currentItem->pos().x();
        qreal y = arg1;
        m_currentItem->setPos(x,y);
    }
}

void CItemPropertyDialog::on_spinBox_Width_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        m_currentItem->m_Width = arg1;
        m_currentItem->update();
    }

}

void CItemPropertyDialog::on_spinBox_Height_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        m_currentItem->m_Height = arg1;
        m_currentItem->update();
    }
}

void CItemPropertyDialog::on_spinBox_Rotation_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        m_currentItem->setRotation(arg1);
    }
}

void CItemPropertyDialog::on_lineEdit_Event_Start_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        QString sEvtStart = arg1;
        QString sEvtStop = ui->lineEdit_Event_Stop->text();
        m_currentItem->setEventNumbers(sEvtStart + "-" + sEvtStop);
    }
}

void CItemPropertyDialog::on_lineEdit_Event_Stop_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        QString sEvtStart = ui->lineEdit_Event_Start->text();
        QString sEvtStop = arg1;
        m_currentItem->setEventNumbers(sEvtStart + "-" + sEvtStop);
    }
}

void CItemPropertyDialog::on_lineEdit_Duration_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        m_currentItem->setShowTime(arg1.toInt());
    }
}

void CItemPropertyDialog::on_lineEdit_StartDelay_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        m_currentItem->setStartDelay(arg1.toInt());
    }
}

void CItemPropertyDialog::on_checkBox_IgnoreEndEvent_toggled(bool checked)
{
    if(m_currentItem)
    {
        m_currentItem->enableIgnoreEndEvt(checked);
    }
}

void CItemPropertyDialog::on_checkBox_Loop_toggled(bool checked)
{
    if(m_currentItem)
    {
        m_currentItem->enableLoopAnimation(checked);
    }
}
