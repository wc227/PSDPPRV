#ifndef MYCOMDATMGR_H
#define MYCOMDATMGR_H

#include <QString>
#include <QMap>
#include "PubHead.h"
#include "MyCompInfo.h"

///
/// \brief 计算机信息管理，用于检测和读取计算机信息文件
///
class CMyComDatMgr
{
public:
    CMyComDatMgr();

    CMyComDatMgr(QString file);

    ~CMyComDatMgr();

    //删除所有的数据
    void releaseAll();

    //设置文件名
    void setFilenName(QString file);

    QString getFileName() const
    {
        return m_sFileName;
    }

    //获取最近更新文件的时间（单位：毫秒），since 1970-01-01T00:00:00.000
    qint64 lastModifiedTime() const
    {
        return CXxwQtPub::getLastModifiedTime(m_sFileName);
    }

    //文件是否发生了改变
    bool isDirty() const
    {
        return m_timeUpdateOld != lastModifiedTime();
    }

    //添加新的计算机
    void addComp(CMyCompInfo* pComp);

    //打开文件，并读取数据
    bool openFile();

    QMap<QString,CMyCompInfo*> datComps() const
    {
        return m_datComps;
    }

private:
    //文件名称
    QString m_sFileName;

    //上次更新时间
    qint64 m_timeUpdateOld;

    //所有的计算机信息的映射，key:计算机ip，value:CMyCompInfo类指针
    QMap<QString,CMyCompInfo*> m_datComps;
};

#endif // MYCOMDATMGR_H
