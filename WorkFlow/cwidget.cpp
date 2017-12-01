#include "cwidget.h"
#include "cbaritem.h"
#include "coutitem.h"
#include "CXAnimatePolyline.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QStringList>
#include <QTimer>

const QString c_GroupName_AnimateBar = "AnimateBar";
const QString c_GroupName_PolyLine = "PolyLine";
const QString c_GroupName_OutItem = "OutItem";

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

CXGraphicsView::CXGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setObjectName(QStringLiteral("CXGraphicsView"));
    setStyleSheet(QStringLiteral("CXGraphicsView{border-image:none; border:0px; background:transparent;}"));//仅仅设置CWidget类对象无边框，背景透明，其子窗口和控件不受影响

    setDragMode(QGraphicsView::RubberBandDrag);//

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

CXGraphicsView::~CXGraphicsView()
{
    //ShareMemoryRelease();
    //killTimer(m_nTimerID);
    saveToIniFile();
    qDebug() << "start destructor";
}

void CXGraphicsView::setFileCfg(const QString &path)
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

QSize CXGraphicsView::getWindowSizie()
{
    return QSize();
}

/**
  **@brief 用来配置当前背景,修改窗体、view、scene的大小位置居中
  **
  */
void CXGraphicsView::initBack()
{
    if(m_sFileBackPic.isEmpty())
        return;

    //设定窗体的大小
    QPixmap pixmap(m_sFileBackPic);
//    setFixedSize(pixmap.size());//不能更改大小,导致主界面可停靠窗口无法调整带下
    resize(pixmap.size());

    m_Scene->setSceneRect(0,0, pixmap.width(), pixmap.height());

    QBrush background(pixmap);
    //对scene设置背景，背景大小是View的大小
    m_Scene->setBackgroundBrush(background);
}

void CXGraphicsView::initAllItems()
{
    QSettings mySettings(m_sFileCfg,QSettings::IniFormat);
    mySettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList allkeys;
    QString val;

    QStringList groups = mySettings.childGroups();
    foreach (QString group, groups)
    {
        mySettings.beginGroup(group);
        allkeys = mySettings.allKeys();//获取In节点下所有的Keys
        for(int i=0; i<allkeys.count(); i++)
        {
            val = mySettings.value(allkeys.at(i)).toString();
            if(val.startsWith('{') && val.endsWith('}'))
                val = val.mid(1,val.size()-2);
            stringToItemData(val,group);
        }
        mySettings.endGroup();
    }
}

void CXGraphicsView::stringToItemData(QString sVal, QString sValsGroupName)
{
    qDebug() << sVal;
    int x;
    int y;
    int w;
    int h;
    QString name;
    int eventNumber;
    if(sValsGroupName == c_GroupName_AnimateBar)
    {
        CXAnimateBar* bar = new CXAnimateBar(m_bIsEdit);
        QStringList splitDatas = sVal.split(";");
        for(int i=0; i<splitDatas.count(); i++)
        {
            QString value = splitDatas.at(i);
            value = value.trimmed();
            if(value.contains("name:", Qt::CaseInsensitive))
            {
                value.remove("name:", Qt::CaseInsensitive);
                value = value.trimmed();
                bar->setCaptainName(value);
            }
            else if(value.contains("pos:", Qt::CaseInsensitive))
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
        m_Scene->addItem(bar);
    }
    else if(sValsGroupName == c_GroupName_PolyLine)
    {
        CXAnimatePolyline* polyline = new CXAnimatePolyline();
        polyline->enableEditMode(m_bIsEdit);
        QStringList splitDatas = sVal.split(";");
        for(int i=0; i<splitDatas.count(); i++)
        {
            QString value = splitDatas.at(i);
            value = value.trimmed();
            if(value.contains("name:", Qt::CaseInsensitive))
            {
                value.remove("name:", Qt::CaseInsensitive);
                value = value.trimmed();
                polyline->setName(value);
            }
            else if(value.contains("pos:", Qt::CaseInsensitive))
            {
                value.remove("pos:", Qt::CaseInsensitive);
                QStringList lstValue = value.split(',');
                x = lstValue.at(0).toDouble();
                y = lstValue.at(1).toDouble();
                polyline->setPos(x,y);
            }
            else if(value.contains("points:", Qt::CaseInsensitive))
            {
                value.remove("points:", Qt::CaseInsensitive);
                QStringList lstValue = value.split('&');
                QPolygonF poly;
                for(int i=0; i<lstValue.count(); ++i)
                {
                    QString spt = lstValue.at(i);
                    spt = spt.mid(1,spt.size()-2);//去掉两端的()
                    QStringList xy = spt.split(',');
                    if(2 == xy.count())
                    {
                        QPointF pt(xy.at(0).toDouble(),xy.at(1).toDouble());
                        poly.append(pt);
                    }
                }
                polyline->setPolygon(poly);
            }
            else if(value.contains("eventNumber:", Qt::CaseInsensitive))
            {
                value.remove("eventNumber:", Qt::CaseInsensitive);
                value = value.trimmed();
                polyline->setEventNumbers(value);
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
                            m_mapEvtNum2BarsStart.insert(sEvt,polyline);
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
                                m_mapEvtNum2BarsStop.insert(sEvt,polyline);
                            }
                        }
                    }
                }
            }
        }
        m_Scene->addItem(polyline);
    }
    else if(sValsGroupName == c_GroupName_OutItem)
    {
        COutItem *outItem= new COutItem(m_bIsEdit);
        connect(outItem, SIGNAL(EvtFileChange(int)), this, SLOT(SLOT_EvtFileChange(int)));
        connect(outItem, SIGNAL(sendCmd(QString)), this, SIGNAL(sendCmd(QString)));
        QStringList splitDatas = sVal.split(";");
        for(int i=0; i<splitDatas.count(); i++)
        {
            QString value = splitDatas.at(i);
            value = value.trimmed();
            if(value.contains("name:", Qt::CaseInsensitive))
            {
                value.remove("name:", Qt::CaseInsensitive);
                value = value.trimmed();
                outItem->setCaptainName(value);
            }
            else if(value.contains("pos:", Qt::CaseInsensitive))
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
            else if(value.contains("eventNumber:", Qt::CaseInsensitive))
            {
                value.remove("eventNumber:", Qt::CaseInsensitive);
                value = value.trimmed();
                //                    eventNumber = value.toInt();
                outItem->setEventNumbers(value.trimmed()); //参数修改成QString
            }
            else if(value.contains("cmd:", Qt::CaseInsensitive))
            {
                value.remove("cmd:", Qt::CaseInsensitive);
                value = value.trimmed();
                if(!value.isEmpty())
                    outItem->setCommands(value);
            }
        }
        m_Scene->addItem(outItem);
    }
}

void CXGraphicsView::saveToIniFile()
{
    if(!m_bIsEdit)
        return;

    QSettings mySettings(m_sFileCfg,QSettings::IniFormat);
    //    mySettings.setIniCodec(QTextCodec::codecForLocale());
    mySettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    mySettings.clear();
    mySettings.setValue("background/pic",m_sFileBackPic);
    mySettings.setValue("editState/isEdit",m_bIsEdit);

    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    QString sValsGroupName;
    QString key;
    foreach (QGraphicsItem *item, m_Scene->items())
    {
        if(item->type() == CXAnimateBar::Type)
        {
            sValsGroupName = c_GroupName_AnimateBar;
            mySettings.beginGroup(sValsGroupName);
            CXAnimateBar *bar = qgraphicsitem_cast<CXAnimateBar*>(item);
            if(bar == NULL)
                continue;
            key = "Item" + QString::number(++i1);
            QString value = QString("{name:%1; pos:%2; size:%3; rotate:%4; eventNumber:%5; aniTime:%6; loop:%7;ignoreEndEvt:%8;startDelay:%9}")
                    .arg(bar->getCaptainName())
                    .arg(QString("%1,%2").arg(bar->pos().x()).arg(bar->pos().y()))
                    .arg(QString("%1,%2").arg(bar->m_Width).arg(bar->m_Height))
                    .arg(QString::number(bar->rotation()))
                    .arg(bar->getEventNumbers())
                    .arg(bar->getShowTime()/1000.0f)
                    .arg(bar->isLoopAnimation())
                    .arg(bar->isIgnoreEndEvt())
                    .arg(bar->getStartDelay()/1000.0f);
            mySettings.setValue(key,value);
            mySettings.endGroup();
        }
        else if(item->type() == CXAnimatePolyline::Type)
        {
            sValsGroupName = c_GroupName_PolyLine;
//            mySettings.beginGroup(sValsGroupName);
            CXAnimatePolyline *line = qgraphicsitem_cast<CXAnimatePolyline*>(item);
            if(line == NULL)
                continue;
            key = "Item" + QString::number(++i2);
            QString sPts("");
            QPolygonF poly = line->polygon();
            if(poly.count() > 0)
            {
                QPointF pt = poly.at(0);
                QString spt = QString("(%1,%2)").arg(pt.x()).arg(pt.y());
                sPts += spt;
                for(int i=1; i<poly.count(); ++i)
                {
                    pt = poly.at(i);
                    spt = QString("(%1,%2)").arg(pt.x()).arg(pt.y());
                    sPts += "&";
                    sPts += spt;
                }
            }
            QString value = QString("{name:%1; pos:%2; points:%3; eventNumber:%4}")
                    .arg(line->getName())
                    .arg(QString("%1,%2").arg(line->pos().x()).arg(line->pos().y()))//%1
                    .arg(sPts)
                    .arg(line->getEventNumbers());
            mySettings.setValue(sValsGroupName + "/" + key,value);
//            mySettings.endGroup();
        }
        else if(item->type() == COutItem::Type)
        {
            sValsGroupName = c_GroupName_OutItem;
            mySettings.beginGroup(sValsGroupName);
            COutItem *outItem = qgraphicsitem_cast<COutItem*> (item);
            key = "Item" + QString::number(++i3);
            QString value = QString("{name:%1; pos:%2; size:%3; eventNumber:%4;cmd:%5}")
                    .arg(outItem->getCaptainName())
                    .arg(QString("%1,%2").arg(outItem->pos().x()).arg(outItem->pos().y()))
                    .arg(QString("%1,%2").arg(outItem->m_Width).arg(outItem->m_Height))
                    .arg(outItem->getEventNumbers())
                    .arg(outItem->getCommands());
            mySettings.setValue(key,value);
            mySettings.endGroup();
        }
    }
    qDebug() << "saveToIniFile end" << endl ;
}

//文件事件改变
void CXGraphicsView::SLOT_EvtFileChange(int e)
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
void CXGraphicsView::btnClickTest()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(this->sender());
    QString sMsg = QString("%1被单击").arg(btn->text());
    QMessageBox::information(this,"提示",sMsg,QMessageBox::Ok);
}

void CXGraphicsView::timerEvent(QTimerEvent *event)
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
                    QList<QGraphicsItem*> items = m_mapEvtNum2BarsStart.values(sEvt);
                    for (int i=0; i<items.count(); i++)
                    {
                        QGraphicsItem *item = items.at(i);
                        if(CXAnimateBar::Type == item->type())
                        {
                            CXAnimateBar *bar = qgraphicsitem_cast<CXAnimateBar *>(item);
                            if(bar->getStartDelay() > 0)
                                QTimer::singleShot(bar->getStartDelay(),bar,&CXAnimateBar::startAnimation);//延时启动动画
                            else
                                bar->startAnimation();//立即启动动画
                        }
                        else if(CXAnimatePolyline::Type == item->type())
                        {
                            CXAnimatePolyline *line = qgraphicsitem_cast<CXAnimatePolyline *>(item);
                            line->startAnimation();//立即启动动画
                        }
                    }
                }

                //结束动画
                if(m_mapEvtNum2BarsStop.contains(sEvt))
                {
                    QList<QGraphicsItem*> items = m_mapEvtNum2BarsStop.values(sEvt);
                    for (int i=0; i<items.count(); i++)
                    {
                        QGraphicsItem *item = items.at(i);
                        if(CXAnimateBar::Type == item->type())
                        {
                            CXAnimateBar *bar = qgraphicsitem_cast<CXAnimateBar *>(item);
                            bar->stopAnimation();
                        }
                        else if(CXAnimatePolyline::Type == item->type())
                        {
                            CXAnimatePolyline *line = qgraphicsitem_cast<CXAnimatePolyline *>(item);
                            line->stopAnimation();
                        }
                    }
                }

                //更新事件计数器
                it.value() =  nCounterNew;
            }
            it++;
        }
    }
}


// 上/下/左/右键向各个方向移动
void CXGraphicsView::keyPressEvent(QKeyEvent *event)
{
    int keyid = event->key();
    switch (keyid)
    {
    case Qt::Key_Up:
        moveSelectedItems(0, -1);  // 上移
        break;
    case Qt::Key_Down:
        moveSelectedItems(0, 1);  // 下移
        break;
    case Qt::Key_Left:
        moveSelectedItems(-1, 0);  // 左移
        break;
    case Qt::Key_Right:
        moveSelectedItems(1, 0);  // 右移
        break;
    case Qt::Key_Delete:
        if(m_Scene)
            m_Scene->slotDelItem();  //删除
        break;
        //    case Qt::Key_Plus:  // 放大
        //        zoomIn();
        //        break;
        //    case Qt::Key_Minus:  // 缩小
        //        zoomOut();
        //        break;
        //    case Qt::Key_Space:  // 逆时针旋转
        //        rotate(-5);
        //        break;
        //    case Qt::Key_Enter:  // 顺时针旋转
        //    case Qt::Key_Return:
        //        rotate(5);
        //        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


//移动选择的图元
void CXGraphicsView::moveSelectedItems(int x,int y)
{
    QList<QGraphicsItem *> items = scene()->selectedItems();
    foreach (QGraphicsItem *item, items)
    {
        item->moveBy(x,y);
    }
}
