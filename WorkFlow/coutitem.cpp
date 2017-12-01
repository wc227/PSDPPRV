#include "coutitem.h"
#include <QInputDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>


//long ShareMemoryBuild();
typedef long(*FUN1)();

//int  EVTFileChange(int nFileNo,int nIncrement=1);
//int  EVTTaskChange(int nTaskNo,int nIncrement=1);
//int   PSDAPPChange(int nAppNo = APP_PSDCLOUD_NO, int nCountOffset = 0);
typedef int(*FUN2)(int,int);

//bool  EVTStatus(int nStatusNo);
typedef bool(*FUN3)(int);

//bool  EVTStatusSet(int nStatusNo,bool bTrue);
typedef bool(*FUN4)(int,bool);

//int  EVTTaskCounter(int nTaskNo);
//int  EVTFileCounter(int nFileNo);
typedef int(*FUN5)(int);

COutItem::COutItem(bool isEditState) :
    CGraphicsObjectItem(isEditState),
    m_shape(ItemShape::Rect)
{
    setZValue(0);

    QString sDllName = "Mems.dll";
    //ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }

    m_Commands = "";
}

void COutItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);//反锯齿

    if(b_IsEditState)
        painter->setOpacity(1);
    else
    {
        if(b_HoverEnter)
            painter->setOpacity(1);
        else
            painter->setOpacity(0);
    }

//    if(!b_HoverEnter && !isSelected())
//    {
//        painter->setOpacity(0);
//    }

//    if(b_HoverEnter && !isSelected())
//    {
//        painter->setOpacity(1);
//    }

    QPen p(Qt::red);
    p.setWidth(2);
    p.setJoinStyle(Qt::RoundJoin);
    painter->setPen(p);
    painter->drawRect(QRectF(0,0,m_Width,m_Height));
    painter->restore();

    CGraphicsObjectItem::paint(painter, option, widget);
}

void COutItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_CursorPos != MouseNo)
        return;

    if(b_IsEditState)
    {
        editProperty();
    }
    else
    {
        SetEvtList(m_TaskNumbersList, 1);
        if(!(m_Commands.isEmpty()))
        {
            emit sendCmd(m_Commands);
        }
    }
}
//编辑属性
void COutItem::editProperty()
{
    COutItemPropertyDialog dlg(this);
         dlg.exec();
}

void COutItem::setEventNumbers(const QString &eventNumbers)
{
    m_EventNumbers = eventNumbers;
    if(!b_IsEditState)
        m_TaskNumbersList = m_EventNumbers.split(":");
}

ItemShape COutItem::getShape()
{
    return m_shape;
}

void COutItem::setShape(ItemShape i)
{
    m_shape = i;
    update();
}

void COutItem::setCommands(QString val)
{
    if(m_Commands != val)
        m_Commands = val;
}

QString COutItem::getCommands() const
{
    return m_Commands;
}

void COutItem::SetEvtList(QList<QString>& arList, int val)
{
    if (arList.size() == 0)
        return;

    QString sDllName = "Mems.dll";
    QLibrary lib(sDllName);
    if (lib.load())
    {
        //        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是 ?ShareMemoryBuild@@YAJXZ
        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
        FUN2 EVTTaskChange = (FUN2)lib.resolve("?EVTTaskChange@@YAHHH@Z");//dll中 EVTTaskChange 对应的实际是 ?EVTTaskChange@@YAHHH@Z
        FUN2 PSDAPPChange = (FUN2)lib.resolve("?PSDAPPChange@@YAHHH@Z");//dll中 PSDAPPChange 对应的实际是 ?PSDAPPChange@@YAHHH@Z
        FUN3 EVTStatus = (FUN3)lib.resolve("?EVTStatus@@YA_NH@Z");//dll中 EVTStatus 对应的实际是 ?EVTStatus@@YA_NH@Z
        FUN4 EVTStatusSet = (FUN4)lib.resolve("?EVTStatusSet@@YA_NH_N@Z");//dll中 EVTStatusSet 对应的实际是 ?EVTStatusSet@@YA_NH_N@Z
        //        FUN5 EVTTaskCounter = (FUN5)lib.resolve("?EVTTaskCounter@@YAHH@Z");//dll中 EVTTaskCounter 对应的实际是 ?EVTTaskCounter@@YAHH@Z
        //        FUN5 EVTFileCounter = (FUN5)lib.resolve("?EVTFileCounter@@YAHH@Z");//dll中 EVTFileCounter 对应的实际是 ?EVTFileCounter@@YAHH@Z

        QString  szTmp;
        szTmp.clear();
        int  nTmp  = 0;
        int  nEvt  = 0;
        bool bInvert =false;
        int  nMax  = arList.count();
        for (int i = 0; i < nMax; i++)
        {
            QString s = arList.at(i);
            nEvt = 0;
            //反逻辑提取
            s = s.trimmed(); //trimmed()返回值是将 \r \t除去 而不是在原来变量上直接删除
            if(s.isEmpty())
                continue;
            szTmp = s;
            bInvert = false;
            if(s.left(1)=="!")
            {
                bInvert = true;
                szTmp.remove(0,1); //删除掉!
            }
            nTmp = EventType(szTmp, &nEvt);
            switch (nTmp)
            {
            case EVT_FILE_TYPE:
            {
                if(bInvert)
                    val*=-1;
                EVTFileChange(nEvt, val);
            }break;
            case EVT_TASK_TYPE:
            {
                if(bInvert)
                    val*=-1;
                EVTTaskChange(nEvt, val);
                //                qDebug() << "Current Task Number:" << nEvt << "counter:"<< EVTTaskCounter(nEvt);
            }break;
            case EVT_STATUS_TYPE:
            {//与val无关
                bool b = EVTStatus(nEvt);
                if(bInvert) //反逻辑标记
                    EVTStatusSet(nEvt, !b);
                else
                    EVTStatusSet(nEvt, true); //直接设置为真状态[到应用时修订];
                //	if (val > 0)
                //	else
                // EVTStatusSet(nEvt, false);
            }break;
            case EVT_APP_TYPE:
            {
                if(bInvert)
                    val*=-1;
                PSDAPPChange(nEvt, val);
            }break;
            default:
            {
                //cout<<"***无效事件类型:"<<nTmp<<endl;
            }break;
            }//end switch
        }//end for..
    }

}

//2014年3月6日 15:22:57 新添加功能 用于$#!的复杂事件操作;
int COutItem::EventType( const QString pchEvent, int* pnEventNo )
{
    int Rtn=-1;
    if(pchEvent == NULL)
    {
        return Rtn;
    }//end if
    int nTmp = 0;
    QString szEvent = pchEvent;
    szEvent.trimmed();
    if(szEvent.isEmpty())
        return Rtn;
    QChar ch=szEvent.at(0);
    if(ch=='#')
    {
        szEvent.remove(0,1);
        nTmp = szEvent.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_FILE_TYPE;
            if(pnEventNo)
                *pnEventNo = nTmp;
        }
    }
    else if(ch=='@')
    {
        szEvent.remove(0,1);
        nTmp = szEvent.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_TASK_TYPE;
            if(pnEventNo)
                *pnEventNo = nTmp;
        }
    }
    else if(ch=='$')
    {
        szEvent.remove(0,1);
        nTmp = szEvent.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_STATUS_TYPE;
            if(pnEventNo)
                *pnEventNo = nTmp;
        }
    }
    else if(ch=='?')
    {                           //20140301  进程编码前到标志;
        szEvent.remove(0,1);	 //必须对标记进行清掉;
        nTmp = szEvent.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_APP_TYPE; //缺省的处理内容;
            if(pnEventNo)
                *pnEventNo = nTmp;
        }
    }
    else
    {
        szEvent = szEvent.trimmed();
        nTmp = szEvent.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_FILE_TYPE;  //缺省的处理内容;
            if(pnEventNo)
                *pnEventNo = nTmp;
        }
    }
    return Rtn;
}

QPainterPath COutItem::shape()
{
    //    QPainterPath  myPath;
    //    switch (m_shape)
    //    {
    //    case ItemShape::Rect:
    //        myPath = CGraphicsObjectItem::shape();
    //        break;
    //    case ItemShape::Line:
    //        myPath.addPolygon(QPolygonF() <<QPointF(0,m_Height) << QPointF(m_Width,m_Height));
    //        break;
    //    case ItemShape::Circle:
    //        myPath.addEllipse(0,0,m_Width,m_Height);
    //        break;
    //    case ItemShape::Triangle:
    //        myPath.addPolygon(QPolygonF() << QPointF(0,0) << QPointF(m_Width,m_Height/2)
    //                                        << QPointF(0,m_Height) << QPointF(0,0));
    //        break;
    //    default:
    //        break;
    //    }
    //    return myPath;

    return CGraphicsObjectItem::shape();
}

//////////////////////////////////////////////////////////////
/// \brief COutItemPropertyDialog::COutItemPropertyDialog
/// \param parent
//////////////////////////////////////////////////////////////
COutItemPropertyDialog::COutItemPropertyDialog(COutItem *item,QWidget *parent) :
    QDialog(parent),
    m_CurrentItem(item)
{
    initUI();
    updateUI();
}


//初始化界面
void COutItemPropertyDialog::initUI()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    m_labelName = new QLabel("名称:", this);
    m_labelName->setAlignment(Qt::AlignVCenter | Qt::AlignRight); //设置Label文字靠右
    m_lineName = new QLineEdit(this);
    layoutName->addWidget(m_labelName);
    layoutName->addWidget(m_lineName);

    QHBoxLayout *layoutEvent = new QHBoxLayout;
    m_labelEvent = new QLabel("事件号:", this);
    m_labelEvent->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEvent = new QLineEdit(this);
    layoutEvent->addWidget(m_labelEvent);
    layoutEvent->addWidget(m_lineEvent);

    //2014年5月6日10:04:11
    QHBoxLayout *layoutCmd = new QHBoxLayout;
    m_labelCmd = new QLabel(tr("命令参数:"), this);
    m_labelCmd->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineCmd = new QLineEdit(this);
    layoutCmd->addWidget(m_labelCmd);
    layoutCmd->addWidget(m_lineCmd);

    m_btnClose = new QPushButton("关闭");
    m_btnClose->setDefault(true);
    m_btnClose->setMinimumSize(60,25);
    m_btnClose->setMaximumSize(60,25);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addLayout(layoutName);
    pMainLayout->addLayout(layoutEvent);
    pMainLayout->addLayout(layoutCmd);
    pMainLayout->addWidget(m_btnClose);
    pMainLayout->setAlignment(m_btnClose,Qt::AlignHCenter);

    this->setLayout(pMainLayout);

    connect(m_lineEvent, SIGNAL(textChanged(QString)),
            this, SLOT(eventChanged(QString)));
    connect(m_lineName, SIGNAL(textChanged(QString)),
            this, SLOT(nameChanged(QString)));
    connect(m_lineCmd, SIGNAL(textChanged(QString)),
            this, SLOT(commandChanged(QString)));
    connect(m_btnClose,&QPushButton::clicked,this,&COutItemPropertyDialog::close);

    this->setWindowTitle(tr("属性"));

}

//更新界面
void COutItemPropertyDialog::updateUI()
{
    if(!m_CurrentItem)
        return;

    m_lineName->setText(m_CurrentItem->getCaptainName());
    m_lineEvent->setText(m_CurrentItem->getEventNumbers());
    m_lineCmd->setText(m_CurrentItem->getCommands());
}

void COutItemPropertyDialog::nameChanged(QString arg1)
{
    m_CurrentItem->setCaptainName(arg1);
}

void COutItemPropertyDialog::eventChanged(QString arg1)
{
    m_CurrentItem->setEventNumbers(arg1);
}

void COutItemPropertyDialog::commandChanged(QString arg1)
{
    m_CurrentItem->setCommands(arg1);
}
