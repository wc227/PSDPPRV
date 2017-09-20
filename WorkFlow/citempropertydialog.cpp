#include "citempropertydialog.h"
#include "ui_citempropertydialog.h"
#include <QPalette>
#include <QColorDialog>
#include <QCheckBox>

CItemPropertyDialog::CItemPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CItemPropertyDialog)
{
    ui->setupUi(this);
    currentItem = NULL;
    createConnect();
    ui->colorshowLabel->setAutoFillBackground(true); //用了填充背景

    ui->colorLabel->hide();
    ui->colorshowLabel->hide();
    ui->fileNumberLabel->hide();
    ui->fileNumberlineEdit->hide();
    ui->setColorButton->hide();
}

CItemPropertyDialog::~CItemPropertyDialog()
{
    delete ui;
}

//连接信号与槽
void CItemPropertyDialog::createConnect()
{
    //连接修改X坐标的信号与槽
    connect(ui->spinBox_X, SIGNAL(valueChanged(int)),
            this, SLOT(SLOT_PosXSpinBoxChanged(int)));
    //连接修改Y坐标的信号与槽
    connect(ui->spinBox_Y, SIGNAL(valueChanged(int)),
            this, SLOT(SLOT_PosYSpinBoxChanged(int)));
    //连接修改高的信号与槽
    connect(ui->spinBox_Height, SIGNAL(valueChanged(int)),
            this, SLOT(SLOT_HeightSpixBoxChanged(int)));
    //连接修改宽的信号与槽
    connect(ui->spinBox_Width, SIGNAL(valueChanged(int)),
            this, SLOT(SLOT_WidthSpinBoxChanged(int)));
    //连接修改事件的信号与槽
    connect(ui->eventNumberlineEdit, SIGNAL(editingFinished()),
            this, SLOT(SLOT_EventNumberEditFinished()));

    connect(ui->setColorButton, SIGNAL(clicked()),
            this, SLOT(SLOT_SetLabelShowColor()));
    //连接修改名称的信号与槽
    connect(ui->namelineEdit, SIGNAL(editingFinished()),
            this, SLOT(SLOT_SetCaptainName()));
    //连接修改显示时间的信号与槽
    connect(ui->showTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(SLOT_SetShowTime()));
    //连接修改是否垂直的信号与槽
    connect(ui->checkBox, SIGNAL(stateChanged(int)),
            this, SLOT(SLOT_SetVertical(int)));
}

void CItemPropertyDialog::setPosXSpinBox(double x)
{
    m_PosX = x;
    ui->spinBox_X->setValue(x);
}

double CItemPropertyDialog::getPosX()
{
    return m_PosX;
}

void CItemPropertyDialog::setPoxYSpinBox(double y)
{
    m_PosY = y;
    ui->spinBox_Y->setValue(y);
}

double CItemPropertyDialog::getPosY()
{
    return m_PosY;
}

void CItemPropertyDialog::setWidthSpinBox(double w)
{
    m_Width = w;
    ui->spinBox_Width->setValue(w);
}

double CItemPropertyDialog::getWidth()
{
    return m_Width;
}

void CItemPropertyDialog::setHeightSpinBox(double h)
{
    m_Height = h;
    ui->spinBox_Height->setValue(h);
}

double CItemPropertyDialog::getHeight()
{
    return m_Height;
}

void CItemPropertyDialog::setBackGroundColorLabel(QColor c)
{
    m_BackgroundColor = c;
    background.setColor(QPalette::Background, c);
    ui->colorshowLabel->setPalette(background);
}

QColor CItemPropertyDialog::getBackgroundColor()
{
    return m_BackgroundColor;
}

void CItemPropertyDialog::setEventNumberLineEdit(QString number)
{
	/*
    m_EventNumber = number;
    if(m_EventNumber == 0)
        ui->eventNumberlineEdit->setText("");
    else
        ui->eventNumberlineEdit->setText(QString::number(m_EventNumber));
		*/
	ui->eventNumberlineEdit->setText(number);
}

double CItemPropertyDialog::getEventNumber()
{
    return m_EventNumber;
}

void CItemPropertyDialog::setCaptatinName(QString &name)
{
    ui->namelineEdit->setText(name);
}

void CItemPropertyDialog::setVerticalCheckBox(bool bCheckState)
{
    ui->checkBox->setChecked(bCheckState);
}

void CItemPropertyDialog::setShowTimeLineEdit(int showTime)
{
    ui->showTimeLineEdit->setText(QString::number(showTime));
}

void CItemPropertyDialog::SLOT_PosXSpinBoxChanged(int x)
{
//    m_PosX = ui->spinBox_X->value();
    if(currentItem != NULL)
        currentItem->setX(x);
    m_PosX = x;
}

void CItemPropertyDialog::SLOT_PosYSpinBoxChanged(int y)
{
//    m_PosY = ui->spinBox_Y->value();
    if(currentItem != NULL)
        currentItem->setY(y);
    m_PosY = y;
}

void CItemPropertyDialog::SLOT_WidthSpinBoxChanged(int w)
{
//    m_Width = ui->spinBox_Width->value();
    if(currentItem != NULL)
        currentItem->setSize(w,currentItem->m_Height);
    m_Width = w;
}

void CItemPropertyDialog::SLOT_HeightSpixBoxChanged(int h)
{
//    m_Height = ui->spinBox_Height->value();
    if(currentItem != NULL)
        currentItem->setSize(m_Width,h);
    m_Height = h;
}

void CItemPropertyDialog::SLOT_SetBackgroundColor()
{
    m_BackgroundColor = ui->colorshowLabel->palette().color(QPalette::Background);
}

void CItemPropertyDialog::SLOT_EventNumberEditFinished()
{
    //m_EventNumber = ui->eventNumberlineEdit->text().toInt();
	m_EventNumbers = ui->eventNumberlineEdit->text();
    if(!m_EventNumbers.isEmpty()){
        if(currentItem != NULL)
            currentItem->setEventNumber(m_EventNumbers);
    }
}

void CItemPropertyDialog::SLOT_SetLabelShowColor()
{
    QColorDialog dlg(this);
    if(dlg.exec())
    {
        background.setColor(QPalette::Background, dlg.selectedColor());
        ui->colorshowLabel->setPalette(background);
        m_BackgroundColor = dlg.selectedColor();
        if(currentItem != NULL){
//            currentItem->setBackGroundColor(dlg.selectedColor());
        }
    }
}

void CItemPropertyDialog::SLOT_SetCaptainName()
{
    m_CaptainName = ui->namelineEdit->text();
    if(currentItem != NULL)
        currentItem->setCaptainName(m_CaptainName);
}

void CItemPropertyDialog::SLOT_SetShowTime()
{
    currentItem->setShowTime(ui->showTimeLineEdit->text().toInt());
}

void CItemPropertyDialog::SLOT_SetVertical(int checkState)
{
    if(checkState == Qt::Checked)
    {
        if(currentItem->rotation() == 270)
            return;

        currentItem->setRotation(270);
    }
    else
    {
        if(currentItem->rotation() == 0)
            return;

        currentItem->setRotation(0);
    }
}

void CItemPropertyDialog::getItemPointer(QGraphicsItem *item)
{
    currentItem = dynamic_cast<CBarItem*>(item);
}
