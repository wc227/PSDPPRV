﻿#ifndef FILEMGRDIDX_H
#define FILEMGRDIDX_H

#include "FileMgrBase.h"
#include "XBar.h"

///
/// \brief The FileMgrDIDX class:DIDX文件管理类
///
class FileMgrDIDX : public FileMgrBase
{
public:
    FileMgrDIDX(QString sFile = "");

    //清空所有数据
    virtual void releaseAll();

    //打开数据文件
    virtual int openFile();

    ListBarInfo getData() const
    {
        return m_Data;
    }

protected:
    ListBarInfo m_Data;//数据
};

#endif // FILEMGRDIDX_H