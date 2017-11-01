#include "XxwQtPub.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QStringList>
#include <QDir>

//删除字符串左侧所有连续的字符c
QString& CXxwQtPub::trimeLeft(QString& str,QChar c)
{
    if(str.isEmpty())
        return str;

    int nLen = str.length();
    int nc = 0;//左侧开始连续的字符c的个数
    for(int i = 0; i < nLen; i++)
    {
        if(c == str[i])
            nc++;
        else
            break;
    }

    if(nc > 0)
        str = str.right(nLen - nc);

    return str;
}


//删除字符串右侧所有连续的字符c
QString& CXxwQtPub::trimeRight(QString& str,QChar c)
{
    if(str.isEmpty())
        return str;

    int nLen = str.length();
    int nc = 0;//左侧开始连续的字符c的个数
    for(int i = nLen - 1; i >= 0; i--)
    {
        if(c == str[i])
            nc++;
        else
            break;
    }

    if(nc > 0)
        str = str.left(nLen - nc);

    return str;
}


//删除字符串左右两侧所有连续的字符c
QString& CXxwQtPub::trimeLeftRight(QString& str,QChar c)
{
    trimeLeft(str,c);
    trimeRight(str,c);
    return str;
}


//静态函数：将文件路径标准化（qt默认路径分隔符是'/',而不是'\\'）
void CXxwQtPub::standardFilePath(QString &filePath)
{
//    filePath.replace('\\','/');
    filePath = QDir::toNativeSeparators(filePath);
}


//判断两个文件是否是同一个文件（文件必须存在，且文件内容相同）
bool CXxwQtPub::isSameFilePath(QString file1,QString file2)
{
    QFileInfo fi1(file1);
    QFileInfo fi2(file2);
    if( !fi1.exists() || !fi1.isFile() || !fi2.exists() || !fi2.isFile() )
        return false;//两个文件必须都存在，否则返回false

    return fi1 == fi2;
}


//获取最近修改文件的时间（单位：毫秒），since 1970-01-01T00:00:00.000
qint64 CXxwQtPub::getLastModifiedTime(QString file)
{
    QFileInfo fi(file);
    if(fi.exists() && fi.isFile())
        return fi.lastModified().toMSecsSinceEpoch();//上次修改的时间
    else
        return 0;//文件不存在或不是文件
}


//获取最近访问文件的时间（单位：毫秒），since 1970-01-01T00:00:00.000
qint64 CXxwQtPub::getLastReadTime(QString file)
{
    QFileInfo fi(file);
    if(fi.exists() && fi.isFile())
        return fi.lastRead().toMSecsSinceEpoch();//上次访问的时间
    else
        return 0;//文件不存在或不是文件
}


#define  MTOIP(x1,x2,x3,x4)     (((x1)<<24)|((x2)<<16)|((x3)<<8)|(x4))
#define  MIP1(x)                (((x)>>24)&0xFF)
#define  MIP2(x)                (((x)>>16)&0xFF)
#define  MIP3(x)                (((x)>>8)&0xFF)
#define  MIP4(x)                ((x)&0xFF)
//ip地址从qlonglong类型转换成字符串类型
QString CXxwQtPub::IPLongToStr(qlonglong lIP)
{
    QString sIP = QString("%1.%2.%3.%4").arg(MIP4(lIP)).arg(MIP3(lIP)).arg(MIP2(lIP)).arg(MIP1(lIP));
    return sIP;
}

//IP地址字符串转换为qlonglong类型
qlonglong CXxwQtPub::IPStrToLong( const QString sIP )
{
    qlonglong Rtn = 0;
    if (sIP == "")
    {
        return Rtn;
    }//end if

    QStringList sl = sIP.split('.');
    if(sl.count() == 4)
    {
        int x1 = sl[3].toInt();
        int x2 = sl[2].toInt();
        int x3 = sl[1].toInt();
        int x4 = sl[0].toInt();
        Rtn = MTOIP(x1,x2,x3,x4);
    }
    return Rtn;
}

//功能：判断字符是否是空白字符（\r,\n,\t,空格 等等）
//例：
//IsWhiteSpace('a')结果是false
//IsWhiteSpace(' ')结果是true
bool CXxwQtPub::IsWhiteSpace(QChar c)
{
    return (' ' == c || '\t' == c  || '\n' == c  || '\r' == c );
}

//功能：用空白字符来分割源字符串（多个连续的空白字符将被视为1个空白字符,首末两端的空白字符将被忽视）,
//     可根据参数来决定是否忽略引号中字符串
//说明：此函数主要用于E格式的文本文件的解析。
//参数:
//	sSrs: const QString &类型，待分割的字符串
//  bIgnoreQuote:是否忽略引号中的字符串（即不对引号中的字符串进行相同的处理），默认是true
//返回值：QStringList类型，分割得到的多个字符串
//例如： “  1  2   ”将被分割成“1”和“2”共2个字符串
// “  1  \"a b\" 2   ”将被分割成“1”,"\"a b\"",“2”共3个字符串
QStringList CXxwQtPub::SplitStrByWhiteSpace(const QString & sSrs,bool bIgnoreQuote)
{
    QStringList retVal;//存储分割后的字符串的容器,同时也是返回值

    if (sSrs.isEmpty())
        return retVal;

    int nLen = sSrs.length();//源字符串长度
    int nWhiteSpace = 0;//连续字符的个数
    int nQuote = 0;//遇到的第几个引号，从1开始计算
    QString sTmp("");
    QChar ch;
    int i = 0;
    int last = 0;
    for (; i < nLen; i++)
    {
        ch = sSrs.at(i);

        //遇到引号
        if('\'' == ch || '\"' == ch )
        {
            nQuote++;//引号数加1
            nWhiteSpace = 0;//nWhiteSpace置0
            continue;
        }

        //不处理引号中中冗余字符内容对引号中
        //1 == nQuote%2表示处于一
        if(1 == nQuote%2 && bIgnoreQuote)
        {
            continue;
        }

        //其它普通的情况
        if (IsWhiteSpace(ch))
        {
            nWhiteSpace++;
            if (1 == nWhiteSpace)//第一次遇到该字符
            {
                if(i > last)
                {
                    sTmp = sSrs.mid(last,i-last);
                    //删除两侧的引号
                    trimeLeftRight(sTmp,'\'');
                    trimeLeftRight(sTmp,'\"');
                    retVal.append(sTmp);
                }
            }
            last = i + 1;//如果遇到了空白字符，last指向i的下一个
        }
        else
        {
            nWhiteSpace = 0;//非该字符，nWhiteSpace置0
        }
    } //End For...i

    if(i > last)
    {
        sTmp = sSrs.mid(last,i-last);
        //删除两侧的引号
        trimeLeftRight(sTmp,'\'');
        trimeLeftRight(sTmp,'\"');
        retVal.append(sTmp);
    }

    return retVal;
}

//获取可执行程序当前路径
QString CXxwQtPub::currentExeDir()
{
    return QCoreApplication::applicationDirPath();
}

//获取当前可执行程序带路径的文件名
QString CXxwQtPub::currentExeFilePath()
{
    return QCoreApplication::applicationFilePath();
}

QFileInfoList CXxwQtPub::GetFileList(QString path, bool bInSub)
{
    QDir dir(path);

    //提取目录下所有的文件
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    //遍历子目录
    if(bInSub)
    {
        QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(int i = 0; i != folder_list.size(); i++)
        {
            QString name = folder_list.at(i).absoluteFilePath();
            QFileInfoList child_file_list = GetFileList(name);
            file_list.append(child_file_list);
        }
    }

    return file_list;
}

// 复制文件路径
bool CXxwQtPub::copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

//清空文件夹目录，保留目录
void CXxwQtPub::clearDir(QString sDir)
{
    QDir dir(sDir);
    if(!dir.exists())
        return;

    QFileInfoList file_list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    foreach (QFileInfo fi, file_list) {
        if(fi.isFile() || fi.isSymLink()){
            QFile::setPermissions(fi.absoluteFilePath(), QFile::WriteOwner);//修改权限，使得可以删除
            QFile::remove(fi.absoluteFilePath());
        }
        else if(fi.isDir()) {
            QDir dir(fi.absoluteFilePath());
            dir.removeRecursively();
        }
    }
}
