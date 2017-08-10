#include "MyComDatMgr.h"
#include <QFileInfo>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

CMyComDatMgr::CMyComDatMgr()
    :m_sFileName(QStringLiteral(""))
    ,m_timeUpdateOld(0)
{
}

CMyComDatMgr::CMyComDatMgr(QString file)
{
    setFilenName(file);
}

CMyComDatMgr::~CMyComDatMgr()
{
    releaseAll();
}

//删除所有的数据
void CMyComDatMgr::releaseAll()
{
    if(m_datComps.isEmpty())
        return;

    foreach (CMyCompInfo* pInfo, m_datComps.values())
    {
        if(pInfo)
        {
            delete pInfo;
            pInfo = NULL;
        }
    }
    m_datComps.clear();
}

//设置文件名
void CMyComDatMgr::setFilenName(QString file)
{
    CXxwQtPub::standardFilePath(file);
    if(CXxwQtPub::isSameFilePath(file,m_sFileName))
        return;//两个文件相同
    m_sFileName = file;

    openFile();//读取文件
}

//添加新的计算机
void CMyComDatMgr::addComp(CMyCompInfo* pComp)
{
    if(pComp)
        m_datComps.insert(pComp->IP(),pComp);
}

//打开文件，并读取数据
bool CMyComDatMgr::openFile()
{
    if(!isDirty())
        return false;

    releaseAll();

    //开始读取文件，格式是自定义
    QFile file(m_sFileName);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << QStringLiteral("文件") << m_sFileName << QStringLiteral("打开失败\n");
        return false;
    }

    QTextStream in(&file);
    QString sLine;
    while (!in.atEnd())
    {
        //@ 序号 ip 名称 操作系统 处理器 内存 类型 是否在线 任务数
        sLine = in.readLine();//读取一行数据
        if(sLine.isEmpty())
            continue;
        QChar ch = sLine.at(0);
        if('#' == ch)
        {
            QStringList svLine = CXxwQtPub::SplitStrByWhiteSpace(sLine);
            if(svLine.count() >= 10)
            {
                CMyCompInfo* pInfo = new CMyCompInfo;
                pInfo->SetIP(svLine.at(2));
                pInfo->SetName(svLine.at(3));
                pInfo->SetOS(svLine.at(4));
                pInfo->SetCPU(svLine.at(5));
                pInfo->SetRAM(svLine.at(6));
                pInfo->SetCompType((CMyCompInfo::Comp_Type)(svLine.at(7).toInt()));
                pInfo->SetOnline((bool)(svLine.at(8).toInt()));
                pInfo->SetTaskCount(svLine.at(9).toInt());
                m_datComps.insert(pInfo->IP(),pInfo);
            }
        }
    }

    m_timeUpdateOld = lastModifiedTime();
    return true;
}
