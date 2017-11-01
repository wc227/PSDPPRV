#ifndef CXXWPUBQT_H
#define CXXWPUBQT_H

#include <QString>
#include <QFileInfoList>

//qt常用的一些公共函数
namespace CXxwQtPub
{

//////////////////////////////////////////////////////////////
//字符串相关的函数

//删除字符串左侧所有连续的字符c
QString& trimeLeft(QString& str,QChar c = ' ');

//删除字符串右侧所有连续的字符c
QString& trimeRight(QString& str,QChar c = ' ');

//删除字符串左右两侧所有连续的字符c
QString& trimeLeftRight(QString& str,QChar c = ' ');

//静态函数：将文件路径标准化（qt默认路径分隔符是'/',而不是'\\'）
void standardFilePath(QString &filePath);

//判断两个文件是否是同一个文件（文件必须存在，且文件内容相同）
bool isSameFilePath(QString file1,QString file2);

//获取最近修改文件的时间（单位：毫秒），since 1970-01-01T00:00:00.000
qint64 getLastModifiedTime(QString file);

//获取最近访问文件的时间（单位：毫秒），since 1970-01-01T00:00:00.000
qint64 getLastReadTime(QString file);


//IP转换字符串
QString IPLongToStr(qlonglong lIP);

//字符串转换为IP
qlonglong IPStrToLong(const QString sIP);

//功能：判断字符是否是空白字符（\r,\n,\t,空格 等等）
//例：
//IsWhiteSpace('a')结果是false
//IsWhiteSpace(' ')结果是true
bool IsWhiteSpace(QChar c);

//功能：用空白字符来分割源字符串（多个连续的空白字符将被视为1个空白字符,首末两端的空白字符将被忽视）,
//     可根据参数来决定是否忽略引号中字符串
//说明：此函数主要用于E格式的文本文件的解析。
//参数:
//	sSrs: const QString &类型，待分割的字符串
//  bIgnoreQuote:是否忽略引号中的字符串（即不对引号中的字符串进行相同的处理），默认是true
//返回值：QStringList类型，分割得到的多个字符串
//例如： “  1  2   ”将被分割成“1”和“2”共2个字符串
// “  1  \"a b\" 2   ”将被分割成“1”,"\"a b\"",“2”共3个字符串
QStringList SplitStrByWhiteSpace(const QString & sSrs,bool bIgnoreQuote = true);

////////////////////////////////////////////////////////
//路径相关的函数

//获取可执行程序当前路径
QString currentExeDir();

//获取当前可执行程序带路径的文件名
QString currentExeFilePath();

///
/// \brief GetFileList:遍历文件夹(可通过参数bInSub来设置是否遍历子目录)
/// \param path:指定的文件夹路径
/// \param bInSub：是否遍历子文件夹，默认true
/// \return
///
QFileInfoList GetFileList(QString path, bool bInSub = true);

// 复制文件路径
bool copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath);

//清空文件夹目录，保留目录
void clearDir(QString sDir);

}

#endif // CXXWPUBQT_H
