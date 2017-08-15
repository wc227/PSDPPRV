#ifndef CFGMGR_H
#define CFGMGR_H

#include <QString>
#include <QMap>
#include <QXmlStreamReader>
#include "FileMgrBase.h"

///*****
/// 配置文件管理
class CfgMgr : public FileMgrBase
{
public:
    CfgMgr(QString sFile = "");
    ~CfgMgr();

    //清空所有数据
    virtual void releaseAll();

    //打开数据文件
    virtual int openFile();

    void readFileCfg(QXmlStreamReader &xmlReader);

    void skipUnknowElement(QXmlStreamReader &xmlReader);

    bool getValue(QString sKey,QString &sValue);

protected:
    QMap<QString,QString> m_mapCfg;//所有的配置信息
};

#endif // CFGMGR_H
