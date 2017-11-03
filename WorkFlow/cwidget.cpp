﻿#include "cwidget.h"
#include "cbaritem.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>


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

    m_Scene->b_IsEdit = m_bIsEdit; //用于是否产生右键菜单，编辑状态下产生
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
        CBarItem* item = new CBarItem(m_bIsEdit);
        item->setShowTime(m_nTimeShow);
        if(!m_bIsEdit)
            item->hide();
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
                    item->setPos(x,y);
                }
                else if(value.contains("size:", Qt::CaseInsensitive))
                {
                    value.remove("size:", Qt::CaseInsensitive);
                    w = value.split(',').at(0).toDouble();
                    h = value.split(',').at(1).toDouble();
                    item->setSize(w,h);
                }
                else if(value.contains("rotate:", Qt::CaseInsensitive))
                {
                    value.remove("rotate:", Qt::CaseInsensitive);
                    double angle = value.toDouble();
                    item->setRotation(angle);
                }
                else if(value.contains("name:", Qt::CaseInsensitive))
                {
                    value.remove("name:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    item->setCaptainName(value);
                }
                else if(value.contains("eventNumber:", Qt::CaseInsensitive))
                {
                    value.remove("eventNumber:", Qt::CaseInsensitive);
                    value = value.trimmed();
//                    eventNumber = value.toInt();
                    item->setEventNumber(value);

                    //判断获取value是否为空;
                    if(!value.isEmpty())
                    {
                        //value是一串事件号组成的字符串，用:进行连接，所以通过:得到所有事件号保存到list中;
                        QStringList lstEvts = value.split(":");
                        //获取当前item每个事件号与其对应的事件计数器;
                        for (int i=0; i<lstEvts.count(); i++)
                        {
                            //将当前item的事件号与计数器保存到Map中;
                            QString sEvt = lstEvts.at(i);
                            int nCounter = m_psdEvts.evtCounter(sEvt);
                            m_mapEvtNum2Counter.insert(sEvt,nCounter);

                            //判断对应事件号的是否对应多个item;
                            //如果没有对应的item则新添加到map中;
                            QMap<QString, QList<QGraphicsItem*>>::iterator it = m_mapEvtNum2Items.find(sEvt);
                            QList<QGraphicsItem*> items;
                            //如果在map中找到已有的事件号，则添加到已有事件号对应的value中;
                            if(it == m_mapEvtNum2Items.end())
                            {
                                items.append(item);
                            }
                            else
                            {
                                items = it.value();
                                items.append(item);
                            }
                            m_mapEvtNum2Items.insert(sEvt,items);
                        }
                    }
                }
//                else if(value.contains("fileNumber:", Qt::CaseInsensitive))
//                {
//                    value.remove("fileNumber:", Qt::CaseInsensitive);
//                    fileNumber = value.toInt();
//                }
                else if(value.contains("aniTime:", Qt::CaseInsensitive))
                {
                    value.remove("aniTime:", Qt::CaseInsensitive);
                    item->setShowTime(value.toInt());
                }
                else if(value.contains("loop:", Qt::CaseInsensitive))
                {
                    value.remove("loop:", Qt::CaseInsensitive);
                    if(value.toInt() == 1)
                        item->enableLoopAnimation(true);
                    else if(value.toInt() == 0)
                        item->enableLoopAnimation(false);
                }
            }
        }
        m_Scene->addItem(item);
    }
    else
    {
        COutItem *item= new COutItem(m_bIsEdit);
        connect(item, SIGNAL(EvtFileChange(int)), this, SLOT(SLOT_EvtFileChange(int)));

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
                    item->setPos(x,y);
                }
                else if(value.contains("size:", Qt::CaseInsensitive))
                {
                    value.remove("size:", Qt::CaseInsensitive);
                    w = value.split(',').at(0).toDouble();
                    h = value.split(',').at(1).toDouble();
                    item->setSize(w,h);
                }
                else if(value.contains("name:", Qt::CaseInsensitive))
                {
                    value.remove("name:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    item->setCaptainName(value);
                }
                else if(value.contains("eventNumber:", Qt::CaseInsensitive))
                {
                    value.remove("eventNumber:", Qt::CaseInsensitive);
                    value = value.trimmed();
//                    eventNumber = value.toInt();
                    item->setEventNumber(value.trimmed()); //参数修改成QString
                }
//                else if(value.contains("fileNumber:", Qt::CaseInsensitive))
//                {
//                    value.remove("fileNumber:", Qt::CaseInsensitive);
//                    fileNumber = value.toInt();
//                }
                else if(value.contains("shape:", Qt::CaseInsensitive))
                {
                    value.remove("shape:", Qt::CaseInsensitive);
                    item->setShape(value.toInt());
                    if(value.toInt() == 1)
                        item->isLine = true;
                }
                else if(value.contains("cmd:", Qt::CaseInsensitive))
                {
                    value.remove("cmd:", Qt::CaseInsensitive);
                    value = value.trimmed();
                    if(!value.isEmpty())
                        item->setCommands(value);
                }
            }
        }
        m_Scene->addItem(item);
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
            CBarItem *currentItem = dynamic_cast<CBarItem*> (item);
            if(currentItem == NULL)
                continue;

            key = "InItem" + QString::number(++i);

            QString value = QString("{pos:%1; size:%2; rotate:%3; name:%4; eventNumber:%5; aniTime:%6; loop:%7}")
                    .arg(QString("%1,%2").arg(currentItem->pos().x()).arg(currentItem->pos().y()))//%1
                    .arg(QString("%1,%2").arg(currentItem->m_Width).arg(currentItem->m_Height))//%2
                    .arg(QString::number(currentItem->rotation()))//%3
                    .arg(currentItem->getCaptainName())//%4
                    .arg(currentItem->getEventNumbers())
                    .arg(currentItem->getShowTime())
                    .arg(currentItem->isLoopAnimation());
            saveInSettings.setValue(key,value);

            saveInSettings.endGroup();
        }

        if(item->type() == COutItem::Type)
        {
            group = "Out";
            saveInSettings.beginGroup(group);
            COutItem *currentItem = dynamic_cast<COutItem*> (item);
            key = "OutItem" + QString::number(++j);
            QString value = QString("{pos:%1; size:%2; eventNumber:%3; name:%4; shape:%5; cmd:%6}")
                    .arg(QString("%1,%2").arg(currentItem->pos().x()).arg(currentItem->pos().y()))
                    .arg(QString("%1,%2").arg(currentItem->m_Width).arg(currentItem->m_Height))
                    .arg(currentItem->getEventNumber())
                    .arg(currentItem->getCaptainName())
                    .arg(currentItem->getShape())
                    .arg(currentItem->getCommands());

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

void CWidget::timerEvent(QTimerEvent *)
{
    QMap<QString, int>::iterator it = m_mapEvtNum2Counter.begin();
    while(it != m_mapEvtNum2Counter.end())
	{
        QString sEvt = it.key();//事件号
        int nCounterOld = it.value();//旧的事件计数器
        int nCounterNew = m_psdEvts.evtCounter(sEvt);//新的事件计数器
        if(nCounterOld != nCounterNew)//事件计数器改变
        {
            //更新事件计数器
            it.value() =  nCounterNew;
            //该事件对应的所有图元显示动画
            if(m_mapEvtNum2Items.contains(sEvt))
            {
                QList<QGraphicsItem*> items = m_mapEvtNum2Items.find(sEvt).value();
                for (int i=0; i<items.count(); i++)
                {
                    CBarItem *item = dynamic_cast<CBarItem*>(items[i]);
                    if(item)
                        item->startAnimation();
                }
            }
		}
		it++;
	}
}

