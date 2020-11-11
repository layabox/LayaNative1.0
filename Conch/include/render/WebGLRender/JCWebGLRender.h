/**
@file			JCWebGLRender.h
@brief
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCWebGLRender_H__
#define __JCWebGLRender_H__

#include <mutex>
#include <misc/JCWorkSemaphore.h>
#include <buffer/JCBuffer.h>
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include "WebGL.h"
#include "JCIDGenerator.h"
#include "JCProgramLocationTable.h"
#include "../Image/JCImageManager.h"
#include "../Text/JCTextMemoryCanvas.h"
#include <JCICmdDispatch.h>
#include <JCIGLRender.h>

namespace laya
{
#define MEM_RENDER_COMMAND_SIZE 256000
    enum WEBGL_FUNCTION_TYPE
    {
        FT_getExtension,
        FT_createBuffer,
        FT_bindBuffer,
        FT_bufferData_vector,
        FT_bufferData_size,
        FT_createShader,
        FT_shaderSource,
        FT_compileShader,
        FT_getShaderParameter,
        FT_createProgram,
        FT_attachShader,
        FT_linkProgram,
        FT_getProgramParameter,
        FT_useProgram,
        FT_getUniformLocation,
        FT_getAttribLocation,
        FT_disable,
        FT_enable,
        FT_colorMask,
        FT_viewport,
        FT_clear,
        FT_clearColor,
        FT_createTexture,
        FT_bindTexture,
        FT_pixelStorei,
        FT_texImage2D_06,
        FT_texImage2D_09,
        FT_texSubImage2D,
        FT_texParameterf,
        FT_texParameteri,
        FT_activeTexture,
        FT_getShaderInfoLog,
        FT_uniform1f,
        FT_uniform1i,
        FT_uniform2f,
        FT_uniform2i,
        FT_uniform3f,
        FT_uniform3i,
        FT_uniform4f,
        FT_uniform4i,
        FT_uniform1fv,
        FT_uniform1iv,
        FT_uniform2fv,
        FT_uniform2iv,
        FT_uniform3fv,
        FT_uniform3iv,
        FT_uniform4fv,
        FT_uniform4iv,
        FT_uniformMatrix2fv,
        FT_uniformMatrix3fv,
        FT_uniformMatrix4fv,
        FT_vertexAttribPointer,
        FT_enableVertexAttribArray,
        FT_blendFunc,
        FT_bufferSubData,
        FT_drawElements,
        FT_deleteTexture,
        FT_drawArrays,
        FT_bindAttribLocation,
        FT_cullFace,
        FT_createFramebuffer,
        FT_getParameter,
        FT_bindFramebuffer,
        FT_depthFunc,
        FT_depthMask,
        FT_depthRange,
        FT_stencilFunc,
        FT_stencilMask,
        FT_stencilOp,
        FT_deleteBuffer,
        FT_deleteFramebuffer,
        FT_deleteProgram,
        FT_deleteRenderbuffer,
        FT_deleteShader,
        FT_isBuffer,
        FT_disableVertexAttribArray,
        FT_scissor,
        FT_generateMipmap,
        FT_framebufferTexture2D,
        FT_checkFramebufferStatus,
        FT_createRenderbuffer,
        FT_bindRenderbuffer,
        FT_renderbufferStorage,
        FT_framebufferRenderbuffer,
        FT_blendFuncSeparate,
        FT_frontFace,
        FT_WEBGL_MAX = 0xffffffff
    };
    struct ParamGetExtension
    {
        GLuint funcID;
    };
    struct ParamViewport
    {
        GLuint funcID;
        GLushort x;
        GLushort y;
        GLushort width;
        GLushort height;
    };
    struct ParamClear
    {
        GLuint funcID;
        GLuint mask;
    };
    struct ParamClearColor
    {
        GLuint funcID;
        GLclampf red;
        GLclampf green;
        GLclampf blue;
        GLclampf alpha;
    };
    struct ParamCreateShader
    {
        GLuint funcID;
        GLuint type;
        GLuint fadeID;
    };
    struct ParamCreateProgram
    {
        GLuint funcID;
        GLuint fadeID;
    };
    struct ParamCreateBuffer
    {
        GLuint funcID;
        GLuint buffer;
    };
    struct ParamBindBuffer
    {
        GLuint funcID;
        GLuint target;
        GLuint buffer;
    };
    struct ParamBufferDataVector
    {
        GLuint   funcID;
        GLuint   target;
        GLuint   usage;
        GLuint   lengthOfBytes;
    };
    struct ParamBufferDataSize
    {
        GLuint   funcID;
        GLuint   target;
        GLuint   usage;
        GLuint   size;
    };
    struct ParamShaderSource
    {
        GLuint funcID;
        GLuint fadeShader;
        GLuint length;
    };
    struct ParamCompileShader
    {
        GLuint funcID;
        GLuint shader;
    };
    struct ParamGetShaderParameter
    {
        GLuint funcID;
        GLuint shader;
        GLuint pname;
    };
    struct ParamAttachShader
    {
        GLuint funcID;
        GLuint program;
        GLuint shader;
    };
    struct ParamUseProgram
    {
        GLuint funcID;
        GLuint program;
    };
    struct ParamLinkProgram
    {
        GLuint funcID;
        GLuint program;
    };
    struct ParamGetProgramParameter
    {
        GLuint funcID;
        GLuint program;
        GLuint name;
    };
    struct ParamEnable
    {
        GLuint funcID;
        GLuint cap;
    };
    struct ParamDisable
    {
        GLuint funcID;
        GLuint cap;
    };
    struct ParamColorMask
    {
        GLuint funcID;
        GLboolean red;
        GLboolean green;
        GLboolean blue;
        GLboolean alpha;
    };
    struct ParamBufferDataWithSize
    {
        GLuint funcID;
        GLenum target;
        GLenum usage;
        GLsizeiptr size;//64位结构体未使用
    };
    struct ParamBufferDataWithVector
    {
        GLuint funcID;
        GLenum target;
        GLenum usage;
    };
    struct ParamBufferSubData
    {
        GLuint funcID;
        GLuint target;
        GLuint offset;
        GLuint lengthOfBytes;
    };
    struct ParamGetUniformLocation
    {
        GLuint funcID;
        GLuint program;
        GLuint fadeLoc;
        GLuint len;
    };
    struct ParamGetAttribLocation
    {
        GLuint funcID;
        GLuint program;
        GLuint fadeLoc;
        GLuint len;
    };
    struct ParamCreateTexture
    {
        GLuint funcID;
        GLuint texID;
    };
    struct ParamBindTexture
    {
        GLuint funcID;
        GLuint target;
        GLuint texID;
    };
    struct ParamSetTextureInvalidFunc
    {
        GLuint funcID;
        GLuint texFakeID;
    };
    struct ParamActiveTexture
    {
        GLuint funcID;
        GLuint texture;
    };
    struct ParamEnableVertexAttribArray
    {
        GLuint funcID;
        GLuint index;
    };
    struct ParamBlendFunc
    {
        GLuint funcID;
        GLushort sfactor;
        GLushort dfactor;
    };
    struct ParamDrawElements
    {
        GLuint funcID;
        GLenum mode;
        GLsizei count;
        GLenum type;
        GLuint offset;
    };
    struct ParamStencilFunc
    {
        GLuint funcID;
        GLuint func;
        GLint ref;
        GLuint mask;
    };
    struct ParamStencilMask
    {
        GLuint funcID;
        GLuint mask;
    };
    struct ParamStencilOp
    {
        GLuint funcID;
        GLuint fail;
        GLuint zfail;
        GLuint zpass;
    };
    struct ParamDeleteTexture
    {
        GLuint funcID;
        WebGLTexture texture;
    };
    struct ParamDrawArrays
    {
        GLuint funcID;
        GLuint mode;
        GLint  first;
        GLint  count;
    };
    struct ParamBindAttribLocation
    {
        GLuint funcID;
        GLuint program;
        GLuint index;
        GLuint size;
    };
    struct ParamCullFace
    {
        GLuint funcID;
        GLuint mode;
    };
    struct ParamCreateFramebuffer
    {
        GLuint funcID;
        GLuint fadeBuffID;
    };
    struct  ParamGetParameter
    {
        GLuint funcID;
        GLuint pname;
        GLuint fadeValue;
    };
    struct ParamBindFramebuffer
    {
        GLuint funcID;
        GLuint target;
        GLuint fadeFrameBuffer;
    };
    struct ParamUniform1f
    {
        GLuint funcID;
        GLuint location;
        GLfloat x;
    };
    struct ParamUniform1i
    {
        GLuint funcID;
        GLuint location;
        GLint x;
    };
    struct ParamUniform2f
    {
        GLuint funcID;
        GLuint location;
        GLfloat x;
        GLfloat y;
    };
    struct ParamUniform2i
    {
        GLuint funcID;
        GLuint location;
        GLint x;
        GLint y;
    };

    struct ParamUniform3f
    {
        GLuint funcID;
        GLuint location;
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    struct ParamUniform3i
    {
        GLuint funcID;
        GLuint location;
        GLint x;
        GLint y;
        GLint z;
    };
    struct ParamUniform4f
    {
        GLuint funcID;
        GLuint location;
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;
    };
    struct ParamUniform4i
    {
        GLuint funcID;
        GLuint location;
        GLint x;
        GLint y;
        GLint z;
        GLint w;
    };
    struct ParamUniformArray
    {
        GLuint funcID;
        GLuint location;
        GLuint lengthOfBytes;
    };
    struct ParamUniformMatrix2fv
    {
        GLuint funcID;
        GLuint location;
        GLuint transpose;
        GLuint lengthOfBytes;
    };
    struct ParamUniformMatrix3fv
    {
        GLuint funcID;
        GLuint location;
        GLuint transpose;
        GLuint lengthOfBytes;
    };
    struct ParamUniformMatrix4fv
    {
        GLuint funcID;
        GLuint location;
        GLuint transpose;
        GLuint lengthOfBytes;
    };
    struct ParamVertexAttribPointer
    {
        GLuint funcID;
        GLuint index;
        GLint  size;
        GLuint type;
        GLuint normalized;
        GLuint stride;
        GLuint offset;
    };
    struct ParamTexImage2D_09
    {
        GLuint   funcID;
        GLuint  target;
        GLuint  level;
        GLuint  internalformat;
        GLuint  width;
        GLuint  height;
        GLuint  border;
        GLuint  format;
        GLuint  type;
        GLuint    premultiplyAlpha;
        GLuint    objType;          //0是image 1是canvas
        GLuint    imageID;
    };
    struct ParamTexImage2D_06
    {
        GLuint    funcID;
        GLuint  target;
        GLuint  level;
        GLuint  internalformat;
        GLuint  format;
        GLuint    type;
        GLuint    premultiplyAlpha;
        GLuint    objType;          //0是image 1是canvas
        GLuint    imageID;
    };
    struct ParamTexSubImage2D
    {
        GLuint funcID;
        GLuint target;
        GLuint level;
        GLuint xoffset;
        GLuint yoffset;
        GLuint format;
        GLuint type;
        GLuint   premultiplyAlpha;
        GLuint   objType;           //0是image 1是canvas
        GLuint   imageID;
    };
    struct ParamPixelStorei
    {
        GLuint funcID;
        GLuint pname;
        GLint  param;
    };
    struct ParamTexParameteri
    {
        GLuint funcID;
        GLuint target;
        GLuint pname;
        GLint  param;
    };
    struct ParamTexParameterf
    {
        GLuint funcID;
        GLuint  target;
        GLuint  pname;
        GLfloat param;
    };
    struct ParamDepthFunc
    {
        GLuint funcID;
        GLuint func;
    };
    struct ParamDepthMask
    {
        GLuint funcID;
        GLuint flag;
    };
    struct ParamDepthRange
    {
        GLuint funcID;
        GLclampf zNear;
        GLclampf zFar;
    };
    struct ParamDeleteBuffer
    {
        GLuint funcID;
        GLuint fadeBuffID;
    };
    struct ParamDeleteFramebuffer
    {
        GLuint funcID;
        GLuint fadeFrameBuffID;
    };
    struct ParamDeleteProgram
    {
        GLuint funcID;
        GLuint fadeProgramID;
    };
    struct ParamDeleteRenderbuffer
    {
        GLuint funcID;
        GLuint fadeRenderbufferID;
    };
    struct ParamDeleteShader
    {
        GLuint funcID;
        GLuint fadeShaderID;
    };
    struct ParamIsBuffer
    {
        GLuint funcID;
        GLuint fadeBuffID;
    };
    struct ParamDisableVertexAttribArray
    {
        GLuint funcID;
        GLuint index;
    };
    struct ParamScissor
    {
        GLuint funcID;
        GLint x;
        GLint y;
        GLsizei width;
        GLsizei height;
    };
    struct ParamGenerateMipmap
    {
        GLuint funcID;
        GLuint target;
    };
    struct ParamFramebufferTexture2D
    {
        GLuint funcID;
        GLuint target;
        GLuint attachment;
        GLuint textarget;
        GLuint fadeTexID;
        GLuint level;
    };
    struct ParamCheckFramebufferStatus
    {
        GLuint funcID;
        GLuint target;
    };
    struct ParamFlushBuffer
    {
        GLuint length;
    };
    struct ParamDataArray
    {
        GLuint pairs;
    };
    struct ParamDataArrayItem
    {
        GLuint dataPtr;
        GLuint length;
    };

    struct ParamRenderBUfferStorage
    {
        GLuint  funcID;
        GLuint  target;
        GLuint  internalformat;
        GLuint  width;
        GLuint  height;
    };

    struct ParamFramebufferRenderbuffer
    {
        GLuint   funcID;
        GLuint target;
        GLuint attachment;
        GLuint renderbuffertarget;
        GLuint renderBuffer;
    };

    struct ParamBlendFuncSeparate
    {
        GLuint   funcID;
        GLushort srcRGB;
        GLushort dstRGB;
        GLushort srcAlpha;
        GLushort dstAlpha;
    };
    struct ParamFrontFace
    {
        GLuint funcID;
        GLuint mode;
    };
    class JCWebGLRender : public JCICmdDispatch , public JCIGLRender
    {
    public:
        typedef bool (JCWebGLRender::*ProcFunction)(JCMemClass& cmdBuffer);
    public:
        JCWebGLRender(int nID,JCImageManager* pImageManager,JCTextMemoryCanvas* pTextCanvas, WebGLRenderingContext* pWebGLContext);
        ~JCWebGLRender();
        void onGLReady();
        void renderFrame(long nCurrentFrame, bool bStopRender);
        void invalidGLRes();
    private:
        bool dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount);
        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);
        bool restartJSThread();

        bool _rendercmd_getExtension(JCMemClass& cmdBuffer);
        bool _rendercmd_createBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_bindBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_bufferData_vector(JCMemClass& cmdBuffer);
        bool _rendercmd_bufferData_size(JCMemClass& cmdBuffer);
        bool _rendercmd_bufferSubData(JCMemClass& cmdBuffer);
        bool _rendercmd_createShader(JCMemClass& cmdBuffer);
        bool _rendercmd_shaderSource(JCMemClass& cmdBuffer);
        bool _rendercmd_compileShader(JCMemClass& cmdBuffer);
        bool _rendercmd_getShaderParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_createProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_attachShader(JCMemClass& cmdBuffer);
        bool _rendercmd_linkProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_getProgramParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_useProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_getUniformLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_getAttribLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_disable(JCMemClass& cmdBuffer);
        bool _rendercmd_enable(JCMemClass& cmdBuffer);
        bool _rendercmd_colorMask(JCMemClass& cmdBuffer);
        bool _rendercmd_viewport(JCMemClass& cmdBuffer);
        bool _rendercmd_clear(JCMemClass& cmdBuffer);
        bool _rendercmd_clearColor(JCMemClass& cmdBuffer);
        bool _rendercmd_createTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_bindTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_pixelStorei(JCMemClass& cmdBuffer);
        bool _rendercmd_texImage2D_06(JCMemClass& cmdBuffer);
        bool _rendercmd_texImage2D_09(JCMemClass& cmdBuffer);
        bool _rendercmd_texSubImage2D(JCMemClass& cmdBuffer);
        bool _rendercmd_texParameterf(JCMemClass& cmdBuffer);
        bool _rendercmd_texParameteri(JCMemClass& cmdBuffer);
        bool _rendercmd_activeTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_getShaderInfoLog(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform1f(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform1i(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform2f(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform2i(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform3f(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform3i(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform4f(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform4i(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform1fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform1iv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform2fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform2iv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform3fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform3iv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform4fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniform4iv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniformMatrix2fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniformMatrix3fv(JCMemClass& cmdBuffer);
        bool _rendercmd_uniformMatrix4fv(JCMemClass& cmdBuffer);
        bool _rendercmd_vertexAttribPointer(JCMemClass& cmdBuffer);
        bool _rendercmd_enableVertexAttribArray(JCMemClass& cmdBuffer);
        bool _rendercmd_blendFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_drawElements(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_drawArrays(JCMemClass& cmdBuffer);
        bool _rendercmd_bindAttribLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_cullFace(JCMemClass& cmdBuffer);
        bool _rendercmd_createFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_getParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_bindFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_depthFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_depthMask(JCMemClass& cmdBuffer);
        bool _rendercmd_depthRange(JCMemClass& cmdBuffer);
        bool _rendercmd_stencilFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_stencilMask(JCMemClass& cmdBuffer);
        bool _rendercmd_stencilOp(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_deleteShader(JCMemClass& cmdBuffer);
        bool _rendercmd_isBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_disableVertexAttribArray(JCMemClass& cmdBuffer);
        bool _rendercmd_scissor(JCMemClass& cmdBuffer);
        bool _rendercmd_generateMipmap(JCMemClass& cmdBuffer);
        bool _rendercmd_framebufferTexture2D(JCMemClass& cmdBuffer);
        bool _rendercmd_checkFramebufferStatus(JCMemClass& cmdBuffer);
        bool _rendercmd_createRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_bindRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_renderbufferStorage(JCMemClass& cmdBuffer);
        bool _rendercmd_framebufferRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_blendFuncSeparate(JCMemClass& cmdBuffer);
        bool _rendercmd_frontFace(JCMemClass& cmdBuffer);
        bool _rendercmd_getActiveAttrib(JCMemClass& cmdBuffer);
        bool _rendercmd_getActiveUniform(JCMemClass& cmdBuffer);

    public:

        bool _rendercmd_only_parse_getExtension(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bindBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bufferData_vector(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bufferData_size(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bufferSubData(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createShader(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_shaderSource(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_compileShader(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getShaderParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_attachShader(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_linkProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getProgramParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_useProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getUniformLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getAttribLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_disable(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_enable(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_colorMask(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_viewport(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_clear(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_clearColor(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bindTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_pixelStorei(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_texImage2D_06(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_texImage2D_09(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_texSubImage2D(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_texParameterf(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_texParameteri(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_activeTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getShaderInfoLog(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform1f(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform1i(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform2f(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform2i(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform3f(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform3i(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform4f(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform4i(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform1fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform1iv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform2fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform2iv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform3fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform3iv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform4fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniform4iv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniformMatrix2fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniformMatrix3fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_uniformMatrix4fv(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_vertexAttribPointer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_enableVertexAttribArray(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_blendFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_drawElements(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteTexture(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_drawArrays(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bindAttribLocation(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_cullFace(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getParameter(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bindFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_depthFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_depthMask(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_depthRange(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_stencilFunc(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_stencilMask(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_stencilOp(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteFramebuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteProgram(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_deleteShader(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_isBuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_disableVertexAttribArray(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_scissor(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_generateMipmap(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_framebufferTexture2D(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_checkFramebufferStatus(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_createRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_bindRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_renderbufferStorage(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_framebufferRenderbuffer(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_blendFuncSeparate(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_frontFace(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getActiveAttrib(JCMemClass& cmdBuffer);
        bool _rendercmd_only_parse_getActiveUniform(JCMemClass& cmdBuffer);

        /** @brief ios系统本身有bug，只为ios使用
         *  @return 
        */
        void setIosViewport();

    private:

        bool layaTexSubImage2D(int p_pObjectID, GLuint target, GLuint level, GLuint internalformat, GLuint w, GLuint h, GLuint border, GLuint format, GLuint type, bool bSubImage, GLint offsetx, GLint offsety, GLuint objType, bool bPremultiplyAlpha);

        void saveViewPortRect( float x,float y,float w,float h);

    public:

        JCTextMemoryCanvas*             m_pTextMemoryCanvas;
        int                             m_nID;                      ///<CmdDispathManager分配的ID
        Rectangle                       m_kViewPortRect;            ///<恶心的苹果，每次恢复ViewPort为原始画布大小，必须保存下来
    private:
        GLint                           m_nMainFrameBuffer;
        JCImageManager*                 m_pImageManager;
        WebGLRenderingContext*          m_pWebGLContext;
        JCIDGenerator                   m_kIDGenerator;             //ID对应
        JCProgramLocationTable          m_kProgramLocationTable;    //shaer列表
    private:
        int                             m_nCurrentBuffer;
        int                             m_nCurrentProgram;
    };
}
//------------------------------------------------------------------------------


#endif //__JCWebGLRender_H__

//-----------------------------END FILE--------------------------------