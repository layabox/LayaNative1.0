
/**
@file			JCPublicCmdDispath.h
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCPublicCmdDispath_H__
#define __JCPublicCmdDispath_H__

#include <JCICmdDispatch.h>
#include "Image/JCImageManager.h"

namespace laya
{
    class JCCmdDispathManager;
    enum PUBLIC_FUNCTION_TYPE
    {
        PFT_startJSThread,
        PFT_createImage,
        PFT_deleteImage,
        PFT_createHtml5Context,
        PFT_deleteHtml5Context,
        PFT_createWebGLContext,
        PFT_deleteWebGLContext,
        PFT_createNode,
        PFT_deleteNode,
        PFT_setContextType,
        PFT_setHtml5CanvasSize,
        PFT_createVetexBuffer,
        PFT_createShader,
        PFT_createGraphics,
        PFT_deleteGraphics,
        PFT_setRootNode,
        PFT_setMainCanvasScaleInfo,
        PFT_setMemoryCanvasClip,
        PFT_setFPSNode,
        PFT_setLoadingViewNode,
		PFT_createParticalTemplate2D,
		PFT_deleteParticalTemplate2D,
		PFT_registShaderDefine,
        PFT_setTransparentMode,
        PFT_setImageReleaseSpaceTime,
        PFT_PerfAddData,
        PFT_PerfUpdateDt,
        PFT_ImageEnableMerageInAtlas,
        PFT_ImageReleaseTexture,
        PFT_deleteVetexBuffer,
    };
    struct ParamPerfAddData
    {
        int funcID;
        int nID;
        int nColor;
        float fScale;
        float fAlert;
    };
    struct PerfUpdateDt
    {
        int funcID;
        int nID;
        float fTimeSpace;
    };
    struct ParamCreateImage
    {
        int funcID;
        int imageID;
        long imagePtr;
    };
    struct ParamDeleteImage
    {
        int funcID;
        int imageID;
    };
    struct ParamInt
    {
        int funcID;
        int nID;
    };
    struct ParamFloat
    {
        int funcID;
        float value;
    };
    struct ParamInt2
    {
        int funcID;
        int nID;
        int nType;
    };
    struct ParamSetCanvasSize
    {
        int funcID;
        int nID;
        int nWidth;
        int nHeight;
    };
    struct ParamVetexBuffer
    {
        int funcID;
        int id;
        int target;
        int bufferUsage;
        int length;
    };
    /** 
     * @brief 
    */
    class JCPublicCmdDispath : public JCICmdDispatch
    {
    public:

        typedef bool (JCPublicCmdDispath::*ProcFunction)(JCMemClass& pRenderCmd);

    public:

        /** @brief构造函数
        */
        JCPublicCmdDispath(JCCmdDispathManager* pCmdDispathManager);

        /** @brief析构函数
        */
        ~JCPublicCmdDispath();

        bool dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount);

        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);

        bool restartJSThread();

    private:

        bool _rendercmd_startJSThread(JCMemClass& pRenderCmd);
        bool _rendercmd_createImage(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteImage(JCMemClass& pRenderCmd);
        bool _rendercmd_createHtml5Context(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteHtml5Context(JCMemClass& pRenderCmd);
        bool _rendercmd_createWebGLContext(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteWebGLContext(JCMemClass& pRenderCmd);
        bool _rendercmd_createNode(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteNode(JCMemClass& pRenderCmd);
        bool _rendercmd_setContextType(JCMemClass& pRenderCmd);
        bool _rendercmd_setHtml5CanvasSize(JCMemClass& pRenderCmd);
        bool _rendercmd_createVetexBuffer(JCMemClass& pRenderCmd);
        bool _rendercmd_createShader(JCMemClass& pRenderCmd);
        bool _rendercmd_createGraphics(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteGraphics(JCMemClass& pRenderCmd);
        bool _rendercmd_setRootNode(JCMemClass& pRenderCmd);
        bool _rendercmd_setMainCanvasScaleInfo(JCMemClass& pRenderCmd);
        bool _rendercmd_setMemoryCanvasClip(JCMemClass& pRenderCmd);
        bool _rendercmd_setFPSNode(JCMemClass& pRenderCmd);
        bool _rendercmd_setLoadingViewNode(JCMemClass& pRenderCmd);
		bool _rendercmd_createParticalTemplate2D(JCMemClass& pRenderCmd);
		bool _rendercmd_deleteParticalTemplate2D(JCMemClass& pRenderCmd);
		bool _rendercmd_registShaderDefine(JCMemClass& pRenderCmd);
        bool _rendercmd_setTransparentMode(JCMemClass& pRenderCmd);
        bool _rendercmd_setImageReleaseSpaceTime(JCMemClass& pRenderCmd);
        bool _rendercmd_PerfAddData(JCMemClass& pRenderCmd);
        bool _rendercmd_PerfUpdateDt(JCMemClass& pRenderCmd);
        bool _rendercmd_ImageEnableMerageInAtlas(JCMemClass& pRenderCmd);
        bool _rendercmd_ImageReleaseTexture(JCMemClass& pRenderCmd);
        bool _rendercmd_deleteVetexBuffer(JCMemClass& pRenderCmd);
        
    public:

        int                     m_nID;                  ///<CmdDispathManager分配的ID
        JCCmdDispathManager*    m_pCmdDispathManager;   ///<cmdDispath的管理器
    };
}

#endif //__JCPublicCmdDispath_H__

//-----------------------------END FILE--------------------------------