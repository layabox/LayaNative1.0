/**
@file			JSWebGLContext.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JSWebGLContext_H__
#define __JSWebGLContext_H__

#include "../JSInterface/jsobjbase.h"
#include "../JSInterface/JSInterface.h"
#include <WebGLRender/JCWebGLRender.h>
#include <3DRS/JCGpuProgram.h>

namespace laya
{
    class JSWebGLContext :public JsObjBase, public JSObjNode
	{
	public:
		static JsObjClassInfo JSCLSINFO;

		static void exportJS();

        JSWebGLContext();

		~JSWebGLContext();

        int getID();

        void setSize(int w, int h);

        int getProgramParameterEx(const char* vs,const char* ps,const char* define,int type);

        JsValue getActiveAttribEx(const char* vs, const char* ps, const char* define,int nIndex );

        JsValue getActiveUniformEx(const char* vs, const char* ps, const char* define,int nIndex );

    public:

        void _getProgramParameterEx(const std::string& vs, const std::string& ps, const std::string& define, int type);

        void _getActiveAttribEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex);

        void _getActiveUniformEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex);

        void _createShader(const std::string& vs, const std::string& ps, const std::string& define);

    public:

        int                             m_nID;

    private:

        int                             m_nProgramParameter;
        WebGLActiveInfo*                m_pShaderActiveInfo;
        JCGpuProgramTemplate*           m_pShaderTemplate;
        JCGpuProgram*                   m_pGpuProgram;
        std::string                     m_sShaderVS;
        std::string                     m_sShaderPS;
        std::string                     m_sShaderDefine;

	};
}
//------------------------------------------------------------------------------


#endif //__JSWebGLContext_H__

//-----------------------------END FILE--------------------------------