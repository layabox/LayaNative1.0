/**
@file			WebGL.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __WebGL_H__
#define __WebGL_H__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
#else
    #include <GLES2/gl2.h>
#endif

namespace laya
{
    struct WebGLContextAttributes 
    {
        GLboolean alpha;
        GLboolean depth;
        GLboolean stencil;
        GLboolean antialias;
        GLboolean premultipliedAlpha;
        GLboolean preserveDrawingBuffer;
        GLboolean preferLowPowerToHighPerformance;
        GLboolean failIfMajorPerformanceCaveat;
        WebGLContextAttributes()
        {
            alpha = true;
            depth = true;
            stencil = false;
            antialias = true;
            premultipliedAlpha = true;
            preserveDrawingBuffer = false;
            preferLowPowerToHighPerformance = false;
            failIfMajorPerformanceCaveat = false;
        }
    };
    typedef GLuint WebGLShader;
    typedef GLuint WebGLProgram;
    typedef GLuint WebGLBuffer;
    typedef GLuint WebGLFramebuffer;
    typedef GLuint WebGLRenderbuffer;
    typedef GLuint WebGLTexture;
    typedef GLuint WebGLUniformLocation;
    struct WebGLActiveInfo
    {
        const static GLsizei  bufsize=255;
        GLsizei  length;
        GLint    size;
        GLenum   type;
        GLchar   name[bufsize];
    };
    template<typename T> class sequence
    {
    public:
        int count;
        T*  data;
    };
    struct WebGLShaderPrecisionFormat
    {
         GLint range;
         GLint precision;
    };
    class ArrayBuffer
    {
    };
    struct BufferDataSource
    {
        GLuint size;
        char*  data;
    };
    typedef BufferDataSource ArrayBufferView;

    class WebGLRenderingContext
    {
    public:
        WebGLRenderingContext();
        ~WebGLRenderingContext();

        //readonly attribute HTMLCanvasElement canvas;
        GLsizei drawingBufferWidth;
        GLsizei drawingBufferHeight;

        WebGLContextAttributes* getContextAttributes();
        bool isContextLost();
    
        sequence<std::string> getSupportedExtensions();
        void* getExtension(const std::string& name);

        void activeTexture(GLenum texture);
        void attachShader(WebGLProgram program, WebGLShader shader);
        void bindAttribLocation(WebGLProgram program, GLuint index, const std::string& name);
        void bindBuffer(GLenum target, WebGLBuffer buffer);
        void bindFramebuffer(GLenum target, WebGLFramebuffer framebuffer);
        void bindRenderbuffer(GLenum target, WebGLRenderbuffer renderbuffer);
        void bindTexture(GLenum target, WebGLTexture texture);
        void blendColor(GLclampf red, GLclampf greenTEXTURE_2D, GLclampf blue, GLclampf alpha);
        void blendEquation(GLenum mode);
        void blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
        void blendFunc(GLenum sfactor, GLenum dfactor);
        void blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
        void bufferData(GLenum target, GLsizeiptr size, GLenum usage);
        void bufferData(GLenum target, BufferDataSource data, GLenum usage);
        void bufferSubData(GLenum target, GLintptr offset, BufferDataSource data);
        GLenum checkFramebufferStatus(GLenum target);
        void clear(GLbitfield mask);
        void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
        void clearDepth(GLclampf depth);
        void clearStencil(GLint s);
        void colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
        void compileShader(WebGLShader shader);
        void compressedTexImage2D(GLenum target, GLint level, GLenum internalformat,GLsizei width, GLsizei height, GLint border,ArrayBufferView data);
        void compressedTexSubImage2D(GLenum target, GLint level,GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format,ArrayBufferView data);
        void copyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
        void copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
        WebGLBuffer createBuffer();
        WebGLFramebuffer createFramebuffer();
        WebGLProgram createProgram();
        WebGLRenderbuffer createRenderbuffer();
        WebGLShader createShader(GLenum type);
        WebGLTexture createTexture();
        void cullFace(GLenum mode);
        void deleteBuffer(WebGLBuffer buffer);
        void deleteFramebuffer(WebGLFramebuffer framebuffer);
        void deleteProgram(WebGLProgram program);
        void deleteRenderbuffer(WebGLRenderbuffer renderbuffer);
        void deleteShader(WebGLShader shader);
        void deleteTexture(WebGLTexture texture);
        void depthFunc(GLenum func);
        void depthMask(GLboolean flag);
        void depthRange(GLclampf zNear, GLclampf zFar);
        void detachShader(WebGLProgram program, WebGLShader shader);
        void disable(GLenum cap);
        void disableVertexAttribArray(GLuint index);
        void drawArrays(GLenum mode, GLint first, GLsizei count);
        void drawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset);
        void enable(GLenum cap);
        void enableVertexAttribArray(GLuint index);
        void finish();
        void flush();
        void framebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, WebGLRenderbuffer renderbuffer);
        void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, WebGLTexture texture, GLint level);
        void frontFace(GLenum mode);
        void generateMipmap(GLenum target);
        WebGLActiveInfo getActiveAttrib(WebGLProgram program, GLuint index);
        WebGLActiveInfo getActiveUniform(WebGLProgram program, GLuint index);
        std::vector<WebGLShader> getAttachedShaders(WebGLProgram program);
        GLint getAttribLocation(WebGLProgram program, const std::string& name);
        GLint getBufferParameter(GLenum target, GLenum pname);
        GLint getParameter(GLenum pname);
        GLenum getError();
        GLint getFramebufferAttachmentParameter(GLenum target, GLenum attachment,GLenum pname);                                  
        GLint getProgramParameter(WebGLProgram program, GLenum pname);
        std::string getProgramInfoLog(WebGLProgram program);
        GLint getRenderbufferParameter(GLenum target, GLenum pname);
        GLint getShaderParameter(WebGLShader shader, GLenum pname);
        WebGLShaderPrecisionFormat getShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype);
        std::string getShaderInfoLog(WebGLShader shader);
        std::string getShaderSource(WebGLShader shader);
        GLint getTexParameter(GLenum target, GLenum pname);
        GLint getUniform(WebGLProgram program, WebGLUniformLocation location);
        WebGLUniformLocation getUniformLocation(WebGLProgram program, const std::string& name);
        GLint getVertexAttrib(GLuint index, GLenum pname);
        GLsizeiptr getVertexAttribOffset(GLuint index, GLenum pname);
        void hint(GLenum target, GLenum mode);
        GLboolean isBuffer(WebGLBuffer buffer);
        GLboolean isEnabled(GLenum cap);
        GLboolean isFramebuffer(WebGLFramebuffer framebuffer);
        GLboolean isProgram(WebGLProgram program);
        GLboolean isRenderbuffer(WebGLRenderbuffer renderbuffer);
        GLboolean isShader(WebGLShader shader);
        GLboolean isTexture(WebGLTexture texture);
        void lineWidth(GLfloat width);
        void linkProgram(WebGLProgram program);
        void pixelStorei(GLenum pname, GLint param);
        void polygonOffset(GLfloat factor, GLfloat units);
        void readPixels(GLint x, GLint y, GLsizei width, GLsizei height,  GLenum format, GLenum type, ArrayBufferView pixels);
        void renderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
        void sampleCoverage(GLclampf value, GLboolean invert);
        void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
        void shaderSource(WebGLShader shader, const std::string& source);
        void stencilFunc(GLenum func, GLint ref, GLuint mask);
        void stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
        void stencilMask(GLuint mask);
        void stencilMaskSeparate(GLenum face, GLuint mask);
        void stencilOp(GLenum fail, GLenum zfail, GLenum zpass);
        void stencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
        void texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
        void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
        void texParameterf(GLenum target, GLenum pname, GLfloat param);
        void texParameteri(GLenum target, GLenum pname, GLint param);
        void uniform1f(WebGLUniformLocation location, GLfloat x);
        void uniform1fv(WebGLUniformLocation location, int count, const GLfloat* v );
        void uniform1i(WebGLUniformLocation location, GLint x);
        void uniform1iv(WebGLUniformLocation location, int count, const GLint* v );
        void uniform2f(WebGLUniformLocation location, GLfloat x, GLfloat y);
        void uniform2fv(WebGLUniformLocation location, int count, const GLfloat* v );
        void uniform2i(WebGLUniformLocation location, GLint x, GLint y);
        void uniform2iv(WebGLUniformLocation location, int count, const GLint* v );
        void uniform3f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z);
        void uniform3fv(WebGLUniformLocation location, int count, const GLfloat* v);
        void uniform3i(WebGLUniformLocation location, GLint x, GLint y, GLint z);
        void uniform3iv(WebGLUniformLocation location, int count, const GLint* v );
        void uniform4f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void uniform4fv(WebGLUniformLocation location, int count, const GLfloat* v );
        void uniform4i(WebGLUniformLocation location, GLint x, GLint y, GLint z, GLint w);
        void uniform4iv(WebGLUniformLocation location, int count, const GLint* v);
        void uniformMatrix2fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number);
        void uniformMatrix3fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number); 
        void uniformMatrix4fv(WebGLUniformLocation location, GLboolean transpose,GLfloat* value,int number);                              
        void useProgram(WebGLProgram program);
        void validateProgram(WebGLProgram program);
        void vertexAttrib1f(GLuint index, GLfloat x);
        void vertexAttrib1fv(GLuint index, const std::vector<GLfloat>& values);
        void vertexAttrib1fv(GLuint index, GLfloat* values);
        void vertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
        void vertexAttrib2fv(GLuint index, const std::vector<GLfloat>& values);
        void vertexAttrib2fv(GLuint index, GLfloat* values);
        void vertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
        void vertexAttrib3fv(GLuint index, const std::vector<GLfloat>& values);
        void vertexAttrib3fv(GLuint index, GLfloat* values);
        void vertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void vertexAttrib4fv(GLuint index, const std::vector<GLfloat>& values);
        void vertexAttrib4fv(GLuint index, GLfloat* values);
        void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
        void viewport(GLint x, GLint y, GLsizei width, GLsizei height);

        //À©Õ¹
        void deleteAllGPURes();

    protected:
        std::unordered_map<GLuint, bool> m_vAllTexture;
        std::unordered_map<GLuint, bool> m_vAllFrameBuffer;;
        std::unordered_map<GLuint, bool> m_vAllProgram;
        std::unordered_map<GLuint, bool> m_vAllShader;
        std::unordered_map<GLuint, bool> m_vAllRenderBuffer;
        std::unordered_map<GLuint, bool> m_vAllBuffer;
        enum
        {
            DUMMY_HEIGHT_MAX = 1024,
            DUMMY_WIDTH_MAX = 1024,
        };
        char m_vDummyData[DUMMY_WIDTH_MAX * DUMMY_HEIGHT_MAX * 4];
      
    };  
  
}
//------------------------------------------------------------------------------


#endif //__WebGL_H__

//-----------------------------END FILE--------------------------------