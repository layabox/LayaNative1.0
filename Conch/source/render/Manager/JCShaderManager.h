/**
@file			JCShaderManager.h
@brief			
@author			hugao
@version		1.0
@date			2016_6_11
*/

#ifndef __JCShaderManager_H__
#define __JCShaderManager_H__


//包含头文件
//------------------------------------------------------------------------------
#include <map>
#include <string>

namespace laya
{
    class JCGpuProgram;
    class JCShaderManager
    {
    public:
        struct ShaderContent
        {
            //const char* ps;
            //const char* vs;
            std::string ps;
            std::string vs;
            JCGpuProgram* pProg;
            ShaderContent() {
                pProg = nullptr;
            }
            ~ShaderContent();
        };
        typedef std::map<int, ShaderContent*>           MapShader;

    public:
        JCShaderManager();
        ~JCShaderManager();
        void createOrUpdateShader(int id, char *ps,char *vs);
        ShaderContent* getShaderById(int id);
        void invalidGLRes();
        void clear();

    private:
        MapShader       m_shaderMap;
    };
}
//------------------------------------------------------------------------------


#endif //__JCShaderManager_H__

//-----------------------------END FILE--------------------------------