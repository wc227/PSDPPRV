#include "CWidgetWork.h"

CWidgetWork::CWidgetWork(QWidget* parent, Qt::WindowFlags f )
    :QWidget(parent,f)
{
    setObjectName(QStringLiteral("MainWnd"));
//    resize(1200, 900);
    setWindowState(Qt::WindowMaximized);//最大化显示

    initUI();
}

//初始化界面
void CWidgetWork::initUI()
{
    View = new CXGraphicsView(this);
    connect(View, SIGNAL(sendCmd(QString)), this, SIGNAL(sendCmd(QString)));
    View->setObjectName(QStringLiteral("View"));
    centerView();
}


//视图居中处理
void CWidgetWork::centerView()
{
    if(View)
    {
        QSize szBack = View->backSize();//背景大小
        QSize szWnd =  this->size();//窗口大小
        int w = szBack.width(),h = szBack.height();

        int xOffset = 0;
        int yOffset = 0;
        if(szBack.width() <= szWnd.width())
        {
            xOffset = (szWnd.width() - szBack.width())/2;
            w = szBack.width();
        }
        else
        {
            xOffset = 0;
            w = szWnd.width();
        }

        if(szBack.height() <= szWnd.height())
        {
            yOffset = (szWnd.height() - szBack.height())/2;
            h = szBack.height();
        }
        else
        {
            yOffset = 0;
            h = szWnd.height();
        }

        View->setGeometry(xOffset,yOffset,w,h);
    }
}

//设置文件配置路径
void CWidgetWork::setFileCfg(QString sFile)
{
    View->setFileCfg(sFile);
}

void CWidgetWork::resizeEvent(QResizeEvent *event)
{
    centerView();
}
