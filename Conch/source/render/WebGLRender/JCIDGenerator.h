/**
@file			JCIDGenerator.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCIDGenerator_H__
#define __JCIDGenerator_H__

#include <string>
#include "util/Log.h"
#include <vector>
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
#else
    #include <GLES2/gl2.h>
#endif

namespace laya
{
    enum IdType
    {
        ID_EXTEND = 0,
        ID_BUFFER = 1,
        ID_SHADER = 2,
        ID_PROGRAM = 3,
        ID_TEXTURE = 4,
        ID_FRAMEBUFFER = 5
    };
    class  JCIDGenerator
    {
    public:

        static const GLuint  INVALIDATE_ID = 0xffffffff;
        
        JCIDGenerator();

        ~JCIDGenerator();
        
        bool deleteID(GLuint p_nID);

        bool setRealID(GLuint id,GLuint realID,GLuint p_nType);

        GLuint getRealID(GLuint id);

        void reset();

    protected:

        bool hasID(GLuint p_nID);

    protected:

        std::vector<GLuint> m_vIDTable;

    };

}
//------------------------------------------------------------------------------


#endif //__JCIDGenerator_H__

//-----------------------------END FILE--------------------------------