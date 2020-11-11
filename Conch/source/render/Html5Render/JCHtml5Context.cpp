/**
@file			JCHtml5Context.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCHtml5Context.h"
#include <util/Log.h>
#include <resource/DisplayRes/JCTexture.h>
#include <3DRS/JCConchMesh2D.h>
#include "JCHtml5Render.h"
#include <3DRS/JCGpuProgram.h>
#include <3DRS/JCRenderGroupData.h>
#include "JCHtml5RenderManager.h"
#include <MonkVG/vgext.h>
#include <algorithm>
#include <mkContext.h>
#include "../Node/JCNodeStyle.h"
#include <functional>
#include <resource/Atlas/JCAtlas.h>
#include "../Particle2D/JCParticleTemplate2D.h"
#include "../Particle2D/JCParticleTemplate2DCmdDispath.h"
using std::min;
using std::max;
extern int g_nInnerWidth ;
extern int g_nInnerHeight ;

//------------------------------------------------------------------------------
namespace laya
{
#define CANVAS_SAVE_CMD_SIZE 1024

    static unsigned short rectIndex[] = { 0,1,2,2,3,0 };

    void setBlurParam(JCHtml5Context* pCtx, float texw, float texh, float blurW, float blurH, float blur, float blurOffx, float blurOffy,
        unsigned int color);

    void onVGTriangles(void* pData, int len, void* pUser) {
        JCHtml5Context* pCtx = (JCHtml5Context*)pUser;
        if (pCtx) {
            pCtx->onVGData(JCHtml5Context::VG_DATA_TYPE::fillcolor, pData, len);
        }
    }

    void onVGTriStrips(void* pData, int len, void* pUser) {
        JCHtml5Context* pCtx = (JCHtml5Context*)pUser;
        if (pCtx) {
            pCtx->onVGData(JCHtml5Context::VG_DATA_TYPE::stokedata, pData, len);
        }
    }
    //------------------------------------------------------------------------------
    JCHtml5Context::JCHtml5Context(JCHtml5Render* pHtml5Render, JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager, JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager, JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispath)
    {
        m_nWidth = m_nHeight = 0;
        m_ShaderData.m_fWidth = m_ShaderData.m_fHeight = 0.0f;
        m_pHtml5Render = pHtml5Render;
        m_pFontManager = pFontManager;
        m_pTextManager = pTextManager;
        m_pTextureManager = pTextureManager;
        m_pHtml5RenderManager = pHtml5RenderManager;
        m_p2DShaderManager = p2DShaderManager;
        m_pBufferManager = pBufferManager;
        m_pShaderManager = pShaderManager;
        m_pContextData = new JCContextSaveData();
        m_pImageManager = pImageManager;
        m_vVectorCmdDispath = vVectorCmdDispath;
        m_pMaterialManager = new JCMaterialManager();
        m_pGeo = new RectGeometry();
        m_pDynamicMesh = new JCConchMesh2D();
        m_kGlobalRenderInfoDesc.add("g_DevInfo", 0, JCNamedData::tp_fvec4, 1).
            add("size", 0, JCNamedData::tp_fvec2, 1).
            add("mmat", offsetof(ShaderData, m_matMat1), JCNamedData::tp_mat4, 1).
            add("g_WorldMat", offsetof(ShaderData, m_matWorldMat), JCNamedData::tp_mat4, 1).
            add("u_color", offsetof(ShaderData, m_vPsColor), JCNamedData::tp_fvec4, 1).
            add("u_blur_off", offsetof(ShaderData, m_vBlurOff), JCNamedData::tp_fvec4, 1).
            add("u_blurInfo", offsetof(ShaderData, m_vVec1), JCNamedData::tp_fvec4, 1).
            add("g_colorMat", offsetof(ShaderData, m_matFilter), JCNamedData::tp_mat4, 1).
            add("g_colorAlpha", offsetof(ShaderData, m_matFilter1), JCNamedData::tp_fvec4, 1).
            add("g_Alpha", offsetof(ShaderData, m_fAlpha), JCNamedData::tp_f32, 1).
            add("u_CurrentTime", offsetof(ShaderData, m_fCurrentTime), JCNamedData::tp_f32, 1).
            add("u_Duration", offsetof(ShaderData, m_fDuration), JCNamedData::tp_f32, 1).
            add("u_EndVelocity", offsetof(ShaderData, m_fEndVelocity), JCNamedData::tp_f32, 1).
            add("u_Gravity", offsetof(ShaderData, m_vGravity), JCNamedData::tp_fvec3, 1);

        //m_kRenderDataDescForSetIBVB.add("size", 0, JCNamedData::tp_fvec2, 1);
        //m_kRenderDataDescForSetIBVB.add("mmat", (int)&((ShaderData*)0)->m_matMat1, JCNamedData::tp_mat4, 1);
        JCMaterial mtl;
        m_pDynamicMesh->setNamedData(mtl.getNamedData(), &m_kGlobalRenderInfoDesc);
        //memset(m_kGlobalRenderData, 0, sizeof(m_kGlobalRenderData));
        memset(m_ShaderData.m_matMat1, 0, sizeof(m_ShaderData.m_matMat1));
        m_ShaderData.m_matMat1[0] = m_ShaderData.m_matMat1[5] = m_ShaderData.m_matMat1[10] = m_ShaderData.m_matMat1[15] = 1.0f;
        memset(m_ShaderData.m_matWorldMat, 0, sizeof(m_ShaderData.m_matWorldMat));
        m_ShaderData.m_matWorldMat[0] = m_ShaderData.m_matWorldMat[5] = m_ShaderData.m_matWorldMat[10] = m_ShaderData.m_matWorldMat[15] = 1.0f;

        m_nCanvasType = CANVAS_TYPE_MEMORY;
        m_pSaveCmd = new JCMemClass(CANVAS_SAVE_CMD_SIZE, CANVAS_SAVE_CMD_SIZE);
        m_pSaveCmd->setAlign(true);
        m_pRenderTarget = NULL;
        m_bRepaint = true;
        vg = new MkVGContext(1024, 768, VG_RENDERING_BACKEND_TYPE_LAYA, onVGTriangles, onVGTriStrips, this);
        //vgCreateContextMNK(1024, 768, VG_RENDERING_BACKEND_TYPE_OPENGLES20,NULL,NULL,NULL);
        //m_vgPath = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        m_vAllPath.reserve(20);
        m_vAllPath.push_back(vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL));
        m_vgCurPath = m_vAllPath.back();
        m_vgPaint = vg->CreatePaint();
        m_vgStrokePaint = vg->CreatePaint();
        vg->SetPaint(m_vgPaint, VG_FILL_PATH);
        vg->SetPaint(m_vgStrokePaint, VG_STROKE_PATH);
        m_pHostGpuContext = NULL;
        m_bGLScissorEnabled = false;
        m_bMemoryCanvasScissor = false;
        m_vTempWordRes.reserve(128);
    }
    //------------------------------------------------------------------------------
    JCHtml5Context::~JCHtml5Context()
    {
        clearAllAtlasCallback();
        for (auto p : m_vAllPath) {
            vg->ClearPath(p, 0);
            vg->DestroyPath(p);
        }

        m_pHtml5Render = NULL;
        if (m_pContextData != NULL)
        {
            delete m_pContextData;
            m_pContextData = NULL;
        }
        if (m_pGeo)
        {
            delete m_pGeo;
            m_pGeo = NULL;
        }
        if (m_pDynamicMesh)
        {
            delete m_pDynamicMesh;
            m_pDynamicMesh = NULL;
        }
        if (m_pMaterialManager)
        {
            delete m_pMaterialManager;
            m_pMaterialManager = NULL;
        }
        if (m_pRenderTarget)
        {
            delete m_pRenderTarget;
            m_pRenderTarget = NULL;
        }
        if (m_pSaveCmd)
        {
            delete m_pSaveCmd;
            m_pSaveCmd = NULL;
        }
        if (vg) {
            delete vg;
            vg = nullptr;
        }
        m_pHostGpuContext = NULL;
    }
    void JCHtml5Context::clearSaveCmd()
    {
        m_bRepaint = true;
        m_pSaveCmd->clearData();
        m_pDynamicMesh->resetData();
    }
    void JCHtml5Context::setCanvasSize(int w, int h)
    {
        if (m_nWidth == w && m_nHeight == h)return;
        m_nWidth = w;
        m_nHeight = h;
        m_ShaderData.m_fWidth = (float)w;
        m_ShaderData.m_fHeight = (float)h;
        m_kLinkRect.x = 0;
        m_kLinkRect.y = 0;
        m_kLinkRect.width = w;
        m_kLinkRect.height = h;
        vg->ResizeSurfaceMNK(w, h);
        m_pContextData->reset(m_nWidth, m_nHeight);
        if (m_nCanvasType == CANVAS_TYPE_RENDER_TARGET)
        {
            //如果大小超过了2048 自动切换成了 memoryCanvas
            if (m_nWidth > 2048 || m_nHeight > 2048)
            {
                if (m_pRenderTarget)
                {
                    delete m_pRenderTarget;
                    m_pRenderTarget = NULL;
                }
                m_nCanvasType = CANVAS_TYPE_MEMORY;
                LOGW("JCHtml5Context::setCanvasSize size > 2048 auto change to memory canvas");
            }
            else
            {
                if (m_pRenderTarget)
                {
                    m_pRenderTarget->setSize(m_nWidth, m_nHeight);
                }
            }
        }
    }
    void JCHtml5Context::setCanvasType(CANVAS_TYPE nType)
    {
        if (m_nCanvasType == nType)return;
        switch (nType)
        {
        case CANVAS_TYPE_MAIN:
            m_pHostGpuContext = this;
            m_nCanvasType = nType;
            break;
        case CANVAS_TYPE_RENDER_TARGET:
            if (m_nWidth > 2048 || m_nHeight > 2048)
            {
                LOGW("JCHtml5Context::setCanvasType error size > 2048");
                m_nCanvasType = CANVAS_TYPE_MEMORY;
            }
            else
            {
                m_pHostGpuContext = this;
                m_nCanvasType = CANVAS_TYPE_RENDER_TARGET;
            }
            break;
        case CANVAS_TYPE_MEMORY:
            m_nCanvasType = CANVAS_TYPE_MEMORY;
            break;
        default:
            m_nCanvasType = CANVAS_TYPE_RENDER_TARGET;
            LOGE("JCHtml5Context::setCanvasType can't support this type=%d", nType);
            break;
        }
    }
    extern unsigned int base64Encode(char *pDest, const char *pSrc, unsigned int srclen);
    extern std::pair<unsigned char*, unsigned long> convertBitmapToJpeg(const char* p_pData, int nWidth, int nHeight, int nBpp);
    extern std::pair<unsigned char*, unsigned long> convertBitmapToPng(const char* p_pData, int nWidth, int nHeight, int nBitDepth);
    void JCHtml5Context::toBase64(const char* type, float encoderOptions, std::function<void(const char* base64)> callback)
    {
        if (m_nWidth == 0 || m_nHeight == 0)
        {
            const char* pstrHeader = "data:";
            int length = strlen(pstrHeader);
            char *pDest = new char[length + 1];
            memcpy(pDest, pstrHeader, length);
            pDest[length] = '\0';
            callback(pDest);
            return;
        }
        string strType(type);
        switch (m_nCanvasType)
        {
        case CANVAS_TYPE_MAIN:
            m_vOnRenderEndFuncs.push_back([this, callback, strType]()
            { 
                int w = m_nWidth;
                int h = m_nHeight;
                int size = sizeof(GLubyte) * w * h * 4;
                unsigned char* pTempPixels = new unsigned char[size];
                unsigned char* pPixels = new unsigned char[size];
                glFinish();
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pTempPixels);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                for (int row = 0; row < h; ++row)
                {
                    memcpy(pPixels + (h - row - 1) * w * 4, pTempPixels + row * w * 4, w * 4);
                }
                delete[] pTempPixels;
                             
                int length = (size + 2) / 3 * 4;
                char *pDest = new char[length];
                memset(pDest, 0, length);
                char *pCurrent = pDest;
                std::pair<unsigned char*, unsigned long> result;
                if (strType == "image/jpeg")
                {
                    const char* pstrHeader = "data:image/jpeg;base64,";
                    int length = strlen(pstrHeader);
                    for (int i = 0; i < length; i++)
                    {
                        *pCurrent = pstrHeader[i];
                        pCurrent++;
                    }
                    result = convertBitmapToJpeg((const char*)pPixels, w, h , 32);
                }
                else
                {
                    const char* pstrHeader = "data:image/png;base64,";
                    int length = strlen(pstrHeader);
                    for (int i = 0; i < length; i++)
                    {
                        *pCurrent = pstrHeader[i];
                        pCurrent++;
                    }
                    result = laya::convertBitmapToPng((const char*)pPixels, w, h, 8);
                }
                
                base64Encode(pCurrent, (const char*)result.first, result.second);
                delete[] result.first;
                delete[] pPixels;
                callback(pDest);
            });
            break;
        case CANVAS_TYPE_RENDER_TARGET:
            m_vOnRenderEndFuncs.push_back([this, callback, strType]()
            {
                int w = m_nWidth;
                int h = m_nHeight;
                int size = sizeof(GLubyte) * w * h * 4;
                unsigned char* pTempPixels = new unsigned char[size];
                unsigned char* pPixels = new unsigned char[size];
                m_pRenderTarget->getPixels(pTempPixels, 0, 0, w, h);
                for (int row = 0; row < h; ++row)
                {
                    memcpy(pPixels + (h - row - 1) * w * 4, pTempPixels + row * w * 4, w * 4);
                }
                delete[] pTempPixels;

                int length = (size + 2) / 3 * 4;
                char *pDest = new char[length];
                memset(pDest, 0, length);
                char *pCurrent = pDest;
                std::pair<unsigned char*, unsigned long> result;
                if (strType == "image/jpeg")
                {
                    const char* pstrHeader = "data:image/jpeg;base64,";
                    int length = strlen(pstrHeader);
                    for (int i = 0; i < length; i++)
                    {
                        *pCurrent = pstrHeader[i];
                        pCurrent++;
                    }
                    result = convertBitmapToJpeg((const char*)pPixels, w, h, 32);
                }
                else
                {
                    const char* pstrHeader = "data:image/png;base64,";
                    int length = strlen(pstrHeader);
                    for (int i = 0; i < length; i++)
                    {
                        *pCurrent = pstrHeader[i];
                        pCurrent++;
                    }
                    result = laya::convertBitmapToPng((const char*)pPixels, w, h, 8);
                }

                base64Encode(pCurrent, (const char*)result.first, result.second);
                delete[] result.first;
                delete[] pPixels;
                callback(pDest);
            });
        break;
        case CANVAS_TYPE_MEMORY:
            LOGE("JCHtml5Context::toBase64 unsupported canvas type");
            break;
        default:
            LOGE("JCHtml5Context::toBase64 unsupported canvas type");
            break;
        }
    }
    void JCHtml5Context::getPixels(int x, int y, int w, int h, std::function<void (unsigned char *pPixels, int size, int w, int h)> callback)
    {
        switch (m_nCanvasType)
        {
        case CANVAS_TYPE_MAIN:
            m_vOnRenderEndFuncs.push_back([x, y, w, h, callback]()
            {
                int size = sizeof(GLubyte) * w * h * 4;
                unsigned char* pTempPixels = new unsigned char[size];
                unsigned char* pPixels = new unsigned char[size];
                glFinish();
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pTempPixels);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                for (int row = 0; row < h; ++row)
                {
                    memcpy(pPixels + (h - row - 1) * w * 4, pTempPixels + row * w * 4, w * 4);
                }
                delete[] pTempPixels;
                callback(pPixels, size, w, h);
            });
            break;
        case CANVAS_TYPE_RENDER_TARGET:
            m_vOnRenderEndFuncs.push_back([x, y, w, h, callback, this]()
            {
                int size = sizeof(GLubyte) * w * h * 4;
                unsigned char* pTempPixels = new unsigned char[size];
                unsigned char* pPixels = new unsigned char[size];
                m_pRenderTarget->getPixels(pTempPixels, x, y, w, h);
                for (int row = 0; row < h; ++row)
                {
                    memcpy(pPixels + (h - row - 1) * w * 4, pTempPixels + row * w * 4, w * 4);
                }
                delete[] pTempPixels;
                callback(pPixels, size, w, h);
            });
            break;
        case CANVAS_TYPE_MEMORY:
            LOGE("JCHtml5Context::getPixels unsupported canvas type");
            break;
        default:
            LOGE("JCHtml5Context::getPixels unsupported canvas type");
            break;
        }
    }
    void JCHtml5Context::setMemoryCanvasClip(bool bClip)
    {
        if (m_nCanvasType == CANVAS_TYPE_MEMORY)
        {
            m_bMemoryCanvasScissor = bClip;
        }
    }
    void JCHtml5Context::invalidGLRes()
    {
        if (m_pDynamicMesh)
        {
            m_pDynamicMesh->freeGLResource();
        }
        m_bRepaint = true;
    }

    void JCHtml5Context::cleareAllPath() {
        for (auto p : m_vAllPath) {
            vg->ClearPath(p, 0);
            vg->DestroyPath(p);
        }
        m_vAllPath.clear();
        m_vAllPath.reserve(20);
        auto p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        m_vAllPath.push_back(p);
        m_vgCurPath = p;
    }

    void JCHtml5Context::onVGData(VG_DATA_TYPE type, void* pData, int len) {
        switch (type){
        case laya::JCHtml5Context::stokedata:{
            //set color
            JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
            pMaterial->setShaderProgram( m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR));
            pMaterial->setFillColorShaderParam(m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
            int nVertNum = len / sizeof(RectGeometry::VertexInfo);
            m_pDynamicMesh->pushVertex(JCConchMesh2D::XYUVRGBA, GL_TRIANGLE_STRIP, pMaterial,
                nVertNum,(void*)pData,len );
            break;
        }
        case laya::JCHtml5Context::fillcolor:{
            //set color
            JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
            pMaterial->setShaderProgram( m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR));
            pMaterial->setFillColorShaderParam(m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
            int nVertNum = len / sizeof(RectGeometry::VertexInfo);
            m_pDynamicMesh->pushVertex(JCConchMesh2D::XYUVRGBA, GL_TRIANGLES, pMaterial,
                nVertNum, (void*)pData, len);
        }
            break;
        case laya::JCHtml5Context::fillimg:
            break;
        default:
            break;
        }
    }
    void JCHtml5Context::renderToGpu()
    {
        switch (m_nCanvasType)
        {
        case CANVAS_TYPE_MAIN: 
        {
            openGLDisableScissor();
            m_bScissorEnabled = false;
            openGLDisableStencil();
            m_bStencilEnabled = false;
            int nVH = m_nHeight*m_fScaleY;
            glViewport(m_fTx, g_nInnerHeight-m_fTy-nVH, m_nWidth*m_fScaleX,nVH);
            m_pDynamicMesh->render((char*)&(m_ShaderData));
        }
            break;
        case CANVAS_TYPE_RENDER_TARGET:
            {
                m_pRenderTarget->beginSampler();
                //运行所有执行
                m_pSaveCmd->setReadPos(0);
                while (true)
                {
                    char* pData = m_pSaveCmd->getReadPtr();
                    if (pData)
                    {
                        m_pHtml5Render->runRenderCmd(*m_pSaveCmd);
                    }
                    else
                    {
                        break;
                    }
                }
                m_pDynamicMesh->render((char*)&(m_ShaderData));
                m_pRenderTarget->endSampler();
                break;
            }
        }
        m_pMaterialManager->reset(); 
        
        std::vector<std::function<void()> >::iterator it = m_vOnRenderEndFuncs.begin();
        for (; it != m_vOnRenderEndFuncs.end(); it++)
        {
            (*it)();
        }
        m_vOnRenderEndFuncs.clear();
    }

    void JCHtml5Context::onRenderEnd() {
        cleareAllPath();     
    }

    bool JCHtml5Context::createRenderTarget()
    {
        if (m_pRenderTarget == NULL && m_nCanvasType == CANVAS_TYPE_RENDER_TARGET )
        {
            m_pRenderTarget = new JCRenderTarget(m_pTextureManager, m_nWidth, m_nHeight);
        }
        return true;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::clearRect(float p_nX, float p_nY, float p_nW, float p_nH )
    {
        if (m_nCanvasType != CANVAS_TYPE_MEMORY)
        {
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::fillRect(float x, float y, float w, float h )
    {
        switch (m_pContextData->m_pCurrentContextData->nFillType)
        {
        case FILL_TYPE_COLOR:
        {
            int nColor = m_pContextData->m_pCurrentContextData->nFillStyle;
            float b = (nColor & 255) / 255.0f;
            float g = ((nColor >> 8) & 255) / 255.0f;
            float r = ((nColor >> 16) & 255) / 255.0f;
            float a = ((nColor >> 24) & 255) / 255.0f;
            JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
            pMaterial->setShaderProgram( m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR));
            pMaterial->setFillColorShaderParam(m_pContextData->m_pCurrentContextData->nAlpha*a, m_pContextData->m_pCurrentContextData->nBlendType);
            m_pGeo->createForColor(x, y, w, h, r, g, b);
            applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
            if (m_pGeo->m_bShow) {
                m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                    4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                    rectIndex, 12);
            }
            break;
        }
        case FILL_TYPE_PATTERN:
        {
           int nImgId = m_pContextData->m_pCurrentContextData->nFillStyle;
           JCImage* pImage = m_pImageManager->getImage(nImgId);
           if (pImage == NULL)
           {
               LOGE("JCHtml5Context::fillRect 图片没有下载成功就开始使用");
               return;
           }
           JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
           if( pImage->enable() == ENALBE_TYPE_FAIL )return;
           JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
           bool bRepeatX = false;
           bool bRepeatY = false;
           Rectangle* pRect = pDisplayRes->getRect();
           switch (m_pContextData->m_pCurrentContextData->nPatternType)
           {
           case FILL_PATTERN_TYPE_REPEAT:
               bRepeatX = bRepeatY = true;
               break;
           case FILL_PATTERN_TYPE_REPEAT_X:
               bRepeatX = true;
               break;
           case FILL_PATTERN_TYPE_REPEAT_Y:
               bRepeatY = true;
               break;
           case FILL_PATTERN_TYPE_NO_REPEAT:
               break;
           }
           
           Rectangle* pPatternRect = &m_pContextData->m_pCurrentContextData->kPattern;
           float dw = bRepeatX ? w : pPatternRect->width;
           float dh = bRepeatY ? h : pPatternRect->height;
           if (pPatternRect->width > w)dw = w;
           if (pPatternRect->height > h)dh = h;

           JCTexture* pTexTure = pDisplayRes->getTexture();
           //x y就是offset偏移
           float tx1 = ( pPatternRect->x + pRect->x+x) / pTexTure->m_nTextureWidth;
           float ty1 = ( pPatternRect->y + pRect->y+y) / pTexTure->m_nTextureHeight;
           float tx2 = tx1 + dw / pTexTure->m_nTextureWidth;
           float ty2 = ty1 + dh / pTexTure->m_nTextureHeight;

           float nParam1 = (pPatternRect->x + pRect->x) / pTexTure->m_nTextureWidth;
           float nParam2 = pPatternRect->width / pTexTure->m_nTextureWidth;
           float nParam3 = (pPatternRect->y + pRect->y) / pTexTure->m_nTextureHeight;
           float nParam4 = pPatternRect->height / pTexTure->m_nTextureHeight;

           switch (m_pContextData->m_pCurrentContextData->nFilterType)
           {
           case FILTER_TYPE_NO:
               xFillTexture(pDisplayRes, x, y, dw, dh, tx1, ty1, tx2, ty2, nParam1, nParam2, nParam3, nParam4, m_pGeo, m_pMaterialManager->getMaterial());
               break;
           /*case FILTER_TYPE_COLOR:
               xDrawFilterTexture(pDisplayRes, x, y, dw, dh, tx1, ty1, tx2, ty2, m_pContextData->m_pCurrentContextData->kFilter, m_pGeo, m_pMaterialManager->getMaterial());
               break;*/
           case FILTER_TYPE_MATRIX:
               xFillFilterMatrixTexture(pDisplayRes, x, y, dw, dh, tx1, ty1, tx2, ty2, nParam1, nParam2, nParam3, nParam4, m_pContextData->m_pCurrentContextData->kFilterMatrix, m_pGeo, m_pMaterialManager->getMaterial());
               break;
           default:
               xFillTexture(pDisplayRes, x, y, dw, dh, tx1, ty1, tx2, ty2, nParam1, nParam2, nParam3, nParam4, m_pGeo, m_pMaterialManager->getMaterial());
               break;
           }
           break;
        }
        default:
            LOGW("JCHtml5Context::fillRect can't support fillStyle =%d", m_pContextData->m_pCurrentContextData->nFillType );
            break;
        }
    }
    void JCHtml5Context::xFillTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, float nParam1, float nParam2, float nParam3, float nParam4, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_FILLIMAGE));
        pMaterial->setFillImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType, nParam1, nParam2, nParam3, nParam4);
        m_pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        if (m_pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12, true);
        }
    }
    
    //------------------------------------------------------------------------------
    void JCHtml5Context::translate( float x,float y )
    {
        m_pContextData->m_pCurrentContextData->kMatrix.translate(x, y);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::save()
    {
        m_pContextData->save();
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::restore()
    {
        if (m_pContextData->m_pCurrentContextData->nClipType == MASK_TYPE_STENCIL)
        {
            if (m_nStencilMask > 0)
            {
                m_nStencilMask--;
            }
            if (m_nStencilMask == 0)
            {
                if (m_bStencilEnabled)
                {
                    m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLDisableStencil, this));
                }
                m_bStencilEnabled = false;
            }
            else
            {
                m_pDynamicMesh->pushCmd([]() {
                    glColorMask(false, false, false, false);
                    //glStencilFunc(GL_ALWAYS, 0, 0xff);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
                });

                //2、创建Rect顶点并绘制Rect
                int nColor = m_pContextData->m_pCurrentContextData->nFillStyle;
                float b = (nColor & 255) / 255.0f;
                float g = ((nColor >> 8) & 255) / 255.0f;
                float r = ((nColor >> 16) & 255) / 255.0f;
                float a = ((nColor >> 24) & 255) / 255.0f;
                JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
                pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR));
                pMaterial->setFillColorShaderParam(m_pContextData->m_pCurrentContextData->nAlpha*a, m_pContextData->m_pCurrentContextData->nBlendType);
                m_pGeo->createForColor(m_pContextData->m_pCurrentContextData->kRect.x,
                    m_pContextData->m_pCurrentContextData->kRect.y,
                    m_pContextData->m_pCurrentContextData->kRect.width,
                    m_pContextData->m_pCurrentContextData->kRect.height, r, g, b);
                applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
                if (m_pGeo->m_bShow) {
                   m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                        4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                        rectIndex, 12, true/*不能合并*/);
                }
                size_t mask = m_nStencilMask;
                assert(mask > 0);
                m_pDynamicMesh->pushCmd([mask]() {
                    glColorMask(true, true, true, true);
                    glStencilFunc(GL_EQUAL, mask, 0xff);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                });
            }
        }
        if (m_pContextData->m_pCurrentContextData->nClipType == MASK_TYPE_SCISSOR)
        {
            m_nScissorCount--;
        }
        m_pContextData->restore();//恢复

        if (m_pContextData->m_pCurrentContextData->nClipType == MASK_TYPE_SCISSOR)
        {
            assert(m_nScissorCount > 0);
            Rectangle& crect = m_pContextData->m_pCurrentContextData->kClipRect;
            if (!m_bScissorEnabled)
            {
                m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLEnableScissor, this));
            }
            m_bScissorEnabled = true;
            m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLScissor, this, crect.x, crect.y, crect.width, crect.height));
        }
        if (m_nScissorCount == 0)
        {
            assert(m_pContextData->m_pCurrentContextData->nClipType != MASK_TYPE_SCISSOR);
            if (m_bScissorEnabled) 
            {
                 m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLDisableScissor, this));
            }
            m_bScissorEnabled = false;
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setGlobalAlpha(float p_fAlpha)
    {
        m_pContextData->m_pCurrentContextData->nAlpha = p_fAlpha;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setFillStyle(int p_nJsObject )
    {
        m_pContextData->m_pCurrentContextData->nFillType = FILL_TYPE_COLOR;
        m_pContextData->m_pCurrentContextData->nFillStyle = p_nJsObject;
        unsigned char* fc = (unsigned char*)&p_nJsObject;
        VGfloat colfill[4];
        colfill[0] = fc[2]/255.0f;
        colfill[1] = fc[1] / 255.0f;
        colfill[2] = fc[0] / 255.0f;
        colfill[3] = fc[3] / 255.0f;
        vg->SetParameterfv(m_vgPaint, VG_PAINT_COLOR, 4, colfill);
    }
    void JCHtml5Context::setFillStyle(int p_nImageId, FILL_PATTERN_TYPE p_ePaternType, float p_nX, float p_nY, float p_nW, float p_nH)
    {
        m_pContextData->m_pCurrentContextData->nFillType = FILL_TYPE_PATTERN;
        m_pContextData->m_pCurrentContextData->nFillStyle = p_nImageId;
        m_pContextData->m_pCurrentContextData->nPatternType = p_ePaternType;
        m_pContextData->m_pCurrentContextData->kPattern.x = p_nX;
        m_pContextData->m_pCurrentContextData->kPattern.y = p_nY;
        m_pContextData->m_pCurrentContextData->kPattern.width = p_nW;
        m_pContextData->m_pCurrentContextData->kPattern.height = p_nH;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setStrokeStyle(int p_nJsObject )
    {
        m_pContextData->m_pCurrentContextData->nStorkeStyle = p_nJsObject;
        unsigned char* sc = (unsigned char*)&p_nJsObject;
        VGfloat col[4];
        col[0] = sc[2] / 255.0f;
        col[1] = sc[1] / 255.0f;
        col[2] = sc[0] / 255.0f;
        col[3] = sc[3] / 255.0f;
        vg->SetParameterfv(m_vgStrokePaint, VG_PAINT_COLOR, 4, col);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::beginPath()
    {
        m_fVecLastX = 0.0f;
        m_fVecLastY = 0.0f;
        m_fVecLastNoScaleX = m_fVecLastNoScaleY = 0.0f;
        m_bVecHasLastPos = false;
        cleareAllPath();
		m_pContextData->m_pCurrentContextData->nClip = 0;
        /*
        auto p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        m_vAllPath.push_back(p);
        m_vgCurPath = p;
        */
    } 
    //------------------------------------------------------------------------------
    void JCHtml5Context::rect(float x, float y, float w, float h )
    {
        m_pContextData->m_pCurrentContextData->kRect.setTo(x, y, w, h);

        int cmdNum = vg->GetParameteri(m_vgCurPath, VG_PATH_NUM_SEGMENTS);
        if (cmdNum > 0) {
            VGPath p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
            m_vAllPath.push_back(p);
            m_vgCurPath = p;
        }
        float tx, ty;
        float ex, ey;
        float v1x, v1y;
        float v2x, v2y;
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(x, y, tx, ty);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(x + w, y, v1x, v1y);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(x + w, y + h, ex, ey);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(x, y + h, v2x, v2y);
        VGubyte cmds[] = { VG_MOVE_TO_ABS,VG_LINE_TO_ABS,VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH };
        VGfloat data[] = {
            tx, ty,
            v1x,v1y,
            ex,ey,
            v2x,v2y
        };
        vgAppendPathData(m_vgCurPath, 5, cmds, data);
        //下面要开始一个新的path
        auto p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        m_vAllPath.push_back(p);
        m_vgCurPath = p;
        m_bVecHasLastPos = false;
        m_fVecLastX = m_fVecLastY = 0.0f;
        m_fVecLastNoScaleX = m_fVecLastNoScaleY = 0.0f;
    }
    //现在这个clip简单的符合了html5的规则，但是太复杂的，beginPath rect rect 取交集 并集等等太JB难了 还是先算了吧
    void JCHtml5Context::clip()
    {
        if (m_pContextData->m_pCurrentContextData->nClip == 1)
            return;

        m_pContextData->m_pCurrentContextData->nClip = 1;

        float* pM = m_pContextData->m_pCurrentContextData->kMatrix.m;
        float rot = atan2(pM[0], pM[1]) * (180 / PI);
        const float EPSINON = 0.1;
        int irot = round(rot);
        bool bScissor = (irot % 90 == 0) && (abs(irot - rot) < EPSINON);
        if (!bScissor)
        {
            m_pContextData->m_pCurrentContextData->nClipType = MASK_TYPE_STENCIL;
            //1、创建模板缓冲区
            if (!m_bStencilEnabled)
            {
                m_pDynamicMesh->pushCmd([]() {
                    glEnable(GL_STENCIL_TEST);
                    glClear(GL_STENCIL_BUFFER_BIT);
                });
            }
            m_bStencilEnabled = true;

            m_pDynamicMesh->pushCmd([]() {
                glColorMask(false, false, false, false);
                //glStencilFunc(GL_ALWAYS, 1, 1);
                glStencilFunc(GL_ALWAYS, 0, 0xff);
                glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
            });

            //2、创建Rect顶点并绘制Rect
             int nColor = m_pContextData->m_pCurrentContextData->nFillStyle;
             float b = (nColor & 255) / 255.0f;
             float g = ((nColor >> 8) & 255) / 255.0f;
             float r = ((nColor >> 16) & 255) / 255.0f;
             float a = ((nColor >> 24) & 255) / 255.0f;
             JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
             pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR));
             pMaterial->setFillColorShaderParam(m_pContextData->m_pCurrentContextData->nAlpha*a, m_pContextData->m_pCurrentContextData->nBlendType);
             m_pGeo->createForColor(m_pContextData->m_pCurrentContextData->kRect.x, 
                                    m_pContextData->m_pCurrentContextData->kRect.y, 
                                    m_pContextData->m_pCurrentContextData->kRect.width,
                                    m_pContextData->m_pCurrentContextData->kRect.height, r, g, b);
             applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
             if (m_pGeo->m_bShow) {
                 m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                     4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                     rectIndex, 12, true/*不能合并*/);
             }

            //3、计算clipRect
             float minx = min(min(min(m_pGeo->m_pVertexRect[0].x, m_pGeo->m_pVertexRect[1].x), m_pGeo->m_pVertexRect[2].x), m_pGeo->m_pVertexRect[3].x);
             float maxx = max(max(max(m_pGeo->m_pVertexRect[0].x, m_pGeo->m_pVertexRect[1].x), m_pGeo->m_pVertexRect[2].x), m_pGeo->m_pVertexRect[3].x);
             float miny = min(min(min(m_pGeo->m_pVertexRect[0].y, m_pGeo->m_pVertexRect[1].y), m_pGeo->m_pVertexRect[2].y), m_pGeo->m_pVertexRect[3].y);
             float maxy = max(max(max(m_pGeo->m_pVertexRect[0].y, m_pGeo->m_pVertexRect[1].y), m_pGeo->m_pVertexRect[2].y), m_pGeo->m_pVertexRect[3].y);

             m_pContextData->m_pCurrentContextData->kClipRect.x = minx;
             m_pContextData->m_pCurrentContextData->kClipRect.y = miny;
             m_pContextData->m_pCurrentContextData->kClipRect.width = maxx - minx;
             m_pContextData->m_pCurrentContextData->kClipRect.height = maxy - miny;
            //4、设置模板缓冲区
             size_t mask = ++m_nStencilMask;
             m_pDynamicMesh->pushCmd([mask]() {
                 glColorMask(true, true, true, true);
                 glStencilFunc(GL_EQUAL, mask, 0xff);
                 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
             });
        }
        else
        {
            m_nScissorCount++;
            m_pContextData->m_pCurrentContextData->nClipType = MASK_TYPE_SCISSOR;
            m_pContextData->m_pCurrentContextData->kRect.applyMatrix(&m_pContextData->m_pCurrentContextData->kMatrix);

			Rectangle& before = m_pContextData->m_pCurrentContextData->kClipRect;
			Rectangle& now = m_pContextData->m_pCurrentContextData->kRect;

			float minr = std::min(now.x + now.width, before.getRight());
			float minb = std::min(now.y + now.height, before.getBottom()); 
			float maxx = std::max(now.x, before.x);
			float maxy = std::max(now.y, before.y);
			//return new Rectangle(maxx, maxy, minr - maxx, minb - maxy);
			before.setTo(maxx, maxy, minr - maxx, minb - maxy);
            //m_pContextData->m_pCurrentContextData->kClipRect = m_pContextData->m_pCurrentContextData->kRect;
            //Rectangle& crect = m_pContextData->m_pCurrentContextData->kClipRect;
            if (!m_bScissorEnabled)
            {
                m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLEnableScissor, this));
            }
            m_bScissorEnabled = true;
            m_pDynamicMesh->pushCmd(std::bind(&JCHtml5Context::openGLScissor, this, before.x, before.y, before.width, before.height));
            m_bScissorEnabled = true;
        }
    }
    void JCHtml5Context::strokeRect(float p_nX, float p_nY, float p_nWidth, float p_nHeight )
    {
        float tx, ty;
        float ex, ey;
        float v1x, v1y;
        float v2x, v2y;
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX, p_nY, tx, ty);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX+p_nWidth, p_nY, v1x, v1y);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX + p_nWidth, p_nY + p_nHeight, ex, ey);
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX , p_nY + p_nHeight, v2x, v2y);

        auto p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        VGubyte cmds[] = { VG_MOVE_TO_ABS,VG_LINE_TO_ABS,VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
        VGfloat data[] = { 
            tx, ty,
            v1x,v1y,
            ex, ey,
            v2x,v2y
        };
        vg->AppendPathData(p, 5, cmds, data);
        _stroke(p);
        vg->ClearPath(p, 0);
        vg->DestroyPath(p);
        //下面要开始一个新的path
        auto np = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
        m_vAllPath.push_back(np);
        m_vgCurPath = np;
        m_bVecHasLastPos = false;
        m_fVecLastX = m_fVecLastY = 0.0f;
        m_fVecLastNoScaleX = m_fVecLastNoScaleY = 0.0f;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::fill()
    {
        vg->Seti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        VGfloat mat[] = { 1.0f,0.0f,0.0f, 0.0f,-1.0f,m_ShaderData.m_fHeight, 0.0f,0.0f,1.0f };
        vg->LoadMatrix(mat);
        for (auto p : m_vAllPath) {
            vg->DrawPath(p, VG_FILL_PATH);
        }
    }
    //------------------------------------------------------------------------------

    void JCHtml5Context::_stroke(VGPath p) {
        vg->Seti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        /*
        要把gl矩阵与{ 1,0,0,-1,0,h};相乘，然后转成vg使用的。
        vg使用的是gl的转置
        */
        VGfloat mat[] = { 1,0,0, 0,-1.0f,m_ShaderData.m_fHeight, 0,0,1 };
        vg->LoadMatrix(mat);
        vg->DrawPath(p, VG_STROKE_PATH);
    }

    void JCHtml5Context::stroke()
    {
        vg->Seti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        /*
            要把gl矩阵与{ 1,0,0,-1,0,h};相乘，然后转成vg使用的。
            vg使用的是gl的转置
        */
        VGfloat mat[] = { 1.0f,0.0f,0.0f, 0.0f,-1.0f,m_ShaderData.m_fHeight, 0.0f,0.0f,1.0f };
        vg->LoadMatrix(mat);
        for (auto p : m_vAllPath) {
            vg->DrawPath(p, VG_STROKE_PATH);
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::moveTo(float p_nX, float p_nY ){
        int cmdNum = vg->GetParameteri(m_vgCurPath,VG_PATH_NUM_SEGMENTS);
        if (cmdNum > 0) {
            VGPath p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
            m_vAllPath.push_back(p);
            m_vgCurPath = p;
        }
        //monkVG在append data的时候并没有应用矩阵，所以自己做。
        float tx, ty;
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX,p_nY,tx,ty);
        m_fVecLastX = tx;
        m_fVecLastY = ty;
        m_fVecLastNoScaleX = p_nX;
        m_fVecLastNoScaleY = p_nY;
        m_bVecHasLastPos = true;

        VGubyte cmds[] = { VG_MOVE_TO | VG_ABSOLUTE };
        //VGfloat scale = vgGetParameterf( m_vgPath, VG_PATH_SCALE);
        //VGfloat bias = vgGetParameterf(m_vgPath, VG_PATH_BIAS);
        //VGfloat data[] = {(p_nX-bias)/scale,(p_nY-bias)/scale};
        VGfloat data[] = {tx,ty};
        vg->AppendPathData(m_vgCurPath, 1,cmds, data );
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::closePath()
    {
        int cmdNum = vg->GetParameteri(m_vgCurPath, VG_PATH_NUM_SEGMENTS);
        if (cmdNum > 0) {
            VGubyte cmds[] = { VG_CLOSE_PATH };
            vg->AppendPathData(m_vgCurPath, 1, cmds, nullptr);
            VGPath p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
            m_vAllPath.push_back(p);
            m_vgCurPath = p;
        }
        m_bVecHasLastPos = false;
        m_fVecLastX = m_fVecLastY = 0.0f;
        m_fVecLastNoScaleX = m_fVecLastNoScaleY = 0.0f;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::lineTo( float p_nX, float p_nY ){

        //monkVG在append data的时候并没有应用矩阵，所以自己做。
        float tx, ty;
        m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX, p_nY, tx, ty);
        m_fVecLastX = tx;
        m_fVecLastY = ty;
        m_fVecLastNoScaleX = p_nX;
        m_fVecLastNoScaleY = p_nY;
        m_bVecHasLastPos = true;
        VGubyte cmds[] = { VG_LINE_TO | VG_ABSOLUTE };
        //VGfloat scale = vgGetParameterf(m_vgPath, VG_PATH_SCALE);
        //VGfloat bias = vgGetParameterf(m_vgPath, VG_PATH_BIAS);
        //VGfloat data[] = { (p_nX - bias) / scale,(p_nY - bias) / scale };
        VGfloat data[] = { tx, ty};
        vg->AppendPathData(m_vgCurPath, 1, cmds, data);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::quadraticCurveTo(float p_nCPX, float p_nCPY, float p_nEndX, float p_nEndY )
    {
        Matrix32& glmat = m_pContextData->m_pCurrentContextData->kMatrix;
        float cpx, cpy, endx, endy;
        glmat.transVert(p_nCPX, p_nCPY, cpx, cpy);
        glmat.transVert(p_nEndX, p_nEndY, endx, endy);
        m_bVecHasLastPos = true;
        m_fVecLastX = endx;
        m_fVecLastY = endy;
        m_fVecLastNoScaleX = p_nEndX;
        m_fVecLastNoScaleY = p_nEndY;
        VGubyte cmds[] = { VG_QUAD_TO_ABS };
        VGfloat data[] = {cpx,cpy,endx,endy};
        vg->AppendPathData(m_vgCurPath, 1, cmds, data);   
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::bezierCurveTo(float p_nCPX, float p_nCPY, float p_nCPX2, float p_nCPY2, float p_nEndX, float p_nEndY )
    {
        Matrix32& glmat = m_pContextData->m_pCurrentContextData->kMatrix;
        float cpx, cpy, cpx2,cpy2, endx, endy;
        glmat.transVert(p_nCPX, p_nCPY, cpx, cpy);
        glmat.transVert(p_nCPX2, p_nCPY2, cpx2, cpy2);
        glmat.transVert(p_nEndX, p_nEndY, endx, endy);
        m_bVecHasLastPos = true;
        m_fVecLastX = endx;
        m_fVecLastY = endy;
        m_fVecLastNoScaleX = p_nEndX;
        m_fVecLastNoScaleY = p_nEndY;
        //TODO 
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::arc( float p_nX, float p_nY, float p_nR,float p_fSAngle,float p_fEAngle,bool p_bCounterclockwise )
    {
        //arc比较特殊,如果前面没有指令，就开一个新的path，否则会与前面的连续起来。
        int cmdNum = vg->GetParameteri(m_vgCurPath, VG_PATH_NUM_SEGMENTS);
        if (cmdNum <= 0) {
            VGPath p = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
            m_vAllPath.push_back(p);
            m_vgCurPath = p;
        }
        const static int segnum = 32;

        Matrix32& glmat = m_pContextData->m_pCurrentContextData->kMatrix;
        bool bIsIMat = glmat.m[0] == 1.0f && glmat.m[1] == 0 && glmat.m[2] == 0 && glmat.m[3] == 1.0f;

        if (!bIsIMat) {
            //缩放已经在顶点部分做了
            //p_nR *= fabsf(glmat.m[0]);  //简化的缩放
        }
        //单位是弧度
        float x0 = p_nR*cosf(p_fSAngle);
        float y0 = p_nR*sinf(p_fSAngle);
        float sx = p_nX + x0;
        float sy = p_nY + y0;
        //由于openvg没有很合适的高效的方式处理这种情况，所以自己生成一下。
        //最合适的是VG_XCCW_ARC_TO 但是这个会按照椭圆来计算中心之类的，计算量大，而这里已经知道中心了
        VGubyte cmds[] = { (VGubyte)(cmdNum>0?VG_LINE_TO_ABS:VG_MOVE_TO_ABS), (VGubyte)VG_LINE_DATA };
        VGfloat data[2+ segnum * 2+1];//起点，段数，段数据
        int dataNum = 2 + segnum * 2 + 1;
        //起点
        VGfloat tcx, tcy;
        if (bIsIMat) {
            tcx = sx+glmat.m[4];
            tcy = sy+glmat.m[5];
        }
        else {
            glmat.transVert(sx, sy, tcx, tcy);
        }
        //起点
        data[0] = tcx;
        data[1] = tcy;
        float fStep = 0;
        float fDAng = p_fEAngle - p_fSAngle;
        //如果超了就是一个圆
        if (fDAng >= 1.99f * PI || fDAng <= -1.99f * PI) {
            fStep = PI*2.0f/segnum;
        }
        else {
            if (p_bCounterclockwise) {
                if (fDAng > 0.0) {
                    fDAng -= PI * 2;
                }
                fStep = fDAng / segnum;
            }
            else {
                if (fDAng < 0.0) {
                    fDAng += PI * 2;
                }
                fStep = fDAng / segnum;
            }
        }

        *(int*)(&data[2]) = segnum;//段数

        float cAng = p_fSAngle + fStep;
        for (int i = 0; i < segnum; i++) {
            float cx = p_nX + p_nR*cosf(cAng);
            float cy = p_nY + p_nR*sinf(cAng);
            if (i == segnum - 1) {
                m_fVecLastNoScaleX = cx;
                m_fVecLastNoScaleY = cy;
            }
            if (!bIsIMat) {
                glmat.transVert(cx, cy, tcx, tcy);
                data[i * 2 + 3] = tcx;
                data[i * 2 + 4] = tcy;
            }
            else {
                data[i * 2 + 3] = cx + glmat.m[4];
                data[i * 2 + 4] = cy + glmat.m[5];
            }
            cAng += fStep;
        }
        m_bVecHasLastPos = true;
        m_fVecLastX = data[dataNum - 2];
        m_fVecLastY = data[dataNum - 1];
        vg->AppendPathData(m_vgCurPath, 2, cmds, data);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::arcTo( float p_nX1, float p_nY1, float p_nX2, float p_nY2, float p_nR )
    {
        Matrix32& glmat = m_pContextData->m_pCurrentContextData->kMatrix;
        VGfloat mat[] = {
            glmat.m[0],glmat.m[2],glmat.m[4],
            glmat.m[1],glmat.m[3],glmat.m[5]
        };
        vg->LoadMatrix(mat);
        /*
            p0=当前点
            cp={p_nX1,p_nY1}
            p1={p_nX2,p_nY2}
            圆弧与p0->cp相切，与cp->p1相切。取圆弧的短的那一部分
            只要相切即可，不必经过p0也不必经过p1...

            估计算法是：
                计算两个切线。根据长度能得到圆心。
                计算起点和终点的角度
        */
        //转换到当前矩阵
        /* 所有的点先按照原始坐标计算，最后再缩放。
        float x1, y1;
        glmat.transVert(p_nX1, p_nY1, x1, y1);
        float x2, y2;
        glmat.transVert(p_nX2, p_nY2, x2, y2);
        //p_nR *= fabsf(glmat.m[0]);  //简化的缩放
        */
        float x1 = p_nX1, y1 = p_nY1;
        float x2 = p_nX2, y2 = p_nY2;

        //计算几个方向  
        //float dx = m_fVecLastX - x1;    //从cp出发的第一个切线方向，从cp指向切点。
        //float dy = m_fVecLastY - y1;
        float dx = m_fVecLastNoScaleX - x1;
        float dy = m_fVecLastNoScaleY - y1;
        //从当前点到控制点的距离，当前点不一定与圆相切。
        float len1 = sqrtf(dx*dx + dy*dy);
        if (len1 <= 1e-6f) {
            LOGE("JCHtml5Context::arcTo error1 cp=start");
            return;
        }
        //规格化p0->cp
        float ndx = dx / len1;
        float ndy = dy / len1;
        //end 
        float dx2 = x2 - x1;
        float dy2 = y2 - y1;
        float len22 = dx2*dx2 + dy2*dy2;
        float len2 = sqrtf(len22);
        if (len2 <= 1e-6f) {
            LOGE("JCHtml5Context::arcTo error2 ed=cp");
            return;
        }
        //规格化cp->p1
        float ndx2 = dx2 / len2;
        float ndy2 = dy2 / len2;
        //圆心的方向。
        float odx = ndx + ndx2;
        float ody = ndy + ndy2;
        float olen = sqrtf(odx*odx +ody*ody);
        if (olen <= 1e-6f) {
            LOGE("JCHtml5Context::arcTo error2");
            return;
        }
        //规格化 cp->o
        float nOdx = odx / olen;
        float nOdy = ody / olen;
        //两个切线的夹角的一半
        float alpha = acosf(nOdx*ndx + nOdy*ndy);
        float halfAng = PI / 2 - alpha; //两个切点与圆心构成的夹角的一半。
        //R/len1 = tg(halfAng)
        //第一个交点
        len1 = p_nR / tanf(halfAng);//cp到交点的实际长度
        float ptx1 = len1*ndx + x1;
        float pty1 = len1*ndy + y1;
        //计算圆心
        float orilen = sqrtf(len1*len1 + p_nR*p_nR);
        float orix = x1 + nOdx*orilen;
        float oriy = y1 + nOdy*orilen;
        //另一个点
        float ptx2 = len1*ndx2 + x1;
        float pty2 = len1*ndy2 + y1;
        //确定从起点到终点的朝向（顺时针还是逆时针）
        float dir = ndx*ndy2 - ndy*ndx2;//两个切线的拐角，如果dir>0 就是逆时针（数学坐标系），
                                        //则两个半径就是顺时针

        //为了快一点采用固定旋转的方法。这样会累积误差，不过应该影响不大
        /*
            顺时针的旋转为
                | cos, -sin |
                | sin, cos  |
        */
        static const int SEGNUM = 32;

        float fChgAng = 0;
        float sinx = 0.0f, cosx = 0.0f;
        if (dir >= 0) {
            fChgAng = halfAng * 2;
            float fda = fChgAng / SEGNUM;
            sinx = sinf(fda);
            cosx = cosf(fda);
        }
        else {//切线变化是顺时针，则半径变化是逆时针，因为要用顺时针旋转，所以角度取负
            fChgAng = -halfAng * 2;
            float fda = fChgAng / SEGNUM;
            sinx = sinf(fda);
            cosx = cosf(fda);
        }
        float cvx = ptx1 - orix;
        float cvy = pty1 - oriy;
        VGubyte cmds[] = { VG_LINE_TO_ABS, VG_LINE_DATA };
        int dataNum = 2 + 1 + SEGNUM * 2;
        float data[2 + 1 + SEGNUM * 2];
        float tx = 0.f, ty = 0.f;
        glmat.transVert(ptx1, pty1, tx, ty);
        data[0] = tx;
        data[1] = ty;
        data[2] = *(float*)&SEGNUM;
        float* pDt = &data[3];
        for (int i = 0; i < SEGNUM; i++) {
            float cx = cvx*cosx + cvy*sinx;
            float cy = -cvx*sinx + cvy*cosx;
            if (i == SEGNUM - 1) {
                m_fVecLastNoScaleX = cx+orix;
                m_fVecLastNoScaleY = cy+oriy;
            }
            glmat.transVert(cx+orix, cy+oriy, tx, ty);
            pDt[i * 2] = tx;
            pDt[i * 2 + 1] = ty;
            cvx = cx;
            cvy = cy;
        }
        vg->AppendPathData(m_vgCurPath, 2, cmds, data);
        m_bVecHasLastPos = true;
        m_fVecLastX = data[dataNum - 2];
        m_fVecLastY = data[dataNum - 1];
        ///*
        //    数据：
        //    rh,rv,rot,cp1x,cp1y
        //    rot是指这个椭圆的旋转，如果是圆形的话，就是0
        //*/
        //float tcpx, tcpy, ttox, ttoy;
        //m_pContextData->m_pCurrentContextData->kMatrix.transVert(p_nX2, p_nY2, ttox, ttoy);

        //VGubyte cmds[] = {  VG_SCCWARC_TO_ABS };
        //VGfloat data[] = { p_nR,p_nR,0,ttox, ttoy };
        //vgAppendPathData(m_vgCurPath, 1, cmds, data);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::scale( float p_fX,float p_fY )
    {
        m_pContextData->m_pCurrentContextData->kMatrix.scale(p_fX, p_fY);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::rotate( float p_fAngle )
    {
        m_pContextData->m_pCurrentContextData->kMatrix.rotate(p_fAngle);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::transform( float a,float b,float c,float d,float e,float f )
    {
        m_pContextData->m_pCurrentContextData->kMatrix.transform(a, b, c, d, e, f);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setTransform( float a,float b,float c,float d,float e,float f )
    {
        m_pContextData->m_pCurrentContextData->kMatrix.setTransform(a, b, c, d, e, f);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::xDrawText(const char* p_sText, float x, float y, char* sFont, int nColor, TEXT_BASE_LINE_TYPE nBaseLineType, TEXT_ALIGN_TYPE nTextAlignType) 
    {
        JCFontInfo* pFont = m_pFontManager->getFontInfoFromText(sFont);
        std::string sText = p_sText;
        int nLen = sText.length();
        if (nLen < 1)return;
        std::vector<std::string> vText = paserUTF8(sText, nLen);
        m_vTempWordRes.reserve(128);
        int nWidthCount = 0;
        for (int i = 0, n = vText.size(); i < n; i++)
        {
            if (vText[i] == " ")
            {
                //空格
                int nWidth = (int)floorf((pFont->m_nFontSize + 1.0f) / 4);
                nWidthCount += nWidth;
                m_vTempWordRes.push_back(NULL);
            }
            else
            {
                float sx = m_pContextData->m_pCurrentContextData->kMatrix.getScaleX();
                float sy = m_pContextData->m_pCurrentContextData->kMatrix.getScaleY();
                JCWordRes* pWordRes = m_pTextManager->getWordRes(pFont, nColor, vText[i].c_str(), sx, sy);
                if (pWordRes != NULL)
                {
                    if( pWordRes->enable() == ENALBE_TYPE_FAIL )continue;
                    nWidthCount += pWordRes->m_kRectNoScaled.width;
                    m_vTempWordRes.push_back(pWordRes);
                }
            }
        }
        xDrawWordRes(m_vTempWordRes, nWidthCount, pFont->m_nFontSize,x,y, nBaseLineType, nTextAlignType );
        m_vTempWordRes.clear();
    }
    void JCHtml5Context::xDrawWordRes(std::vector<JCWordRes*>& vWordres, int nCountWidth,int nFontSize,int x, int y, TEXT_BASE_LINE_TYPE nBaseLineType, TEXT_ALIGN_TYPE nTextAlignType)
    {
        int nOffsetX = 0;
        switch (nTextAlignType)
        {
        case laya::TEXT_ALIGN_START:
        case laya::TEXT_ALIGN_LEFT:
            nOffsetX = 0;
            break;
        case laya::TEXT_ALIGN_CENTER:
            nOffsetX = -nCountWidth / 2;
            break;
        case laya::TEXT_ALIGN_END:
        case laya::TEXT_ALIGN_RIGHT:
            nOffsetX = -nCountWidth;
            break;
        default:
            break;
        }
        float nRate = 0;
        switch (nBaseLineType)
        {
        case laya::TBL_TYPE_ALPHABETIC:
            nRate = 0.85f;
            break;
        case laya::TBL_TYPE_TOP:
            nRate = 0.0f;
            break;
        case laya::TBL_TYPE_HANGING:
            nRate = 0.3f;
            break;
        case laya::TBL_TYPE_MIDDLE:
            nRate = 0.5f;
            break;
        case laya::TBL_TYPE_IDEOGRAPHIC:
            nRate = 0.9f;
            break;
        case laya::TBL_TYPE_BOTTOM:
            nRate = 1.0f;
            break;
        default:
            break;
        }
        int nLastPos = 0;
        for (int i = 0, n = vWordres.size(); i < n; i++)
        {
            JCWordRes* pWordRes = vWordres[i];
            if (pWordRes != NULL)
            {
                if (pWordRes->enable() == ENALBE_TYPE_FAIL)continue;;
                Rectangle* pRect = pWordRes->getRect();
                Rectangle* pRectNoScaled = pWordRes->getRectNoScaled();
                int nY = y - nRate * pRectNoScaled->height;
                JCTexture* pTexTure = pWordRes->getTexture();
                float tx1 = pRect->x / pTexTure->m_nTextureWidth;
                float ty1 = pRect->y / pTexTure->m_nTextureHeight;
                float tx2 = pRect->getRight() / pTexTure->m_nTextureWidth;
                float ty2 = pRect->getBottom() / pTexTure->m_nTextureHeight;

                switch (m_pContextData->m_pCurrentContextData->nFilterType)
                {
                case FILTER_TYPE_NO:
                    xDrawTexturePixelPerfect(pWordRes, x + nLastPos + nOffsetX, nY, pRectNoScaled->width, pRectNoScaled->height, tx1, ty1, tx2, ty2, m_pGeo, m_pMaterialManager->getMaterial());
                    break;
                case FILTER_TYPE_COLOR:
                    xDrawFilterTexturePixelPerfect(pWordRes, x + nLastPos + nOffsetX, nY, pRectNoScaled->width, pRectNoScaled->height, tx1, ty1, tx2, ty2, m_pContextData->m_pCurrentContextData->kFilter, m_pGeo, m_pMaterialManager->getMaterial());
                    break;
                case FILTER_TYPE_MATRIX:
                    xDrawFilterMatrixTexturePixelPerfect(pWordRes, x + nLastPos + nOffsetX, nY, pRectNoScaled->width, pRectNoScaled->height, tx1, ty1, tx2, ty2, m_pContextData->m_pCurrentContextData->kFilterMatrix, m_pGeo, m_pMaterialManager->getMaterial());
                    break;
                default:
                    xDrawTexturePixelPerfect(pWordRes, x + nLastPos + nOffsetX, nY, pRectNoScaled->width, pRectNoScaled->height, tx1, ty1, tx2, ty2, m_pGeo, m_pMaterialManager->getMaterial());
                    break;
                }

                nLastPos += pWordRes->m_kRectNoScaled.width;
            }
            else
            {
                nLastPos += (int)floorf((nFontSize + 1.0f) / 4);
            }
        }
    }
    void JCHtml5Context::flushToRenderTarget()
    {
        this->createRenderTarget();
        if (this->m_bRepaint || this->m_pRenderTarget->m_pGpuTexture <= 0)
        {
            this->renderToGpu();
            this->m_bRepaint = false;
        }
    }
    void JCHtml5Context::fillText( const char* p_sText,float x, float y )
    {
	    if(p_sText != NULL )
	    {
            ContextDataInfo * pCurrentData = m_pContextData->m_pCurrentContextData;
            xDrawText(p_sText, x, y, pCurrentData->sFont, pCurrentData->nFillStyle, (TEXT_BASE_LINE_TYPE)pCurrentData->nTextBaseLineType,(TEXT_ALIGN_TYPE)pCurrentData->nTextAlignType);
	    }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::fillText4( const char* p_sText, float x, float y, int nMaxWidth )
    {
        if (p_sText != NULL)
        {
            ContextDataInfo * pCurrentData = m_pContextData->m_pCurrentContextData;
            xDrawText(p_sText, x, y, pCurrentData->sFont, pCurrentData->nFillStyle, (TEXT_BASE_LINE_TYPE)pCurrentData->nTextBaseLineType, (TEXT_ALIGN_TYPE)pCurrentData->nTextAlignType);
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::strokeText( const char* p_sText,float x, float y )
    {
        if( p_sText != NULL )
        {
            ContextDataInfo * pCurrentData = m_pContextData->m_pCurrentContextData;
            xDrawText(p_sText, x, y, pCurrentData->sFont, pCurrentData->nStorkeStyle, (TEXT_BASE_LINE_TYPE)pCurrentData->nTextBaseLineType, (TEXT_ALIGN_TYPE)pCurrentData->nTextAlignType);
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::strokeText4( const char* p_sText, float x, float y, int p_nMaxWidth )
    {
        if (p_sText != NULL)
        {
            ContextDataInfo * pCurrentData = m_pContextData->m_pCurrentContextData;
            xDrawText(p_sText, x, y, pCurrentData->sFont, pCurrentData->nStorkeStyle, (TEXT_BASE_LINE_TYPE)pCurrentData->nTextBaseLineType, (TEXT_ALIGN_TYPE)pCurrentData->nTextAlignType);
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::drawImage(int p_nImgId,float p_nX, float p_nY)
    {
        JCImage* pImage = m_pImageManager->getImage(p_nImgId);
        if (pImage)
        {
            xDrawImage5(pImage, p_nX, p_nY, pImage->getWidth(), pImage->getHeight(), m_pGeo, m_pMaterialManager->getMaterial());
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::drawImage(int p_nImgId, float p_nX,float p_nY,float p_nWidth,float p_nHeight)
    {
        JCImage* pImage = m_pImageManager->getImage(p_nImgId);
        if (pImage)
        {
            xDrawImage5(pImage, p_nX, p_nY, p_nWidth, p_nHeight, m_pGeo, m_pMaterialManager->getMaterial());
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::drawImage(int p_nImgId,float p_nSX,float p_nSY,float p_nSWidth,float p_nSHeight,float p_nDX,float p_nDY, float p_nDWidth, float p_nDHeight )
    {
		JCImage* pImage = m_pImageManager->getImage(p_nImgId);
		if (pImage == NULL)
		{
			LOGE("JCHtml5Context::drawImage get image error,imgid:%d", p_nImgId);
			return;
		}
		if (p_nSWidth < 0)
		{
			p_nSWidth = -p_nSWidth;
			p_nSX -= p_nSWidth;
		}
		if (p_nSHeight < 0)
		{
			p_nSHeight = -p_nSHeight;
			p_nSY -= p_nSHeight;
		}
		if (p_nDWidth < 0)
		{
			p_nDWidth = -p_nDWidth;
		}
		if (p_nDHeight < 0)
		{
			p_nDHeight = -p_nDHeight;
		}
		if (pImage->enable() == ENALBE_TYPE_FAIL)return;
		JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
		Rectangle* pRect = pDisplayRes->getRect();
		int pw = pRect->width;
		int ph = pRect->height;
		//下面一段为了兼容 sx sy < 0  和 sw sh > image.size的情况
		//------------------------------------------------------------------------------
		if (p_nSX < 0) {
			p_nDX = p_nDX - p_nDWidth*(p_nSX / p_nSWidth);
			p_nDWidth = p_nDWidth*(1 + p_nSX / p_nSWidth);
			p_nSWidth += p_nSX;
			p_nSX = 0;
		}
		if (p_nSY < 0) {
			p_nDY = p_nDY - p_nDHeight*(p_nSY / p_nSHeight);
			p_nDHeight = p_nDHeight*(1 + p_nSY / p_nSHeight);
			p_nSHeight += p_nSY;
			p_nSY = 0;
		}
		if (p_nSWidth <= 0 || p_nSHeight <= 0)return;
		if (p_nSWidth > pw)p_nSWidth = pw;
		if (p_nSHeight > ph)p_nSHeight = ph;
		//------------------------------------------------------------------------------
		JCTexture * pTexture = pDisplayRes->getTexture();
		switch (m_pContextData->m_pCurrentContextData->nFilterType)
		{
		case FILTER_TYPE_NO:
			xDrawTexture(pDisplayRes, p_nDX, p_nDY, p_nDWidth, p_nDHeight, (p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSY + pRect->y) / pTexture->m_nTextureHeight, (p_nSWidth + p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSHeight + p_nSY + pRect->y) / pTexture->m_nTextureHeight, m_pGeo, m_pMaterialManager->getMaterial());
			break;
		case FILTER_TYPE_COLOR:
			xDrawFilterTexture(pDisplayRes, p_nDX, p_nDY, p_nDWidth, p_nDHeight, (p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSY + pRect->y) / pTexture->m_nTextureHeight, (p_nSWidth + p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSHeight + p_nSY + pRect->y) / pTexture->m_nTextureHeight, m_pContextData->m_pCurrentContextData->kFilter, m_pGeo, m_pMaterialManager->getMaterial());
			break;
		case FILTER_TYPE_MATRIX:
			xDrawFilterMatrixTexture(pDisplayRes, p_nDX, p_nDY, p_nDWidth, p_nDHeight, (p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSY + pRect->y) / pTexture->m_nTextureHeight, (p_nSWidth + p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSHeight + p_nSY + pRect->y) / pTexture->m_nTextureHeight, m_pContextData->m_pCurrentContextData->kFilterMatrix, m_pGeo, m_pMaterialManager->getMaterial());
			break;
		default:
			xDrawTexture(pDisplayRes, p_nDX, p_nDY, p_nDWidth, p_nDHeight, (p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSY + pRect->y) / pTexture->m_nTextureHeight, (p_nSWidth + p_nSX + pRect->x) / pTexture->m_nTextureWidth, (p_nSHeight + p_nSY + pRect->y) / pTexture->m_nTextureHeight, m_pGeo, m_pMaterialManager->getMaterial());
			break;
		}
    }
    void JCHtml5Context::drawCanvas(int nCanvasId, float x, float y)
    {
        JCHtml5Render* pSrcHtml5Render = m_pHtml5RenderManager->getHtml5Render(nCanvasId);
        if (pSrcHtml5Render == NULL) return;
        JCHtml5Context* pSrcContext = pSrcHtml5Render->m_pContext;

        switch (pSrcContext->m_nCanvasType)
        {
        case CANVAS_TYPE_RENDER_TARGET:
            xDrawRenderTarget(pSrcContext, 0, 0, pSrcContext->m_nWidth, pSrcContext->m_nHeight, x, y, pSrcContext->m_nWidth, pSrcContext->m_nHeight);
            break;
        case CANVAS_TYPE_MEMORY:
        {
            save();
            Rectangle kRect(x, y, pSrcContext->m_nWidth, pSrcContext->m_nHeight);
            kRect.applyMatrix(&(m_pContextData->m_pCurrentContextData->kMatrix));
            translate(x, y);
            xDrawMemoryCanvas(pSrcContext, kRect.x, kRect.y, kRect.width, kRect.height, m_pContextData->m_pCurrentContextData->nAlpha);
            restore();
            break;
        }
        default:
            LOGE("JCHtml5Context::drawCanvas3 src canvas type error");
            break;
        }
    }
    void JCHtml5Context::drawCanvas(int nCanvasId, float x, float y, float w, float h)
    {
        JCHtml5Render* pSrcHtml5Render = m_pHtml5RenderManager->getHtml5Render(nCanvasId);
        if (pSrcHtml5Render == NULL) return;
        JCHtml5Context* pSrcContext = pSrcHtml5Render->m_pContext;
        switch (pSrcContext->m_nCanvasType)
        {
        case CANVAS_TYPE_RENDER_TARGET:
            xDrawRenderTarget(pSrcContext, 0, 0, pSrcContext->m_nWidth, pSrcContext->m_nHeight, x, y, w, h);
            break;
        case CANVAS_TYPE_MEMORY:
        {
            save();
            Rectangle kRect(x, y, w, h);
            kRect.applyMatrix(&(m_pContextData->m_pCurrentContextData->kMatrix));
            translate(x, y);
            scale(w / pSrcContext->m_ShaderData.m_fWidth, h / pSrcContext->m_ShaderData.m_fHeight);
            xDrawMemoryCanvas(pSrcContext, kRect.x, kRect.y, kRect.width, kRect.height, m_pContextData->m_pCurrentContextData->nAlpha);
            restore();
            break;
        }
        default:
            LOGE("JCHtml5Context::drawCanvas5 src canvas type error");
            break;
        }
    }
    void JCHtml5Context::drawCanvas(int nCanvasId, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh )
    {
        JCHtml5Render* pSrcHtml5Render = m_pHtml5RenderManager->getHtml5Render(nCanvasId);
        if (pSrcHtml5Render == NULL) return;
        JCHtml5Context* pSrcContext = pSrcHtml5Render->m_pContext;
        switch (pSrcContext->m_nCanvasType)
        {
        case CANVAS_TYPE_RENDER_TARGET:
            xDrawRenderTarget(pSrcContext, sx, sy, sw, sh, dx, dy, dw, dh);
            break;
        case CANVAS_TYPE_MEMORY:
            {
                save();
                float fSx = dw / sw;
                float fSy = dh / sh;
                Rectangle kRect(dx, dy, dw, dh);
                kRect.applyMatrix(&(m_pContextData->m_pCurrentContextData->kMatrix));
                translate(dx-sx*fSx, dy-sy*fSy );
                scale(fSx,fSy);
                xDrawMemoryCanvas(pSrcContext, kRect.x, kRect.y, kRect.width, kRect.height, m_pContextData->m_pCurrentContextData->nAlpha);
                restore();
            }
            break;
        default:
            LOGE("JCHtml5Context::drawCanvas9 src canvas type error");
            break;
        }
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setFilter( float r,float g,float b, float gray )
    {
        ContextDataInfo* pCurDataInfo = m_pContextData->m_pCurrentContextData;
        JCFilter& kFilter = pCurDataInfo->kFilter;
        pCurDataInfo->nFilterType = FILTER_TYPE_COLOR;
        kFilter.r = r;
        kFilter.g = g;
        kFilter.b = b;
        kFilter.gray = gray;
    }
    void JCHtml5Context::setFilterMatrix(float* pColorMatrix, float* pAlpha)
    {
        ContextDataInfo* pCurDataInfo = m_pContextData->m_pCurrentContextData;
        pCurDataInfo->nFilterType = FILTER_TYPE_MATRIX;
        JCFilterMatrix& kFilterMat = pCurDataInfo->kFilterMatrix;
        memcpy(kFilterMat.matrix, pColorMatrix, sizeof(float) * 16);
        memcpy(kFilterMat.alpha, pAlpha, sizeof(float) * 4);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setShadowColor(int p_nShadowColor )
    {

    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setShadowBlur(float p_nShadowBlur )
    {

    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setShadowOffsetX(float p_nShadowOffsetX )
    {

    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setShadowOffsetY( float p_nShadowOffsetY )
    {

    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setLineCap(LINECAP_TYPE p_eLineCap )
    {
        LOGI("setLineCap:%d", p_eLineCap);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setLineJoin(LINEJOIN_TYPE p_eLineJoin )
    {
        LOGI("setLineJoin:%d", p_eLineJoin);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setLineWidth( float p_nLineWidth )
    {
        m_pContextData->m_pCurrentContextData->nLineWidth = p_nLineWidth;
        vg->Setf(VG_STROKE_LINE_WIDTH, m_pContextData->m_pCurrentContextData->nLineWidth);
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setMiterLimit( float p_nMiterLimit )
    {

    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setFont( const char* p_sFont )
    {
        strcpy( m_pContextData->m_pCurrentContextData->sFont,p_sFont );
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setTextAlign(TEXT_ALIGN_TYPE p_eTextAlign )
    {
        m_pContextData->m_pCurrentContextData->nTextAlignType = p_eTextAlign;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setTextBaseline(TEXT_BASE_LINE_TYPE p_eTextBaseline )
    {
        m_pContextData->m_pCurrentContextData->nTextBaseLineType = p_eTextBaseline;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::setGlobalCompositeOperation(BLEND_TYPE p_eGlobalCompositeOperation )
    {
        m_pContextData->m_pCurrentContextData->nBlendType = p_eGlobalCompositeOperation;
    }
    //------------------------------------------------------------------------------
    void JCHtml5Context::disableClip( bool bDisableClip )
    {

    }
    void JCHtml5Context::fillText(const char* sText, const char* sFont, float x, float y, int nFillColor,TEXT_ALIGN_TYPE eTextAlign)
    {
        setFont(sFont);
        setFillStyle(nFillColor);
        setTextAlign(eTextAlign);
        setTextBaseline(TBL_TYPE_TOP);
        JCFontInfo* pFont = m_pFontManager->getFontInfoFromText(sFont);
        fillText(sText, x, y);
    }
    void JCHtml5Context::clipRect(float x, float y, float w, float h)
    {
        beginPath();
        rect(x, y, w, h);
        clip();
    }

    void setCtxParticleData(JCHtml5Context* p_mCtx, JCParticleTemplate2D* pParticleTemplate2D)
    {
        p_mCtx->m_ShaderData.m_fCurrentTime = pParticleTemplate2D->m_nCurrentTime;
        p_mCtx->m_ShaderData.m_fDuration = pParticleTemplate2D->m_pParticleSetting->m_nDuration;
        p_mCtx->m_ShaderData.m_fEndVelocity = pParticleTemplate2D->m_pParticleSetting->m_nEndVelocity;
        // float temp[3] = { 0.0f,20.0f,0.0f };
        memcpy(p_mCtx->m_ShaderData.m_vGravity, pParticleTemplate2D->m_pParticleSetting->m_vGravity, 12);
    }

    void JCHtml5Context::drawParticle(int p_nParticleTemplatid, float p_nX, float p_nY)
    {
		JCParticleTemplate2DCmdDispath * pParticleTemplate2DCmdDispath = (JCParticleTemplate2DCmdDispath *)m_vVectorCmdDispath->operator[](p_nParticleTemplatid);
		if (pParticleTemplate2DCmdDispath == NULL)return;
		JCParticleTemplate2D *particleTemplate2D = pParticleTemplate2DCmdDispath->m_pParticleTemplate2D;//m_pParticleTemplate2DManger->getParticleTemplate2D(p_nParticleTemplatid);
        if (particleTemplate2D!=NULL)
        {
            JCImage* pImage = m_pImageManager->getImage(particleTemplate2D->m_nImageID);
            if (pImage == NULL)return;
            if (pImage->enable() == ENALBE_TYPE_FAIL)return;
            JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
            Rectangle* pRect = pDisplayRes->getRect();
            JCTexture* pTexTure = pDisplayRes->getTexture();
            float rectInAtlas[4];
            rectInAtlas[0] = pRect->x / pTexTure->m_nTextureWidth;
            rectInAtlas[1] = pRect->y / pTexTure->m_nTextureHeight;
            rectInAtlas[2] = (pRect->getRight()) / pTexTure->m_nTextureWidth;
            rectInAtlas[3] = (pRect->getBottom()) / pTexTure->m_nTextureHeight;
            save();
            particleTemplate2D->loop(p_nX, p_nY, rectInAtlas);
            JCShaderManager::ShaderContent* pshaderContent = m_pShaderManager->getShaderById(particleTemplate2D->m_nShaderID);
            JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
            if (!pshaderContent->pProg) {
                pshaderContent->pProg = (new JCGpuProgramTemplate(pshaderContent->vs.c_str(), pshaderContent->ps.c_str()))->getInst("", 0);
            }
            pMaterial->setShaderProgram(pshaderContent->pProg);
            pMaterial->setDrawImageShaderParam(pTexTure, m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
            Matrix32& cmat = m_pContextData->m_pCurrentContextData->kMatrix;
            float objmat[16] = { cmat.m[0],cmat.m[1],0,0,
                cmat.m[2],cmat.m[3],0,0,
                0,0,1.0f,0,
                cmat.m[4], cmat.m[5],0,1.0f
            };
            pMaterial->setBlendType(particleTemplate2D->m_pParticleSetting->m_nBlendState);
            /*int vertnum = particleTemplate2D->m_nFirstNewElement*4;// m_pDynamicMesh->getVertexDesc(4).m_VertDescs[0].m_nStride;
            //if (p_nNumElement == 39 || p_nNumElement == 174)
                int p_nStartIndex=0, p_nNumElement=0;
                if (particleTemplate2D->m_nFirstActiveElement < particleTemplate2D->m_nFirstFreeElement)
                {
                    p_nStartIndex = particleTemplate2D->m_nFirstActiveElement * 12;  
                    p_nNumElement = (particleTemplate2D->m_nFirstFreeElement - particleTemplate2D->m_nFirstActiveElement) * 6;
                }
                else
                {
                    if (particleTemplate2D->m_nFirstFreeElement > 0)
                    {
                        p_nStartIndex = 0;
                        p_nNumElement = particleTemplate2D->m_nFirstFreeElement * 6;
                    }
                }*/
            if (particleTemplate2D->m_nFirstActiveElement != particleTemplate2D->m_nFirstFreeElement) {

                m_pDynamicMesh->pushCmd(std::bind(setCtxParticleData, this, particleTemplate2D));

                // pushParticle2D(particleTemplate2D->m_nFirstNewElement, particleTemplate2D->m_nFirstActiveElement, particleTemplate2D->m_nFirstFreeElement, particleTemplate2D->m_pVetexBuffer2D, particleTemplate2D->m_pIndexBuffer2D,objmat, pMaterial, rectInAtlas)
                // JCRenderGroupData* pGroup = m_pDynamicMesh->pushParticle2D(4, particleTemplate2D->m_pVetexBuffer2D, 0, particleTemplate2D->m_pIndexBuffer2D, p_nStartIndex, p_nNumElement, objmat, pMaterial, rectInAtlas, vertnum);
                 //pGroup->m_nIBBegin += p_nStartIndex;
                JCRenderGroupData *pGroup = m_pDynamicMesh->pushParticle2D(particleTemplate2D->m_pParticleSetting->m_nMaxPartices, particleTemplate2D->m_nFirstNewElement, particleTemplate2D->m_nFirstActiveElement, particleTemplate2D->m_nFirstFreeElement, particleTemplate2D->m_pVetexBuffer2D, particleTemplate2D->m_pIndexBuffer2D, objmat, pMaterial);
            }
            restore();
        }
    }

    void JCHtml5Context::setSkinMesh(int p_nIbId, int p_nVbId, int p_nVertType, int p_nImgId, int p_nShaderId, int p_nStartIndex, int p_nNumElement, float *mat, float p_nX, float p_nY)
    {
        JCShaderManager::ShaderContent* pshaderContent = m_pShaderManager->getShaderById(p_nShaderId);
        JCBufferManager::BufferContent* pib = m_pBufferManager->getBufferById(p_nIbId);
        JCBufferManager::BufferContent* pvb = m_pBufferManager->getBufferById(p_nVbId);
        JCImage* pImage = m_pImageManager->getImage(p_nImgId);
        if (pImage == NULL || pib == NULL || pvb ==NULL)return;
        if (pImage->enable() == ENALBE_TYPE_FAIL)return;
        JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
        Rectangle* pRect = pDisplayRes->getRect();
        JCTexture* pTexTure = pDisplayRes->getTexture();
        float rectInAtlas[4];
        rectInAtlas[0] = pRect->x / pTexTure->m_nTextureWidth;
        rectInAtlas[1] = pRect->y / pTexTure->m_nTextureHeight;
        rectInAtlas[2] = (pRect->getRight()) / pTexTure->m_nTextureWidth;
        rectInAtlas[3] = (pRect->getBottom()) / pTexTure->m_nTextureHeight;
        JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
        if (!pshaderContent->pProg) {
            pshaderContent->pProg = (new JCGpuProgramTemplate(pshaderContent->vs.c_str(), pshaderContent->ps.c_str()))->getInst("", 0);
        }
        pMaterial->setShaderProgram(pshaderContent->pProg);
        pMaterial->setDrawImageShaderParam(pTexTure, m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        Matrix32& cmat = m_pContextData->m_pCurrentContextData->kMatrix;
        Matrix32 mymat;
        if (mat)
        {
            mymat.m[0] = mat[0]; mymat.m[1] = mat[1];
            mymat.m[2] = mat[2]; mymat.m[3] = mat[3];
            mymat.m[4] = mat[4];
            mymat.m[5] = mat[5];
        }
        mymat.m[4] += p_nX;
        mymat.m[5] += p_nY;
        Matrix32::mul(&mymat, &mymat, &cmat);

        float objmat[16] = { mymat.m[0],mymat.m[1],0,0,
            mymat.m[2],mymat.m[3],0,0,
            0,0,1.0f,0,
            mymat.m[4], mymat.m[5],0,1.0f
        };


        pMaterial->setBlendType(m_pContextData->m_pCurrentContextData->nBlendType);
        int vertnum = pvb->nLength / m_pDynamicMesh->getVertexDesc(p_nVertType).m_VertDescs[0].m_nStride;
        //if (p_nNumElement == 39 || p_nNumElement == 174)
        {
           // m_pDynamicMesh->pushCmd(std::bind(setctxparticledata, this, mat[0]));
            JCRenderGroupData* pGroup = m_pDynamicMesh->pushIBVB(p_nVertType, pvb->buffer, 0, pib->buffer, p_nStartIndex, p_nNumElement, objmat, pMaterial, rectInAtlas, vertnum);
            //pGroup->m_nIBBegin += p_nStartIndex;
        }

    }

    void JCHtml5Context::setIBVB(int p_nIbId, int p_nVbId, int p_nVertType, int p_nShaderId, int p_nImgId,
        int p_nStartIndex, int p_nOffset, int p_nNumElement, float x, float y, float* pMat)
    {
        JCShaderManager::ShaderContent* pshaderContent = m_pShaderManager->getShaderById(p_nShaderId);
        JCBufferManager::BufferContent* pib = m_pBufferManager->getBufferById(p_nIbId);
        JCBufferManager::BufferContent* pvb = m_pBufferManager->getBufferById(p_nVbId);
        JCImage* pImage = m_pImageManager->getImage(p_nImgId);
        if (pshaderContent == NULL || pImage == NULL || pib == NULL || pvb == NULL)return;
        if (pImage->enable() == ENALBE_TYPE_FAIL)return;
        JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
        Rectangle* pRect = pDisplayRes->getRect();
        JCTexture* pTexTure = pDisplayRes->getTexture();
        float rectInAtlas[4];
        rectInAtlas[0] = pRect->x / pTexTure->m_nTextureWidth;
        rectInAtlas[1] = pRect->y / pTexTure->m_nTextureHeight;
        rectInAtlas[2] = (pRect->getRight()) / pTexTure->m_nTextureWidth;
        rectInAtlas[3] = (pRect->getBottom()) / pTexTure->m_nTextureHeight;
        JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
        if (!pshaderContent->pProg) {
            pshaderContent->pProg = (new JCGpuProgramTemplate(pshaderContent->vs.c_str(), pshaderContent->ps.c_str()))->getInst("",0);
        }
        pMaterial->setShaderProgram(pshaderContent->pProg);
        pMaterial->setDrawImageShaderParam(pTexTure, m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        Matrix32& cmat = m_pContextData->m_pCurrentContextData->kMatrix;
        Matrix32 mymat;
        if (pMat) {
            mymat.m[0] = pMat[0]; mymat.m[1] = pMat[1];
            mymat.m[2] = pMat[2]; mymat.m[3] = pMat[3];
            mymat.m[4] = pMat[4];
            mymat.m[5] = pMat[5];
        }
        mymat.m[4] += x;
        mymat.m[5] += y;
        Matrix32::mul(&mymat, &mymat, &cmat );

        float objmat[16] = { mymat.m[0],mymat.m[1],0,0,
            mymat.m[2],mymat.m[3],0,0,
            0,0,1.0f,0,
            mymat.m[4], mymat.m[5],0,1.0f
        };
        pMaterial->setBlendType(m_pContextData->m_pCurrentContextData->nBlendType);
        JCRenderGroupData* pGroup = m_pDynamicMesh->pushIBVB(p_nVertType, pvb->buffer,p_nOffset,pib->buffer,0,p_nNumElement, objmat, pMaterial, rectInAtlas);
        //pGroup->m_bScissor = true;
        //Rectangle& crect = m_pContextData->m_pCurrentContextData->kClipRect;
        //float sy = g_nInnerHeight - (crect.y + crect.height)*m_fScaleY;
        //pGroup->m_ClipRect[0] = crect.x*m_fScaleX;
        //pGroup->m_ClipRect[1] = sy;
        //pGroup->m_ClipRect[2] = crect.width*m_fScaleX;
        //pGroup->m_ClipRect[3] = crect.height*m_fScaleY;
    }
    void JCHtml5Context::xDrawTexture( JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, RectGeometry* pGeo, JCMaterial* pMaterial )
    {
        //TEST
        //pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_BLURIMG));
        //m_pDynamicMesh->pushCmd(std::bind(setBlurParam, this, 274, 341, 10, 1,
        //    1, 0, 0, 0xff00ff00));
        //TEST
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWIMAGE));
        pMaterial->setDrawImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        //TEST
        //pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_BLURIMG));
        //m_pDynamicMesh->pushCmd(std::bind(setBlurParam, this, 274, 341, 10, 1,
        //    1, 0, 0, 0xff00ff00));
        //TEST
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWIMAGE));
        pMaterial->setDrawImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);

        RectGeometry::VertexInfo* pVertex = m_pGeo->m_pVertexRect;
        pVertex[0].x = (int)(round(pVertex[0].x));
        pVertex[0].y = (int)(round(pVertex[0].y));
        pVertex[1].x = (int)(round(pVertex[1].x));
        pVertex[1].y = (int)(round(pVertex[1].y));
        pVertex[2].x = (int)(round(pVertex[2].x));
        pVertex[2].y = (int)(round(pVertex[2].y));
        pVertex[3].x = (int)(round(pVertex[3].x));
        pVertex[3].y = (int)(round(pVertex[3].y));

        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawFilterTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilter& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWFILTERIMAGE));
        pMaterial->setDrawFilterImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType, kFilter.r, kFilter.g, kFilter.b, kFilter.gray);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawFilterTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilter& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWFILTERIMAGE));
        pMaterial->setDrawFilterImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType, kFilter.r, kFilter.g, kFilter.b, kFilter.gray);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);

        RectGeometry::VertexInfo* pVertex = m_pGeo->m_pVertexRect;
        pVertex[0].x = (int)(round(pVertex[0].x));
        pVertex[0].y = (int)(round(pVertex[0].y));
        pVertex[1].x = (int)(round(pVertex[1].x));
        pVertex[1].y = (int)(round(pVertex[1].y));
        pVertex[2].x = (int)(round(pVertex[2].x));
        pVertex[2].y = (int)(round(pVertex[2].y));
        pVertex[3].x = (int)(round(pVertex[3].x));
        pVertex[3].y = (int)(round(pVertex[3].y));

        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void setMatFilterParam(JCHtml5Context* pCtx, JCFilterMatrix kFilterMatrix)
    {
        memcpy(pCtx->m_ShaderData.m_matFilter, &kFilterMatrix.matrix, 64);
        memcpy(pCtx->m_ShaderData.m_matFilter1, &kFilterMatrix.alpha, 16);
    }
    void JCHtml5Context::xDrawFilterMatrixTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        m_pDynamicMesh->pushCmd( std::bind(setMatFilterParam, isGpuCanvas() ? this : this->m_pHostGpuContext, m_pContextData->m_pCurrentContextData->kFilterMatrix) );
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_MATFILTER));
        pMaterial->setDrawImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawFilterMatrixTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        m_pDynamicMesh->pushCmd(std::bind(setMatFilterParam, isGpuCanvas() ? this : this->m_pHostGpuContext, m_pContextData->m_pCurrentContextData->kFilterMatrix));
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_MATFILTER));
        pMaterial->setDrawImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);

        RectGeometry::VertexInfo* pVertex = m_pGeo->m_pVertexRect;
        pVertex[0].x = (int)(round(pVertex[0].x));
        pVertex[0].y = (int)(round(pVertex[0].y));
        pVertex[1].x = (int)(round(pVertex[1].x));
        pVertex[1].y = (int)(round(pVertex[1].y));
        pVertex[2].x = (int)(round(pVertex[2].x));
        pVertex[2].y = (int)(round(pVertex[2].y));
        pVertex[3].x = (int)(round(pVertex[3].x));
        pVertex[3].y = (int)(round(pVertex[3].y));

        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
    }
    void JCHtml5Context::xFillFilterMatrixTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, float nParam1, float nParam2, float nParam3, float nParam4, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        m_pDynamicMesh->pushCmd(std::bind(setMatFilterParam, isGpuCanvas() ? this : this->m_pHostGpuContext, m_pContextData->m_pCurrentContextData->kFilterMatrix));
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_FILLIMAGE_MATFILTER));
        pMaterial->setFillImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType, nParam1, nParam2, nParam3, nParam4);
        m_pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        if (m_pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12, true);
        }
    }
    void JCHtml5Context::xDrawImage5(JCImage* pImage, float x, float y, float w, float h, RectGeometry* pGeo, JCMaterial* pMaterial)
    {
        if (pImage->enable() == ENALBE_TYPE_FAIL)return;
        JCDisplayRes* pDisplayRes = pImage->m_pDisplayRes;
        Rectangle* pRect = pDisplayRes->getRect();
        JCTexture* pTexTure = pDisplayRes->getTexture();
        float tx1 = pRect->x / pTexTure->m_nTextureWidth;
        float ty1 = pRect->y / pTexTure->m_nTextureHeight;
        float tx2 = (pRect->getRight()) / pTexTure->m_nTextureWidth;
        float ty2 = (pRect->getBottom()) / pTexTure->m_nTextureHeight;
        switch (m_pContextData->m_pCurrentContextData->nFilterType)
        {
        case FILTER_TYPE_NO:
            xDrawTexture(pDisplayRes, x, y, w, h, tx1, ty1, tx2, ty2, pGeo, pMaterial);
            break;
        case FILTER_TYPE_COLOR:
            xDrawFilterTexture(pDisplayRes, x, y, w, h, tx1, ty1, tx2, ty2, m_pContextData->m_pCurrentContextData->kFilter, pGeo, pMaterial);
            break;;
        case FILTER_TYPE_MATRIX:
            xDrawFilterMatrixTexture(pDisplayRes, x, y, w, h, tx1, ty1, tx2, ty2, m_pContextData->m_pCurrentContextData->kFilterMatrix, pGeo, pMaterial);
            break;
        default:
            xDrawTexture(pDisplayRes, x, y, w, h, tx1, ty1, tx2, ty2, pGeo, pMaterial);
            break;
        }
    }
    void JCHtml5Context::xDrawRenderTarget(JCHtml5Context* pSrcContext,float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh)
    {
        pSrcContext->createRenderTarget();
        if (pSrcContext->m_bRepaint || pSrcContext->m_pRenderTarget->m_pGpuTexture <= 0)
        {
            pSrcContext->renderToGpu();
            pSrcContext->m_bRepaint = false;
        }
        pSrcContext->m_pRenderTarget->touch(false);
        JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
        JCTexture* pTexture = pSrcContext->m_pRenderTarget;
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWIMAGE));
        pMaterial->setDrawImageShaderParam(pTexture->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        float tx1 = sx / pTexture->m_nTextureWidth;
        float ty1 = sy / pTexture->m_nTextureHeight;
        float tx2 = (sx + sw) / pTexture->m_nTextureWidth;
        float ty2 = (sy + sh) / pTexture->m_nTextureHeight;
        m_pGeo->createForUV(dx, dy, dw, dh, tx1, ty1, tx2, ty2);
        applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        m_pGeo->ScaleUV(1, -1);
        m_pGeo->OffUV(0, 1);
        if (m_pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect), rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawGlowRenderTarget(JCHtml5Context* pSrcContext, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,
        int nBlurColor, float fBlurValue, float fOffsetX, float fOffsetY)
    {
        pSrcContext->createRenderTarget();
        if (pSrcContext->m_bRepaint || pSrcContext->m_pRenderTarget->m_pGpuTexture <= 0)
        {
            pSrcContext->renderToGpu();
            pSrcContext->m_bRepaint = false;
        }
        pSrcContext->m_pRenderTarget->touch(false);
        JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
        JCTexture* pTexture = pSrcContext->m_pRenderTarget;
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_GLOWIMG));
        pMaterial->setDrawImageShaderParam(pTexture->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        m_pDynamicMesh->pushCmd(std::bind(setBlurParam, this, pTexture->m_nTextureWidth, pTexture->m_nTextureHeight, fBlurValue, fBlurValue,
            1, fOffsetX, fOffsetY, nBlurColor));
        float tx1 = sx / pTexture->m_nTextureWidth;
        float ty1 = sy / pTexture->m_nTextureHeight;
        float tx2 = (sx + sw) / pTexture->m_nTextureWidth;
        float ty2 = (sy + sh) / pTexture->m_nTextureHeight;
        m_pGeo->createForUV(dx, dy, dw, dh, tx1, ty1, tx2, ty2);
        applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        m_pGeo->ScaleUV(1, -1);
        m_pGeo->OffUV(0, 1);
        if (m_pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect), rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawBlurRenderTarget(JCHtml5Context* pSrcContext, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, float fBlurValue)
    {
        pSrcContext->createRenderTarget();
        if (pSrcContext->m_bRepaint || pSrcContext->m_pRenderTarget->m_pGpuTexture <= 0)
        {
            pSrcContext->renderToGpu();
            pSrcContext->m_bRepaint = false;
        }
        pSrcContext->m_pRenderTarget->touch(false);
        JCMaterial* pMaterial = m_pMaterialManager->getMaterial();
        JCTexture* pTexture = pSrcContext->m_pRenderTarget;
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_BLURIMG));
        pMaterial->setDrawImageShaderParam(pTexture->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        m_pDynamicMesh->pushCmd(std::bind(setBlurParam, this, pTexture->m_nTextureWidth, pTexture->m_nTextureHeight, fBlurValue, fBlurValue,1,0,0,0));
        float tx1 = sx / pTexture->m_nTextureWidth;
        float ty1 = sy / pTexture->m_nTextureHeight;
        float tx2 = (sx + sw) / pTexture->m_nTextureWidth;
        float ty2 = (sy + sh) / pTexture->m_nTextureHeight;
        m_pGeo->createForUV(dx, dy, dw, dh, tx1, ty1, tx2, ty2);
        applyMatrix(m_pGeo, /*CLIP_TYPE_NORMAL*/CLIP_TYPE_ONLY_VISIBLE);
        m_pGeo->ScaleUV(1, -1);
        m_pGeo->OffUV(0, 1);
        if (m_pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect), rectIndex, 12);
        }
    }
    void JCHtml5Context::xDrawMemoryCanvas(JCHtml5Context* pSrcContext, float nClipX, float nClipY, float nClipW, float nClipH,float fAlpha)
    {
        float* m = m_pContextData->m_pCurrentContextData->kMatrix.m;
        std::function<void(void)> pFunction = std::bind(&JCHtml5Context::renderMemoryCanvasMesh,this, pSrcContext,nClipX,nClipY,nClipW,nClipH, m[0], m[1], m[2], m[3], m[4], m[5],fAlpha);
        m_pDynamicMesh->pushCmd(pFunction);
    }
    void matrix32ToMatrix44( float* m32,float* m44 )
    {
        m44[0] = m32[0];  m44[1] = m32[1];  m44[2] = 0;  m44[3] = 0;
        m44[4] = m32[2];  m44[5] = m32[3];  m44[6] = 0;  m44[7] = 0;
        m44[8] = 0;       m44[9] = 0;       m44[10] = 1; m44[11] = 0;
        m44[12] = m32[4]; m44[13] = m32[5]; m44[14] = 0; m44[15] = 1;
    }
    void JCHtml5Context::renderMemoryCanvasMesh(JCHtml5Context* pSrcContext, float nClipX, float nClipY, float nClipW, float nClipH, float m11, float m12, float m21, float m22, float m31, float m32,float fAlpha)
    {
        //目的为了canvas还能绘制canvas
        if (isGpuCanvas())
        {
            pSrcContext->m_pHostGpuContext = this;
        }
        else
        {
            pSrcContext->m_pHostGpuContext = m_pHostGpuContext;
        }
        JCHtml5Context* pGpuContext = pSrcContext->m_pHostGpuContext;

        //进行绘制
        if (pSrcContext->m_bRepaint == true)
        {
            JCMemClass* pSrcContextCmd = pSrcContext->m_pSaveCmd;
            pSrcContextCmd->setReadPos(0);
            while (true)
            {
                char* pData = pSrcContextCmd->getReadPtr();
                if (pData)
                {
                    pSrcContext->m_pHtml5Render->runRenderCmd(*pSrcContextCmd);
                }
                else
                {
                    break;
                }
            }
            pSrcContext->setAtlasCallback(pSrcContext->m_pDynamicMesh);
            pSrcContext->m_bRepaint = false;
        }
        //获得link的Matrix
        Matrix32 kPreMatrix = pGpuContext->m_kLinkMatrix;

        //有裁剪的
        if (pSrcContext->m_bMemoryCanvasScissor)
        {
            //裁剪  两个memoryCanvas如果一起 裁剪应该取最小集合
            Rectangle kTempRect = m_kLinkRect;
            Rectangle kRect(nClipX, nClipY, nClipW, nClipH);
            kRect.applyMatrix(&kPreMatrix);
            kTempRect.intersectionThis(&kRect);

            //混合裁剪部分，外部已经设置了裁剪
            bool bPreScissor = pGpuContext->m_bGLScissorEnabled;
            Rectangle pSaveRect = pGpuContext->m_kScissorRect;
            if (bPreScissor == false)
            {
                glEnable(GL_SCISSOR_TEST);
            }
            else
            {
                kTempRect.intersectionThis(&pSaveRect);
            }
            //裁剪函数
            {
                Rectangle kTempRect1;
                pGpuContext->calcScissorRect(kTempRect, kTempRect1);
                glScissor(kTempRect1.x, kTempRect1.y, kTempRect1.width, kTempRect1.height);
            }
            //向下传递
            pSrcContext->m_kLinkRect = kTempRect;

            //link Transform
            pGpuContext->m_kLinkMatrix.transform(m11, m12, m21, m22, m31, m32);
            matrix32ToMatrix44(pGpuContext->m_kLinkMatrix.m, pGpuContext->m_ShaderData.m_matWorldMat);
            //绘制memoryCanvas
            pSrcContext->m_pDynamicMesh->renderToMemoryCanvas((char*)&(pGpuContext->m_ShaderData),fAlpha);

            //还原矩阵
            pGpuContext->m_kLinkMatrix = kPreMatrix;
            matrix32ToMatrix44(pGpuContext->m_kLinkMatrix.m, pGpuContext->m_ShaderData.m_matWorldMat);

            //还原裁剪
            if (bPreScissor == false)
            {
                glDisable(GL_SCISSOR_TEST);
            }
            else
            {
                Rectangle kOutRect;
                pGpuContext->calcScissorRect(pSaveRect, kOutRect);
                glScissor(kOutRect.x, kOutRect.y, kOutRect.width, kOutRect.height);
            }
        }
        else
        {
            //link Transform
            pGpuContext->m_kLinkMatrix.transform(m11, m12, m21, m22, m31, m32);
            matrix32ToMatrix44(pGpuContext->m_kLinkMatrix.m, pGpuContext->m_ShaderData.m_matWorldMat);
            //绘制memoryCanvas
            pSrcContext->m_pDynamicMesh->renderToMemoryCanvas((char*)&(pGpuContext->m_ShaderData),fAlpha);

            //还原矩阵
            pGpuContext->m_kLinkMatrix = kPreMatrix;
            matrix32ToMatrix44(pGpuContext->m_kLinkMatrix.m, pGpuContext->m_ShaderData.m_matWorldMat);
        }
    }
    void JCHtml5Context::applyMatrix(RectGeometry* pGeoResult, CLIP_TYPE nClipType)
    {
        bool bHasRotate = false;
        //matrix
        Matrix32* pMatrix = &(m_pContextData->m_pCurrentContextData->kMatrix);
        float* m = pMatrix->m;
        (!pMatrix->bOnlyTrans) ? (pGeoResult->applyMatrix(m)) : (pGeoResult->applyTranslate(m[4], m[5]));
        bHasRotate = pMatrix->bHasRotate;
        //clipRect
        float nClipX, nClipY, nClipWith, nClipHeight;
        if (m_pContextData->m_pCurrentContextData->nClip)
        {
            Rectangle* pClipRect = &(m_pContextData->m_pCurrentContextData->kClipRect);
            nClipX = pClipRect->x;
            nClipY = pClipRect->y;
            nClipWith = pClipRect->width;
            nClipHeight = pClipRect->height;
        }
        else
        {
            nClipX =nClipY= 0;
            nClipWith = m_nWidth;
            nClipHeight = m_nHeight;
        }
        switch (nClipType)
        {
        case CLIP_TYPE_NO:
            break;
        case CLIP_TYPE_NORMAL:
            //裁剪
            if (bHasRotate == false)
            {
                pGeoResult->m_bShow = RectGeometry::clipWithRect(pGeoResult, nClipX, nClipY, nClipWith, nClipHeight, true);
            }
            else
            {
                float minx = min(min(min(pGeoResult->m_pVertexRect[0].x, pGeoResult->m_pVertexRect[1].x), pGeoResult->m_pVertexRect[2].x), pGeoResult->m_pVertexRect[3].x);
                float maxx = max(max(max(pGeoResult->m_pVertexRect[0].x, pGeoResult->m_pVertexRect[1].x), pGeoResult->m_pVertexRect[2].x), pGeoResult->m_pVertexRect[3].x);
                float miny = min(min(min(pGeoResult->m_pVertexRect[0].y, pGeoResult->m_pVertexRect[1].y), pGeoResult->m_pVertexRect[2].y), pGeoResult->m_pVertexRect[3].y);
                float maxy = max(max(max(pGeoResult->m_pVertexRect[0].y, pGeoResult->m_pVertexRect[1].y), pGeoResult->m_pVertexRect[2].y), pGeoResult->m_pVertexRect[3].y);

                if (maxx<nClipX) pGeoResult->m_bShow = false;
                else if (minx>nClipX + nClipWith)pGeoResult->m_bShow = false;
                else if (maxy < nClipY) pGeoResult->m_bShow = false;
                else if (miny > nClipY + nClipHeight) pGeoResult->m_bShow = false;
                else pGeoResult->m_bShow = true;
            }
            break;
        case CLIP_TYPE_ONLY_VISIBLE:
            {
                float minx = min(min(min(pGeoResult->m_pVertexRect[0].x, pGeoResult->m_pVertexRect[1].x), pGeoResult->m_pVertexRect[2].x), pGeoResult->m_pVertexRect[3].x);
                float maxx = max(max(max(pGeoResult->m_pVertexRect[0].x, pGeoResult->m_pVertexRect[1].x), pGeoResult->m_pVertexRect[2].x), pGeoResult->m_pVertexRect[3].x);
                float miny = min(min(min(pGeoResult->m_pVertexRect[0].y, pGeoResult->m_pVertexRect[1].y), pGeoResult->m_pVertexRect[2].y), pGeoResult->m_pVertexRect[3].y);
                float maxy = max(max(max(pGeoResult->m_pVertexRect[0].y, pGeoResult->m_pVertexRect[1].y), pGeoResult->m_pVertexRect[2].y), pGeoResult->m_pVertexRect[3].y);

                if (maxx<nClipX) pGeoResult->m_bShow = false;
                else if (minx>nClipX + nClipWith)pGeoResult->m_bShow = false;
                else if (maxy < nClipY) pGeoResult->m_bShow = false;
                else if (miny > nClipY + nClipHeight) pGeoResult->m_bShow = false;
                else pGeoResult->m_bShow = true;
            }
            break;
        }
    }
    bool JCHtml5Context::isGpuCanvas()
    {
        return m_nCanvasType != CANVAS_TYPE_MEMORY;
    }

    void setBlurParam(JCHtml5Context* pCtx, float texw, float texh, float blurW, float blurH, float blur, float blurOffx, float blurOffy,
        unsigned int color) {
        unsigned char* pcolor =(unsigned char*) &color;

        pCtx->m_ShaderData.m_vPsColor[0] = pcolor[2] / 255.0f;
        pCtx->m_ShaderData.m_vPsColor[1] = pcolor[1] / 255.0f;
        pCtx->m_ShaderData.m_vPsColor[2] = pcolor[0] / 255.0f;
        pCtx->m_ShaderData.m_vPsColor[3] = blur;

        pCtx->m_ShaderData.m_vBlurOff[0] = blurW / texw;
        pCtx->m_ShaderData.m_vBlurOff[1] = blurH / texh;
        pCtx->m_ShaderData.m_vBlurOff[2] = blurOffx / texw;
        pCtx->m_ShaderData.m_vBlurOff[3] = blurOffy / texh;

        pCtx->m_ShaderData.m_vVec1[0] = blurW ;
        pCtx->m_ShaderData.m_vVec1[1] = blurH;
        pCtx->m_ShaderData.m_vVec1[2] = texw;
        pCtx->m_ShaderData.m_vVec1[3] = texh;
    }

    void JCHtml5Context::drawTexBlur(JCDisplayRes* pDisplayRes, float blurW, float blurH, float blur, float blurOffx, float blurOffy, unsigned int color){
        float texW = 1;
        float texH = 1;

        /*
        pMaterial->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWIMAGE));
        pMaterial->setDrawImageShaderParam(pDisplayRes->getTexture(), m_pContextData->m_pCurrentContextData->nAlpha, m_pContextData->m_pCurrentContextData->nBlendType);
        pGeo->createForUV(x, y, w, h, tx1, ty1, tx2, ty2);
        applyMatrix(pGeo, CLIP_TYPE_NORMAL);
        if (pGeo->m_bShow) {
            m_pDynamicMesh->pushElements(JCConchMesh::XYUVRGBA, pMaterial,
                4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
                rectIndex, 12);
        }
        */
        JCMaterial* pMtl;
        pMtl->setShaderProgram(m_p2DShaderManager->getShader(SHADER_TYPE_DRAWIMAGE));
        m_pDynamicMesh->pushCmd(std::bind(setBlurParam,this,texW,texH,blurW,blurH,
            blur,blurOffx, blurOffy,color));
        JCRenderGroupData* pGData = m_pDynamicMesh->pushElements(JCConchMesh2D::XYUVRGBA, pMtl,
            4, m_pGeo->m_pVertexRect, sizeof(m_pGeo->m_pVertexRect),
            rectIndex, 12);
    }
    void JCHtml5Context::memoryCanvasNeedRefresh(JCAtlas* pAtlas)
    {
        m_bRepaint = true;
        std::map<JCAtlas*, int>::iterator iter = m_vMapAtlas.find(pAtlas);
        if (iter != m_vMapAtlas.end())
        {
            iter = m_vMapAtlas.erase(iter);
        }
    }
    void JCHtml5Context::clearAllAtlasCallback()
    {
        for (std::map<JCAtlas*, int>::iterator iter = m_vMapAtlas.begin(); iter != m_vMapAtlas.end(); iter++)
        {
            JCAtlas* pAtlas = iter->first;
            if (pAtlas != NULL)
            {
                pAtlas->removeFunction(m_pHtml5Render->m_nID);
            }
        }
        m_vMapAtlas.clear();
    }
    void JCHtml5Context::setAtlasCallback(JCConchMesh2D* pConchMesh)
    {
        //TODO效率测试，是全部清空，全部添加快
        //还是每次对比差异快
        clearAllAtlasCallback();
        
        int nCurrentGroupPos = pConchMesh->getCurrentGroupPos();
        for (int i = 0; i < nCurrentGroupPos; i++)
        {
            JCRenderGroupData *pRenderGroup = &pConchMesh->m_vRenderGroupData[i];
			JCMaterial* pMaterial = (JCMaterial*)pRenderGroup->m_pMaterial;
            if (pMaterial == NULL) continue;
			pMaterial->setTargetBlendType(pMaterial->m_nBlendType);//设置targetBlendType
            JCTexture* pTexture = pMaterial->getTexture(0);
            if (pTexture && pTexture->m_nType == TEXTURE_TYPE_ATLAS)
            {
                m_vMapAtlas[pTexture->m_pAtlas] = 1;
            }
        }
        for (std::map<JCAtlas*, int>::iterator iter = m_vMapAtlas.begin(); iter != m_vMapAtlas.end(); iter++)
        {
            JCAtlas* pAtlas = iter->first;
            if (pAtlas != NULL)
            {
                std::function<void(JCAtlas*)> pFunc = std::bind( &JCHtml5Context::memoryCanvasNeedRefresh,this,pAtlas );
                pAtlas->setFunction(m_pHtml5Render->m_nID, pFunc);
            }
        }
    }
    void JCHtml5Context::openGLScissor(int x, int y, int w, int h)
    {
        Rectangle kRect(x, y, w, h);
        if (isGpuCanvas() == false)
        {
            kRect.applyMatrix(&(m_pHostGpuContext->m_kLinkMatrix));
            if (m_bMemoryCanvasScissor)
            {
                kRect.intersectionThis(&m_kLinkRect);
            }
        }
        m_kScissorRect.setTo(kRect.x, kRect.y, kRect.width, kRect.height);
        calcScissorRect(kRect, kRect);
        glScissor(kRect.x, kRect.y, kRect.width, kRect.height);
    }
    void JCHtml5Context::openGLDisableStencil()
    {
        glDisable(GL_STENCIL_TEST);
    }
    void JCHtml5Context::openGLEnableScissor()
    {
        m_bGLScissorEnabled = true;
        glEnable(GL_SCISSOR_TEST);
    }
    void JCHtml5Context::openGLDisableScissor()
    {
        m_bGLScissorEnabled = false;
        glDisable(GL_SCISSOR_TEST);
    }
    void JCHtml5Context::calcScissorRect(const Rectangle& pInRect, Rectangle& kOutRect)
    {
        JCHtml5Context* pGpuContext = m_pHostGpuContext;
        if( pGpuContext == NULL) pGpuContext = this;
        //转到裁剪的坐标系中
        int cx = (int)(pInRect.x*pGpuContext->m_fScaleX + pGpuContext->m_fTx);
        int cy = 0;
        if (pGpuContext->m_nCanvasType == CANVAS_TYPE_MAIN)
        {
            int nHeight = g_nInnerHeight;
            cy = (int)(nHeight - (pInRect.y + pInRect.height)*pGpuContext->m_fScaleY - pGpuContext->m_fTy);
        }
        else
        {
            int nHeight =  pGpuContext->m_nHeight;
            int nTextureHeight = pGpuContext->m_pRenderTarget ? pGpuContext->m_pRenderTarget->m_nTextureHeight : nHeight;
            cy = (int)(nHeight - (pInRect.y + pInRect.height)) + nTextureHeight -nHeight;
        }
        GLsizei cw = (GLsizei)(pInRect.width*pGpuContext->m_fScaleX);
        GLsizei ch = (GLsizei)(pInRect.height*pGpuContext->m_fScaleY);
        kOutRect.setTo(cx,cy,cw,ch);
    }
    void JCHtml5Context::calcScissorRect(int x, int y, int w, int h, Rectangle& kOutRect)
    {
        JCHtml5Context* pGpuContext = m_pHostGpuContext;
        if (pGpuContext == NULL) pGpuContext = this;
        //转到裁剪的坐标系中
        int cx = (int)(x*pGpuContext->m_fScaleX + pGpuContext->m_fTx);
        int cy = 0;
        if (pGpuContext->m_nCanvasType == CANVAS_TYPE_MAIN)
        {
            int nHeight = g_nInnerHeight;
            cy = (int)(nHeight - (y + h)*pGpuContext->m_fScaleY - pGpuContext->m_fTy);
        }
        else
        {
            int nHeight = pGpuContext->m_nHeight;
            int nTextureHeight = pGpuContext->m_pRenderTarget ? pGpuContext->m_pRenderTarget->m_nTextureHeight : nHeight;
            cy = (int)(nHeight - (y + h)) + nTextureHeight - nHeight;
        }
        GLsizei cw = (GLsizei)(w*pGpuContext->m_fScaleX);
        GLsizei ch = (GLsizei)(h*pGpuContext->m_fScaleY);
        kOutRect.setTo(cx, cy, cw, ch);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
