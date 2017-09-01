#include "FormWeb.h"
#include "webaxwidget.h"
#include <QVBoxLayout>

FormWebBase::FormWebBase(QWidget *parent) :
    QWidget(parent)
{
    m_webWidget = new WebAxWidget;
    m_webWidget->setControl(QStringLiteral("{8856f961-340a-11d0-a96b-00c04fd705a2}"));
    m_webWidget->setObjectName(QStringLiteral("m_webWidget"));
    m_webWidget->setProperty("focusPolicy", QVariant::fromValue(Qt::StrongFocus));
    m_webWidget->setProperty("DisplayAlerts",false);//不显示任何警告信息。
    m_webWidget->setProperty("DisplayScrollBars",false);// 显示滚动条

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_webWidget);
    setLayout(mainLayout);
}

FormWebBase::~FormWebBase()
{
}

void FormWebBase::loadUrl(QString url)
{
    m_webWidget->dynamicCall("Navigate(const QString&)",url);
}
