#include "filemgrbase.h"
#include <QDir>
#include "XxwQtPub.h"

FileMgrBase::FileMgrBase(QString sFile)
{
    m_sFileName = sFile;
}

    //设置文件名称（含绝对路径）
void FileMgrBase::setFileName(const QString& sFile)
{
    if(0 == m_sFileName.compare(sFile,Qt::CaseInsensitive) && !isDirty())
    {
        return;//如果文件名没有改变，且文件没有改变，就不用重新打开文件
    }
    m_sFileName = sFile;
    openFile();
    m_nTimeLastModify = CXxwQtPub::getLastModifiedTime(m_sFileName);
}

//文件是否改变
bool FileMgrBase::isDirty()
{
    if(QFile::exists(m_sFileName))
    {
        qint64 nTime = CXxwQtPub::getLastModifiedTime(m_sFileName);
        if(m_nTimeLastModify != nTime)
        {
            m_nTimeLastModify = nTime;
            return true;
        }
    }
    return false;
}

int FileMgrBase::openFile()
{
    m_nTimeLastModify = CXxwQtPub::getLastModifiedTime(m_sFileName);
    return 1;
}
