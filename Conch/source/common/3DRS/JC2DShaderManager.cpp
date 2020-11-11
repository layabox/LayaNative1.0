/**
@file			JC2DShaderManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#include "JC2DShaderManager.h"


//------------------------------------------------------------------------------
namespace laya
{
    JC2DShaderManager::JC2DShaderManager()
    {
        for (int i = 0; i < SHADER_TYPE_COUNT; i++)
        {
            m_vShaders[i] = NULL;
        }
    }
    JC2DShaderManager::~JC2DShaderManager()
    {
        for (int i = 0; i < SHADER_TYPE_COUNT; i++)
        {
            if (m_vShaders[i])
            {
                delete m_vShaders[i];
                m_vShaders[i] = NULL;
            }
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
