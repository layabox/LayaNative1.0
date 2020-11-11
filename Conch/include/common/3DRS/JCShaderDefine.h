/**
@file			JCShaderDefine.h
@brief			
@author			James
@version		1.0
@date			2016_10_28
*/

#ifndef __JCShaderDefine_H__
#define __JCShaderDefine_H__

#include <stdio.h>
#include <string>
#include <map>

namespace laya
{
    /** 
     * @brief 
    */
    class JCShaderDefine
    {
    public:
        typedef std::map<long long, std::string>     MapValues;

    public:

        JCShaderDefine();

        ~JCShaderDefine();

        void addShaderDefine( long long nValue );

        void removeShaderDefine( long long nValue );

		void appendShaderDefine(JCShaderDefine p_kShaderDefine);

		static void destory();

		static void registShaderDefine(const char* name, long long value);

    public:

        void calcShaderDefineValue();

    public:
        static    MapValues     s_vDefineValues;
        static    int           s_nMaxLen;
        std::string             m_sDefineValue;
        long long               m_nDefineValue;
		bool			        m_bNeedCalc;

    };
}
//------------------------------------------------------------------------------


#endif //__JCShaderDefine_H__

//-----------------------------END FILE--------------------------------