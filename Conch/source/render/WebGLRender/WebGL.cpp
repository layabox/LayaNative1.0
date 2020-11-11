/**
@file			WebGL.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include <util/Log.h>
#include "WebGL.h"
#include <stdio.h>
#include <stdlib.h>

namespace laya
{
    static void check_opengl_error_debug() {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) 
        {
            LOGI("GL_ERROR (0x%x)", error);
        }
    }
//#define CHECK_GL_ERROR \
    check_opengl_error_debug();
#define CHECK_GL_ERROR
    WebGLRenderingContext::WebGLRenderingContext()
    {
        memset(m_vDummyData, 0, DUMMY_HEIGHT_MAX * DUMMY_WIDTH_MAX * 4);
        drawingBufferWidth  = 0;
        drawingBufferHeight = 0;
    }
    WebGLRenderingContext::~WebGLRenderingContext()
    {
        deleteAllGPURes();
    }
    WebGLContextAttributes* WebGLRenderingContext::getContextAttributes()
    {
        return NULL;
    }
    bool WebGLRenderingContext::isContextLost()
    {
        return false;
    }
    sequence<std::string> WebGLRenderingContext::getSupportedExtensions()
    {
        return sequence<std::string>();
    }
    void* WebGLRenderingContext::getExtension(const std::string& name)
    {
        return NULL;
    }
    void WebGLRenderingContext::activeTexture(GLenum texture)
    {
        ::glActiveTexture( texture-GL_TEXTURE0+GL_TEXTURE0 );
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::attachShader(WebGLProgram program, WebGLShader shader)
    {
        ::glAttachShader(program, shader);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bindAttribLocation(WebGLProgram program, GLuint index, const std::string& name)
    {
        ::glBindAttribLocation(program,index,name.c_str());
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bindBuffer(GLenum target, WebGLBuffer buffer)
    {
        ::glBindBuffer(GL_ARRAY_BUFFER+target-GL_ARRAY_BUFFER,buffer);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bindFramebuffer(GLenum target, WebGLFramebuffer framebuffer)
    {
        ::glBindFramebuffer(target,framebuffer);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bindRenderbuffer(GLenum target, WebGLRenderbuffer renderbuffer)
    {
        ::glBindRenderbuffer(target,renderbuffer);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bindTexture(GLenum target, WebGLTexture texture)
    {
        ::glBindTexture( GL_TEXTURE_2D+target-GL_TEXTURE_2D, texture );
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::blendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        ::glBlendColor(red, green, blue, alpha);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::blendEquation(GLenum mode)
    {
        ::glBlendEquation(mode);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        ::glBlendEquationSeparate(modeRGB, modeAlpha);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::blendFunc(GLenum sfactor, GLenum dfactor)
    {
        ::glBlendFunc(sfactor, dfactor);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
    {
        ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bufferData(GLenum target, GLsizeiptr size, GLenum usage)
    {
        ::glBufferData(target,size,NULL,usage);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bufferData(GLenum target, BufferDataSource data, GLenum usage)
    {
        ::glBufferData(target,data.size,(GLvoid*)data.data,usage);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::bufferSubData(GLenum target, GLintptr offset, BufferDataSource data)
    {
        ::glBufferSubData(target,offset,data.size,data.data);
        CHECK_GL_ERROR
    }
    GLenum WebGLRenderingContext::checkFramebufferStatus(GLenum target)
    {
        bool ret = ::glCheckFramebufferStatus(target);
        CHECK_GL_ERROR
        return ret;
    }
    void WebGLRenderingContext::clear(GLbitfield mask)
    {
        ::glClear(mask);
        CHECK_GL_ERROR
    }
    void WebGLRenderingContext::clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        ::glClearColor(red, green, blue, alpha); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::clearDepth(GLclampf depth)
    {
        ::glClearDepthf(depth); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::clearStencil(GLint s)
    {
        ::glClearStencil(s); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ::glColorMask(red, green, blue, alpha); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::compileShader(WebGLShader shader)
    {
        ::glCompileShader(shader); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::compressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
                                GLsizei width, GLsizei height, GLint border,
                                ArrayBufferView data)
    {
        ::glCompressedTexImage2D(target, level, internalformat,width, height, border,data.size,(GLvoid*)data.data); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::compressedTexSubImage2D(GLenum target, GLint level,
                                    GLint xoffset, GLint yoffset,
                                    GLsizei width, GLsizei height, GLenum format,
                                    ArrayBufferView data)
    {
        ::glCompressedTexSubImage2D(target, level,xoffset, yoffset,
                                    width, height, format,data.size,(GLvoid*)data.data); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::copyTexImage2D(GLenum target, GLint level, GLenum internalformat, 
                        GLint x, GLint y, GLsizei width, GLsizei height, 
                        GLint border)
    {
        ::glCopyTexImage2D(target, level, internalformat, x, y, width, height, border); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, 
                            GLint x, GLint y, GLsizei width, GLsizei height)
    {
        ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height); CHECK_GL_ERROR
    }
    WebGLBuffer WebGLRenderingContext::createBuffer()
    {
        WebGLBuffer buffer;
        ::glGenBuffers(1,&buffer); CHECK_GL_ERROR
        m_vAllBuffer[buffer] = true;
        return buffer;
    }
    WebGLFramebuffer WebGLRenderingContext::createFramebuffer()
    {
        WebGLFramebuffer  buffer;
        ::glGenFramebuffers(1,&buffer); CHECK_GL_ERROR
        m_vAllFrameBuffer[buffer] = true;
        return buffer;
    }
    WebGLProgram WebGLRenderingContext::createProgram()
    {
        auto ret = ::glCreateProgram(); CHECK_GL_ERROR
        m_vAllProgram[ret] = true;
        return ret;
    }
    WebGLRenderbuffer WebGLRenderingContext::createRenderbuffer()
    {
        WebGLRenderbuffer buffer;
        ::glGenRenderbuffers(1,&buffer); CHECK_GL_ERROR
        m_vAllRenderBuffer[buffer] = true;
        return buffer;
    }
    WebGLShader WebGLRenderingContext::createShader(GLenum type)
    {
        auto ret = ::glCreateShader(type); CHECK_GL_ERROR
        m_vAllShader[ret] = true;
        return ret;
    }
    WebGLTexture WebGLRenderingContext::createTexture()
    {
        GLuint tex;
        glGenTextures(1,&tex); CHECK_GL_ERROR
        m_vAllTexture[tex] = true;
        return tex;
    }
    void WebGLRenderingContext::cullFace(GLenum mode)
    {
        ::glCullFace(mode); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteBuffer(WebGLBuffer buffer)
    {
        auto it = m_vAllBuffer.find(buffer);
        if (it != m_vAllBuffer.end()) {
            m_vAllBuffer.erase(it);
        }
        ::glDeleteBuffers(1,&buffer); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteFramebuffer(WebGLFramebuffer framebuffer)
    {
        auto it = m_vAllFrameBuffer.find(framebuffer);
        if (it != m_vAllFrameBuffer.end()) {
            m_vAllFrameBuffer.erase(it);
        }
        ::glDeleteFramebuffers(1,&framebuffer); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteProgram(WebGLProgram program)
    {
        auto it = m_vAllProgram.find(program);
        if (it != m_vAllProgram.end()) {
            m_vAllProgram.erase(it);
        }
        ::glDeleteProgram(program); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteRenderbuffer(WebGLRenderbuffer renderbuffer)
    {
        auto it = m_vAllRenderBuffer.find(renderbuffer);
        if (it != m_vAllRenderBuffer.end()) {
            m_vAllRenderBuffer.erase(it);
        }
        ::glDeleteRenderbuffers(1,&renderbuffer); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteShader(WebGLShader shader)
    {
        auto it = m_vAllShader.find(shader);
        if (it != m_vAllShader.end()) {
            m_vAllShader.erase(it);
        }
        ::glDeleteShader(shader); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::deleteTexture(WebGLTexture texture)
    {
        auto it = m_vAllTexture.find(texture);
        if (it != m_vAllTexture.end()) {
            m_vAllTexture.erase(it);
        }
        ::glDeleteTextures(1,&texture); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::depthFunc(GLenum func)
    {
        ::glDepthFunc(func); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::depthMask(GLboolean flag)
    {
        ::glDepthMask(flag); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::depthRange(GLclampf zNear, GLclampf zFar)
    {
        ::glDepthRangef(zNear,zFar); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::detachShader(WebGLProgram program, WebGLShader shader)
    {
        ::glDetachShader(program,shader); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::disable(GLenum cap)
    {
        glDisable(cap); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::disableVertexAttribArray(GLuint index)
    {
        ::glDisableVertexAttribArray(index); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::drawArrays(GLenum mode, GLint first, GLsizei count)
    {
        ::glDrawArrays(mode,first,count); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::drawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset)
    {
        ::glDrawElements(mode,count,type,(void*)offset); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::enable(GLenum cap)
    {
        ::glEnable(cap); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::enableVertexAttribArray(GLuint index)
    {
        ::glEnableVertexAttribArray(index); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::finish()
    {
        ::glFinish(); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::flush()
    {
        ::glFlush(); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::framebufferRenderbuffer(GLenum target, GLenum attachment, 
                                    GLenum renderbuffertarget, 
                                    WebGLRenderbuffer renderbuffer)
    {
        ::glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, 
                                WebGLTexture texture, GLint level)

    {
        ::glFramebufferTexture2D(target,attachment,textarget,texture,level); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::frontFace(GLenum mode)
    {
        ::glFrontFace(mode); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::generateMipmap(GLenum target)
    {
        ::glGenerateMipmap(target); CHECK_GL_ERROR
    }
    WebGLActiveInfo WebGLRenderingContext::getActiveAttrib(WebGLProgram program, GLuint index)
    {
        WebGLActiveInfo info;
        ::glGetActiveAttrib(program,index,info.bufsize,&info.length,&info.size,&info.type,info.name); CHECK_GL_ERROR
        return info;
    }
    WebGLActiveInfo WebGLRenderingContext::getActiveUniform(WebGLProgram program, GLuint index)
    {
        WebGLActiveInfo info;
        ::glGetActiveUniform(program,index,info.bufsize,&info.length,&info.size,&info.type,info.name); CHECK_GL_ERROR
        return info;
    }
    std::vector<WebGLShader> WebGLRenderingContext::getAttachedShaders(WebGLProgram program)
    {
        GLuint shaders[10];
        GLsizei number;
        std::vector<WebGLShader> result;
        ::glGetAttachedShaders(program,10,&number,shaders); CHECK_GL_ERROR
        for(int i=0;i<number;++i)
        {
            result.push_back(shaders[i]);
        }
        return result;
    }
    GLint WebGLRenderingContext::getAttribLocation(WebGLProgram program, const std::string& name)
    {
        GLint localtion = ::glGetAttribLocation(program,name.c_str()); CHECK_GL_ERROR
        return localtion;
    }
    GLint WebGLRenderingContext::getBufferParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetBufferParameteriv(target,pname,&params); CHECK_GL_ERROR
        return params;
    }
    GLint WebGLRenderingContext::getParameter(GLenum pname)
    {
        //DONOT_SUPPORT("Donot support webgl getParameter(...) function");
        GLint value;
        ::glGetIntegerv(pname,&value); CHECK_GL_ERROR
        return value;
    }
    GLenum WebGLRenderingContext::getError()
    {
        return ::glGetError();
    }
    GLint WebGLRenderingContext::getFramebufferAttachmentParameter(GLenum target, GLenum attachment,GLenum pname)
    {
        GLint params;
        ::glGetFramebufferAttachmentParameteriv(target,attachment,pname,&params); CHECK_GL_ERROR
        return params;
    }                                    
    GLint WebGLRenderingContext::getProgramParameter(WebGLProgram program, GLenum pname)
    {
        GLint params;
        ::glGetProgramiv(program,pname,&params); CHECK_GL_ERROR
        return params;
    }
    std::string WebGLRenderingContext::getProgramInfoLog(WebGLProgram program)
    {
        int infoLen = getShaderParameter( program,GL_INFO_LOG_LENGTH);
        char *buffer = new char[infoLen];
        GLsizei length;
        ::glGetProgramInfoLog(program,infoLen,&length,buffer); CHECK_GL_ERROR
        std::string result(buffer); 
        delete [] buffer;
        return result;
    }
    GLint WebGLRenderingContext::getRenderbufferParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetRenderbufferParameteriv(target,pname,&params); CHECK_GL_ERROR
        return params;
    }
    GLint WebGLRenderingContext::getShaderParameter(WebGLShader shader, GLenum pname)
    {
        GLint params;
        ::glGetShaderiv(shader,pname,&params); CHECK_GL_ERROR
        return params;
    }
    WebGLShaderPrecisionFormat WebGLRenderingContext::getShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype)
    {
        WebGLShaderPrecisionFormat format;
        ::glGetShaderPrecisionFormat(shadertype, precisiontype,&format.range,&format.precision); CHECK_GL_ERROR
        return format;
    }
    std::string WebGLRenderingContext::getShaderInfoLog(WebGLShader shader)
    {
        GLint length;
        ::glGetShaderiv((GLuint) shader, GL_INFO_LOG_LENGTH, &length); CHECK_GL_ERROR
        GLsizei size;
        GLchar* info = (GLchar*) malloc(length);
        if (!info)
        {
            return "";
        }
        ::glGetShaderInfoLog((GLuint) shader, length, &size, info); CHECK_GL_ERROR
        std::string s(info);
        free(info);
        return s;
    }
    std::string WebGLRenderingContext::getShaderSource(WebGLShader shader)
    {
        GLint length;
        ::glGetShaderiv((GLuint) shader, GL_SHADER_SOURCE_LENGTH, &length); CHECK_GL_ERROR
        GLsizei size;
        GLchar* info = (GLchar*) malloc(length);
        if (!info)
        {
            return "";
        }
        ::glGetShaderSource((GLuint) shader, length, &size, info); CHECK_GL_ERROR
        std::string s(info);
        free(info);
        return s;
    }
    GLint WebGLRenderingContext::getTexParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetTexParameteriv(target,pname,&params); CHECK_GL_ERROR
        return params;
    }
    GLint WebGLRenderingContext::getUniform(WebGLProgram program, WebGLUniformLocation location)
    {
        GLint value;
        ::glGetUniformiv(program, location, &value); CHECK_GL_ERROR
        return value;
    }
    WebGLUniformLocation WebGLRenderingContext::getUniformLocation(WebGLProgram program, const std::string& name)
    {
        return ::glGetUniformLocation(program,name.c_str());
    }
    GLint WebGLRenderingContext::getVertexAttrib(GLuint index, GLenum pname)
    {
        GLint value;
        ::glGetVertexAttribiv(index,pname,&value); CHECK_GL_ERROR
        return value;
    }
    GLsizeiptr WebGLRenderingContext::getVertexAttribOffset(GLuint index, GLenum pname)
    {
        LOGW("Can't support webgl getVertexAttribOffset(...) function");
        return NULL;
    }
    void WebGLRenderingContext::hint(GLenum target, GLenum mode)
    {
        ::glHint(target, mode); CHECK_GL_ERROR
    }
    GLboolean WebGLRenderingContext::isBuffer(WebGLBuffer buffer)
    {
        return ::glIsBuffer(buffer);
    }
    GLboolean WebGLRenderingContext::isEnabled(GLenum cap)
    {
        return ::glIsEnabled(cap);
    }
    GLboolean WebGLRenderingContext::isFramebuffer(WebGLFramebuffer framebuffer)
    {
        return ::glIsFramebuffer(framebuffer);
    }
    GLboolean WebGLRenderingContext::isProgram(WebGLProgram program)
    {
        return ::glIsProgram(program);
    }
    GLboolean WebGLRenderingContext::isRenderbuffer(WebGLRenderbuffer renderbuffer)
    {
        return ::glIsRenderbuffer(renderbuffer);
    }
    GLboolean WebGLRenderingContext::isShader(WebGLShader shader)
    {
        return ::glIsShader(shader);
    }
    GLboolean WebGLRenderingContext::isTexture(WebGLTexture texture)
    {
        return ::glIsTexture(texture);
    }
    void WebGLRenderingContext::lineWidth(GLfloat width)
    {
        ::glLineWidth(width); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::linkProgram(WebGLProgram program)
    {
        ::glLinkProgram(program); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::pixelStorei(GLenum pname, GLint param)
    {
        if(pname!=GL_UNPACK_ALIGNMENT && pname!=GL_PACK_ALIGNMENT)
        {
            return;
        }
        ::glPixelStorei(pname, param); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::polygonOffset(GLfloat factor, GLfloat units)
    {
        ::glPolygonOffset(factor,units); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, 
                    GLenum format, GLenum type, ArrayBufferView pixels)
    {
        ::glReadPixels(x, y, width, height, format, type, (GLvoid*)pixels.data); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::renderbufferStorage(GLenum target, GLenum internalformat, 
                                GLsizei width, GLsizei height)
    {
        ::glRenderbufferStorage( target, internalformat, width, height); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::sampleCoverage(GLclampf value, GLboolean invert)
    {
        ::glSampleCoverage(value,invert); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        ::glScissor(x, y, width, height); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::shaderSource(WebGLShader shader, const std::string& source)
    {
        const char* s = source.c_str();
        int length = source.length();
        ::glShaderSource((GLuint) shader, 1, &s, &length); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        ::glStencilFunc(func, ref, mask); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        ::glStencilFuncSeparate(face, func,ref, mask); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilMask(GLuint mask)
    {
        ::glStencilMask(mask); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilMaskSeparate(GLenum face, GLuint mask)
    {
        ::glStencilMaskSeparate(face, mask); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        ::glStencilOp(fail, zfail, zpass); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::stencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
    {
        ::glStencilOpSeparate(face, fail, zfail, zpass); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, 
                                GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels )
    {
        if (pixels == NULL) {
            //÷È×å
            if (width * height <= DUMMY_HEIGHT_MAX * DUMMY_WIDTH_MAX)
            {
                ::glTexImage2D(target, level, internalformat, width, height, border, format, type, m_vDummyData); CHECK_GL_ERROR
                memset(m_vDummyData, 0, DUMMY_HEIGHT_MAX * DUMMY_WIDTH_MAX * 4);
            }
            else
            {  
                char* dummyData = new char[width * height * 4];
                memset(dummyData, 0, width * height * 4);
                ::glTexImage2D(target, level, internalformat, width, height, border, format, type, dummyData); CHECK_GL_ERROR
                delete[] dummyData;
            }
        }
        else {
            ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels); CHECK_GL_ERROR
        }
    }
    void WebGLRenderingContext::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,GLsizei width, 
                                GLsizei height, GLenum format, GLenum type, const void* pixels )
    {
        ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::texParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        ::glTexParameterf(target, pname, param); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::texParameteri(GLenum target, GLenum pname, GLint param)
    {
        ::glTexParameteri(target, pname, param); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform1f(WebGLUniformLocation location, GLfloat x)
    {
        ::glUniform1f(location,x); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform1fv(WebGLUniformLocation location, int count, const GLfloat* v )
    {
        ::glUniform1fv( location,count,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform1i(WebGLUniformLocation location, GLint x)
    {
        ::glUniform1i(location, x); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform1iv(WebGLUniformLocation location, int count,const GLint* v )
    {
        ::glUniform1iv(location, count,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform2f(WebGLUniformLocation location, GLfloat x, GLfloat y)
    {
        ::glUniform2f(location, x,y); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform2fv(WebGLUniformLocation location, int count, const GLfloat* v )
    {
        ::glUniform2fv(location, count/2,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform2i(WebGLUniformLocation location, GLint x, GLint y)
    {
        ::glUniform2i(location, x ,y); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform2iv(WebGLUniformLocation location, int count, const GLint* v)
    {
        ::glUniform2iv(location, count/2,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform3f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z)
    {
        ::glUniform3f(location, x, y, z); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform3fv(WebGLUniformLocation location, int count, const GLfloat* v )
    {
        ::glUniform3fv(location,count/3,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform3i(WebGLUniformLocation location, GLint x, GLint y, GLint z)
    {
        ::glUniform3i(location,x, y, z); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform3iv(WebGLUniformLocation location, int count, const GLint* v )
    {
        ::glUniform3iv(location,count/3,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform4f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        ::glUniform4f(location, x, y, z, w); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform4fv(WebGLUniformLocation location, int count, const GLfloat* v )
    {
        ::glUniform4fv(location, count/4,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform4i(WebGLUniformLocation location, GLint x, GLint y, GLint z, GLint w)
    {
        ::glUniform4i(location, x, y, z, w); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniform4iv(WebGLUniformLocation location, int count, const GLint* v )
    {
        ::glUniform4iv(location,count/4,v ); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::uniformMatrix2fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number)
    {
        ::glUniformMatrix2fv(location,number/4,transpose,value); CHECK_GL_ERROR
    }                        
    void WebGLRenderingContext::uniformMatrix3fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number)
    {
        ::glUniformMatrix3fv(location, number/9,transpose,value); CHECK_GL_ERROR
    }                         
    void WebGLRenderingContext::uniformMatrix4fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number)
    {
        ::glUniformMatrix4fv(location,number/16,transpose,value); CHECK_GL_ERROR
    }                        
    void WebGLRenderingContext::useProgram(WebGLProgram program)
    {
        ::glUseProgram(program); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::validateProgram(WebGLProgram program)
    {
        ::glValidateProgram(program); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib1f(GLuint index, GLfloat x)
    {
        ::glVertexAttrib1f(index,x); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib1fv(GLuint index, const std::vector<GLfloat>& values)
    {
        ::glVertexAttrib1fv(index,&values[0]); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib1fv(GLuint index, GLfloat* values)
    {
        ::glVertexAttrib1fv(index,values); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
    {
        ::glVertexAttrib2f(index,x,y); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib2fv(GLuint index, const std::vector<GLfloat>& values)
    {
        ::glVertexAttrib2fv(index,&values[0]); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib2fv(GLuint index, GLfloat* values)
    {
        glVertexAttrib2fv(index,values); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
    {
        ::glVertexAttrib3f(index,x,y,z); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib3fv(GLuint index, const std::vector<GLfloat>& values)
    {
        ::glVertexAttrib3fv(index,&values[0]); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib3fv(GLuint index, GLfloat* values)
    {
        ::glVertexAttrib3fv(index,values); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        ::glVertexAttrib4f(index, x, y, z, w); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib4fv(GLuint index, const std::vector<GLfloat>& values)
    {
        ::glVertexAttrib4fv(index,&values[0]); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttrib4fv(GLuint index, GLfloat *values)
    {
        ::glVertexAttrib4fv(index,values); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::vertexAttribPointer(GLuint index, GLint size, GLenum type, 
                                GLboolean normalized, GLsizei stride, GLintptr offset)
    {
        ::glVertexAttribPointer(index,size,type,normalized,stride,(void*)offset); CHECK_GL_ERROR
    }
    void WebGLRenderingContext::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        ::glViewport(x,y,width,height); CHECK_GL_ERROR
    }

    void WebGLRenderingContext::deleteAllGPURes() {
        for (auto tex : m_vAllTexture) {
            ::glDeleteTextures(1,&(tex.first)); CHECK_GL_ERROR
        }
        m_vAllTexture.clear();

        for (auto fb : m_vAllFrameBuffer) {
            ::glDeleteFramebuffers(1, &(fb.first)); CHECK_GL_ERROR
        }
        m_vAllFrameBuffer.clear();

        for (auto prog : m_vAllProgram) {
            ::glDeleteProgram(prog.first); CHECK_GL_ERROR
        }
        m_vAllProgram.clear();

        for (auto s : m_vAllShader) {
            ::glDeleteShader(s.first); CHECK_GL_ERROR
        }
        m_vAllShader.clear();

        for (auto rb : m_vAllRenderBuffer) {
            ::glDeleteRenderbuffers(1, &(rb.first)); CHECK_GL_ERROR
        }
        m_vAllRenderBuffer.clear();

        for (auto buf : m_vAllBuffer) {
            ::glDeleteBuffers(1, &(buf.first)); CHECK_GL_ERROR
        }
        m_vAllBuffer.clear();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------