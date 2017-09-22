#include "tpsdevts.h"
#include <QDebug>
#include <QLibrary>


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

TPsdEvts::TPsdEvts(QObject *parent) :
    QObject(parent)
{
//    QString sDllName = "";
//#ifdef QT_NO_DEBUG
//    sDllName = "Mems.dll";
//#else
//    sDllName = "Memsd.dll";
//#endif
    QString sDllName = "Mems.dll";
    //ShareMemoryBuild();
    QLibrary lib(sDllName);
    if (lib.load())
    {
        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是?ShareMemoryBuild@@YAJXZ
        if (ShareMemoryBuild)
            ShareMemoryBuild();
    }
}

TPsdEvts::~TPsdEvts()
{

}

void TPsdEvts::setEventList(QList<QString> &arList, int val)
{
    QString sDllName = "Mems.dll";
    if (arList.size() == 0)
        return;

    QLibrary lib(sDllName);
    if (lib.load())
    {
//        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是 ?ShareMemoryBuild@@YAJXZ
        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
        FUN2 EVTTaskChange = (FUN2)lib.resolve("?EVTTaskChange@@YAHHH@Z");//dll中 EVTTaskChange 对应的实际是 ?EVTTaskChange@@YAHHH@Z
        FUN2 PSDAPPChange = (FUN2)lib.resolve("?PSDAPPChange@@YAHHH@Z");//dll中 PSDAPPChange 对应的实际是 ?PSDAPPChange@@YAHHH@Z
        FUN3 EVTStatus = (FUN3)lib.resolve("?EVTStatus@@YA_NH@Z");//dll中 EVTStatus 对应的实际是 ?EVTStatus@@YA_NH@Z
        FUN4 EVTStatusSet = (FUN4)lib.resolve("?EVTStatusSet@@YA_NH_N@Z");//dll中 EVTStatusSet 对应的实际是 ?EVTStatusSet@@YA_NH_N@Z
        FUN5 EVTTaskCounter = (FUN5)lib.resolve("?EVTTaskCounter@@YAHH@Z");//dll中 EVTTaskCounter 对应的实际是 ?EVTTaskCounter@@YAHH@Z
        FUN5 EVTFileCounter = (FUN5)lib.resolve("?EVTFileCounter@@YAHH@Z");//dll中 EVTFileCounter 对应的实际是 ?EVTFileCounter@@YAHH@Z

        QString  szTmp;
        szTmp.clear();
        int  nTmp  = 0;
        int  nEvtNo  = 0;
        bool bInvert = false;
        int  nMax  = arList.count();
        for (int i = 0; i < nMax; i++)
        {
            QString s = arList.at(i);
            nEvtNo = 0;
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
            nTmp = eventType(szTmp, nEvtNo);
            switch (nTmp)
            {
            case EVT_FILE_TYPE:
            {
                if(bInvert)
                    val*=-1;
                EVTFileChange(nEvtNo, val);
            }break;
            case EVT_TASK_TYPE:
            {
                if(bInvert)
                    val*=-1;
                EVTTaskChange(nEvtNo, val);
//                qDebug() << "Current Task Number:" << nEvtNo << "counter:"<< EVTTaskCounter(nEvtNo);
            }break;
            case EVT_STATUS_TYPE:
            {//与val无关
                bool b = EVTStatus(nEvtNo);
                if(bInvert) //反逻辑标记
                    EVTStatusSet(nEvtNo, !b);
                else
                    EVTStatusSet(nEvtNo, true); //直接设置为真状态[到应用时修订];
            }break;
            case EVT_APP_TYPE:
            {
                if(bInvert)
                    val*=-1;
                PSDAPPChange(nEvtNo, val);
            }break;
            default:
            {
                //cout<<"***无效事件类型:"<<nTmp<<endl;
            }break;
            }//end switch
        }//end for..
    }
}

int TPsdEvts::eventType(QString &sEvent, int &nEventNo)
{
    int Rtn=-1;
    if(sEvent == NULL)
    {
        return Rtn;
    }//end if

    int nTmp = 0;
    QString sEventTemp = sEvent;
    sEventTemp = sEventTemp.trimmed();
    if(sEventTemp.isEmpty())
        return Rtn;

    QChar ch = sEventTemp.at(0);
    if(ch == '#')//文件事件
    {
        sEventTemp.remove(0,1);
        nTmp = sEventTemp.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_FILE_TYPE;
            nEventNo = nTmp;
        }
    }
    else if(ch=='@')//任务事件
    {
        sEventTemp.remove(0,1);
        nTmp = sEventTemp.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_TASK_TYPE;
            nEventNo = nTmp;
        }
    }
    else if(ch=='$')//状态事件
    {
        sEventTemp.remove(0,1);
        nTmp = sEventTemp.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_STATUS_TYPE;
            nEventNo = nTmp;
        }
    }
    else if(ch=='?')//应用事件
    {
        sEventTemp.remove(0,1);
        nTmp = sEventTemp.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_APP_TYPE;
            nEventNo = nTmp;
        }
    }
    else
    {
        sEventTemp = sEventTemp.trimmed();
        sEvent.prepend('#');
        nTmp = sEventTemp.toInt();
        if(nTmp>0)
        {
            Rtn = EVT_FILE_TYPE;//缺省的处理内容;
            nEventNo = nTmp;
        }
    }
    return Rtn;
}

int TPsdEvts::evtCounter(QString &sEvent)
{
    int nCounter = 0;

    QString sDllName = "Mems.dll";
    QLibrary lib(sDllName);
    if (lib.load())
    {
//        FUN1 ShareMemoryBuild = (FUN1)lib.resolve("?ShareMemoryBuild@@YAJXZ");//dll中ShareMemoryBuild对应的实际是 ?ShareMemoryBuild@@YAJXZ
//        FUN2 EVTFileChange = (FUN2)lib.resolve("?EVTFileChange@@YAHHH@Z");//dll中 EVTFileChange 对应的实际是 ?EVTFileChange@@YAHHH@Z
//        FUN2 EVTTaskChange = (FUN2)lib.resolve("?EVTTaskChange@@YAHHH@Z");//dll中 EVTTaskChange 对应的实际是 ?EVTTaskChange@@YAHHH@Z
        FUN2 PSDAPPChange = (FUN2)lib.resolve("?PSDAPPChange@@YAHHH@Z");//dll中 PSDAPPChange 对应的实际是 ?PSDAPPChange@@YAHHH@Z
        FUN3 EVTStatus = (FUN3)lib.resolve("?EVTStatus@@YA_NH@Z");//dll中 EVTStatus 对应的实际是 ?EVTStatus@@YA_NH@Z
//        FUN4 EVTStatusSet = (FUN4)lib.resolve("?EVTStatusSet@@YA_NH_N@Z");//dll中 EVTStatusSet 对应的实际是 ?EVTStatusSet@@YA_NH_N@Z
        FUN5 EVTTaskCounter = (FUN5)lib.resolve("?EVTTaskCounter@@YAHH@Z");//dll中 EVTTaskCounter 对应的实际是 ?EVTTaskCounter@@YAHH@Z
        FUN5 EVTFileCounter = (FUN5)lib.resolve("?EVTFileCounter@@YAHH@Z");//dll中 EVTFileCounter 对应的实际是 ?EVTFileCounter@@YAHH@Z

        int nType = -1;  //事件类型
        int nEvtNo;        //事件值
        nType = eventType(sEvent, nEvtNo);
        switch (nType) {
        case EVT_FILE_TYPE:
            if(EVTFileCounter)
                nCounter = EVTFileCounter(nEvtNo);
            break;
        case EVT_TASK_TYPE:
            if(EVTTaskCounter)
                nCounter = EVTTaskCounter(nEvtNo);
            break;
        case EVT_STATUS_TYPE:
            if(EVTStatus)
                nCounter = (int)EVTStatus(nEvtNo);
            break;
        case EVT_APP_TYPE:
            if(PSDAPPChange)
                nCounter = PSDAPPChange(nEvtNo,0);
            break;
        default:
            break;
        }
    }
    return nCounter;
}
