#ifndef CFGMGR_H
#define CFGMGR_H

#include <QString>
#include <QMap>
#include <QXmlStreamReader>

///*****
/// 配置文件管理
class CfgMgr
{
public:
    CfgMgr(QString sFile = "");
    ~CfgMgr();

    virtual int readFile();
    void readLanMonitor(QXmlStreamReader &xmlReader);
    void skipUnknowElement(QXmlStreamReader &xmlReader);

    void setFile(QString sFile);

    bool getValue(QString sKey,QString &sValue);

protected:
    QString m_sFile;//配置文件名
    QMap<QString,QString> m_mapCfg;//所有的配置信息
};

#endif // CFGMGR_H
