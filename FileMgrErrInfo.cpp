#include "FileMgrErrInfo.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "XxwQtPub.h"

FileMgrErrInfo::FileMgrErrInfo(QString sFile)
    :FileMgrBase(sFile)
{
}

//打开数据文件
int FileMgrErrInfo::openFile()
{
    //开始读取文件，格式是自定义
    QFile file(m_sFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << QStringLiteral("文件") << m_sFileName << QStringLiteral("打开失败\n");
        return 0;
    }

    QTextStream in(&file);
//    in.setCodec("UTF-8");//设置编码方式
    m_Data = in.readAll();

    file.close();

    FileMgrBase::openFile();

    return 1;
}
