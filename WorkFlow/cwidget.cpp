#include "cwidget.h"
#include "cbaritem.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>


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

    m_Scene = new CGraphicsScene(0,0,500,300);
    this->setScene(m_Scene);

/***********************************************************************/
    QString sDllName = "";
#ifdef QT_NO_DEBUG
    sDllName = "Mems.dll";
#else
    sDllName = "Memsd.dll";
#endif
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

QSize CWidget::getWindowSizie()
{
    return QSize();
}

/**
  **@brief 用来配置当前背景,修改窗体、view、scene的大小位置居中
  **
  */
void CWidget::currentBackground()
{
    if(m_BackgroundPicName.isEmpty())
        return;

    //设定窗体的大小
    QPixmap pixmap(m_BackgroundPicName);
	//this->setFixedSize(pixmap.size());
	this->resize(pixmap.size());
    m_Scene->setSceneRect(0,0, pixmap.width(), pixmap.height());

    //scaled()函数返回一个QPixmap 给定大小的窗口
    QBrush background(pixmap/*.scaled(800,600)*/);

    //对scene设置背景，背景大小是View的大小
    m_Scene->setBackgroundBrush(background);

	m_picWidth = pixmap.size().width();
	m_picHeight = pixmap.size().height();
}

void CWidget::initAllItemsFromIniFile()
{
    QSettings saveInSettings(m_IniCFGPath,QSettings::IniFormat);

    QString group;
    group = "In";
    saveInSettings.beginGroup(group);

    QStringList allkeys = saveInSettings.allKeys();//获取In节点下所有的Keys
    QString value;

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

void CWidget::saveToIniFile()
{
	if(!isEdit)
        return;
    QSettings saveInSettings(m_IniCFGPath,QSettings::IniFormat);
    saveInSettings.setIniCodec(QTextCodec::codecForLocale());

    saveInSettings.clear();
    saveInSettings.setValue("background/pic",m_BackgroundPicName);
    saveInSettings.setValue("editState/isEdit",!isEdit);

    int i=0,j=0;
    QString group;
    QString key;
//    m_Scene->items()包括了item中Widget的项，所以counts = 2 * item的count()

    qDebug() << saveInSettings.group();
    foreach (QGraphicsItem *item, m_Scene->items()) {
        if(item->type() == CBarItem::Type)
        {
            group = "In";
            saveInSettings.beginGroup(group);
            CBarItem *currentItem = dynamic_cast<CBarItem*> (item);
            if(currentItem == NULL)
                continue;

            key = "InItem" + QString::number(++i);

            QString value = QString("{pos:%1; size:%2; rotate:%3; name:%4; eventNumber:%5; fileNumber:%6; showTime:%7}")
                    .arg(QString("%1,%2").arg(currentItem->pos().x()).arg(currentItem->pos().y()))//%1
                    .arg(QString("%1,%2").arg(currentItem->m_Width).arg(currentItem->m_Height))//%2
                    .arg(QString::number(currentItem->rotation()))//%3
                    .arg(currentItem->getCaptainName())//%4
                    .arg(currentItem->getEventNumbers())
                    .arg(" ")
                    .arg(currentItem->getShowTime());
            saveInSettings.setValue(key,value);

            saveInSettings.endGroup();
        }

        if(item->type() == COutItem::Type)
        {
            group = "Out";
            saveInSettings.beginGroup(group);
            COutItem *currentItem = dynamic_cast<COutItem*> (item);
            key = "OutItem" + QString::number(++j);
            QString value = QString("{pos:%1; size:%2; eventNumber:%3; name:%4; shape:%5}")
                    .arg(QString("%1,%2").arg(currentItem->pos().x()).arg(currentItem->pos().y()))
                    .arg(QString("%1,%2").arg(currentItem->m_Width).arg(currentItem->m_Height))
                    .arg(currentItem->getEventNumber())
                    .arg(currentItem->getCaptainName())
                    .arg(currentItem->getShape());

            saveInSettings.setValue(key,value);
            saveInSettings.endGroup();
        }

    }
    qDebug() << "saveToIniFile end" << endl ;
}

void CWidget::stringToItemData(QString data, QString type)
{
    int x;
    int y;
    int w;
    int h;
    int eventNumber;
    int fileNumber;
    QColor color;
    QString name;

    QGraphicsItem *item;

    if(type == "In")
    {
        item = new CBarItem(isEdit);
        dynamic_cast<CBarItem*> (item)->setShowTime(showTime);
        barItems.append(dynamic_cast<CBarItem*> (item));
        if(!isEdit)
            item->hide();
    }
    else
    {
        item= new COutItem(isEdit);
        connect(static_cast<COutItem*>(item), SIGNAL(EvtFileChange(int)), this, SLOT(SLOT_EvtFileChange(int)));
    }

    if(data.startsWith('{') && data.endsWith('}'))
    {
        data = data.mid(1);    //去除首位{}
        /*data = */data.chop(1);
        qDebug() << data;

        QStringList splitDatas = data.split(";");
        for(int i=0; i<splitDatas.count(); i++)
        {
            QString value = splitDatas.at(i);
            value = value.trimmed();
            if(value.contains("pos:", Qt::CaseInsensitive))
            {
                value.remove("pos:", Qt::CaseInsensitive);
                x = value.split(',').at(0).toDouble();
                y = value.split(',').at(1).toDouble();
                if(type == "In")
                {
                    dynamic_cast<CBarItem*> (item)->setPos(x,y);
                }
                else
                {
                    dynamic_cast<COutItem*> (item)->setPos(x,y);
                }
            }
            else if(value.contains("size:", Qt::CaseInsensitive))
            {
                value.remove("size:", Qt::CaseInsensitive);
                w = value.split(',').at(0).toDouble();
                h = value.split(',').at(1).toDouble();
                if(type == "In")
                {
                    dynamic_cast<CBarItem*> (item)->setSize(w,h);
                }
                else
                {
                    dynamic_cast<COutItem*> (item)->setSize(w,h);
                }

            }
            else if(value.contains("rotate:", Qt::CaseInsensitive))
            {
                value.remove("rotate:", Qt::CaseInsensitive);
//                color = QColor(value);
                double angle = value.toDouble();
                if(type == "In")
                {
//                    dynamic_cast<CBarItem*> (item)->setBackGroundColor(color);
                     dynamic_cast<CBarItem*> (item)->setRotation(angle);
                }
            }
            else if(value.contains("name:", Qt::CaseInsensitive))
            {
                value.remove("name:", Qt::CaseInsensitive);
                value = value.trimmed();

                if(type == "In")
                    dynamic_cast<CBarItem*> (item)->setCaptainName(value);
                else
                    dynamic_cast<COutItem*> (item)->setCaptainName(value);
            }
            else if(value.contains("eventNumber:", Qt::CaseInsensitive))
            {
                value.remove("eventNumber:", Qt::CaseInsensitive);
				value = value.trimmed();
                eventNumber = value.toInt();

                if(type == "In")
                {
                    dynamic_cast<CBarItem*> (item)->setEventNumber(value);
                    /*if(eventNumber != 0)
                    {
                        m_NumberToCounter.insert(eventNumber, EVTFileCounter(eventNumber));
						m_StrEvtsToCounters.insert(value, m_psdEvts.evtCounter(value));
                    }*/
					//判断获取value是否为空;
					if(!value.isEmpty())
					{
						//value是一串事件号组成的字符串，用:进行连接，所以通过:得到所有事件号保存到list中;
						value = value.trimmed();
						QStringList _evts = value.split(":");
                        //qDebug() << _evts.at(1) << _evts.at(0);

						//获取当前item每个事件号与其对应的事件计数器;
						for (int i=0; i<_evts.count(); i++)
						{
							qDebug() << "add _evts to map";
							//将当前item的事件号与计数器保存到Map中;

							m_StrEvtsToCounters.insert(_evts.at(i),m_psdEvts.evtCounter(_evts[i]));
							//判断对应事件号的是否对应多个item;
							//如果没有对应的item则新添加到map中;
							QMap<QString, QList<QGraphicsItem*>>::iterator it = m_StrEvtsToItems.find(_evts[i]);
							QList<QGraphicsItem*> items;
							//如果在map中找到已有的事件号，则添加到已有事件号对应的value中;
							if(it == m_StrEvtsToItems.end())
							{
								items.append(item);
							}
							else
							{
								items = it.value();
								items.append(item);
							}
							m_StrEvtsToItems.insert(_evts[i],items);
						}
					}
                }
                else
                {
                    dynamic_cast<COutItem*> (item)->setEventNumber(value.trimmed()); //参数修改成QString
                }
            }
            else if(value.contains("fileNumber:", Qt::CaseInsensitive))
            {
                value.remove("fileNumber:", Qt::CaseInsensitive);
                fileNumber = value.toInt();
            }
            else if(value.contains("shape:", Qt::CaseInsensitive))
            {
                value.remove("shape:", Qt::CaseInsensitive);
                dynamic_cast<COutItem*> (item)->setShape(value.toInt());
                if(value.toInt() == 1)
                    dynamic_cast<COutItem*> (item)->isLine = true;
            }
            else if(value.contains("showTime:", Qt::CaseInsensitive))
			{
                value.remove("showTime:", Qt::CaseInsensitive);
				dynamic_cast<CBarItem*> (item)->setShowTime(value.toInt());
			}
            else
            {

            }
        }
    }
    this->m_Scene->addItem(item);
}

void CWidget::SLOT_EvtFileChange(int e)
{
    QString sDllName = "";
#ifdef QT_NO_DEBUG
    sDllName = "Mems.dll";
#else
    sDllName = "Memsd.dll";
#endif
//    EVTFileChange(e,1);
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
        if (EVTFileChange)
            EVTFileChange(e,1);
    }
}

void CWidget::timerEvent(QTimerEvent *)
{

	QMap<QString, int>::iterator it = m_StrEvtsToCounters.begin();
	while(it != m_StrEvtsToCounters.end())
	{
		QString key = it.key();
		if(it.value() != m_psdEvts.evtCounter(key))
		{
			it.value() =  m_psdEvts.evtCounter(key);
			QList<QGraphicsItem*> items = m_StrEvtsToItems.find(key).value();
			for (int i=0; i<items.count(); i++)
			{
				CBarItem *item = dynamic_cast<CBarItem*>(items[i]);
				if(item)
					item->startAnimation();
			}
		}
		it++;
	}
}

void CWidget::setIniPath(const QString &path)
{
	m_IniCFGPath = path;
	
	QSettings *s = new QSettings(/*"in.ini"*/m_IniCFGPath, QSettings::IniFormat);
	isEdit = s->value("editState/isEdit", 1).toBool();

	m_Scene->b_IsEdit = isEdit; //用于是否产生右键菜单，编辑状态下产生
	s->setIniCodec(QTextCodec::codecForLocale());
	m_BackgroundPicName = s->value("background/pic").toString();

	if(isEdit)
	{
		int botton = QMessageBox::information(this, "Change", tr("是否要更新背景"),
			QMessageBox::Ok, QMessageBox::Cancel);
		if(botton == QMessageBox::Ok)
		{
			QString openfilename = QFileDialog::getOpenFileName( this, tr("打开一张图片作为背景图"), ".", "Images (*.bmp *.jpeg *.png)");
			if(!openfilename.isEmpty())
				m_BackgroundPicName = openfilename;
		}
	}

	delete s;
	s = NULL;

	currentBackground();
	initAllItemsFromIniFile();

	if(!isEdit)
		m_nTimerID = startTimer(1000);
}
