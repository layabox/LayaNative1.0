/**
@file           JCGpuProgram.h
@brief            
@author         guo
@version        1.0
@date           2016_5_26
*/

#ifndef __JCGpuProgram_H__
#define __JCGpuProgram_H__
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include <string>
#include <map>
#include <vector>

namespace laya{

    /** 
     * @brief uniform属于哪个类型的
    */
    enum UNIFORM_PERIOD
    {
        UNIFORM_PERIOD_SUBMESH = 0,  //submesh
        UNIFORM_PERIOD_MATERIAL,     //材质
        UNIFORM_PERIOD_RENDER_OBJECT,//renderObject
        UNIFORM_PERIOD_CAMERA,       //摄像机
        UNIFORM_PERIOD_SCENE,        //场景
        UNIFORM_PERIOD_COUNT,        //
    };

    class JCNamedData;
    class JCGpuProgram;
    class JCGpuProgramTemplate {
    public:
        struct UniformInfo
        {
            //type是指 unifrom属于哪个类的，目前有的种类
            UniformInfo(UNIFORM_PERIOD nType, int nSemanticsEnum)
            {
                m_nType = nType;
                m_nSemanticsEnum = nSemanticsEnum;
            }
            UNIFORM_PERIOD m_nType;
            int m_nSemanticsEnum;
        };
    public:

        JCGpuProgramTemplate(const char* p_pszVS, const char* p_pszPS);

        ~JCGpuProgramTemplate();

        void setSrc(const char* p_pszVS, const char* p_pszPS);
        
        JCGpuProgram* getInst(const char* p_Macro,int p_nKey);

        void freeGLResource();

        void addAttrSemantics(int nSemanticsEnum,const char* sName);

        void addUniformSemantics(UNIFORM_PERIOD nType,int nSemanticsEnum,const char* sName);
        
        int getAttrSemanticsName(const char* pVarName);

        UniformInfo* getUniformInfo(const char* pVarName);
        
        bool splitKeyValue(const char* pStr, std::map<std::string, std::string>& out);

    public:
        std::string                         m_strVS;
        std::string                         m_strPS;
        std::map<int, JCGpuProgram*>        m_ShaderCache;
        std::map<std::string,int>           m_vAttrSemantics;
        std::map<std::string, UniformInfo*> m_vUniformSemantics;
    };

    /*
        gpuProgram 是一种资源，为了恢复，需要保留源代码或者对应的文件
    */
    class JCGpuProgram{
    public:
        //固定的名字
        static const char* shader_SamplerPre;
        static const char* shader_CubeSamplerPre;
    public:
        
        JCGpuProgram();

        ~JCGpuProgram();

        bool checkCompile(int p_nShader, char* desc);
        
        void freeGLResource();

        void useProgram();

        GLuint getGpuProgram();
        
        bool compile();

    public:
        int                     m_nVS = 0;
        int                     m_nPS = 0;
        JCGpuProgramTemplate*   m_pTemp = nullptr;
        JCNamedData*            m_pUniformDesc=nullptr;
        std::string             m_strMacro;
    private:
        GLuint                  m_nProgram;
    };
}

#endif //__JCGpuProgram_H__

