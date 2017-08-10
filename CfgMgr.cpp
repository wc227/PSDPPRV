#include "CfgMgr.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>


CfgMgr::CfgMgr(QString sFile)
    :m_sFile(sFile)
{
}

CfgMgr::~CfgMgr()
{
    m_mapCfg.clear();
}

void CfgMgr::setFile(QString sFile)
{
    if(0 == m_sFile.compare(sFile,Qt::CaseInsensitive))
        return;
    m_sFile = sFile;
    readFile();
}

int CfgMgr::readFile()
{
    m_mapCfg.clear();

    QFile file(m_sFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QStringLiteral("错误"),QStringLiteral("无法打开文件——%1").arg(m_sFile));
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
            if (xmlReader.name() == "LanMonitor")
            {
                readLanMonitor(xmlReader);
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
        QMessageBox::critical(NULL,QStringLiteral("错误"),QStringLiteral("文件——%1 解析失败").arg(m_sFile));
        qDebug() << "Failed to parse file " << m_sFile << "\n";
        return 0;
    }
    else if (file.error() != QFile::NoError)
    {
        QMessageBox::critical(NULL, QStringLiteral("错误"),QStringLiteral("无法读取文件——%1").arg(m_sFile));
        qDebug() << "Cannot read file " << m_sFile << "\n";
        return 0;
    }
    return 1;
}


void CfgMgr::readLanMonitor(QXmlStreamReader &xmlReader)
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
            if(xmlReader.name() == "LanMonitor")
            {
                readLanMonitor(xmlReader);
            }
            else if(xmlReader.name() == "FileCfg")
            {
                QString sKey = xmlReader.attributes().value("key").toString();
                QString sValue = xmlReader.attributes().value("filename").toString();
                m_mapCfg.insert(sKey,sValue);
                QString sText = xmlReader.readElementText();
                if(xmlReader.isEndElement())
                    xmlReader.readNext();
            }
            else if(xmlReader.name() == "UrlCfg")
            {
                QString sKey = xmlReader.attributes().value("key").toString();
                QString sValue = xmlReader.attributes().value("url").toString();
                m_mapCfg.insert(sKey,sValue);
                QString sText = xmlReader.readElementText();
                if(xmlReader.isEndElement())
                    xmlReader.readNext();
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
