#include "CfgMgr.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>


CfgMgr::CfgMgr(QString sFile)
    :FileMgrBase(sFile)
{
}

CfgMgr::~CfgMgr()
{
    releaseAll();
}

void CfgMgr::releaseAll()
{
    m_mapCfg.clear();
}

int CfgMgr::openFile()
{
    releaseAll();

    QFile file(m_sFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QStringLiteral("错误"),QStringLiteral("无法打开文件——%1").arg(m_sFileName));
//        qDebug() << "Cannot read file " << m_sFile << "\n";
        return 0;
    }

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "PSDPPRV")
            {
                readPSDPPRV(xmlReader);
            }
            else
            {
                xmlReader.raiseError(QStringLiteral("不是一个有效的节点"));
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }

    file.close();
    if (xmlReader.hasError())
    {
        QMessageBox::critical(NULL,QStringLiteral("错误"),
                              QStringLiteral("文件——%1 解析失败:%2").arg(m_sFileName).arg(xmlReader.errorString()));
//        qDebug() << "Failed to parse file " << m_sFileName << "\n";
        return 0;
    }
    else if (file.error() != QFile::NoError)
    {
        QMessageBox::critical(NULL, QStringLiteral("错误"),
                              QStringLiteral("无法读取文件——%1:%2").arg(m_sFileName).arg(xmlReader.errorString()));
//        qDebug() << "Cannot read file " << m_sFileName << "\n";
        return 0;
    }
    return 1;
}

void CfgMgr::readPSDPPRV(QXmlStreamReader &xmlReader)
{
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if(xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            if(xmlReader.name() == "FileCfg")
            {
                readFileCfg(xmlReader);
            }
            else
            {
                skipUnknowElement(xmlReader);
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
}


void CfgMgr::readFileCfg(QXmlStreamReader &xmlReader)
{
//    QString sKey = xmlReader.attributes().value("key").toString();
//    QString sValue = xmlReader.attributes().value("value").toString();
//    m_mapCfg.insert(sKey,sValue);

//    QString sText = xmlReader.readElementText();
//    if(xmlReader.isEndElement())

    QString sKey="",sValue="",sCom="";
    xmlReader.readNext();

    while (!xmlReader.atEnd())
    {
        if(xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            if(xmlReader.name() == "PSDPPRV")
            {
                readPSDPPRV(xmlReader);
            }
            else if(xmlReader.name() == "FileCfg")
            {
                readFileCfg(xmlReader);
            }
            else if(xmlReader.name() == "key")
            {
                sKey = xmlReader.readElementText();
                if(xmlReader.isEndElement())
                {
                    xmlReader.readNext();
                }
            }
            else if(xmlReader.name() == "value")
            {
                sValue = xmlReader.readElementText();
                m_mapCfg.insert(sKey,sValue);
                if(xmlReader.isEndElement())
                {
                    xmlReader.readNext();
                }
            }
            else if(xmlReader.name() == "comment")
            {
                sCom = xmlReader.readElementText();
                if(xmlReader.isEndElement())
                {
                    xmlReader.readNext();
                }
            }
            else
            {
                skipUnknowElement(xmlReader);
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

void CfgMgr::skipUnknowElement(QXmlStreamReader &xmlReader)
{
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if(xmlReader.isStartElement())
            skipUnknowElement(xmlReader);
        else
            xmlReader.readNext();
    }
}

bool CfgMgr::getValue(QString sKey,QString &sValue)
{
    if(m_mapCfg.contains(sKey))
    {
        sValue = m_mapCfg.value(sKey);
        return true;
    }
    else
        return false;
}
