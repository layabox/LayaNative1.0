/**
@file			JCProgramLocationTable.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCProgramLocationTable.h"

namespace laya
{
    #define   FADELOC_BASE   50000
    LocEnterValue::LocEnterValue() 
    {
        fadeProgramID = fadeLocIndex = realLocIndex = 0; 
    }
    JCProgramLocationTable::JCProgramLocationTable()
    {
        m_nMaxsize = 1024*4;
        m_pLocTable = new LocEnterValue[m_nMaxsize];
    }
    JCProgramLocationTable::~JCProgramLocationTable()
    {
        if (m_pLocTable)
        {
            delete[]m_pLocTable;
            m_pLocTable = NULL;
        }
    }      
    void JCProgramLocationTable::reset()
    {
        m_nMaxsize = 1024 * 4;
        if( m_pLocTable )
        {
            delete[]m_pLocTable;
            m_pLocTable = NULL;
        }
        m_pLocTable = new LocEnterValue[m_nMaxsize];
    }
    GLuint JCProgramLocationTable::getRealLocation(GLuint loc)
    {
        if(loc<FADELOC_BASE)
        {
            return loc;
        }
        GLuint  index = loc-FADELOC_BASE;

        if(index>= m_nMaxsize)
        {
            return 0xffffffff;
        }
        return m_pLocTable[index].realLocIndex;
    }
    void  JCProgramLocationTable::setRealLocation(GLuint fadeLoc,GLuint realLoc)
    {
        GLuint  index = fadeLoc-FADELOC_BASE;

        if(index>= m_nMaxsize)
        {
            return;
        }
        m_pLocTable[index].realLocIndex = realLoc;
    }
    GLuint JCProgramLocationTable::getFadeLocation(GLuint fadeProgramID,const char* name)
    {
        int i;
        int first=-1;
        for(i=0;i<(int)m_nMaxsize;++i)
        {
            if(fadeProgramID== m_pLocTable[i].fadeProgramID && m_pLocTable[i].name==name )
            {
                return (FADELOC_BASE+i);
            }
            if(m_pLocTable[i].fadeProgramID==0 && first<0)
            {
                first = i;
            }
        }
        if(first<(int)m_nMaxsize)
        {
            m_pLocTable[first].name = name;
            m_pLocTable[first].fadeLocIndex = FADELOC_BASE+first;
            m_pLocTable[first].fadeProgramID = fadeProgramID;
        }
        else
        {
            //TODO : expand buffer
        }
        return m_pLocTable[first].fadeLocIndex;
    }

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------