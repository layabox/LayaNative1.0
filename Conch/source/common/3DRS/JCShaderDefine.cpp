/**
@file			JCShaderDefine.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_28
*/

#include "JCShaderDefine.h"
#include "../util/Log.h"
#include <string>
#include <cmath>

//------------------------------------------------------------------------------
namespace laya
{
	JCShaderDefine::MapValues JCShaderDefine::s_vDefineValues;
	int JCShaderDefine::s_nMaxLen = 0;
	void JCShaderDefine::destory()
	{
		s_nMaxLen = 0;
		s_vDefineValues.clear();
	}

    //由于android编不过std::log2,所以用这个函数了
    int mylog2(long long v) {
        switch (v){
        case 0x80000000:return 31;
        case 0x40000000:return 30;
        case 0x20000000:return 29;
        case 0x10000000:return 28;
        case 0x8000000:return 27;
        case 0x4000000:return 26;
        case 0x2000000:return 25;
        case 0x1000000:return 24;
        case 0x800000:return 23;
        case 0x400000:return 22;
        case 0x200000:return 21;
        case 0x100000:return 20;
        case 0x80000:return 19;
        case 0x40000:return 18;
        case 0x20000:return 17;
        case 0x10000:return 16;
        case 0x8000:return 15;
        case 0x4000:return 14;
        case 0x2000:return 13;
        case 0x1000:return 12;
        case 0x800:return 11;
        case 0x400:return 10;
        case 0x200:return 9;
        case 0x100:return 8;
        case 0x80:return 7;
        case 0x40:return 6;
        case 0x20:return 5;
        case 0x10:return 4;
        case 0x8:return 3;
        case 0x4:return 2;
        case 0x2:return 1;
        case 0x1:return 0;
        default:
            return 0;
        }
    }

	void JCShaderDefine::registShaderDefine(const char* name, long long value)
	{
		int temp=mylog2(value);
		if (temp > s_nMaxLen)s_nMaxLen = temp;
		s_vDefineValues[value] = name;
	}
    JCShaderDefine::JCShaderDefine()
    {
        m_sDefineValue = "";
        m_nDefineValue = 0l;
        m_bNeedCalc = false;
    }
    JCShaderDefine::~JCShaderDefine()
    {

    }
    void JCShaderDefine::addShaderDefine(long long nValue)
    {
		if ((m_nDefineValue&nValue)!=nValue)
		{
			m_nDefineValue |= nValue;
            m_bNeedCalc = true;
		}
    }
    void JCShaderDefine::removeShaderDefine(long long nValue)
    {
		if ((m_nDefineValue&nValue)==nValue)
		{
			m_nDefineValue &= ~nValue;
            m_bNeedCalc = true;
		}
    }

	void JCShaderDefine::appendShaderDefine(JCShaderDefine p_kShaderDefine)
	{
		m_nDefineValue |= p_kShaderDefine.m_nDefineValue;
        m_bNeedCalc = true;
	}

    void JCShaderDefine::calcShaderDefineValue()
    {
		if (m_bNeedCalc)
		{
			m_sDefineValue = "";
			std::string sBuffer;
			int d = 1;
			for (int i = 0; i <= s_nMaxLen; i++) 
            {
				d = 1 << i;
				if (d > m_nDefineValue) break;
				if (m_nDefineValue & d) 
                {
					std::string name = s_vDefineValues[d];
					if (name != "")
					{
						sBuffer = "#define " + name + "\n";
						m_sDefineValue += sBuffer;
					}
				}
			}
            m_bNeedCalc = false;
		}
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------