#include "CWidgetWork.h"

CWidgetWork::CWidgetWork(QWidget* parent, Qt::WindowFlags f )
    :QWidget(parent,f)
{
    setObjectName(QStringLiteral("MainWnd"));
    resize(1200, 900);
    setWindowState(Qt::WindowMaximized);//最大化显示

    initUI();
}

//初始化界面
void CWidgetWork::initUI()
{
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

    View = new CWidget(this);
    View->setObjectName(QStringLiteral("View"));
    gridLayout->addWidget(View, 1, 1, 1, 1);//居中显示

    this->setLayout(gridLayout);
}

//设置文件配置路径
void CWidgetWork::setFileCfg(QString sFile)
{
//        View->setFileCfg("D:/DKY/20170721_PSDPPRV/Doc/cfg.ini");
    View->setFileCfg(sFile);
}
