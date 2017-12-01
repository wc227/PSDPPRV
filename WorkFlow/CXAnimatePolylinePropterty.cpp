#include "CXAnimatePolylinePropterty.h"
#include "ui_CXAnimatePolylinePropterty.h"

CXAnimatePolylinePropterty::CXAnimatePolylinePropterty(CXAnimatePolyline *polyline,QWidget *parent) :
    QDialog(parent),m_currentItem(polyline),
    ui(new Ui::CXAnimatePolylinePropterty)
{
    ui->setupUi(this);
    updateUI();
}

CXAnimatePolylinePropterty::~CXAnimatePolylinePropterty()
{
    delete ui;
}

//更新界面
void CXAnimatePolylinePropterty::updateUI()
{
    if(!m_currentItem)
        return;

    ui->lineEdit_Name->setText(m_currentItem->getName());

    QString sEvt = m_currentItem->getEventNumbers();
    QStringList lstEvt = sEvt.split('-');
    if(lstEvt.count()>0)
    {
        ui->lineEdit_Event_Start->setText(lstEvt.at(0));
        if(lstEvt.count()>1)
            ui->lineEdit_Event_Stop->setText(lstEvt.at(1));
    }

    ui->spinBox_X->setValue((int)(m_currentItem->pos().x()));
    ui->spinBox_Y->setValue((int)(m_currentItem->pos().y()));
}

void CXAnimatePolylinePropterty::on_lineEdit_Name_textChanged(const QString &arg1)
{
    if(m_currentItem)
        m_currentItem->setName(arg1);
}

void CXAnimatePolylinePropterty::on_lineEdit_Event_Start_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        QString sEvtStart = arg1;
        QString sEvtStop = ui->lineEdit_Event_Stop->text();
        m_currentItem->setEventNumbers(sEvtStart + "-" + sEvtStop);
    }
}

void CXAnimatePolylinePropterty::on_lineEdit_Event_Stop_textChanged(const QString &arg1)
{
    if(m_currentItem)
    {
        QString sEvtStart = ui->lineEdit_Event_Start->text();
        QString sEvtStop = arg1;
        m_currentItem->setEventNumbers(sEvtStart + "-" + sEvtStop);
    }
}

void CXAnimatePolylinePropterty::on_spinBox_X_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        qreal x = arg1;
        qreal y = m_currentItem->pos().y();
        m_currentItem->setPos(x,y);
    }
}

void CXAnimatePolylinePropterty::on_spinBox_Y_valueChanged(int arg1)
{
    if(m_currentItem)
    {
        qreal x = m_currentItem->pos().x();
        qreal y = arg1;
        m_currentItem->setPos(x,y);
    }
}

void CXAnimatePolylinePropterty::on_btnOK_clicked()
{
    close();
}
