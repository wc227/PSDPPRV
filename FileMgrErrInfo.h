#ifndef FILEMGRERRINFO_H
#define FILEMGRERRINFO_H

#include "FileMgrBase.h"

///
/// \brief 错误预警信息管理类
///
class FileMgrErrInfo: public FileMgrBase
{
public:
    FileMgrErrInfo(QString sFile = "");

    //打开数据文件
    virtual int openFile();

    QString getData() const
    {
        return m_Data;
    }

protected:
    QString m_Data;//数据
};

#endif // FILEMGRERRINFO_H
