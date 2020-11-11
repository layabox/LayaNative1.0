/**
@file			JCWebGLRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifdef WIN32
    #include <windows.h>
#endif
#include <string>
#include <util/JCCommonMethod.h>
#include "JCWebGLRender.h"
#include "../Image/JCImage.h"

namespace laya
{
    #define DEBUG_WEBGL 0
    #define DEBUG_PRINT_WEBGLCMDS 1
    #define DEBUG_PRINT_SCREEN 0
    #define DEBUG_SAVE_SCREENSHOT 0
    #define DEBUG_TRACING_FRAMES  500 
    #define DEBUG_SAVE_CANVAS_TEX 0

    void _saveCanvasBitmap(const char * function, char* pixels, int width, int height)
    {
#if DEBUG_SAVE_CANVAS_TEX
        static int ii = 0;
        char pngfile[128];
#ifdef WIN32
        sprintf(pngfile, "c:/temp/%s_%d.png", function, ++ii);
#elif ANDROID
        sprintf(pngfile, "/sdcard/layabox/%s_%d.png", function, ++ii);
#endif
        saveAsPng(pixels, width, height, pngfile);
#endif
    }

#if DEBUG_WEBGL
    std::vector<std::string>   s_webgl_info;
    static const int s_trace_frame = DEBUG_TRACING_FRAMES;   //for static webgl demo ,this trace frame should be 1
    static int s_cur_frame = 0;
    static bool s_flush = false;
#if DEBUG_PRINT_WEBGLCMDS
    static bool s_print_cmd = true;
#else
    static bool s_print_cmd = false;
#endif
    const int FORCE_CMD[] = {
        //WEBGL_FUNCTION_TYPE::FT_bufferSubData,
        //WEBGL_FUNCTION_TYPE::FT_framebufferTexture2D,
        //WEBGL_FUNCTION_TYPE::FT_bindFramebuffer,
        //WEBGL_FUNCTION_TYPE::FT_createTexture,
        //WEBGL_FUNCTION_TYPE::FT_deleteTexture,
        WEBGL_FUNCTION_TYPE::FT_drawElements,
    };
    extern bool SavePng(char* png_file_name, char **data, int width, int height, int bit_depth);
    static bool _saveAsPng(const char* p_pData, int w, int h, const char* p_pszFile) {
        char** pngline = new char*[h];
        for (int i = h - 1, j = 0; i >= 0; i--, j++) {
            pngline[j] = (char*)(p_pData + w * 4 * i);
        }
        bool ret = SavePng((char*)p_pszFile, pngline, w, h, 8);
        delete[] pngline;

        delete[]p_pData;
        return ret;
    }
    bool CHECK_FORCE_CMDS(int cmd)
    {
        for (int i = 0; i < sizeof(FORCE_CMD) / sizeof(int); ++i)
        {
            if (FORCE_CMD[i] == cmd)
            {
                return true;
            }
        }
        return false;
    }
    bool CHECK_OUTPUT_DEBUG(int cmd)
    {
        return (!s_flush && (s_print_cmd ? true : CHECK_FORCE_CMDS(cmd)));
    }
    const char* s_webgl_commands[] =
    {
        "getExtension",
        "createBuffer",
        "bindBuffer",
        "bufferData_vector",
        "bufferData_size",
        "createShader",
        "shaderSource",
        "compileShader",
        "getShaderParameter",
        "createProgram",
        "attachShader",
        "linkProgram",
        "getProgramParameter",
        "useProgram",
        "getUniformLocation",
        "getAttribLocation",
        "disable",
        "enable",
        "colorMask",
        "viewport",
        "clear",
        "clearColor",
        "createTexture",
        "bindTexture",
        "pixelStorei",
        "texImage2D_06",
        "texImage2D_09",
        "texSubImage2D",
        "texParameterf",
        "texParameteri",
        "activeTexture",
        "getShaderInfoLog",
        "uniform1f",
        "uniform1i",
        "uniform2f",
        "uniform2i",
        "uniform3f",
        "uniform3i",
        "uniform4f",
        "uniform4i",
        "uniform1fv",
        "uniform1iv",
        "uniform2fv",
        "uniform2iv",
        "uniform3fv",
        "uniform3iv",
        "uniform4fv",
        "uniform4iv",
        "uniformMatrix2fv",
        "uniformMatrix3fv",
        "uniformMatrix4fv",
        "vertexAttribPointer",
        "enableVertexAttribArray",
        "blendFunc",
        "bufferSubData",
        "drawElements",
        "deleteTexture",
        "drawArrays",
        "bindAttribLocation",
        "cullFace",
        "createFramebuffer",
        "getParameter",
        "bindFramebuffer",
        "depthFunc",
        "depthMask",
        "depthRange",
        "stencilFunc",
        "stencilMask",
        "stencilOp",
        "deleteBuffer",
        "deleteFramebuffer",
        "deleteProgram",
        "deleteRenderbuffer",
        "deleteShader",
        "isBuffer",
        "disableVertexAttribArray",
        "scissor",
        "generateMipmap",
        "framebufferTexture2D",
        "checkFramebufferStatus",
        "createImage",
        "deleteImage",
    };
    struct FrameRenderStates
    {
        int renderCmds;
        int drawBatchTimes;
        int triangles;
        int64_t deltaFrameMS;
        int64_t startFrameMS;
        int64_t totalRenderMS;
        int64_t minRenderMS;
        int64_t maxRenderMS;
        FrameRenderStates()
        {
            renderCmds = 0;
            drawBatchTimes = 0;
            triangles = 0;
            deltaFrameMS = 0;
            startFrameMS = 0;
            totalRenderMS = 0;
            minRenderMS = 1000000;
            maxRenderMS = -1000000;
        }
        void reset()
        {
            renderCmds = 0;
            drawBatchTimes = 0;
            deltaFrameMS = 0;
            startFrameMS = tmGetCurms();
            triangles = 0;
        }
        void finishFrame()
        {
            deltaFrameMS = tmGetCurms() - startFrameMS;
            totalRenderMS += deltaFrameMS;
            if (deltaFrameMS > maxRenderMS)
            {
                maxRenderMS = deltaFrameMS;
            }
            else if (deltaFrameMS < minRenderMS)
            {
                minRenderMS = deltaFrameMS;
            }
        }
    };
    static FrameRenderStates      s_FrameRenderStates;
#endif

    static bool _isValidateRealID(unsigned int p_nID)
    {
        if (p_nID == JCIDGenerator::INVALIDATE_ID)
        {
            return false;
        }
        return true;
    }
    JCWebGLRender::JCWebGLRender(int nID,JCImageManager* pImageManager,JCTextMemoryCanvas* pTextMemoryCanvas,WebGLRenderingContext* pWebGLContext )
    {
        m_nID = nID;
        m_pWebGLContext = pWebGLContext;
        m_pTextMemoryCanvas = pTextMemoryCanvas;
        m_kIDGenerator.reset();
        m_pImageManager = pImageManager;
        m_nMainFrameBuffer = 0;
        m_nCurrentBuffer = 0;
        m_nCurrentProgram = 0;
#ifdef __APPLE__
        m_nMainFrameBuffer = m_pWebGLContext->getParameter(GL_FRAMEBUFFER_BINDING);
#endif
    }
    JCWebGLRender::~JCWebGLRender()
    {

    }
    bool JCWebGLRender::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        static const ProcFunction g_svOnlyParseProcFunctions[] =
        {
            &JCWebGLRender::_rendercmd_only_parse_getExtension,
            &JCWebGLRender::_rendercmd_only_parse_createBuffer,
            &JCWebGLRender::_rendercmd_only_parse_bindBuffer,
            &JCWebGLRender::_rendercmd_only_parse_bufferData_vector,
            &JCWebGLRender::_rendercmd_only_parse_bufferData_size,
            &JCWebGLRender::_rendercmd_only_parse_createShader,
            &JCWebGLRender::_rendercmd_only_parse_shaderSource,
            &JCWebGLRender::_rendercmd_only_parse_compileShader,
            &JCWebGLRender::_rendercmd_only_parse_getShaderParameter,
            &JCWebGLRender::_rendercmd_only_parse_createProgram,
            &JCWebGLRender::_rendercmd_only_parse_attachShader,
            &JCWebGLRender::_rendercmd_only_parse_linkProgram,
            &JCWebGLRender::_rendercmd_only_parse_getProgramParameter,
            &JCWebGLRender::_rendercmd_only_parse_useProgram,
            &JCWebGLRender::_rendercmd_only_parse_getUniformLocation,
            &JCWebGLRender::_rendercmd_only_parse_getAttribLocation,
            &JCWebGLRender::_rendercmd_only_parse_disable,
            &JCWebGLRender::_rendercmd_only_parse_enable,
            &JCWebGLRender::_rendercmd_only_parse_colorMask,
            &JCWebGLRender::_rendercmd_only_parse_viewport,
            &JCWebGLRender::_rendercmd_only_parse_clear,
            &JCWebGLRender::_rendercmd_only_parse_clearColor,
            &JCWebGLRender::_rendercmd_only_parse_createTexture,
            &JCWebGLRender::_rendercmd_only_parse_bindTexture,
            &JCWebGLRender::_rendercmd_only_parse_pixelStorei,
            &JCWebGLRender::_rendercmd_only_parse_texImage2D_06,
            &JCWebGLRender::_rendercmd_only_parse_texImage2D_09,
            &JCWebGLRender::_rendercmd_only_parse_texSubImage2D,
            &JCWebGLRender::_rendercmd_only_parse_texParameterf,
            &JCWebGLRender::_rendercmd_only_parse_texParameteri,
            &JCWebGLRender::_rendercmd_only_parse_activeTexture,
            &JCWebGLRender::_rendercmd_only_parse_getShaderInfoLog,
            &JCWebGLRender::_rendercmd_only_parse_uniform1f,
            &JCWebGLRender::_rendercmd_only_parse_uniform1i,
            &JCWebGLRender::_rendercmd_only_parse_uniform2f,
            &JCWebGLRender::_rendercmd_only_parse_uniform2i,
            &JCWebGLRender::_rendercmd_only_parse_uniform3f,
            &JCWebGLRender::_rendercmd_only_parse_uniform3i,
            &JCWebGLRender::_rendercmd_only_parse_uniform4f,
            &JCWebGLRender::_rendercmd_only_parse_uniform4i,
            &JCWebGLRender::_rendercmd_only_parse_uniform1fv,
            &JCWebGLRender::_rendercmd_only_parse_uniform1iv,
            &JCWebGLRender::_rendercmd_only_parse_uniform2fv,
            &JCWebGLRender::_rendercmd_only_parse_uniform2iv,
            &JCWebGLRender::_rendercmd_only_parse_uniform3fv,
            &JCWebGLRender::_rendercmd_only_parse_uniform3iv,
            &JCWebGLRender::_rendercmd_only_parse_uniform4fv,
            &JCWebGLRender::_rendercmd_only_parse_uniform4iv,
            &JCWebGLRender::_rendercmd_only_parse_uniformMatrix2fv,
            &JCWebGLRender::_rendercmd_only_parse_uniformMatrix3fv,
            &JCWebGLRender::_rendercmd_only_parse_uniformMatrix4fv,
            &JCWebGLRender::_rendercmd_only_parse_vertexAttribPointer,
            &JCWebGLRender::_rendercmd_only_parse_enableVertexAttribArray,
            &JCWebGLRender::_rendercmd_only_parse_blendFunc,
            &JCWebGLRender::_rendercmd_only_parse_bufferSubData,
            &JCWebGLRender::_rendercmd_only_parse_drawElements,
            &JCWebGLRender::_rendercmd_only_parse_deleteTexture,
            &JCWebGLRender::_rendercmd_only_parse_drawArrays,
            &JCWebGLRender::_rendercmd_only_parse_bindAttribLocation,
            &JCWebGLRender::_rendercmd_only_parse_cullFace,
            &JCWebGLRender::_rendercmd_only_parse_createFramebuffer,
            &JCWebGLRender::_rendercmd_only_parse_getParameter,
            &JCWebGLRender::_rendercmd_only_parse_bindFramebuffer,
            &JCWebGLRender::_rendercmd_only_parse_depthFunc,
            &JCWebGLRender::_rendercmd_only_parse_depthMask,
            &JCWebGLRender::_rendercmd_only_parse_depthRange,
            &JCWebGLRender::_rendercmd_only_parse_stencilFunc,
            &JCWebGLRender::_rendercmd_only_parse_stencilMask,
            &JCWebGLRender::_rendercmd_only_parse_stencilOp,
            &JCWebGLRender::_rendercmd_only_parse_deleteBuffer,
            &JCWebGLRender::_rendercmd_only_parse_deleteFramebuffer,
            &JCWebGLRender::_rendercmd_only_parse_deleteProgram,
            &JCWebGLRender::_rendercmd_only_parse_deleteRenderbuffer,
            &JCWebGLRender::_rendercmd_only_parse_deleteShader,
            &JCWebGLRender::_rendercmd_only_parse_isBuffer,
            &JCWebGLRender::_rendercmd_only_parse_disableVertexAttribArray,
            &JCWebGLRender::_rendercmd_only_parse_scissor,
            &JCWebGLRender::_rendercmd_only_parse_generateMipmap,
            &JCWebGLRender::_rendercmd_only_parse_framebufferTexture2D,
            &JCWebGLRender::_rendercmd_only_parse_checkFramebufferStatus,
            &JCWebGLRender::_rendercmd_only_parse_createRenderbuffer,
            &JCWebGLRender::_rendercmd_only_parse_bindRenderbuffer,
            &JCWebGLRender::_rendercmd_only_parse_renderbufferStorage,
            &JCWebGLRender::_rendercmd_only_parse_framebufferRenderbuffer,
            &JCWebGLRender::_rendercmd_only_parse_blendFuncSeparate,
            &JCWebGLRender::_rendercmd_only_parse_frontFace,
            &JCWebGLRender::_rendercmd_only_parse_getActiveAttrib,
            &JCWebGLRender::_rendercmd_only_parse_getActiveUniform,
        };
        static const int nFuncs = sizeof(g_svOnlyParseProcFunctions) / sizeof(g_svOnlyParseProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                if ((this->*g_svOnlyParseProcFunctions[nFunctionID])(pRenderCmd) == false)
                {
                    LOGE("JCWebGLRender::dispatchRenderCmdWhenLostedDevice Fail to execute command : %d ", nFunctionID);
                    return false;
                }
            }
            else
            {
                LOGE( "JCWebGLRender::dispatchRenderCmdWhenLostedDevice Wrong gl render command number,cmd=%d", nFunctionID );
            }
        }
        return true;
    }
    bool JCWebGLRender::dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount)
    {
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCWebGLRender::_rendercmd_getExtension,
            &JCWebGLRender::_rendercmd_createBuffer,
            &JCWebGLRender::_rendercmd_bindBuffer,
            &JCWebGLRender::_rendercmd_bufferData_vector,
            &JCWebGLRender::_rendercmd_bufferData_size,
            &JCWebGLRender::_rendercmd_createShader,
            &JCWebGLRender::_rendercmd_shaderSource,
            &JCWebGLRender::_rendercmd_compileShader,
            &JCWebGLRender::_rendercmd_getShaderParameter,
            &JCWebGLRender::_rendercmd_createProgram,
            &JCWebGLRender::_rendercmd_attachShader,
            &JCWebGLRender::_rendercmd_linkProgram,
            &JCWebGLRender::_rendercmd_getProgramParameter,
            &JCWebGLRender::_rendercmd_useProgram,
            &JCWebGLRender::_rendercmd_getUniformLocation,
            &JCWebGLRender::_rendercmd_getAttribLocation,
            &JCWebGLRender::_rendercmd_disable,
            &JCWebGLRender::_rendercmd_enable,
            &JCWebGLRender::_rendercmd_colorMask,
            &JCWebGLRender::_rendercmd_viewport,
            &JCWebGLRender::_rendercmd_clear,
            &JCWebGLRender::_rendercmd_clearColor,
            &JCWebGLRender::_rendercmd_createTexture,
            &JCWebGLRender::_rendercmd_bindTexture,
            &JCWebGLRender::_rendercmd_pixelStorei,
            &JCWebGLRender::_rendercmd_texImage2D_06,
            &JCWebGLRender::_rendercmd_texImage2D_09,
            &JCWebGLRender::_rendercmd_texSubImage2D,
            &JCWebGLRender::_rendercmd_texParameterf,
            &JCWebGLRender::_rendercmd_texParameteri,
            &JCWebGLRender::_rendercmd_activeTexture,
            &JCWebGLRender::_rendercmd_getShaderInfoLog,
            &JCWebGLRender::_rendercmd_uniform1f,
            &JCWebGLRender::_rendercmd_uniform1i,
            &JCWebGLRender::_rendercmd_uniform2f,
            &JCWebGLRender::_rendercmd_uniform2i,
            &JCWebGLRender::_rendercmd_uniform3f,
            &JCWebGLRender::_rendercmd_uniform3i,
            &JCWebGLRender::_rendercmd_uniform4f,
            &JCWebGLRender::_rendercmd_uniform4i,
            &JCWebGLRender::_rendercmd_uniform1fv,
            &JCWebGLRender::_rendercmd_uniform1iv,
            &JCWebGLRender::_rendercmd_uniform2fv,
            &JCWebGLRender::_rendercmd_uniform2iv,
            &JCWebGLRender::_rendercmd_uniform3fv,
            &JCWebGLRender::_rendercmd_uniform3iv,
            &JCWebGLRender::_rendercmd_uniform4fv,
            &JCWebGLRender::_rendercmd_uniform4iv,
            &JCWebGLRender::_rendercmd_uniformMatrix2fv,
            &JCWebGLRender::_rendercmd_uniformMatrix3fv,
            &JCWebGLRender::_rendercmd_uniformMatrix4fv,
            &JCWebGLRender::_rendercmd_vertexAttribPointer,
            &JCWebGLRender::_rendercmd_enableVertexAttribArray,
            &JCWebGLRender::_rendercmd_blendFunc,
            &JCWebGLRender::_rendercmd_bufferSubData,
            &JCWebGLRender::_rendercmd_drawElements,
            &JCWebGLRender::_rendercmd_deleteTexture,
            &JCWebGLRender::_rendercmd_drawArrays,
            &JCWebGLRender::_rendercmd_bindAttribLocation,
            &JCWebGLRender::_rendercmd_cullFace,
            &JCWebGLRender::_rendercmd_createFramebuffer,
            &JCWebGLRender::_rendercmd_getParameter,
            &JCWebGLRender::_rendercmd_bindFramebuffer,
            &JCWebGLRender::_rendercmd_depthFunc,
            &JCWebGLRender::_rendercmd_depthMask,
            &JCWebGLRender::_rendercmd_depthRange,
            &JCWebGLRender::_rendercmd_stencilFunc,
            &JCWebGLRender::_rendercmd_stencilMask,
            &JCWebGLRender::_rendercmd_stencilOp,
            &JCWebGLRender::_rendercmd_deleteBuffer,
            &JCWebGLRender::_rendercmd_deleteFramebuffer,
            &JCWebGLRender::_rendercmd_deleteProgram,
            &JCWebGLRender::_rendercmd_deleteRenderbuffer,
            &JCWebGLRender::_rendercmd_deleteShader,
            &JCWebGLRender::_rendercmd_isBuffer,
            &JCWebGLRender::_rendercmd_disableVertexAttribArray,
            &JCWebGLRender::_rendercmd_scissor,
            &JCWebGLRender::_rendercmd_generateMipmap,
            &JCWebGLRender::_rendercmd_framebufferTexture2D,
            &JCWebGLRender::_rendercmd_checkFramebufferStatus,
            &JCWebGLRender::_rendercmd_createRenderbuffer,
            &JCWebGLRender::_rendercmd_bindRenderbuffer,
            &JCWebGLRender::_rendercmd_renderbufferStorage,
            &JCWebGLRender::_rendercmd_framebufferRenderbuffer,
            &JCWebGLRender::_rendercmd_blendFuncSeparate,
            &JCWebGLRender::_rendercmd_frontFace,
            &JCWebGLRender::_rendercmd_getActiveAttrib,
            &JCWebGLRender::_rendercmd_getActiveUniform,

        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
#if DEBUG_WEBGL
        s_FrameRenderStates.reset();
#endif
        int numberRenderCmd = 0;
        static int frame = 0;
        static int lastCmd = 0;
        frame++;

        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                if ((this->*g_svProcFunctions[nFunctionID])(pRenderCmd) == false)
                {
                    LOGE("JCWebGLRender::dispatchRenderCmd Fail to execute command : %d ", nFunctionID);
                    return false;
                }
            }
            else
            {
                LOGE("JCWebGLRender::dispatchRenderCmd Wrong gl render command number!,cmd=%d,lastCmd=%d,numberRenderCmd=%d", nFunctionID, lastCmd, numberRenderCmd);
            }
        }
#if DEBUG_WEBGL
        s_FrameRenderStates.renderCmds = numberRenderCmd;
        s_FrameRenderStates.finishFrame();
        if (!s_flush && numberRenderCmd > 0 && s_FrameRenderStates.drawBatchTimes > 0)
        {
            s_cur_frame++;
            char buf[128];
            sprintf(buf, "\n\nFrame states: milli second cost = %d\n", s_FrameRenderStates.deltaFrameMS);
            s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
            LOGI("%s", buf);
#endif
            sprintf(buf, "Frame states: total cmds  = %d\n", s_FrameRenderStates.renderCmds);
            s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
            LOGI("%s", buf);
#endif
            sprintf(buf, "Frame states: total trianles = %d\n", s_FrameRenderStates.triangles);
            s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
            LOGI("%s", buf);
#endif
            sprintf(buf, "Frame states: total draw call = %d\n", s_FrameRenderStates.drawBatchTimes);
            s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
            LOGI("%s", buf);
#endif
            sprintf(buf, "=======frame=%d========\n", s_cur_frame);
            s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
            LOGI("%s", buf);
#endif

#if DEBUG_SAVE_SCREENSHOT
            {
#ifdef WIN32
                sprintf(buf, "e:/temp/screen_%d.png", s_cur_frame);
#elif ANDROID
                sprintf(buf, "/sdcard/layabox/screen_%d.png", s_cur_frame);
#endif

                //int width = 960;
                //int height = 640;
                int width = 640;
                int height = 960;
                char* pixels = new char[4 * width * height];
                glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                _saveAsPng(pixels, width, height, buf);
            }
#endif

            if (s_cur_frame == s_trace_frame)
            {
                sprintf(buf, "Frame states: Average frame render cost = %d\n", (int)s_FrameRenderStates.totalRenderMS / s_trace_frame);
                s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
                LOGI("%s", buf);
#endif
                sprintf(buf, "Frame states: Max frame render cost = %d\n", (int)s_FrameRenderStates.maxRenderMS);
                s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
                LOGI("%s", buf);
#endif
                sprintf(buf, "Frame states: Min frame render cost = %d\n", (int)s_FrameRenderStates.minRenderMS);
                s_webgl_info.push_back(std::string(buf));
#if DEBUG_PRINT_SCREEN
                LOGI("%s", buf);
#endif

#ifdef WIN32
                char* jsportfile = "e:/temp/webgl-debug.txt";
#elif ANDROID
                char* jsportfile = "/sdcard/layabox/webgl-debug.txt";
#elif __APPLE__
                char* jsportfile = "";
#endif
                FILE* fp = fopen(jsportfile, "w+");
                if (fp)
                {
                    for (size_t i = 0; i < s_webgl_info.size(); ++i)
                    {
                        std::string str = s_webgl_info.at(i);
                        fwrite(str.c_str(), 1, str.length(), fp);
                    }
                    fclose(fp);
                    s_webgl_info.clear();
                    s_flush = true;
                }
            }
        }
#endif
        return true;
    }
    bool JCWebGLRender::restartJSThread()
    {
        return true;
    }
    bool JCWebGLRender::_rendercmd_getExtension(JCMemClass& cmdBuffer)
    {
        ParamGetExtension* pParam = cmdBuffer.popp<ParamGetExtension>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_createBuffer(JCMemClass& cmdBuffer)
    {
        ParamCreateBuffer* pParam = cmdBuffer.popp<ParamCreateBuffer>();
        GLuint realBuffer = m_pWebGLContext->createBuffer();
        m_kIDGenerator.setRealID(pParam->buffer, realBuffer, IdType::ID_BUFFER);
#if DEBUG_WEBGL
        if (CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf, "%s():fadeID=%d,realID=%d \n", s_webgl_commands[pParam->funcID], pParam->buffer, realBuffer);
            LOGI("%s",buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_bindBuffer(JCMemClass& cmdBuffer)
    {
        ParamBindBuffer* pParam = cmdBuffer.popp<ParamBindBuffer>();
        GLuint realID = 0;
        bool bValidateID = false;
        if (pParam->buffer == 0)
        {
            bValidateID = false;
        }
        else
        {
            realID = m_kIDGenerator.getRealID(pParam->buffer);
            bValidateID = _isValidateRealID(realID);
        }
        if (bValidateID)
        {
            m_pWebGLContext->bindBuffer(pParam->target, realID);
            m_nCurrentBuffer = realID;
        }
        else
        {
            m_pWebGLContext->bindBuffer(pParam->target, 0);
            m_nCurrentBuffer = 0;
        }

#if DEBUG_WEBGL
        if (CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if (bValidateID)
            {
                sprintf(buf, "%s():fadeID=%d,realID=%d \n", s_webgl_commands[pParam->funcID], pParam->buffer, realID);
            }
            else
            {
                sprintf(buf, "%s():fadeID=%d, <Invalidate ID!!!>\n", s_webgl_commands[pParam->funcID], pParam->buffer);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_bufferData_vector(JCMemClass& cmdBuffer)
    {
        ParamBufferDataVector * pParam = cmdBuffer.popp<ParamBufferDataVector>();
        if (pParam->lengthOfBytes > 0)
        {
            char* pData = cmdBuffer.readBuffer(pParam->lengthOfBytes);
            BufferDataSource  ds = { pParam->lengthOfBytes,pData };
            m_pWebGLContext->bufferData(pParam->target, ds, pParam->usage);
        }
#if DEBUG_WEBGL
        if (CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf, "%s():size=%d,usage=0x%x,target=0x%x \n", s_webgl_commands[pParam->funcID], pParam->lengthOfBytes, pParam->usage, pParam->target);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_bufferData_size(JCMemClass& cmdBuffer)
    {
        ParamBufferDataSize* pParam = cmdBuffer.popp<ParamBufferDataSize>();
        m_pWebGLContext->bufferData(pParam->target,pParam->size,pParam->usage);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():size=%d,usage=0x%x,target=0x%x \n",s_webgl_commands[pParam->funcID],pParam->size,pParam->usage,pParam->target);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_bufferSubData( JCMemClass& cmdBuffer )
    {
        ParamBufferSubData* pParam = cmdBuffer.popp<ParamBufferSubData>();
        BufferDataSource  ds;
        int size = pParam->lengthOfBytes;
        if (size > 0)
        {
            if (size % 4 != 0)
            {
                size = size + 4 - (size % 4);
            }
            char* pData = cmdBuffer.readBuffer(size);
            ds.size = pParam->lengthOfBytes;
            ds.data = pData;
            m_pWebGLContext->bufferSubData(pParam->target, pParam->offset, ds);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():byte length=%d,offset=%d,target=0x%x ,addr=0x%x\n",s_webgl_commands[pParam->funcID],pParam->lengthOfBytes,pParam->offset,pParam->target,ds.data);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_createShader( JCMemClass& cmdBuffer )
    {
        ParamCreateShader* pParam = cmdBuffer.popp<ParamCreateShader>();
        WebGLShader realShader = m_pWebGLContext->createShader(pParam->type);
        m_kIDGenerator.setRealID(pParam->fadeID, realShader, IdType::ID_SHADER);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeID=%d \n",s_webgl_commands[pParam->funcID],pParam->fadeID);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_shaderSource( JCMemClass& cmdBuffer )
    {
        ParamShaderSource* pParam = cmdBuffer.popp<ParamShaderSource>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeShader);
        bool bValidateID = _isValidateRealID(realID);
        char* pData = (char*)cmdBuffer.readBuffer(pParam->length);
        if (bValidateID)
        {
            m_pWebGLContext->shaderSource(realID, pData);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeID=%d,reaID=%d,source :  ==>\n",s_webgl_commands[pParam->funcID],pParam->fadeShader,realID);
                LOGI("%s", buf);
                s_webgl_info.push_back(std::string(buf));
                s_webgl_info.push_back(std::string(pData));
                LOGI("shader source%s", pData);
            }
            else
            {
                sprintf(buf,"%s():fadeID=%d, <Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->fadeShader);
                s_webgl_info.push_back(std::string(buf));
                LOGI("%s", buf);
                LOGI("Failed in _rendercmd_shaderSource: invalidate shader ID!");
            }
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_compileShader(JCMemClass& cmdBuffer)
    {
        ParamCompileShader* pParam = cmdBuffer.popp<ParamCompileShader>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->shader);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->compileShader(realID);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeID=%d,reaID=%d\n",s_webgl_commands[pParam->funcID],pParam->shader,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->shader);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_getShaderParameter( JCMemClass& cmdBuffer )
    {
        ParamGetShaderParameter* pParam = cmdBuffer.popp<ParamGetShaderParameter>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->shader);
        bool bValidateID = _isValidateRealID(realID);
        if(bValidateID)
        {
            m_pWebGLContext->getShaderParameter(realID, pParam->pname);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeID=%d,realID=%d,name=0x%x \n",s_webgl_commands[pParam->funcID],pParam->shader,realID,pParam->pname);
            }
            else
            {
                sprintf(buf,"%s():fadeID=%d,name=0x%x <Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->shader,pParam->pname);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_createProgram( JCMemClass& cmdBuffer )
    {
        ParamCreateProgram* pParam = cmdBuffer.popp<ParamCreateProgram>();
        GLuint realProgram = realProgram = m_pWebGLContext->createProgram();
        m_kIDGenerator.setRealID(pParam->fadeID,realProgram,IdType::ID_PROGRAM);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeID=%d,realID=%d \n",s_webgl_commands[pParam->funcID],pParam->fadeID,realProgram);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_attachShader( JCMemClass& cmdBuffer )
    {
        ParamAttachShader* pParam = cmdBuffer.popp<ParamAttachShader>();
        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
        bool bValidateProgramID = _isValidateRealID(realProgram);
        GLuint realShader = m_kIDGenerator.getRealID(pParam->shader);
        bool bValidateShaderID = _isValidateRealID(realShader);
        if (bValidateProgramID && bValidateShaderID)
        {
            m_pWebGLContext->attachShader(realProgram, realShader);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateProgramID && bValidateShaderID)
            {
                sprintf(buf,"%s():fadeProgID=%d,realProgID=%d,fadeShaderID=%d,realShaderID=%d \n",
                    s_webgl_commands[pParam->funcID],pParam->program,realProgram,pParam->shader,realShader);
            }
            else
            {
                sprintf(buf,"%s():fadeProgID=%d,fadeShaderID=%d <Invalidate ID!!!>\n",
                    s_webgl_commands[pParam->funcID],pParam->program,pParam->shader);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_linkProgram( JCMemClass& cmdBuffer )
    {
        ParamLinkProgram* pParam = cmdBuffer.popp<ParamLinkProgram>();
        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
        bool bValidateID = _isValidateRealID(realProgram);
        if (bValidateID)
        {
            m_pWebGLContext->linkProgram(realProgram);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeID=%d,realID=%d \n",s_webgl_commands[pParam->funcID],pParam->program,realProgram);
            }
            else
            {
                sprintf(buf,"%s():fadeID=%d <Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->program);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_getProgramParameter( JCMemClass& cmdBuffer )
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];

            sprintf(buf,"%s():fadeID=%d,name=0x%x,value=%d \n",
                    s_webgl_commands[pParam->funcID],pParam->program,pParam->name);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_useProgram(JCMemClass& cmdBuffer)
    {
        ParamUseProgram* pParam = cmdBuffer.popp<ParamUseProgram>();
        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
       
        bool bValidateID = _isValidateRealID(realProgram);
        if (bValidateID)
        { 
            m_nCurrentProgram = realProgram;
            m_pWebGLContext->useProgram(realProgram);
        }
#if DEBUG_WEBGL
        if (CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if (bValidateID)
            {
                sprintf(buf, "%s():fadeID=%d,realID=%d\n", s_webgl_commands[pParam->funcID], pParam->program, realProgram);
            }
            else
            {
                sprintf(buf, "%s():fadeID=%d, <Invalidate ID!!!>\n", s_webgl_commands[pParam->funcID], pParam->program);

            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_getUniformLocation( JCMemClass& cmdBuffer )
    {
        ParamGetUniformLocation* pParam = cmdBuffer.popp<ParamGetUniformLocation>();
        char* pData = (char*)cmdBuffer.readBuffer(pParam->len);
        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
        WebGLUniformLocation realLoc=0;
        bool bValidateID = _isValidateRealID(realProgram);
        if(bValidateID)
        {
            realLoc = m_pWebGLContext->getUniformLocation(realProgram, pData);
            m_kProgramLocationTable.setRealLocation(pParam->fadeLoc,realLoc);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():uniform=%s,fadeID=%d,realID=%d,fadeLoc=%d,realLoc=%d\n",
                    s_webgl_commands[pParam->funcID],pData,pParam->program,realProgram,pParam->fadeLoc,realLoc);
            }
            else
            {
                sprintf(buf,"%s():uniform=%s,fadeID=%d <Invalidate ID!!!>\n",
                    s_webgl_commands[pParam->funcID],pData,pParam->program);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_getAttribLocation( JCMemClass& cmdBuffer )
    {
        ParamGetAttribLocation* pParam = cmdBuffer.popp<ParamGetAttribLocation>();
        char* pData = (char*)cmdBuffer.readBuffer(pParam->len);
        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
        bool bValidateID = _isValidateRealID(realProgram);
        WebGLUniformLocation realLoc=0;
        if(bValidateID)
        {
            realLoc = m_pWebGLContext->getAttribLocation(realProgram, pData);
            m_kProgramLocationTable.setRealLocation(pParam->fadeLoc,realLoc);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():attrib=%s,fadeID=%d,realID=%d,fadeLoc=%d,realLoc=%d\n",
                    s_webgl_commands[pParam->funcID],pData,pParam->program,realProgram,pParam->fadeLoc,realLoc);
            }
            else
            {
                sprintf(buf,"%s():attrib=%s,fadeID=%d, <Invalidate ID!!!>\n",
                    s_webgl_commands[pParam->funcID],pData,pParam->program);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_disable( JCMemClass& cmdBuffer )
    {
        ParamDisable* pParam = cmdBuffer.popp<ParamDisable>();
        m_pWebGLContext->disable(pParam->cap);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():cap=0x%x\n",s_webgl_commands[pParam->funcID],pParam->cap);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_enable( JCMemClass& cmdBuffer )
    {
        ParamEnable* pParam = cmdBuffer.popp<ParamEnable>();
        m_pWebGLContext->enable(pParam->cap);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():cap=0x%x\n",s_webgl_commands[pParam->funcID],pParam->cap);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_colorMask( JCMemClass& cmdBuffer )
    {
        ParamColorMask* pParam = cmdBuffer.popp<ParamColorMask>();
        m_pWebGLContext->colorMask(pParam->red,pParam->green,pParam->blue,pParam->alpha);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():r=%d,g=%d,b=%d,a=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->red,pParam->green,pParam->blue,pParam->alpha);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_viewport( JCMemClass& cmdBuffer )
    {
        ParamViewport* pParam = cmdBuffer.popp<ParamViewport>();
        m_pWebGLContext->viewport(pParam->x,pParam->y,pParam->width,pParam->height);
#ifdef __APPLE__
        saveViewPortRect(pParam->x, pParam->y, pParam->width, pParam->height);
#endif
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():x=%d,y=%d,w=%d,h=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->x,pParam->y,pParam->width,pParam->height);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_clear( JCMemClass& cmdBuffer )
    {
        ParamClear* pParam = cmdBuffer.popp<ParamClear>();
        m_pWebGLContext->clear(pParam->mask);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():mask=0x%x \n",s_webgl_commands[pParam->funcID],pParam->mask);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_clearColor( JCMemClass& cmdBuffer )
    {
        ParamClearColor* pParam = cmdBuffer.popp<ParamClearColor>();
        m_pWebGLContext->clearColor(pParam->red,pParam->green,pParam->blue,pParam->alpha);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():r=%f,g=%f,b=%f,a=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->red,pParam->green,pParam->blue,pParam->alpha);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    
    bool JCWebGLRender::_rendercmd_createTexture(JCMemClass& cmdBuffer)
    {
        ParamCreateTexture* pParam = cmdBuffer.popp<ParamCreateTexture>();
        WebGLTexture realTexID = m_pWebGLContext->createTexture();
        m_kIDGenerator.setRealID(pParam->texID, realTexID, IdType::ID_TEXTURE);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeTexID=%d,realTexID=%d \n",s_webgl_commands[pParam->funcID],
                    pParam->texID,realTexID);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }       
    bool JCWebGLRender::_rendercmd_bindTexture( JCMemClass& cmdBuffer )
    {
        ParamBindTexture* pParam = cmdBuffer.popp<ParamBindTexture>();
        GLuint realTex = 0;
        bool bValidateID=false;
        if(pParam->texID==0)
        {
            bValidateID = false;
        }
        else
        {
            realTex = m_kIDGenerator.getRealID(pParam->texID);
            bValidateID = _isValidateRealID(realTex);
        }
        if (bValidateID)
        {
             m_pWebGLContext->bindTexture(pParam->target, realTex);
        }
        else
        {
            m_pWebGLContext->bindTexture(pParam->target, 0);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeID=%d,realID=%d \n",s_webgl_commands[pParam->funcID],pParam->texID,realTex);
            }
            else
            {
                sprintf(buf,"%s():fadeID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->texID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }       
    bool JCWebGLRender::_rendercmd_pixelStorei( JCMemClass& cmdBuffer )
    {
        ParamPixelStorei* pParam = cmdBuffer.popp<ParamPixelStorei>();
        m_pWebGLContext->pixelStorei(pParam->pname,pParam->param);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():pname=0x%x,param=%d \n",s_webgl_commands[pParam->funcID],
                    pParam->pname,pParam->param);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }       
    bool JCWebGLRender::_rendercmd_texImage2D_06( JCMemClass& cmdBuffer )
    {
        ParamTexImage2D_06* pParam = cmdBuffer.popp<ParamTexImage2D_06>();
        layaTexSubImage2D(pParam->imageID, pParam->target, pParam->level, pParam->internalformat, 0, 0, false, pParam->format, pParam->type,false,0,0,pParam->objType, pParam->premultiplyAlpha);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
			if(pParam->imageID !=-1)
			{
				sprintf(buf,"%s():target=0x%x,level=%d,interFormat=0x%xformat=0x%x,type=0x%x \n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->internalformat,pParam->format,pParam->type);
			}
			else
			{
				sprintf(buf,"%s():target=0x%x,level=%d,interFormat=0x%x,format=0x%x,type=0x%x {IMAGE is NULL<<--}\n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->internalformat,pParam->format,pParam->type);
			}
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }     
    bool JCWebGLRender::_rendercmd_texImage2D_09( JCMemClass& cmdBuffer )
    {
        ParamTexImage2D_09* pParam = cmdBuffer.popp<ParamTexImage2D_09>();
        layaTexSubImage2D(pParam->imageID,pParam->target, pParam->level, pParam->internalformat,pParam->width, pParam->height, pParam->border,pParam->format, pParam->type,false,0,0,pParam->objType, pParam->premultiplyAlpha);
 #if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
			if(pParam->imageID != -1)
			{
				sprintf(buf,"%s():target=0x%x,level=%d,interFormat=0x%x,w=%d,h=%d,format=0x%x,type=0x%x \n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->internalformat,pParam->width,pParam->height,pParam->format,pParam->type);
			}
			else
			{
				sprintf(buf,"%s():target=0x%x,level=%d,interFormat=0x%x,w=%d,h=%d,format=0x%x,type=0x%x  {IMAGE is NULL<<--}\n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->internalformat,pParam->width,pParam->height,pParam->format,pParam->type);
			}
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    } 
    bool JCWebGLRender::_rendercmd_texSubImage2D( JCMemClass& cmdBuffer )
    {
        ParamTexSubImage2D* pParam = cmdBuffer.popp<ParamTexSubImage2D>();
        layaTexSubImage2D(pParam->imageID, pParam->target, pParam->level, 0, 0, 0, false, pParam->format, pParam->type, true, pParam->xoffset, pParam->yoffset, pParam->objType, pParam->premultiplyAlpha);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
			if(pParam->imageID !=-1)
			{
				sprintf(buf,"%s():target=0x%x,level=%d,xoffset=%d,yoffset=%d,format=0x%x,type=0x%x \n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->xoffset,pParam->yoffset,pParam->format,pParam->type);
			}
			else
			{
				sprintf(buf,"%s():target=0x%x,level=%d,xoffset=%d,yoffset=%d,format=0x%x,type=0x%x,  {IMAGE is NULL<<--} \n",s_webgl_commands[pParam->funcID],
						pParam->target,pParam->level,pParam->xoffset,pParam->yoffset,pParam->format,pParam->type);

			}
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_texParameterf( JCMemClass& cmdBuffer )
    {
        ParamTexParameterf* pParam = cmdBuffer.popp<ParamTexParameterf>();
        m_pWebGLContext->texParameterf(pParam->target,pParam->pname,pParam->param);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():target=0x%x,name=0x%x,param=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->pname,pParam->param);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    } 
    bool JCWebGLRender::_rendercmd_texParameteri( JCMemClass& cmdBuffer )
    {
        ParamTexParameteri* pParam = cmdBuffer.popp<ParamTexParameteri>();
        m_pWebGLContext->texParameteri(pParam->target,pParam->pname,pParam->param);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():target=0x%x,name=0x%x,param=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->pname,pParam->param);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif        
        return true;
    }      
    bool JCWebGLRender::_rendercmd_activeTexture( JCMemClass& cmdBuffer )
    {
        ParamActiveTexture* pParam = cmdBuffer.popp<ParamActiveTexture>();
        m_pWebGLContext->activeTexture(pParam->texture);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():texture=0x%x\n",s_webgl_commands[pParam->funcID],pParam->texture);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif   
        return true;
    } 
    bool JCWebGLRender::_rendercmd_getShaderInfoLog( JCMemClass& cmdBuffer )
    {
        return true;
    } 
    bool JCWebGLRender::_rendercmd_uniform1f( JCMemClass& cmdBuffer )
    {
        ParamUniform1f* pParam = cmdBuffer.popp<ParamUniform1f>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform1f(realLoc,pParam->x);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif   
        return true;
    }    
    bool JCWebGLRender::_rendercmd_uniform1i( JCMemClass& cmdBuffer )
    {
        ParamUniform1i* pParam = cmdBuffer.popp<ParamUniform1i>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        m_pWebGLContext->uniform1i(realLoc,pParam->x);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif   
        return true;
    }     
    bool JCWebGLRender::_rendercmd_uniform2f( JCMemClass& cmdBuffer )
    {
        ParamUniform2f* pParam = cmdBuffer.popp<ParamUniform2f>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform2f(realLoc,pParam->x,pParam->y);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%f,y=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif   
        return true;
    }   
    bool JCWebGLRender::_rendercmd_uniform2i( JCMemClass& cmdBuffer )
    {
        ParamUniform2i* pParam = cmdBuffer.popp<ParamUniform2i>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform2i(realLoc,pParam->x,pParam->y);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%d,y=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif  
        return true;
    }   
    bool JCWebGLRender::_rendercmd_uniform3f( JCMemClass& cmdBuffer )
    {
        ParamUniform3f* pParam = cmdBuffer.popp<ParamUniform3f>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform3f(realLoc,pParam->x,pParam->y,pParam->z);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%f,y=%f,z=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y,pParam->z);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif  
        return true;
    }   
    bool JCWebGLRender::_rendercmd_uniform3i( JCMemClass& cmdBuffer )
    {
        ParamUniform3i* pParam = cmdBuffer.popp<ParamUniform3i>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform3i(realLoc,pParam->x,pParam->y,pParam->z);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%d,y=%d,z=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y,pParam->z);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif  
        return true;
    }    
    bool JCWebGLRender::_rendercmd_uniform4f( JCMemClass& cmdBuffer )
    {
        ParamUniform4f* pParam = cmdBuffer.popp<ParamUniform4f>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform4f(realLoc,pParam->x,pParam->y,pParam->z,pParam->w);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%f,y=%f,z=%f,w=%f\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y,pParam->z,pParam->w);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif  
        return true;
    }  
    bool JCWebGLRender::_rendercmd_uniform4i( JCMemClass& cmdBuffer )
    {
        ParamUniform4i* pParam = cmdBuffer.popp<ParamUniform4i>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if(realLoc==0xffffffff)
        {
            return true;
        }
        m_pWebGLContext->uniform4i(realLoc,pParam->x,pParam->y,pParam->z,pParam->w);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeLoc=%d,realLoc=%d,x=%d,y=%d,z=%d,w=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->location,realLoc,pParam->x,pParam->y,pParam->z,pParam->w);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif  
        return true;
    }  
    bool JCWebGLRender::_rendercmd_uniform1fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if ( pParam->lengthOfBytes > 0 )
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform1fv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform1iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform1iv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform2fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform2fv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform2iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform2iv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform3fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform3fv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform3iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform3iv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform4fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform4fv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniform4iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);
        if (pParam->lengthOfBytes > 0)
        {
            int numberOfElements = pParam->lengthOfBytes / 4;
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            m_pWebGLContext->uniform4iv(realLoc, numberOfElements, pData);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_uniformMatrix2fv( JCMemClass& cmdBuffer )
    {
        ParamUniformMatrix2fv* pParam = cmdBuffer.popp<ParamUniformMatrix2fv>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);

        if(pParam->lengthOfBytes>0)
        {
            int numberOfElements = pParam->lengthOfBytes/4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            if(realLoc==0xffffffff)
            {
                //return true;
            }
            else
            {
                m_pWebGLContext->uniformMatrix2fv(realLoc,pParam->transpose,pData,numberOfElements);
            }
        }

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():\n",s_webgl_commands[pParam->funcID]);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }    

    bool JCWebGLRender::_rendercmd_uniformMatrix3fv( JCMemClass& cmdBuffer )
    {
        ParamUniformMatrix3fv* pParam = cmdBuffer.popp<ParamUniformMatrix3fv>();

        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);

        if(pParam->lengthOfBytes>0)
        {
            int numberOfElements = pParam->lengthOfBytes/4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            if(realLoc==0xffffffff)
            {
                ///return true;
            }
            else
            {
                m_pWebGLContext->uniformMatrix3fv(realLoc,pParam->transpose,pData,numberOfElements);
            }
        }

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():\n",s_webgl_commands[pParam->funcID]);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }     

    bool JCWebGLRender::_rendercmd_uniformMatrix4fv( JCMemClass& cmdBuffer )
    {
        ParamUniformMatrix4fv* pParam = cmdBuffer.popp<ParamUniformMatrix4fv>();
        GLuint realLoc = m_kProgramLocationTable.getRealLocation(pParam->location);

        if(pParam->lengthOfBytes>0)
        {
            int numberOfElements = pParam->lengthOfBytes/4;
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
            if(realLoc==0xffffffff)
            {
                //return true;
            }
            else
            {
                m_pWebGLContext->uniformMatrix4fv(realLoc,pParam->transpose,pData,numberOfElements);
            }
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s(),location=%d,transpose=%d,lengthOfBytes=%d",s_webgl_commands[pParam->funcID],
                pParam->location,pParam->transpose,pParam->lengthOfBytes );
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }    

    bool JCWebGLRender::_rendercmd_vertexAttribPointer( JCMemClass& cmdBuffer )
    {
        ParamVertexAttribPointer* pParam = cmdBuffer.popp<ParamVertexAttribPointer>();
        GLuint realIndex = m_kProgramLocationTable.getRealLocation(pParam->index);
        m_pWebGLContext->vertexAttribPointer(realIndex,pParam->size,pParam->type,pParam->normalized,pParam->stride,pParam->offset);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeIndex=%d,realIndex=%d,size=%d,type=0x%x,normal=%d,stride=%d,offset=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->index,realIndex,pParam->size,pParam->type,pParam->normalized,pParam->stride,pParam->offset);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }   

    bool JCWebGLRender::_rendercmd_enableVertexAttribArray( JCMemClass& cmdBuffer )
    {
        ParamEnableVertexAttribArray* pParam = cmdBuffer.popp<ParamEnableVertexAttribArray>();
        GLuint realIndex = m_kProgramLocationTable.getRealLocation(pParam->index);
        m_pWebGLContext->enableVertexAttribArray(realIndex);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeIndex=%d,realIndex=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->index,realIndex);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }     

    bool JCWebGLRender::_rendercmd_blendFunc( JCMemClass& cmdBuffer )
    {
        ParamBlendFunc* pParam = cmdBuffer.popp<ParamBlendFunc>();
        m_pWebGLContext->blendFunc(pParam->sfactor, pParam->dfactor);

		//fixme:temp code
		//m_pWebGLContext->blendFunc(0x302,pParam->dfactor);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():sfactor=0x%x,dfactor=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->sfactor,pParam->dfactor);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }    

    bool JCWebGLRender::_rendercmd_drawElements( JCMemClass& cmdBuffer )
    {  
        ParamDrawElements* pParam = cmdBuffer.popp<ParamDrawElements>();
        if (m_nCurrentProgram == 0 || m_nCurrentBuffer == 0)return true;
      
        m_pWebGLContext->drawElements(pParam->mode, pParam->count, pParam->type, pParam->offset);
#if DEBUG_WEBGL
        /*
        if(pParam->mode==WebGLRenderingContext::TRIANGLES)
        {
            s_FrameRenderStates.triangles+=(pParam->count/3);
        }
        else
        {
            s_FrameRenderStates.triangles+=(pParam->count-2);
        }
        */
        s_FrameRenderStates.drawBatchTimes++;
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():mode=0x%x,count=%d,type=0x%x,offset=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->mode,pParam->count,pParam->type,pParam->offset);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }      

    bool JCWebGLRender::_rendercmd_deleteTexture( JCMemClass& cmdBuffer )
    {
        ParamDeleteTexture* pParam = cmdBuffer.popp<ParamDeleteTexture>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->texture);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->deleteTexture(realID);
            m_kIDGenerator.deleteID(pParam->texture);   
        }
        
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeTexID=%d,realTexID=%d\n",s_webgl_commands[pParam->funcID],pParam->texture,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeTexID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->texture);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif 
        return true;
    }       
    bool JCWebGLRender::_rendercmd_drawArrays( JCMemClass& cmdBuffer )
    {
        ParamDrawArrays* pParam = cmdBuffer.popp<ParamDrawArrays>();
        m_pWebGLContext->drawArrays(pParam->mode, pParam->first, pParam->count);
        
#if DEBUG_WEBGL
        s_FrameRenderStates.triangles+=pParam->count;
        s_FrameRenderStates.drawBatchTimes++;
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():mode=0x%x,first=%d,count=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->mode,pParam->first,pParam->count);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }  
    bool JCWebGLRender::_rendercmd_bindAttribLocation( JCMemClass& cmdBuffer )
    {
        ParamBindAttribLocation* pParam = cmdBuffer.popp<ParamBindAttribLocation>();
        char* strName = (char*)cmdBuffer.readBuffer(pParam->size);

        GLuint realProgram = m_kIDGenerator.getRealID(pParam->program);
        bool bValidateID = _isValidateRealID(realProgram);
        if (bValidateID)
        {
            m_pWebGLContext->bindAttribLocation(realProgram, pParam->index, strName);
        }
        
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeProgID=%d,realProgID=%d,index=%d,name=%s\n",s_webgl_commands[pParam->funcID],
                    pParam->program,realProgram,pParam->index,strName);
            }
            else
            {
                sprintf(buf,"%s():fadeProgID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],pParam->program);

            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }  
    bool JCWebGLRender::_rendercmd_cullFace( JCMemClass& cmdBuffer )
    {
        ParamCullFace* pParam = cmdBuffer.popp<ParamCullFace>();
        m_pWebGLContext->cullFace(pParam->mode);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():mode=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->mode);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_createFramebuffer( JCMemClass& cmdBuffer )
    {
        ParamCreateFramebuffer* pParam = cmdBuffer.popp<ParamCreateFramebuffer>();
        bool bInvalidate = pParam->fadeBuffID>0?true:false;
        WebGLFramebuffer realBuffID=0;
        if (bInvalidate)
        {
            realBuffID = m_pWebGLContext->createFramebuffer();
            m_kIDGenerator.setRealID(pParam->fadeBuffID, realBuffID, IdType::ID_FRAMEBUFFER);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bInvalidate)
            {
                sprintf(buf,"%s():fadeBuffID=%d,realBuffID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID,realBuffID);
            }
            else
            {
                sprintf(buf,"%s():fadeBuffID=%d, <Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_getParameter( JCMemClass& cmdBuffer )
    {
        ParamGetParameter* pParam = cmdBuffer.popp<ParamGetParameter>();
        GLuint value=0;
        if(pParam->pname== GL_FRAMEBUFFER_BINDING)
        {
            value = m_pWebGLContext->getParameter(GL_FRAMEBUFFER_BINDING);
            m_kIDGenerator.setRealID(pParam->fadeValue, value, IdType::ID_FRAMEBUFFER);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():name=0x%x,realValue=%d,fadeValue=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->pname,value,pParam->fadeValue);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_bindFramebuffer( JCMemClass& cmdBuffer )
    {
        ParamBindFramebuffer* pParam = cmdBuffer.popp<ParamBindFramebuffer>();
        
        GLuint realFrameBuffer = 0;
        bool bValidateID = false;

        if(pParam->fadeFrameBuffer==0)
        {
            bValidateID = true;
        }
        else
        {
            realFrameBuffer = m_kIDGenerator.getRealID(pParam->fadeFrameBuffer);
            bValidateID = _isValidateRealID(realFrameBuffer);
        }
        if (bValidateID)
        {
#ifdef __APPLE__
            if (realFrameBuffer == 0)
                realFrameBuffer = m_nMainFrameBuffer;
#endif
            m_pWebGLContext->bindFramebuffer(pParam->target, realFrameBuffer);
            //if (realFrameBuffer != 0)
            //{
            //    m_pWebGLContext->clearColor(0, 0, 0, 0);
            //    m_pWebGLContext->clear(0x4100);
            //}
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():target=0x%x,fadeFB=%d,realFB=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->fadeFrameBuffer,realFrameBuffer);
            }
            else
            {
                sprintf(buf,"%s():target=0x%x,fadeFB=%d ,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->fadeFrameBuffer);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_depthFunc( JCMemClass& cmdBuffer )
    {
        ParamDepthFunc* pParam = cmdBuffer.popp<ParamDepthFunc>();
        m_pWebGLContext->depthFunc(pParam->func);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():func=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->func);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }

    bool JCWebGLRender::_rendercmd_depthMask( JCMemClass& cmdBuffer )
    {
        ParamDepthMask* pParam = cmdBuffer.popp<ParamDepthMask>();
        m_pWebGLContext->depthMask(pParam->flag);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():flag=0x%x\n",s_webgl_commands[pParam->funcID],pParam->flag);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_depthRange( JCMemClass& cmdBuffer )
    {
        ParamDepthRange* pParam = cmdBuffer.popp<ParamDepthRange>();
        m_pWebGLContext->depthRange(pParam->zNear,pParam->zFar);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():near=%f,far=%f\n",s_webgl_commands[pParam->funcID],pParam->zNear,pParam->zFar);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_stencilFunc( JCMemClass& cmdBuffer )
    {
        ParamStencilFunc* pParam = cmdBuffer.popp<ParamStencilFunc>();
        m_pWebGLContext->stencilFunc(pParam->func,pParam->ref,pParam->mask);

#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():func=0x%x,ref=0x%x,mask=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->func,pParam->ref,pParam->mask);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_stencilMask( JCMemClass& cmdBuffer )
    {
        ParamStencilMask* pParam = cmdBuffer.popp<ParamStencilMask>();
        m_pWebGLContext->stencilMask(pParam->mask);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():mask=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->mask);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_stencilOp( JCMemClass& cmdBuffer )
    {
        ParamStencilOp* pParam = cmdBuffer.popp<ParamStencilOp>();
        m_pWebGLContext->stencilOp(pParam->fail,pParam->zfail,pParam->zpass);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fail=0x%x,zfail=0x%x,zpass=0x%x\n",s_webgl_commands[pParam->funcID],
                    pParam->fail,pParam->zfail,pParam->zpass);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_deleteBuffer(JCMemClass& cmdBuffer)
    {
        ParamDeleteBuffer* pParam = cmdBuffer.popp<ParamDeleteBuffer>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeBuffID);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->deleteBuffer(realID);
            m_kIDGenerator.deleteID(pParam->fadeBuffID);
        }    
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeBuffID=%d,realBuffID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeBuffID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_deleteFramebuffer(JCMemClass& cmdBuffer)
    {
        ParamDeleteFramebuffer* pParam = cmdBuffer.popp<ParamDeleteFramebuffer>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeFrameBuffID);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->deleteFramebuffer(realID);
            m_kIDGenerator.deleteID(pParam->fadeFrameBuffID);
       }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeBuffID=%d,realBuffID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeFrameBuffID,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeBuffID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeFrameBuffID);

            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
       return true;
    }
    bool JCWebGLRender::_rendercmd_deleteProgram(JCMemClass& cmdBuffer)
    {
        ParamDeleteProgram* pParam = cmdBuffer.popp<ParamDeleteProgram>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeProgramID);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->deleteProgram(realID);
            m_kIDGenerator.deleteID(pParam->fadeProgramID);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeProgramID=%d,realProgramID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeProgramID,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeProgramID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeProgramID);

            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
       return true;
    }
    bool JCWebGLRender::_rendercmd_deleteRenderbuffer( JCMemClass& cmdBuffer )
    {
       ParamDeleteRenderbuffer* pParam = cmdBuffer.popp<ParamDeleteRenderbuffer>();
       GLuint realID = m_kIDGenerator.getRealID(pParam->fadeRenderbufferID);
       bool bValidateID = _isValidateRealID(realID);
       if(bValidateID)
       {
           m_pWebGLContext->deleteRenderbuffer(realID);
           m_kIDGenerator.deleteID(pParam->fadeRenderbufferID);
       }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeRenderbufferID=%d,realRenderbufferID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeRenderbufferID,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeRenderbufferID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeRenderbufferID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
       return true;
    }
    bool JCWebGLRender::_rendercmd_deleteShader( JCMemClass& cmdBuffer )
    {
        ParamDeleteShader* pParam = cmdBuffer.popp<ParamDeleteShader>();
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeShaderID);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            m_pWebGLContext->deleteShader(realID);
            m_kIDGenerator.deleteID(pParam->fadeShaderID);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeShaderID=%d,realShaderID=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeShaderID,realID);
            }
            else
            {
                sprintf(buf,"%s():fadeShaderID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeShaderID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_isBuffer(JCMemClass& cmdBuffer)
    {
        ParamIsBuffer* pParam = cmdBuffer.popp<ParamIsBuffer>();

        GLboolean ret = false;
        GLuint realID = m_kIDGenerator.getRealID(pParam->fadeBuffID);
        bool bValidateID = _isValidateRealID(realID);
        if (bValidateID)
        {
            ret = m_pWebGLContext->isBuffer(realID);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():fadeBuffID=%d,realBuffID=%d,ret=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID,realID,ret);
            }
            else
            {
                sprintf(buf,"%s():fadeBuffID=%d,<Invalidate ID!!!>\n",s_webgl_commands[pParam->funcID],
                    pParam->fadeBuffID);
            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_disableVertexAttribArray(JCMemClass& cmdBuffer)
    {
        ParamDisableVertexAttribArray* pParam = cmdBuffer.popp<ParamDisableVertexAttribArray>();
        GLuint realIndex = m_kProgramLocationTable.getRealLocation(pParam->index);
        m_pWebGLContext->disableVertexAttribArray(realIndex);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():fadeIndex=%d,realIndex=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->index,realIndex);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_scissor(JCMemClass& cmdBuffer)
    {
        ParamScissor* pParam = cmdBuffer.popp<ParamScissor>();
        m_pWebGLContext->scissor(pParam->x,pParam->y,pParam->width,pParam->height);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():x=%d,y=%d,width=%d,height=%d\n",s_webgl_commands[pParam->funcID],
                    pParam->x,pParam->y,pParam->width,pParam->height);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_generateMipmap(JCMemClass& cmdBuffer)
    {
        ParamGenerateMipmap* pParam = cmdBuffer.popp<ParamGenerateMipmap>();
        m_pWebGLContext->generateMipmap(pParam->target);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():target=0x%x\n",s_webgl_commands[pParam->funcID],pParam->target);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_framebufferTexture2D( JCMemClass& cmdBuffer )
    {
        ParamFramebufferTexture2D* pParam = cmdBuffer.popp<ParamFramebufferTexture2D>();
        GLuint realTex = m_kIDGenerator.getRealID(pParam->fadeTexID);
        bool bValidateID = _isValidateRealID(realTex);
        if(bValidateID)
        {
            m_pWebGLContext->framebufferTexture2D(pParam->target, pParam->attachment, pParam->textarget, realTex, pParam->level);
        }
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            if(bValidateID)
            {
                sprintf(buf,"%s():target=0x%x,attachment=%d,textarget=%d,realTexID=%d,fadeTexID=%d,level=%d \n",
                    s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->attachment,pParam->textarget,realTex,pParam->fadeTexID,pParam->level);
            }
            else
            {
                sprintf(buf,"%s():target=0x%x,attachment=%d,textarget=%d,fadeTexID=%d,level=%d ,<Invalidate ID!!!>\n",
                    s_webgl_commands[pParam->funcID],
                    pParam->target,pParam->attachment,pParam->textarget,pParam->fadeTexID,pParam->level);

            }
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_checkFramebufferStatus( JCMemClass& cmdBuffer )
    {
        ParamCheckFramebufferStatus* pParam = cmdBuffer.popp<ParamCheckFramebufferStatus>();
        GLenum state = m_pWebGLContext->checkFramebufferStatus(pParam->target);
#if DEBUG_WEBGL
        if(CHECK_OUTPUT_DEBUG(pParam->funcID))
        {
            char buf[128];
            sprintf(buf,"%s():target=0x%x,state=0x%x\n",
                    s_webgl_commands[pParam->funcID],
                    pParam->target,state);
            LOGI("%s", buf);
            s_webgl_info.push_back(std::string(buf));
        }
#endif
        return true;
    }
    bool JCWebGLRender::_rendercmd_createRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamCreateBuffer* pParam = cmdBuffer.popp<ParamCreateBuffer>();
        GLuint realBuffer = m_pWebGLContext->createRenderbuffer();
        m_kIDGenerator.setRealID(pParam->buffer, realBuffer, IdType::ID_BUFFER);
        return true;
    }
    bool JCWebGLRender::_rendercmd_bindRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamBindBuffer* pParam = cmdBuffer.popp<ParamBindBuffer>();
        GLuint realID = 0;
        bool bValidateID = false;
        if (pParam->buffer == 0)
        {
            bValidateID = false;
        }
        else
        {
            realID = m_kIDGenerator.getRealID(pParam->buffer);
            bValidateID = _isValidateRealID(realID);
        }
        if (bValidateID)
        {
            m_pWebGLContext->bindRenderbuffer(pParam->target, realID);
        }
        else
        {
            m_pWebGLContext->bindRenderbuffer(pParam->target, 0);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_renderbufferStorage(JCMemClass& cmdBuffer)
    {
        ParamRenderBUfferStorage* pParam = cmdBuffer.popp<ParamRenderBUfferStorage>();
        m_pWebGLContext->renderbufferStorage(pParam->target, pParam->internalformat, pParam->width, pParam->height);
        return true;
    }
    bool JCWebGLRender::_rendercmd_framebufferRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamFramebufferRenderbuffer* pParam = cmdBuffer.popp<ParamFramebufferRenderbuffer>();
        GLuint realID = 0;
        bool bValidateID = false;
        if (pParam->renderBuffer == 0)
        {
            bValidateID = false;
        }
        else
        {
            realID = m_kIDGenerator.getRealID(pParam->renderBuffer);
            bValidateID = _isValidateRealID(realID);
        }
        if (bValidateID)
        {
            m_pWebGLContext->framebufferRenderbuffer(pParam->target, pParam->attachment, pParam->renderbuffertarget, realID);
        }
        else
        {
            m_pWebGLContext->framebufferRenderbuffer(pParam->target, pParam->attachment, pParam->renderbuffertarget, 0);
        }
        return true;
    }

    bool JCWebGLRender::_rendercmd_blendFuncSeparate(JCMemClass& cmdBuffer)
    {
         ParamBlendFuncSeparate* pParam = cmdBuffer.popp<ParamBlendFuncSeparate>();
         m_pWebGLContext->blendFuncSeparate(pParam->srcRGB, pParam->dstRGB, pParam->srcAlpha, pParam->dstAlpha);
         return true;
    }

    bool JCWebGLRender::_rendercmd_frontFace(JCMemClass& cmdBuffer)
    {
        ParamFrontFace* pParam = cmdBuffer.popp<ParamFrontFace>();
        m_pWebGLContext->frontFace(pParam->mode);
        return true;
    }

    bool JCWebGLRender::_rendercmd_getActiveAttrib(JCMemClass& cmdBuffer)
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_getActiveUniform(JCMemClass& cmdBuffer)
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
    }

    void JCWebGLRender::onGLReady()
    {
    }
    void JCWebGLRender::renderFrame(long nCurrentFrame, bool bStopRender)
    {

    }
    void JCWebGLRender::invalidGLRes()
    {
        LOGI("JCWebGLRender::invalidGLRes");
    }
    bool JCWebGLRender::layaTexSubImage2D(int p_pObjectID, GLuint target, GLuint level, GLuint internalformat, GLuint w, GLuint h, GLuint border, GLuint format, GLuint type, bool bSubImage, GLint offsetx, GLint offsety, GLuint objType, bool bPremultiplyAlpha)
    {
        if (p_pObjectID == -1)
        {
            if (w <= 0 || h <= 0) return false;
            m_pWebGLContext->texImage2D(target, level, internalformat, w , h, border, format, type, NULL);
            return true;
        }
        else
        {
            int width = 0;
            int height = 0;
            switch (objType) 
            {
            case 0:
            {
                JCImage* pImage = m_pImageManager->getImage(p_pObjectID);
                if (pImage)
                {
                    if (pImage->enableBitmapData() == false)
                    {
                        return false;
                    }
                    width = pImage->getWidth();
                    height = pImage->getHeight();
                    char* pBufferData = bPremultiplyAlpha ? pImage->m_kBitmapData.premultiplyAlphaData() : (char*)pImage->m_kBitmapData.m_pImageData;
                    if (pBufferData)
                    {
                        if (bSubImage)
                        {
                            m_pWebGLContext->texSubImage2D(target, level, offsetx, offsety, width, height, format, type, pBufferData);
                        }
                        else
                        {
                            m_pWebGLContext->texImage2D(target, level, internalformat, w > 0 ? w : width, h > 0 ? h : height, border, format, type, pBufferData);
                        }
                    }
                    else
                    {
                        //TODO
                    }
                }
                break;
            }
            case 1:
            {
                //TODO canvas
                /*
                if (bSubImage)
                {
                    m_pWebGLContext->texSubImage2D(target, level, offsetx, offsety, width, height, format, type, m_kCurrentBitmapData.m_pImageData);
                }
                else
                {
                    m_pWebGLContext->texImage2D(target, level, internalformat, w > 0 ? w : width, h > 0 ? h : height, border, format, type, m_kCurrentBitmapData.m_pImageData);
                }
                */
                break;
            }
            case 2:
            {
                //TODO 文字
                break;
            }
            case 3:
            {
                JCTextUnitInfo* pTextInfo = m_pTextMemoryCanvas->getTextInfoFromRenderThread();
                if (m_pTextMemoryCanvas->drawTextToBitmapData(pTextInfo))
                {
                    char* pBufferData = bPremultiplyAlpha ? m_pTextMemoryCanvas->m_pBitmapData.premultiplyAlphaData() : m_pTextMemoryCanvas->m_pBitmapData.m_pImageData;
                    if (pBufferData != 0)
                    {
                        if (bSubImage)
                        {
                            m_pWebGLContext->texSubImage2D(target, level, offsetx + pTextInfo->x, offsety + pTextInfo->y, w > 0 ? w : m_pTextMemoryCanvas->m_pBitmapData.m_nWidth, h > 0 ? h : m_pTextMemoryCanvas->m_pBitmapData.m_nHeight, format, type, pBufferData);
                        }
                        else
                        {
                            m_pWebGLContext->texImage2D(target, level, internalformat, w > 0 ? w : m_pTextMemoryCanvas->m_pBitmapData.m_nWidth, h > 0 ? h : m_pTextMemoryCanvas->m_pBitmapData.m_nHeight, border, format, type, pBufferData);
                        }
                    }
                }
                break;
            }
            }
        }
        return true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool JCWebGLRender::_rendercmd_only_parse_getExtension(JCMemClass& cmdBuffer)
    {
        ParamGetExtension* pParam = cmdBuffer.popp<ParamGetExtension>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createBuffer(JCMemClass& cmdBuffer)
    {
        ParamCreateBuffer* pParam = cmdBuffer.popp<ParamCreateBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bindBuffer(JCMemClass& cmdBuffer)
    {
        ParamBindBuffer* pParam = cmdBuffer.popp<ParamBindBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bufferData_vector(JCMemClass& cmdBuffer)
    {
        ParamBufferDataVector * pParam = cmdBuffer.popp<ParamBufferDataVector>();
        if (pParam->lengthOfBytes > 0)
        {
            char* pData = cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bufferData_size(JCMemClass& cmdBuffer)
    {
        ParamBufferDataSize* pParam = cmdBuffer.popp<ParamBufferDataSize>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bufferSubData(JCMemClass& cmdBuffer)
    {
        ParamBufferSubData* pParam = cmdBuffer.popp<ParamBufferSubData>();
        if (pParam->lengthOfBytes > 0)
        {
            char* pData = cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createShader(JCMemClass& cmdBuffer)
    {
        ParamCreateShader* pParam = cmdBuffer.popp<ParamCreateShader>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_shaderSource(JCMemClass& cmdBuffer)
    {
        ParamShaderSource* pParam = cmdBuffer.popp<ParamShaderSource>();
        char* pData = (char*)cmdBuffer.readBuffer(pParam->length);;
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_compileShader(JCMemClass& cmdBuffer)
    {
        ParamCompileShader* pParam = cmdBuffer.popp<ParamCompileShader>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getShaderParameter(JCMemClass& cmdBuffer)
    {
        ParamGetShaderParameter* pParam = cmdBuffer.popp<ParamGetShaderParameter>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createProgram(JCMemClass& cmdBuffer)
    {
        ParamCreateProgram* pParam = cmdBuffer.popp<ParamCreateProgram>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_attachShader(JCMemClass& cmdBuffer)
    {
        ParamAttachShader* pParam = cmdBuffer.popp<ParamAttachShader>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_linkProgram(JCMemClass& cmdBuffer)
    {
        ParamLinkProgram* pParam = cmdBuffer.popp<ParamLinkProgram>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getProgramParameter(JCMemClass& cmdBuffer)
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_useProgram(JCMemClass& cmdBuffer)
    {
        ParamUseProgram* pParam = cmdBuffer.popp<ParamUseProgram>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getUniformLocation(JCMemClass& cmdBuffer)
    {
        ParamGetUniformLocation* pParam = cmdBuffer.popp<ParamGetUniformLocation>();
        char* pData = (char*)cmdBuffer.readBuffer(pParam->len);
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getAttribLocation(JCMemClass& cmdBuffer)
    {
        ParamGetAttribLocation* pParam = cmdBuffer.popp<ParamGetAttribLocation>();
        char* pData = (char*)cmdBuffer.readBuffer(pParam->len);
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_disable(JCMemClass& cmdBuffer)
    {
        ParamDisable* pParam = cmdBuffer.popp<ParamDisable>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_enable(JCMemClass& cmdBuffer)
    {
        ParamEnable* pParam = cmdBuffer.popp<ParamEnable>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_colorMask(JCMemClass& cmdBuffer)
    {
        ParamColorMask* pParam = cmdBuffer.popp<ParamColorMask>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_viewport(JCMemClass& cmdBuffer)
    {
        ParamViewport* pParam = cmdBuffer.popp<ParamViewport>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_clear(JCMemClass& cmdBuffer)
    {
        ParamClear* pParam = cmdBuffer.popp<ParamClear>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_clearColor(JCMemClass& cmdBuffer)
    {
        ParamClearColor* pParam = cmdBuffer.popp<ParamClearColor>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createTexture(JCMemClass& cmdBuffer)
    {
        ParamCreateTexture* pParam = cmdBuffer.popp<ParamCreateTexture>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bindTexture(JCMemClass& cmdBuffer)
    {
        ParamBindTexture* pParam = cmdBuffer.popp<ParamBindTexture>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_pixelStorei(JCMemClass& cmdBuffer)
    {
        ParamPixelStorei* pParam = cmdBuffer.popp<ParamPixelStorei>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_texImage2D_06(JCMemClass& cmdBuffer)
    {
        ParamTexImage2D_06* pParam = cmdBuffer.popp<ParamTexImage2D_06>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_texImage2D_09(JCMemClass& cmdBuffer)
    {
        ParamTexImage2D_09* pParam = cmdBuffer.popp<ParamTexImage2D_09>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_texSubImage2D(JCMemClass& cmdBuffer)
    {
        ParamTexSubImage2D* pParam = cmdBuffer.popp<ParamTexSubImage2D>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_texParameterf(JCMemClass& cmdBuffer)
    {
        ParamTexParameterf* pParam = cmdBuffer.popp<ParamTexParameterf>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_texParameteri(JCMemClass& cmdBuffer)
    {
        ParamTexParameteri* pParam = cmdBuffer.popp<ParamTexParameteri>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_activeTexture(JCMemClass& cmdBuffer)
    {
        ParamActiveTexture* pParam = cmdBuffer.popp<ParamActiveTexture>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getShaderInfoLog(JCMemClass& cmdBuffer)
    {
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform1f(JCMemClass& cmdBuffer)
    {
        ParamUniform1f* pParam = cmdBuffer.popp<ParamUniform1f>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform1i(JCMemClass& cmdBuffer)
    {
        ParamUniform1i* pParam = cmdBuffer.popp<ParamUniform1i>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform2f(JCMemClass& cmdBuffer)
    {
        ParamUniform2f* pParam = cmdBuffer.popp<ParamUniform2f>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform2i(JCMemClass& cmdBuffer)
    {
        ParamUniform2i* pParam = cmdBuffer.popp<ParamUniform2i>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform3f(JCMemClass& cmdBuffer)
    {
        ParamUniform3f* pParam = cmdBuffer.popp<ParamUniform3f>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform3i(JCMemClass& cmdBuffer)
    {
        ParamUniform3i* pParam = cmdBuffer.popp<ParamUniform3i>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform4f(JCMemClass& cmdBuffer)
    {
        ParamUniform4f* pParam = cmdBuffer.popp<ParamUniform4f>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform4i(JCMemClass& cmdBuffer)
    {
        ParamUniform4i* pParam = cmdBuffer.popp<ParamUniform4i>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform1fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform1iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform2fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform2iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform3fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform3iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform4fv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniform4iv(JCMemClass& cmdBuffer)
    {
        ParamUniformArray* pParam = cmdBuffer.popp<ParamUniformArray>();
        if (pParam->lengthOfBytes > 0)
        {
            GLint* pData = (GLint*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniformMatrix2fv(JCMemClass& cmdBuffer)
    {
        ParamUniformMatrix2fv* pParam = cmdBuffer.popp<ParamUniformMatrix2fv>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniformMatrix3fv(JCMemClass& cmdBuffer)
    {
        ParamUniformMatrix3fv* pParam = cmdBuffer.popp<ParamUniformMatrix3fv>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_uniformMatrix4fv(JCMemClass& cmdBuffer)
    {
        ParamUniformMatrix4fv* pParam = cmdBuffer.popp<ParamUniformMatrix4fv>();
        if (pParam->lengthOfBytes > 0)
        {
            GLfloat* pData = (GLfloat*)cmdBuffer.readBuffer(pParam->lengthOfBytes);
        }
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_vertexAttribPointer(JCMemClass& cmdBuffer)
    {
        ParamVertexAttribPointer* pParam = cmdBuffer.popp<ParamVertexAttribPointer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_enableVertexAttribArray(JCMemClass& cmdBuffer)
    {
        ParamEnableVertexAttribArray* pParam = cmdBuffer.popp<ParamEnableVertexAttribArray>();
        return true;
    }

    bool JCWebGLRender::_rendercmd_only_parse_blendFunc(JCMemClass& cmdBuffer)
    {
        ParamBlendFunc* pParam = cmdBuffer.popp<ParamBlendFunc>();
        return true;
    }

    bool JCWebGLRender::_rendercmd_only_parse_drawElements(JCMemClass& cmdBuffer)
    {
        ParamDrawElements* pParam = cmdBuffer.popp<ParamDrawElements>();
        return true;
    }

    bool JCWebGLRender::_rendercmd_only_parse_deleteTexture(JCMemClass& cmdBuffer)
    {
        ParamDeleteTexture* pParam = cmdBuffer.popp<ParamDeleteTexture>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_drawArrays(JCMemClass& cmdBuffer)
    {
        ParamDrawArrays* pParam = cmdBuffer.popp<ParamDrawArrays>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bindAttribLocation(JCMemClass& cmdBuffer)
    {
        ParamBindAttribLocation* pParam = cmdBuffer.popp<ParamBindAttribLocation>();
        char* strName = (char*)cmdBuffer.readBuffer(pParam->size);
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_cullFace(JCMemClass& cmdBuffer)
    {
        ParamCullFace* pParam = cmdBuffer.popp<ParamCullFace>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createFramebuffer(JCMemClass& cmdBuffer)
    {
        ParamCreateFramebuffer* pParam = cmdBuffer.popp<ParamCreateFramebuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getParameter(JCMemClass& cmdBuffer)
    {
        ParamGetParameter* pParam = cmdBuffer.popp<ParamGetParameter>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bindFramebuffer(JCMemClass& cmdBuffer)
    {
        ParamBindFramebuffer* pParam = cmdBuffer.popp<ParamBindFramebuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_depthFunc(JCMemClass& cmdBuffer)
    {
        ParamDepthFunc* pParam = cmdBuffer.popp<ParamDepthFunc>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_depthMask(JCMemClass& cmdBuffer)
    {
        ParamDepthMask* pParam = cmdBuffer.popp<ParamDepthMask>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_depthRange(JCMemClass& cmdBuffer)
    {
        ParamDepthRange* pParam = cmdBuffer.popp<ParamDepthRange>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_stencilFunc(JCMemClass& cmdBuffer)
    {
        ParamStencilFunc* pParam = cmdBuffer.popp<ParamStencilFunc>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_stencilMask(JCMemClass& cmdBuffer)
    {
        ParamStencilMask* pParam = cmdBuffer.popp<ParamStencilMask>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_stencilOp(JCMemClass& cmdBuffer)
    {
        ParamStencilOp* pParam = cmdBuffer.popp<ParamStencilOp>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_deleteBuffer(JCMemClass& cmdBuffer)
    {
        ParamDeleteBuffer* pParam = cmdBuffer.popp<ParamDeleteBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_deleteFramebuffer(JCMemClass& cmdBuffer)
    {
        ParamDeleteFramebuffer* pParam = cmdBuffer.popp<ParamDeleteFramebuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_deleteProgram(JCMemClass& cmdBuffer)
    {
        ParamDeleteProgram* pParam = cmdBuffer.popp<ParamDeleteProgram>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_deleteRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamDeleteRenderbuffer* pParam = cmdBuffer.popp<ParamDeleteRenderbuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_deleteShader(JCMemClass& cmdBuffer)
    {
        ParamDeleteShader* pParam = cmdBuffer.popp<ParamDeleteShader>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_isBuffer(JCMemClass& cmdBuffer)
    {
        ParamIsBuffer* pParam = cmdBuffer.popp<ParamIsBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_disableVertexAttribArray(JCMemClass& cmdBuffer)
    {
        ParamDisableVertexAttribArray* pParam = cmdBuffer.popp<ParamDisableVertexAttribArray>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_scissor(JCMemClass& cmdBuffer)
    {
        ParamScissor* pParam = cmdBuffer.popp<ParamScissor>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_generateMipmap(JCMemClass& cmdBuffer)
    {
        ParamGenerateMipmap* pParam = cmdBuffer.popp<ParamGenerateMipmap>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_framebufferTexture2D(JCMemClass& cmdBuffer)
    {
        ParamFramebufferTexture2D* pParam = cmdBuffer.popp<ParamFramebufferTexture2D>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_checkFramebufferStatus(JCMemClass& cmdBuffer)
    {
        ParamCheckFramebufferStatus* pParam = cmdBuffer.popp<ParamCheckFramebufferStatus>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_createRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamCreateBuffer* pParam = cmdBuffer.popp<ParamCreateBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_bindRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamBindBuffer* pParam = cmdBuffer.popp<ParamBindBuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_renderbufferStorage(JCMemClass& cmdBuffer)
    {
        ParamRenderBUfferStorage* pParam = cmdBuffer.popp<ParamRenderBUfferStorage>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_framebufferRenderbuffer(JCMemClass& cmdBuffer)
    {
        ParamFramebufferRenderbuffer* pParam = cmdBuffer.popp<ParamFramebufferRenderbuffer>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_blendFuncSeparate(JCMemClass& cmdBuffer)
    {
        ParamBlendFuncSeparate* pParam = cmdBuffer.popp<ParamBlendFuncSeparate>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_frontFace(JCMemClass& cmdBuffer)
    {
        ParamFrontFace* pParam = cmdBuffer.popp<ParamFrontFace>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getActiveAttrib(JCMemClass& cmdBuffer)
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
    }
    bool JCWebGLRender::_rendercmd_only_parse_getActiveUniform(JCMemClass& cmdBuffer)
    {
        ParamGetProgramParameter* pParam = cmdBuffer.popp<ParamGetProgramParameter>();
        return true;
    }
    void JCWebGLRender::setIosViewport()
    {
#ifdef __APPLE__
        if (m_kViewPortRect.width > 0)
        {
            glViewport(m_kViewPortRect.x, m_kViewPortRect.y, m_kViewPortRect.width, m_kViewPortRect.height);
        }
#endif

    }
    void JCWebGLRender::saveViewPortRect(float x, float y, float w, float h)
    {
#ifdef __APPLE__
        if (m_pWebGLContext->getParameter(GL_FRAMEBUFFER_BINDING) == m_nMainFrameBuffer)
        {
            m_kViewPortRect.x = x;
            m_kViewPortRect.y = y;
            m_kViewPortRect.width = w;
            m_kViewPortRect.height = h;
        }
#endif
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
