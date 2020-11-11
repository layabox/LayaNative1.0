/**
@file			JCIDGenerator.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCIDGenerator.h"

namespace laya
{
    JCIDGenerator::JCIDGenerator()
    {
    }
    JCIDGenerator::~JCIDGenerator()
    {
    }
    bool JCIDGenerator::hasID(GLuint p_nID)
    {
        return (p_nID >= 0 && p_nID < m_vIDTable.size());
    }
    bool JCIDGenerator::deleteID(GLuint p_nFadeID)
    {
        //只设置为无效，不从vector中删除
        if (p_nFadeID >= 0 && p_nFadeID < m_vIDTable.size())
        {
            m_vIDTable[p_nFadeID] = INVALIDATE_ID;
        }
        return true;
    }
    bool JCIDGenerator::setRealID(GLuint p_nFadeID,GLuint p_nRealID,GLuint p_nType)
    {
        if (p_nFadeID == m_vIDTable.size())
        {
            m_vIDTable.push_back(p_nRealID);
        }
        else if (p_nFadeID > m_vIDTable.size())
        {
            int nLastSize = m_vIDTable.size();
            m_vIDTable.resize(p_nFadeID+1);
            //把中间的空的，全部填充成无效的
            for (int i = nLastSize,n=m_vIDTable.size(); i < n; i++)
            {
                m_vIDTable[i] = INVALIDATE_ID;
            }
            m_vIDTable[p_nFadeID] = p_nRealID;
        }
        else
        {
            m_vIDTable[p_nFadeID] = p_nRealID;
        }
        return true;
    }
    GLuint JCIDGenerator::getRealID(GLuint p_nFadeID)
    {
        if (p_nFadeID < 0 || p_nFadeID >= m_vIDTable.size())return INVALIDATE_ID;
        return m_vIDTable[p_nFadeID];
    }
    void JCIDGenerator::reset()
    {
        m_vIDTable.clear();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------