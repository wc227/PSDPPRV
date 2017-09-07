#include "FormLanView.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QInputDialog>
#include "DlgSetting.h"
#include "MyPolylineItem.h"


int FormLanView::s_nGap =40;//边距
int FormLanView::s_nMax = 8;//每行的最大个数
int FormLanView::s_nTimeUpdate = 10;//刷新时间间隔,单位：秒

FormLanView::FormLanView(QWidget *parent) :
    QWidget(parent)
{
    Init();

    m_bUpdateRun = true;
    connect(&m_timerUpdate, SIGNAL(timeout()), this, SLOT(UpdateLan()));
    m_timerUpdate.start(s_nTimeUpdate * 1000);
}


//初始化
void FormLanView::Init()
{
    m_viewLan = new QGraphicsView();
    m_viewLan->setObjectName(QStringLiteral("m_viewLan"));
//    m_viewLan->setStyleSheet(QLatin1String("#m_viewLan{background-color: rgb(212, 229, 255);}\n"
//""));
    m_viewLan->setLineWidth(0);

    qreal w = CMyCompItem::s_nLen * s_nMax + s_nGap * (s_nMax -1);
    m_scene = new QGraphicsScene(0,0,w,600);
    m_viewLan->setScene(m_scene);
    m_viewLan->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_viewLan);
    setLayout(mainLayout);
}

FormLanView::~FormLanView()
{
}


//删除图形，并释放指针，防止内存泄露
void FormLanView::RemoveItem(QGraphicsItem *item)
{
    if(m_scene && item)
    {
        m_scene->removeItem(item);
        if(item)
        {
            delete item;
            item = 0;
        }
    }
}

//设置计算机信息文件
void FormLanView::SetFileCom(QString sFile)
{
    m_myComDat.setFilenName(sFile);
}

//clear all computers
void FormLanView::ClearCompData()
{
    m_myComDat.releaseAll();
}


//update the lan
void FormLanView::UpdateLan()
{
    if(!m_bUpdateRun)
        return;//不更新

    UpdateCompData();
    UpdateGraph();
}

void FormLanView::UpdateCompData()
{
    int nFlag = 0;//1-真实情况,0-测试情况
    if(nFlag)
    {
        //        //1 真实情况，从文本文件读取数据
        m_myComDat.openFile();
    }
    else
    {
        //2 测试
        m_myComDat.releaseAll();
        int nCount = rand() % 30;
        if(nCount < 5)
            nCount = 5;
        for(int i = 0; i < nCount; ++i)
        {
            QString sIP = QString("192.168.1.%1").arg(i+1);
            CMyCompInfo *pCompInfo = 0;
            pCompInfo = new CMyCompInfo();
            pCompInfo->SetIP(sIP);
            bool bOnline = rand() % 2;
            pCompInfo->SetOnline(bOnline);
            if(bOnline)
                pCompInfo->SetTaskCount(rand() % 10);
            else
                pCompInfo->SetTaskCount(0);

            if(nCount-1 == i)
            {
                pCompInfo->SetCompType(CMyCompInfo::Comp_Server);
            }
            else
            {
                int nTmp = rand() % 2;
                if(0 == nTmp)
                    pCompInfo->SetCompType(CMyCompInfo::Comp_Terminal);
                else
                    pCompInfo->SetCompType(CMyCompInfo::Comp_Calculate);
            }
            m_myComDat.addComp(pCompInfo);
        }
    }
}

void FormLanView::UpdateGraph()
{
    if(!m_scene)
        return;

    m_scene->clear();//删除所有图形

    QMap<QString, CMyCompItem *> mapCompsNew;//场景中已有的计算机图形
    CMyCompItem *pServer = 0;//管理服务器
    QStringList lstCalc;//计算节点IP
    QStringList lstTerm;//终端IP
    foreach (CMyCompInfo *pInfo, m_myComDat.datComps().values())
    {
        CMyCompItem* pComp = 0;
        pComp = new CMyCompItem();
        QString sMsg = QStringLiteral("IP：") + pInfo->IP()
                + QStringLiteral("\n操作系统：") + pInfo->OS()
                + QStringLiteral("\n处理器：") + pInfo->CPU()
                + QStringLiteral("\n内存：") + pInfo->RAM();
        pComp->setToolTip(sMsg);//设置提示，用于显示更多内容
        pComp->UpdateComp(*pInfo);
        pComp->setRect(0,0,CMyCompItem::s_nLen,CMyCompItem::s_nLen);
        mapCompsNew.insert(pInfo->IP(), pComp);
        m_scene->addItem(pComp);
        if(CMyCompInfo::Comp_Server == pInfo->CompType())
            pServer = pComp;//找到了管理节点
        else if(CMyCompInfo::Comp_Calculate == pInfo->CompType())
            lstCalc.append(pInfo->IP());
        else if(CMyCompInfo::Comp_Terminal == pInfo->CompType())
            lstTerm.append(pInfo->IP());
    }

    ///////////////////////////////////////////////////////////////////
    /*
        (1)计算节点和终端多（多于nMax），就分行显示
        控制节点处于中部，水平居中显示。
        计算节点处于上部，多行显示，每行最多显示nMax个，第一行可能排不满（排不满就水平居中显示），但是其他行都能排满。
        终端处于下部，多行显示，每行最多显示nMax个，最后一行可能排不满（排不满就水平居中显示），但是其他行都能排满。
    */
    int nCalc = lstCalc.count();//计算节点总数
    int nTerm = lstTerm.count();//终端总数
    int nRowsCalc = nCalc > 0 ? (1 + (nCalc - 1)/s_nMax) : 0;//计算节点行数
    int nRowsTerm = nTerm > 0 ? (1 + (nTerm - 1)/s_nMax) : 0;//终端节点行数
    int nRowsServer = (pServer == 0 ? 0 : 1);//管理节点行数（存在管理节点就占一行，不存在就占0行）
    int nRowsTotal = nRowsCalc + nRowsServer + nRowsTerm;//总行数
    qreal sceneWid = CMyCompItem::s_nLen * s_nMax + s_nGap * (s_nMax -1);//场景宽度
    qreal sceneHei = CMyCompItem::s_nLen * nRowsTotal + s_nGap * (nRowsTotal -1);//场景高度
    m_scene->setSceneRect(0, 0, sceneWid, sceneHei);//更新场景大小

    //管理节点单独一行,居中显示
    if(pServer)
    {
        qreal offX = (CMyCompItem::s_nLen + s_nGap) * (s_nMax/2 - 0.5f);
        qreal offY = (CMyCompItem::s_nLen + s_nGap) * nRowsCalc;
        pServer->setPos(offX, offY);
    }

    //计算节点处于上方，分行排列，每行最多只能有nMax个，从左到右依次排列
    if(nCalc > 0)
    {
        for(int i = 0; i < nCalc; ++i)
        {
            QString sIP = lstCalc.at(i);
            CMyCompItem* itemComp = mapCompsNew.value(sIP);
            if(itemComp)
            {
                int iRow = i / s_nMax;//所处的行（从0开始）
                int iCol = i % s_nMax;//所处的列（从0开始）
                qreal offX, offY;
                qreal nOffX;
                if(iRow < nRowsCalc - 1)
                {
                    //非最后一行，直接从左到右排满即可
                    offX = (CMyCompItem::s_nLen + s_nGap) * iCol;
                }
                else
                {
                    //最后一行，可能排不满，尽量居中对齐排列
                    int nComInLasRow1 = 1 + (nCalc - 1) % s_nMax;//最后一行的计算机个数
                    nOffX = (s_nMax - nComInLasRow1) / 2.0f;//水平平移的个数
                    offX = (CMyCompItem::s_nLen + s_nGap) * (iCol + nOffX);
                }
                //从最后一行往第一行逆向排列，这样做的好处是：让排不满的放在最上面一行，其他行都是排满的，好看。
                offY = (CMyCompItem::s_nLen + s_nGap) * (nRowsCalc - 1 - iRow);
                itemComp->setPos(offX, offY);
                if(pServer)
                    Link2Comps(pServer,itemComp);
            }
        }
    }

    //终端节点处于下方，分行排列，每行最多只能有nMax个，从左到右依次排列
    if(nTerm > 0)
    {
        int nRowOff = nRowsCalc + nRowsServer;
        for(int i = 0; i < nTerm; ++i)
        {
            QString sIP = lstTerm.at(i);
            CMyCompItem* itemComp = mapCompsNew.value(sIP);
            if(itemComp)
            {
                int iRow = i / s_nMax;//所处的行（从0开始）
                int iCol = i % s_nMax;//所处的列（从0开始）
                qreal offX, offY;
                qreal nOffX;
                if(iRow < nRowsTerm - 1)
                {
                    //非最后一行，直接从左到右排满即可
                    offX = (CMyCompItem::s_nLen + s_nGap) * iCol;
                }
                else
                {
                    //最后一行，可能排不满，尽量居中对齐排列
                    int nComInLasRow2 = 1 + (nTerm - 1) % s_nMax;//最后一行的计算机个数
                    nOffX = (s_nMax - nComInLasRow2) / 2.0f;//水平平移的个数
                    offX = (CMyCompItem::s_nLen + s_nGap) * (iCol + nOffX);
                }
                offY = (CMyCompItem::s_nLen + s_nGap) * (iRow + nRowOff);
                itemComp->setPos(offX, offY);
                if(pServer)
                    Link2Comps(pServer,itemComp);
            }
        }
    }

    m_scene->update();
}

//link 2 computers by polyline
//server is server
//client is client
bool FormLanView::Link2Comps(CMyCompItem *compServer, CMyCompItem *compClient)
{
    if( !compServer || !compClient)
        return false;

    QPointF pt1, pt2, pt3, pt4;
    if(CMyCompInfo::Comp_Calculate == compClient->Comp().CompType())
    {
        //pComp2是计算节点
        pt1 = compServer->pos() + compServer->rect().center();
        pt4 = compClient->pos() + compClient->rect().center();
        pt3 = pt4 + QPointF(0, (CMyCompItem::s_nLen + s_nGap) / 2.0f);;
        pt2 = QPointF(pt1.x(),pt3.y());
    }
    else if(CMyCompInfo::Comp_Terminal == compClient->Comp().CompType())
    {
        //pComp2是终端
        pt1 = compServer->pos() + compServer->rect().center();
        pt4 = compClient->pos() + compClient->rect().center();
        pt3 = pt4 - QPointF(0, (CMyCompItem::s_nLen + s_nGap) / 2.0f);;
        pt2 = QPointF(pt1.x(),pt3.y());
    }

    //用一条折线来表示连接线
    QPolygonF poly;
    poly.append(pt1);
    poly.append(pt2);
    poly.append(pt3);
    poly.append(pt4);
    CMyPolylineItem *polyItem = new CMyPolylineItem;
    polyItem->SetPolygon(poly);
    polyItem->setZValue(-1);//放到其他图形下面
    m_scene->addItem(polyItem);

    return true;
}



//set the timer step
void FormLanView::SetTimeStep()
{
    bool ok;
    int nStep = QInputDialog::getInt(this,QStringLiteral("更新时间间隔"),QStringLiteral("请输入秒数："),s_nTimeUpdate,1,100,1,&ok);
    if (ok && nStep > 0 && s_nTimeUpdate != nStep)
        //更新时间改变，定时器也得改变
    {
        s_nTimeUpdate = nStep;
        //        m_timerUpdate->start(s_nTimeUpdate * 1000);
        m_timerUpdate.setInterval(s_nTimeUpdate * 1000);
    }
}

//显示flag
void FormLanView::ShowFlag(bool show)
{
    if(CMyCompItem::bShowFlag != show)
    {
        CMyCompItem::bShowFlag = show;
        UpdateGraph();
    }
}


void FormLanView::UpdateSetting()
{
    DlgSetting dlg;
    dlg.nLength = CMyCompItem::s_nLen;
    dlg.nGap = s_nGap;
    dlg.nMax = s_nMax;
    dlg.updateUI(true);
    if(QDialog::Accepted == dlg.exec())
    {
        bool bSceneChange = false;//是否改变图形

        //计算机边长改变,就要修改计算机图形
        if(CMyCompItem::s_nLen != dlg.nLength)
        {
            bSceneChange = true;
            CMyCompItem::s_nLen = dlg.nLength;
            if(m_scene)
            {
                foreach (QGraphicsItem *item, m_scene->items())
                {
                    if(item->type() == CMyCompItem::Type)
                    {
                        CMyCompItem *pComp = static_cast<CMyCompItem *>(item);
                        pComp->setRect(QRectF(0,0,CMyCompItem::s_nLen,CMyCompItem::s_nLen));
                    }
                }
            }
        }

        if(s_nGap != dlg.nGap)
        {
            s_nGap = dlg.nGap;
            bSceneChange = true;
        }

        if(s_nMax != dlg.nMax)
        {
            s_nMax = dlg.nMax;
            bSceneChange = true;
        }

        if(bSceneChange)
            UpdateGraph();
    }
}
