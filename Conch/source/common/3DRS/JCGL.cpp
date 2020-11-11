
#include "JCGL.h"
#include <memory>
namespace laya {

    void myglBindBuffer(GLenum target, GLuint buffer) {
        int* pAddr =(int*)(JCGL::s_GLData + target);
        if (*pAddr == buffer)return;
        *pAddr = buffer;
        glBindBuffer(target, buffer);
    }

    void myglEnable(GLenum cap) {
        unsigned int* pAddr = (unsigned int*)(JCGL::s_GLData + cap);
        if (*pAddr == cap)return;
        *pAddr = cap;
        glEnable(cap);
    }

    void myglUseProgram(GLuint program) {

    }

    void myglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {
         
    }

    char    JCGL::s_GLData[64 * 1024];
    JCGL::JCGL() {
        memset(s_GLData, 0, sizeof(s_GLData));
        m_nCurProg = 0;
        this->glBindBuffer = myglBindBuffer;
        this->glEnable = myglEnable;
        this->glDrawElements = myglDrawElements;
    }

    JCGL::~JCGL() {

    }

    void JCGL::glUseProgram(GLuint program) {
        if (m_nCurProg == program)
            return;
        m_nCurProg = program;
        ::glUseProgram(program);
    }

}
