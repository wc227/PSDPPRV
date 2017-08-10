// MyComputerInfo.cpp: implementation of the CMyCompInfo class.
//
#include "MyCompInfo.h"

CMyCompInfo::CMyCompInfo():
    m_sName("New Computer"),
    m_sIP(""),
    m_sOS(""),
    m_sCPU(""),
    m_sRAM(""),
    m_CompType(Comp_Terminal),
    m_bOnline(false),
    m_nTaskCount(0)
{
}

CMyCompInfo::~CMyCompInfo()
{
}	

CMyCompInfo::CMyCompInfo(const CMyCompInfo& comp)//拷贝构造函数
{
    copy(comp);
}

CMyCompInfo& CMyCompInfo::operator=(const CMyCompInfo& comp)//拷贝赋值操作符
{
    copy(comp);
    return *this;
}


//复制
void CMyCompInfo::copy(const CMyCompInfo& comp)
{
    m_sName = comp.m_sName;
    m_sIP = comp.m_sIP;
    m_sOS = comp.m_sOS;
    m_sCPU = comp.m_sCPU;
    m_sRAM = comp.m_sRAM;
    m_CompType = comp.m_CompType;
    m_bOnline = comp.m_bOnline;
    m_nTaskCount = comp.m_nTaskCount;
}

//比较两个计算机信息是否相同
bool CMyCompInfo::EqualTo(const CMyCompInfo& comp) const
{
    return m_sName == comp.m_sName
            && m_sIP == comp.m_sIP
            && m_sOS == comp.m_sOS
            && m_sCPU == comp.m_sCPU
            && m_sRAM == comp.m_sRAM
            && m_CompType == comp.m_CompType
            && m_bOnline == comp.m_bOnline
            && m_nTaskCount == comp.m_nTaskCount;
}
