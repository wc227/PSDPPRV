// MyComputerInfo.h: interface for the CMyCompInfo class.
//

#ifndef _CMyCompInfo_H_
#define _CMyCompInfo_H_

#include <QString>


//自定义的计算机信息类
class CMyCompInfo
{
public:
    //计算机类型
    enum Comp_Type
    {
        Comp_Server,      //管理节点（局域网中只有一个管理节点）
        Comp_Calculate,   //计算节点
        Comp_Terminal     //终端
    };

private:
    //静态类信息
    QString m_sIP;//ip地址
    QString m_sName;//名称
    QString m_sOS;//操作系统
    QString m_sCPU;//处理器
    QString m_sRAM;//内存

    //动态类信息
    Comp_Type m_CompType;//计算机类型
    bool m_bOnline;//是否在线
    uint m_nTaskCount;  //任务数

public:
    CMyCompInfo();

    virtual ~CMyCompInfo();

    //拷贝构造函数
    CMyCompInfo(const CMyCompInfo& comp);

    //拷贝赋值操作符
    CMyCompInfo & operator=(const CMyCompInfo& comp);

    //复制
    void copy(const CMyCompInfo& comp);

    //比较两个计算机信息是否相同
    bool EqualTo(const CMyCompInfo& comp) const;

public:

    //获取名称
    QString Name() const
    {
        return m_sName;
    }

    //设置名称
    void SetName(QString name)
    {
        m_sName = name;
    }

    //获取IP
    QString IP() const
    {
        return m_sIP;
    }

    //设置IP
    void SetIP(QString ip)
    {
        m_sIP = ip;
    }

    //获取OS
    QString OS() const
    {
        return m_sOS;
    }

    //设置OS
    void SetOS(QString os)
    {
        m_sOS = os;
    }

    //获取CPU
    QString CPU() const
    {
        return m_sCPU;
    }

    //设置CPU
    void SetCPU(QString cpu)
    {
        m_sCPU = cpu;
    }

    //获取RAM
    QString RAM() const
    {
        return m_sRAM;
    }

    //设置RAM
    void SetRAM(QString ram)
    {
        m_sRAM = ram;
    }

    //是否在线
    bool IsOnline() const
    {
        return m_bOnline;
    }

    //设置在线/离线
    void SetOnline(bool ol)
    {
        m_bOnline = ol;
    }


    //获取类型
    Comp_Type CompType() const
    {
        return m_CompType;
    }

    bool IsServer() const
    {
        return Comp_Server == m_CompType;
    }

    //设置类型
    void SetCompType(Comp_Type ct)
    {
        m_CompType = ct;
    }

    //任务数
    uint TaskCount() const
    {
        return m_nTaskCount;
    }

    //设置任务数
    void SetTaskCount(uint n)
    {
        m_nTaskCount = n;
    }
};

#endif //_CMyCompInfo_H_
