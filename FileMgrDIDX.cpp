#include "FileMgrDIDX.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "XxwQtPub.h"
#include <QStringList>

FileMgrDIDX::FileMgrDIDX(QString sFile)
    :FileMgrBase(sFile)
{
}

//清空所有数据
void FileMgrDIDX::releaseAll()
{
    m_Data.clear();
}

//打开文件
int FileMgrDIDX::openFile()
{
//    if(!isDirty())
//        return 0;

    releaseAll();

    //开始读取文件，格式是自定义
    QFile file(m_sFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << QStringLiteral("文件") << m_sFileName << QStringLiteral("打开失败\n");
        return 0;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");//设置编码方式
    QString sLine;
    while (!in.atEnd())
    {
        sLine = in.readLine();//读取一行数据
        sLine = CXxwQtPub::trimeLeftRight(sLine);
        if(sLine.isEmpty())
            continue;

        QChar ch = sLine.at(0);
        if('#' == ch)//读取数据行
        {
            //$ 时间 时长 站点名称 裕度 类型 文件地址
            QStringList lstStr = CXxwQtPub::SplitStrByWhiteSpace(sLine);
            if(lstStr.count() >= 7)
            {
                BarInfo bi;
                bi.m_Time = lstStr.at(1);
                bi.m_Duration = lstStr.at(2).toLong();
                bi.m_Name = lstStr.at(3);
                bi.m_Margin = lstStr.at(4).toDouble();
                bi.m_Type = lstStr.at(5).toInt();
                bi.m_FilePath = lstStr.at(6);
                m_Data.append(bi);
            }
        }
    }

    file.close();

    FileMgrBase::openFile();

    return 1;
}
