/**
@file			JCHtml5Context.h
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCHtml5Context_H__
#define __JCHtml5Context_H__

#include "../Image/JCImage.h"
#include "../SaveData/JCContextSaveData.h"
#include <util/JCColor.h>
#include "../SaveData/JCContextSaveData.h"
#include "../Image/JCImageManager.h"
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include <3DRS/JCMaterial.h>
#include <3DRS/JCMaterialManager.h>
#include <geom/RectGeometry.h>
#include <3DRS/JC2DShaderManager.h>
#include <buffer/JCBuffer.h>
#include <resource/JCResManager.h>
#include <resource/text/JCTextManager.h>
#include <FontRender/JCFreeTypeRender.h>
#include <resource/text/JCFontManager.h>
#include <MonkVG/openvg.h>
#include <JCICmdDispatch.h>
#include <resource/DisplayRes/JCRenderTarget.h>

/** 
 * @brief 
*/
namespace laya 
{
    enum CANVAS_TYPE
    {
        CANVAS_TYPE_MAIN,                   //主画布
        CANVAS_TYPE_RENDER_TARGET,          //GPU的画布
        CANVAS_TYPE_MEMORY,
    };
    enum CLIP_TYPE
    {
        CLIP_TYPE_NO,			//不进行裁剪
        CLIP_TYPE_NORMAL,		//正常裁剪，裁剪的时候
        CLIP_TYPE_ONLY_VISIBLE,	//不对图像进行处理，只是判断是否显示
    };
    class JCHtml5RenderManager;
    class MkVGContext;
    struct JCPattern
    {
        FILL_PATTERN_TYPE nRepetType;
        float x;
        float y;
        float w;
        float h;
        JCImage* pImage;
    };
    class JCHtml5Render;
    class JCConchMesh2D;
    class JCHtml5Context
    {
    public:
        enum VG_DATA_TYPE {
            stokedata = 0,
            fillcolor = 1,
            fillimg = 2
        };

        JCHtml5Context(JCHtml5Render* pHtml5Render,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
                JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager,JCFontManager* pFontManager, JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispath);

	    ~JCHtml5Context();

        void renderToGpu();

        /** 
         *  @brief 用来调用每次渲染完成的清理。
         *  @return 
         */
        void onRenderEnd();

    public:
	    void clearRect( float p_nX, float p_nY, float p_nW, float p_nH );

	    void fillRect(float p_nX, float p_nY, float p_nW, float p_nH );

	    void translate( float p_nTx,float p_nTy );

	    void save();

	    void restore();

	    void beginPath();

	    void rect(float p_nX, float p_nY, float p_nWidth, float p_nHeight );

	    void clip();

	    void strokeRect(float p_nX, float p_nY, float p_nWidth, float p_nHeight );

	    void fill();

        void _stroke(VGPath p);

	    void stroke();

	    void moveTo(float p_nX, float p_nY );

	    void closePath();

	    void lineTo(float p_nX, float p_nY );

	    void quadraticCurveTo(float p_nCPX, float p_nCPY, float p_nEndX, float p_nEndY );

	    void bezierCurveTo(float p_nCPX, float p_nCPY, float p_nCPX2, float p_nCPY2, float p_nEndX, float p_nEndY );

	    void arc(float p_nX, float p_nY, float p_nR, float p_fSAngle,float p_fEAngle,bool p_bCounterclockwise );

	    void arcTo(float p_nX1, float p_nY1, float p_nX2, float p_nY2, float p_nR );

	    void scale( float p_fX,float p_fY );

	    void rotate( float p_fAngle );

	    void transform( float a,float b,float c,float d,float e,float f );

	    void setTransform( float a,float b,float c,float d,float e,float f );

	    void fillText( const char* p_sText, float p_nX, float p_nY );

	    void fillText4( const char* p_sText, float p_nX, float p_nY, int p_nMaxWidth );

	    void strokeText( const char* p_sText, float p_nX, float p_nY );

	    void strokeText4( const char* p_sText, float p_nX, float p_nY, int p_nMaxWidth );
    
        void drawImage(int p_nImgId, float p_nX, float p_nY );
    
	    void drawImage(int p_nImgId, float p_nX, float p_nY, float p_nWidth, float p_nHeight );
    
	    void drawImage(int  p_nImgId, float p_nSX, float p_nSY, float p_nSWidth, float p_nSHeight, float p_nDX, float p_nDY, float p_nDWidth, float p_nDHeight );

        void drawCanvas(int p_nCanvasId, float p_nX, float p_nY);

        void drawCanvas(int p_nCanvasId, float p_nX, float p_nY, float p_nWidth, float p_nHeight);

        void drawCanvas(int p_nCanvasId, float p_nSX, float p_nSY, float p_nSWidth, float p_nSHeight, float p_nDX, float p_nDY, float p_nDWidth, float p_nDHeight);
	   
        void setFilter( float r,float g,float b, float gray );

        void setFilterMatrix( float* pColorMatrix,float* pAlpha);

	    void setGlobalAlpha( float p_fAlpha );
    
        void setFillStyle(int p_nFillStyle);

        void setFillStyle(int p_nImageId, FILL_PATTERN_TYPE p_ePaternType, float p_nX, float p_nY, float p_nW, float p_nH);

        void setStrokeStyle(int p_nStrokeStyle );

	    void setShadowColor(int p_nShadowColor);

	    void setShadowBlur(float p_nShadowBlur );

	    void setShadowOffsetX( float p_nShadowOffsetX );

	    void setShadowOffsetY( float p_nShadowOffsetY );

	    void setLineCap(LINECAP_TYPE p_eLineCap );

	    void setLineJoin(LINEJOIN_TYPE p_eLineJoin );
	
	    void setLineWidth( float p_nLineWidth );

	    void setMiterLimit(float p_nMiterLimit );

        /*  函数说明：设置字体(最基本必须为 normal 100 16px Arial  这4个，但是可以扩展描边和下划线)
         *  参数说明：格式为	样式(normal、italic、oblique)	加粗	font-size	字体	borderSize	border颜色
         *  参数说明：格式为	normal 100 16px Arial 1 #ff0000
        */
	    void setFont( const char* p_sFont );

	    void setTextAlign(TEXT_ALIGN_TYPE p_eTextAlign );

	    void setTextBaseline(TEXT_BASE_LINE_TYPE p_eTextBaseline );

	    void setGlobalCompositeOperation(BLEND_TYPE p_eGlobalCompositeOperation);

        void disableClip(bool p_bDisableClip);

        void fillText(const char* sText, const char* sFont, float x, float y, int nFillColor, TEXT_ALIGN_TYPE eTextAlign);

        void clipRect(float x, float y, float w, float h);

        void setIBVB(int p_nIbId, int p_nVbId, int p_nVertType, int p_nShaderId, int p_nImgId, int p_nStartIndex, int p_nOffset, int p_nNumElement, float x, float y, float* pMat);

        void setSkinMesh(int p_nIbId,int p_nVbId,int p_nVertType,int p_nImgId,int p_nShaderId,int p_nStartIndex, int p_nNumElement,float *mat,float p_nX,float p_nY);
        
        void drawParticle(int p_nParticleTemplatid, float p_nX, float p_nY);
   public:

        void applyMatrix(RectGeometry* p_pGeoResult, CLIP_TYPE p_nClipType);

        void clearSaveCmd();
        
        void setCanvasType(CANVAS_TYPE nType);

        void getPixels(int x, int y, int w, int h, std::function<void(unsigned char *pPixels, int size, int w, int h)> callback);

        void toBase64(const char* type, float encoderOptions, std::function<void(const char* base64)> callback);

        void setMemoryCanvasClip( bool bClip );

        void setCanvasSize(int w, int h);

        void invalidGLRes();

        void onVGData(VG_DATA_TYPE type, void* pData, int len);

        JCTexture* getRenderTargetTexture()
        {
            return m_pRenderTarget;
        }

        void xDrawRenderTarget(JCHtml5Context* pSrcContext, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);
        void xDrawGlowRenderTarget(JCHtml5Context* pSrcContext, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,
                                int nBlurColor,float fBlurValue,float fOffsetX,float fOffsetY );
        void xDrawBlurRenderTarget(JCHtml5Context* pSrcContext, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,float fBlurValue);

        void xDrawMemoryCanvas(JCHtml5Context* pSrcContext,float nClipX, float nClipY, float nClipW,float nClipH,float fAlpha);

        void renderMemoryCanvasMesh(JCHtml5Context* pSrcContext, float nClipX, float nClipY, float nClipW, float nClipH,float m11,float m12,float m21,float m22,float m31,float m32,float fAlpha);

        void drawTexBlur(JCDisplayRes* pDisplayRes, 
            float blurW, float blurH, float blur, float blurOffx, float blurOffy,
            unsigned int color);

        void setAtlasCallback(JCConchMesh2D* pConchMesh);

        void xDrawWordRes(std::vector<JCWordRes*>& vWordres, int nCountWidth, int nFontSize, int x, int y, TEXT_BASE_LINE_TYPE nBaseLineType, TEXT_ALIGN_TYPE nTextAlignType);

        void flushToRenderTarget();

        void xFillTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, float nParam1, float nParam2, float nParam3, float nParam4, RectGeometry* pGeo, JCMaterial* pMaterial);

        void xFillFilterMatrixTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, float nParam1, float nParam2, float nParam3, float nParam4, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial);
    private:

        void xDrawImage5(JCImage* pImage, float x, float y, float w, float h, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawFilterTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilter& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawFilterTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilter& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawFilterMatrixTexture(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawFilterMatrixTexturePixelPerfect(JCDisplayRes* pDisplayRes, float x, float y, float w, float h, float tx1, float ty1, float tx2, float ty2, JCFilterMatrix& kFilter, RectGeometry* pGeo, JCMaterial* pMaterial);
        void xDrawText(const char* p_sText, float x, float y,char* sFont,int nColor,TEXT_BASE_LINE_TYPE nBaseLineType,TEXT_ALIGN_TYPE nTextAlignType);
        bool createRenderTarget();
        void cleareAllPath();
        bool isGpuCanvas();

        void memoryCanvasNeedRefresh( JCAtlas* pAtlas );
        void clearAllAtlasCallback();
        void openGLScissor(int x,int y,int w,int h);
        void openGLEnableScissor();
        void openGLDisableScissor();
        //计算ScissorRect
        void calcScissorRect( const Rectangle& pInRect,Rectangle& kOutRect );
        void calcScissorRect( int x,int y,int w,int h, Rectangle& kOutRect);

        void openGLDisableStencil();
    public:
        JCHtml5Render*          m_pHtml5Render;             ///<保存html5Render
		JCMemClass*             m_pSaveCmd;                 ///<小Canvas存储的cmd
        JCContextSaveData*      m_pContextData;             ///<保存的渲染状态
        JCImageManager*         m_pImageManager;            ///<ImageManager从外部传入的
        JCMaterialManager*      m_pMaterialManager;         ///<MaterialManager从外部传入的
        JCResManager*           m_pTextureManager;          ///<resManager
        JCConchMesh2D*          m_pDynamicMesh;             ///<动态Mesh
        JCHtml5RenderManager*   m_pHtml5RenderManager;      ///<管理所有canvas的
        JCTextManager*          m_pTextManager;             ///<文字管理器
        JCFontManager*          m_pFontManager;             ///<Font管理器
        JCBufferManager*        m_pBufferManager;
        JCShaderManager*        m_pShaderManager;
        JC2DShaderManager*      m_p2DShaderManager;         ///<2Dshader管理器
		std::vector<JCICmdDispatch*>*       m_vVectorCmdDispath;
        CANVAS_TYPE             m_nCanvasType;              ///<canvas类型
        JCNamedData             m_kGlobalRenderInfoDesc;    ///<画布大小等全局的信息的描述。
        float                   m_fScaleX = 1.0f;
        float                   m_fScaleY = 1.0f;
        float                   m_fTx = 0.0f;
        float                   m_fTy = 0.0f;
        //-======注意：下面的数据会用到shader中，不允许改变他们的内存分布======-//
        struct ShaderData {
            float                   m_fWidth;                   ///<context的宽
            float                   m_fHeight;                  ///<context的高
            float                   m_matMat1[16];              //4x4matrix
            float                   m_matWorldMat[16];          //4x4matrix 为了canvas用的
            float                   m_vPsColor[4];
            float                   m_vBlurOff[4];  //blurw/texw, blurh/texh,offx/texw,offy/texh
            //float                   m_fFilterStrength;
            float                   m_vVec1[4]; //扩展
            float                   m_matFilter[16];
            float                   m_matFilter1[4];
            float                   m_fAlpha = 1.0f;   //66
            float                   m_fCurrentTime;
            float                   m_fDuration;
            float                   m_fEndVelocity;
            float                   m_vGravity[3];
        }m_ShaderData;
        //-================================================================-//
        int                     m_nWidth;
        int                     m_nHeight;
        bool                    m_bRepaint;                 ///<Canvas是否刷新
    private:
        
        JCRenderTarget*         m_pRenderTarget;
        MkVGContext*            vg = nullptr;
        VGPaint                 m_vgPaint;
        VGPaint                 m_vgStrokePaint;
        VGPath                  m_vgCurPath;
        std::vector<VGPath>     m_vAllPath;                 //可能会有多个path，每个moveTo就会产生一个。必须用
                                                            //vector来保存，否则都会连到一起。
        float                   m_fVecLastX = 0.0f;
        float                   m_fVecLastY = 0.0f;
        float                   m_fVecLastNoScaleX = 0.0f;
        float                   m_fVecLastNoScaleY = 0.0f;
        bool                    m_bVecHasLastPos = false;   //通过确定的movto，lineto等知道当前点
                                                            //初始化，beginpath等会导致变成false
        bool                    m_bScissorEnabled = false;
        bool                    m_bStencilEnabled = false;
        size_t                  m_nStencilMask = 0;
        Rectangle               m_kLinkRect;
        Matrix32                m_kLinkMatrix;
    private:
        RectGeometry*           m_pGeo;                     ///<计算Geo用的    
        JCHtml5Context*         m_pHostGpuContext;          ///<主canvas 绘制小canvasA  CanvasA再绘制CanvasB使用的
        std::map<JCAtlas*,int>  m_vMapAtlas;                ///<memoryCanvas中需要存储Atlas信息，这当Altas发生了变化，可以通知canvas刷新
        Rectangle               m_kScissorRect;             ///<以及计算好的ScissorRect
        bool                    m_bGLScissorEnabled;
        bool                    m_bMemoryCanvasScissor;     ///<memoryCanvas是否有裁剪，默认是没有的
        std::vector<JCWordRes*> m_vTempWordRes;             ///<临时保存文字用的
        int                     m_nScissorCount = 0;
        std::vector<std::function<void ()> > m_vOnRenderEndFuncs;
    };
}
//------------------------------------------------------------------------------


#endif //__JCHtml5Context_H__

//-----------------------------END FILE--------------------------------