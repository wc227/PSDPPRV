#ifndef FILEMGRBASE_H
#define FILEMGRBASE_H

#include <QString>

///
/// \brief The FileMgrBase class:数据管理基类
///
class FileMgrBase
{
public:
    FileMgrBase(QString sFile = "");

    //设置文件名称（含绝对路径）
    void setFileName(const QString& sFile);

    //获取文件名称（含绝对路径）
    QString getFileName(){return m_sFileName;}

    //文件是否改变
    bool isDirty();

    //清空所有数据
    virtual void releaseAll(){}

    //打开数据文件
    virtual int openFile();

protected:
    QString m_sFileName;//文件名称（含绝对路径）
    qint64 m_nTimeLastModify;//最近更新的时间
};

#endif // FILEMGRBASE_H
