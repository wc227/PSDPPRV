#include "cwidget.h"
#include "cbaritem.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QStringList>
#include <QTimer>


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

CWidget::CWidget(QWidget *parent)
	: QGraphicsView(parent)
{
	//this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setObjectName(QStringLiteral("CWidget"));
    setStyleSheet(QStringLiteral("CWidget{border-image:none; border:0px; background:transparent;}"));//仅仅设置CWidget类对象无边框，背景透明，其子窗口和控件不受影响

    m_Scene = new CGraphicsScene(0,0,1200,900);
    setScene(m_Scene);
    resize(1200,900);//尽量大

/***********************************************************************/

    QString sDllName = "Mems.dll";
//    ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }
}

CWidget::~CWidget()
{
	//ShareMemoryRelease();
	//killTimer(m_nTimerID);
    saveToIniFile();
	qDebug() << "start destructor";
}

void CWidget::setFileCfg(const QString &path)
{
    m_sFileCfg = path;

    QSettings *s = new QSettings(m_sFileCfg, QSettings::IniFormat);
    m_bIsEdit = s->value("editState/isEdit", 1).toBool();

    m_Scene->enableEdit(m_bIsEdit); //用于是否产生右键菜单，编辑状态下产生
//    s->setIniCodec(QTextCodec::codecForLocale());
    s->setIniCodec(QTextCodec::codecForName("UTF-8"));
    m_sFileBackPic = s->value("background/pic").toString();

    if(m_bIsEdit)
    {
        int botton = QMessageBox::information(this, "Change", tr("是否要更新背景"),
            QMessageBox::Ok, QMessageBox::Cancel);
        if(botton == QMessageBox::Ok)
        {
            QString openfilename = QFileDialog::getOpenFileName( this, tr("打开一张图片作为背景图"), ".", "Images (*.bmp *.jpeg *.png)");
            if(!openfilename.isEmpty())
                m_sFileBackPic = openfilename;
        }
    }

    delete s;
    s = NULL;

    initBack();
    initAllItems();

    if(!m_bIsEdit)
        m_nTimerID = startTimer(1000);
}

QSize CWidget::getWindowSizie()
{
    return QSize();
}

/**
  **@brief 用来配置当前背景,修改窗体、view、scene的大小位置居中
  **
  */
void CWidget::initBack()
{
    if(m_sFileBackPic.isEmpty())
        return;

    //设定窗体的大小
    QPixmap pixmap(m_sFileBackPic);
//    setFixedSize(pixmap.size());
    resize(pixmap.size());

    m_Scene->setSceneRect(0,0, pixmap.width(), pixmap.height());

    QBrush background(pixmap);
    //对scene设置背景，背景大小是View的大小
    m_Scene->setBackgroundBrush(background);
}

void CWidget::initAllItems()
{
    QSettings saveInSettings(m_sFileCfg,QSettings::IniFormat);
    saveInSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList allkeys;
    QString value;
    QString group;

    group = "In";
    saveInSettings.beginGroup(group);
    allkeys = saveInSettings.allKeys();//获取In节点下所有的Keys
    for(int i=0; i<allkeys.count(); i++)
    {
        value = saveInSettings.value(allkeys.at(i)).toString();
        stringToItemData(value,group);
    }
    saveInSettings.endGroup();

    group = "Out";
    saveInSettings.beginGroup(group);
    allkeys = saveInSettings.allKeys();//获取Out节点下所有的Keys
    for(int i=0; i<allkeys.count(); i++)
    {
        value = saveInSettings.value(allkeys.at(i)).toString();
        stringToItemData(value,group);
    }
    saveInSettings.endGroup();
}

void CWidget::stringToItemData(QString data, QString type)
{
    int x;
    int y;
    int w;
    int h;
    QString name;
    int eventNumber;
    if(type == "In")
    {
        CBarItem* bar = new CBarItem(m_bIsEdit);
        if(!m_bIsEdit)
            bar->hide();
        if(data.startsWith('{') && data.endsWith('}'))
        {
            data = data.mid(1,data.size()-2);//去除{}
            qDebug() << data;
            QStringList splitDatas = data.split(";");
            for(int i=0; i<splitDatas.count(); i++)
            {
                QString value = splitDatas.at(i);
                value = value.trimmed();
                if(value.contains("pos:", Qt::CaseInsensitive))
                {
                    value.remove("pos:", Qt::CaseInsensitive);
                    QStringList lstValue = value.split(',');
                    x = lstValue.at(0).toDouble();
                    y = lstValue.at(1).toDouble();
                    bar->setPos(x,y);
                }
                else if(value.contains("size:", Qt::CaseInsensitive))
                {
                    value.remove("size:", Qt::CaseInsensitive);
                    w = value.split(',').at(0).toDouble();
                    h = value.split(',').at(1).toDouble();
                    bar->setSize(w,h);
                }
                else if(value.contains("rotate:", Qt::CaseInsensitive))
                {
                    value.remove("rotate:", Qt::CaseInsensitive);
                    double angle = value.toDouble();
                    bar->setRotation(angle);
                }
                else if(value.contains("name:", Qt::CaseInsensitive))
                {
                    value.remove("name:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    bar->setCaptainName(value);
                }
                else if(value.contains("eventNumber:", Qt::CaseInsensitive))
                {
                    value.remove("eventNumber:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    bar->setEventNumbers(value);
                    /*
                     * value是一串事件号组成的字符串，由前后2部分组成，之间由"-"分割。
                     * 第一部分是启动事件号，第二部分是结束事件号。
                     * 每个部分都是由多个事件号组成的字符串，用:进行连接。
                     * 例如：#1:#2-#3:#4
                    */
                    if(!value.isEmpty())
                    {
                        QStringList list1 = value.split('-');
                        if(list1.count() > 0)
                        {
                            QStringList list2 = list1.at(0).split(':');
                            //获取当前item每个事件号与其对应的事件计数器;
                            for (int i = 0; i < list2.count(); i++)
                            {
                                //将当前item的事件号与计数器保存到Map中;
                                QString sEvt = list2.at(i);
                                sEvt = sEvt.trimmed();
                                int nCounter = m_psdEvts.evtCounter(sEvt);
                                m_mapEvtNum2Counter.insert(sEvt,nCounter);
                                m_mapEvtNum2BarsStart.insert(sEvt,bar);
                            }

                            if(list1.count() > 1)
                            {
                                QStringList list3 = list1.at(1).split(":");
                                //获取当前item每个事件号与其对应的事件计数器;
                                for (int i = 0; i < list3.count(); i++)
                                {
                                    //将当前item的事件号与计数器保存到Map中;
                                    QString sEvt = list3.at(i);
                                    sEvt = sEvt.trimmed();
                                    int nCounter = m_psdEvts.evtCounter(sEvt);
                                    m_mapEvtNum2Counter.insert(sEvt,nCounter);
                                    m_mapEvtNum2BarsStop.insert(sEvt,bar);
                                }
                            }
                        }
                    }
                }
                else if(value.contains("aniTime:", Qt::CaseInsensitive))
                {
                    value.remove("aniTime:", Qt::CaseInsensitive);
                    bar->setShowTime((int)(value.toDouble()*1000));
                }
                else if(value.contains("loop:", Qt::CaseInsensitive))
                {
                    value.remove("loop:", Qt::CaseInsensitive);
                    if(value.toInt() == 1)
                        bar->enableLoopAnimation(true);
                    else if(value.toInt() == 0)
                        bar->enableLoopAnimation(false);
                }
                else if(value.contains("ignoreEndEvt:", Qt::CaseInsensitive))
                {
                    value.remove("ignoreEndEvt:", Qt::CaseInsensitive);
                    if(value.toInt() == 1)
                        bar->enableIgnoreEndEvt(true);
                    else if(value.toInt() == 0)
                        bar->enableIgnoreEndEvt(false);
                }
                else if(value.contains("startDelay:", Qt::CaseInsensitive))
                {
                    value.remove("startDelay:", Qt::CaseInsensitive);
                    bar->setStartDelay((int)(value.toDouble()*1000));
                }
            }
        }
        m_Scene->addItem(bar);
    }
    else
    {
        COutItem *outItem= new COutItem(m_bIsEdit);
        connect(outItem, SIGNAL(EvtFileChange(int)), this, SLOT(SLOT_EvtFileChange(int)));
        connect(outItem, SIGNAL(sendCmd(QString)), this, SIGNAL(sendCmd(QString)));

        if(data.startsWith('{') && data.endsWith('}'))
        {
            data = data.mid(1,data.size()-2);    //去除{}
            qDebug() << data;
            QStringList splitDatas = data.split(";");
            for(int i=0; i<splitDatas.count(); i++)
            {
                QString value = splitDatas.at(i);
                value = value.trimmed();
                if(value.contains("pos:", Qt::CaseInsensitive))
                {
                    value.remove("pos:", Qt::CaseInsensitive);
                    QStringList lstValue = value.split(',');
                    x = lstValue.at(0).toDouble();
                    y = lstValue.at(1).toDouble();
                    outItem->setPos(x,y);
                }
                else if(value.contains("size:", Qt::CaseInsensitive))
                {
                    value.remove("size:", Qt::CaseInsensitive);
                    w = value.split(',').at(0).toDouble();
                    h = value.split(',').at(1).toDouble();
                    outItem->setSize(w,h);
                }
                else if(value.contains("name:", Qt::CaseInsensitive))
                {
                    value.remove("name:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    outItem->setCaptainName(value);
                }
                else if(value.contains("eventNumber:", Qt::CaseInsensitive))
                {
                    value.remove("eventNumber:", Qt::CaseInsensitive);
                    value = value.trimmed();
//                    eventNumber = value.toInt();
                    outItem->setEventNumbers(value.trimmed()); //参数修改成QString
                }
//                else if(value.contains("fileNumber:", Qt::CaseInsensitive))
//                {
//                    value.remove("fileNumber:", Qt::CaseInsensitive);
//                    fileNumber = value.toInt();
//                }
                else if(value.contains("shape:", Qt::CaseInsensitive))
                {
                    value.remove("shape:", Qt::CaseInsensitive);
                    outItem->setShape(value.toInt());
                    if(value.toInt() == 1)
                        outItem->isLine = true;
                }
                else if(value.contains("cmd:", Qt::CaseInsensitive))
                {
                    value.remove("cmd:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    if(!value.isEmpty())
                        outItem->setCommands(value);
                }
            }
        }
        m_Scene->addItem(outItem);
    }
}

void CWidget::saveToIniFile()
{
    if(!m_bIsEdit)
        return;

    QSettings saveInSettings(m_sFileCfg,QSettings::IniFormat);
//    saveInSettings.setIniCodec(QTextCodec::codecForLocale());
    saveInSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    saveInSettings.clear();
    saveInSettings.setValue("background/pic",m_sFileBackPic);
    saveInSettings.setValue("editState/isEdit",m_bIsEdit);

    int i=0,j=0;
    QString group;
    QString key;
//    m_Scene->items()包括了item中Widget的项，所以counts = 2 * item的count()

    qDebug() << saveInSettings.group();
    foreach (QGraphicsItem *item, m_Scene->items())
    {
        if(item->type() == CBarItem::Type)
        {
            group = "In";
            saveInSettings.beginGroup(group);
            CBarItem *bar = dynamic_cast<CBarItem*> (item);
            if(bar == NULL)
                continue;

            key = "InItem" + QString::number(++i);

            QString value = QString("{pos:%1; size:%2; rotate:%3; name:%4; eventNumber:%5; aniTime:%6; loop:%7;ignoreEndEvt:%8;startDelay:%9}")
                    .arg(QString("%1,%2").arg(bar->pos().x()).arg(bar->pos().y()))//%1
                    .arg(QString("%1,%2").arg(bar->m_Width).arg(bar->m_Height))//%2
                    .arg(QString::number(bar->rotation()))//%3
                    .arg(bar->getCaptainName())//%4
                    .arg(bar->getEventNumbers())//%5
                    .arg(bar->getShowTime()/1000.0f)//%6
                    .arg(bar->isLoopAnimation())//%7
                    .arg(bar->isIgnoreEndEvt())//%8
                    .arg(bar->getStartDelay()/1000.0f);//%9
            saveInSettings.setValue(key,value);

            saveInSettings.endGroup();
        }

        if(item->type() == COutItem::Type)
        {
            group = "Out";
            saveInSettings.beginGroup(group);
            COutItem *outItem = dynamic_cast<COutItem*> (item);
            key = "OutItem" + QString::number(++j);
            QString value = QString("{pos:%1; size:%2; eventNumber:%3; name:%4; shape:%5; cmd:%6}")
                    .arg(QString("%1,%2").arg(outItem->pos().x()).arg(outItem->pos().y()))
                    .arg(QString("%1,%2").arg(outItem->m_Width).arg(outItem->m_Height))
                    .arg(outItem->getEventNumbers())
                    .arg(outItem->getCaptainName())
                    .arg(outItem->getShape())
                    .arg(outItem->getCommands());

            saveInSettings.setValue(key,value);
            saveInSettings.endGroup();
        }

    }
    qDebug() << "saveToIniFile end" << endl ;
}

//文件事件改变
void CWidget::SLOT_EvtFileChange(int e)
{
    QString sDllName = "Mems.dll";
//    EVTFileChange(e,1);
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
        if (EVTFileChange)
            EVTFileChange(e,1);
    }
}

//单击按钮测试
void CWidget::btnClickTest()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(this->sender());
    QString sMsg = QString("%1被单击").arg(btn->text());
    QMessageBox::information(this,"提示",sMsg,QMessageBox::Ok);
}

void CWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID)
    {
        QMap<QString, int>::iterator it = m_mapEvtNum2Counter.begin();
        while(it != m_mapEvtNum2Counter.end())
        {
            QString sEvt = it.key();//事件号
            int nCounterOld = it.value();//旧的事件计数器
            int nCounterNew = m_psdEvts.evtCounter(sEvt);//新的事件计数器
            if(nCounterOld != nCounterNew)//事件计数器改变
            {
                //开始动画
                if(m_mapEvtNum2BarsStart.contains(sEvt))
                {
                    QList<CBarItem*> items = m_mapEvtNum2BarsStart.values(sEvt);
                    for (int i=0; i<items.count(); i++)
                    {
                        CBarItem *bar = items.at(i);
                        if(bar)
                        {
                            if(bar->getStartDelay() > 0)
                                QTimer::singleShot(bar->getStartDelay(),bar,&CBarItem::startAnimation);//延时启动动画
                            else
                                bar->startAnimation();//立即启动动画
                        }
                    }
                }

                //结束动画
                if(m_mapEvtNum2BarsStop.contains(sEvt))
                {
                    QList<CBarItem*> items = m_mapEvtNum2BarsStop.values(sEvt);
                    for (int i=0; i<items.count(); i++)
                    {
                        CBarItem *bar = items.at(i);
                        if(bar)
                            bar->stopAnimation();
                    }
                }

                //更新事件计数器
                it.value() =  nCounterNew;
            }
            it++;
        }
    }
}
