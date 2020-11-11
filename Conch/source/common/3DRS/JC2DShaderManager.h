/**
@file			JC2DShaderManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#ifndef __JC2DShaderManager_H__
#define __JC2DShaderManager_H__

#include "JCGpuProgram.h"

namespace laya
{
    enum SHADER_TYPE
    {
        SHADER_TYPE_FILLCOLOR,
        SHADER_TYPE_DRAWIMAGE,
        SHADER_TYPE_FILLIMAGE,
        SHADER_TYPE_DRAWFILTERIMAGE,
        SHADER_TYPE_BLURIMG,
        SHADER_TYPE_GLOWIMG,
        SHADER_TYPE_MATFILTER,
        SHADER_TYPE_FILLIMAGE_MATFILTER,
        SHADER_TYPE_COUNT,
    };
    /** 
     * @brief 
    */
    class JC2DShaderManager
    {
    public:

        /** @brief构造函数
        */
        JC2DShaderManager();

        /** @brief析构函数
        */
        ~JC2DShaderManager();

        inline JCGpuProgram* getShader(SHADER_TYPE nShaderType)
        {
            return m_vShaders[nShaderType];
        }

    public:

        JCGpuProgram*    m_vShaders[SHADER_TYPE_COUNT];

    };
}

#endif //__JC2DShaderManager_H__

//-----------------------------END FILE--------------------------------
