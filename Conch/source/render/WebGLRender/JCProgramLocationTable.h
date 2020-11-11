/**
@file			JCProgramLocationTable.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCProgramLocationTable_H__
#define __JCProgramLocationTable_H__

#include <string>
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
#else
    #include <GLES2/gl2.h>
#endif

namespace laya
{
    class LocEnterValue
    {
    public:
        std::string name;
        GLuint fadeProgramID;
        GLuint fadeLocIndex;
        GLuint realLocIndex;
        LocEnterValue();
    };

    class JCProgramLocationTable
    {
    public:
        
        JCProgramLocationTable();

        ~JCProgramLocationTable();
        
        GLuint getRealLocation(GLuint loc);

        void   setRealLocation(GLuint fadeLoc,GLuint realLoc);

        GLuint getFadeLocation(GLuint realProgramID,const char* name);

        void reset();

    protected:

        GLuint              m_nMaxsize;

        LocEnterValue*      m_pLocTable;
    };

}
//------------------------------------------------------------------------------


#endif //__JCProgramLocationTable_H__

//-----------------------------END FILE--------------------------------