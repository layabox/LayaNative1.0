/**
@file			JSRuntime.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JSRuntime_H__
#define __JSRuntime_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "../../JCScrpitRuntime.h"

/** 
 * @brief 这个获得的是桌面分辨率
*/
namespace laya 
{
    class JSRuntime : public JsObjBase, public JSObjNode
    {
    public:
        enum 
        {
            onframeid, ondrawid, onresizeid, ontouchevtid,ondevicemotionevtid, onkeyevtid, onmouseevtid, oninvalidglid,onotherevtid,onnetworkevt,onbackpressed,cmdABObjSharedWithJS,cmdOtherBufferSharedWithJS
        };

	    static JsObjClassInfo JSCLSINFO;
	    
	    void exportJS();
	    
        JSRuntime();

	    ~JSRuntime();

    public:

        void setOnFrameFunction( JSValueAsParam p_pFunction );
        void setOnDrawFunction(JSValueAsParam p_pFunction);

        void setOnResizeFunction(JSValueAsParam p_onresize);

        void setHref(JSValueAsParam p_sHref);

        void disableConchResManager();

        void disableConchAutoRestoreLostedDevice();

        void setOnInvalidGLRes(JSValueAsParam p_pFunction);

        void setMouseEvtFunction(JSValueAsParam p_pFunction);

        void setTouchEvtFunction(JSValueAsParam p_pFunction);

        void setDeviceMotionEvtFunction(JSValueAsParam p_pFunction);

        void captureScreen(JSValueAsParam p_pFunction);

        void setKeyEvtFunction(JSValueAsParam p_pFunction);

        void setNetworkEvtFunction(JSValueAsParam p_pFunction);

		void setOnBackPressedFunction(JSValueAsParam p_pFunction);
			
        void setCmdBuffer(JSValueAsParam pArrayBuffer);

        void showLoadingView(bool p_bShow);

        void setScreenWakeLock(bool p_bWakeLock);

        void setSensorAble(bool p_bSensorAble);

        void flushRenderCommands();

        void appendBuffer(JSValueAsParam pArrayBufferArgs, int nDataSize, int nOffset);

        void showAssistantTouch(bool p_bShow);

        void setBuffer(JSValueAsParam pArrayBuffer);

        bool saveAsPng(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);

        bool saveAsJpeg(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);

        JsValue convertBitmapToPng(JSValueAsParam pArrayBufferArgs, int w, int h);

        JsValue convertBitmapToJpeg(JSValueAsParam pArrayBufferArgs, int w, int h);

    public:

        /** @brief 初始化freeType默认字体
         *  @param[in] 字体的路径，多个字体中间用 \4 分割
         *  @return 返回是否成功
        */
        bool initFreeTypeDefaultFontFromFile( const char* sDefaultTTFs );

        /** @brief 初始化freeType默认字体
        *  @param[in] arrayBuffer
        *  @return 返回是否成功
        */
        bool initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs);


        /** @brief 设置字体从FileName中读取
        *  @param[in] 字体名字
        *  @param[in] ttf的路径
        *  @return 是否读取成功
        */
        bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName);

        /** @brief 设置字体从buffer中读取
        *  @param[in] 字体的名字
        *  @param[in] buffer
        *  @param[in] bufferLength
        *  @return 是否读取成功
        */
        bool setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs);

        /** @brief 移除字体
        *  @param[in] 字体的名字
        *  @return
        */
        bool removeFont(const char* sFontFamily);

        JsValue readFileFromAsset(const char* file, const char* encode);

        JsValue strTobufer(const char* s);

        std::string callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr);

        const char* getCachePath();

        const char* getPresetUrl();

        void printCorpseImages();

        int getCountVertext();

        int getCountGroup();

        int getCountNode();

        void setExternalLink(const char* sUrl);

        void setExternalLinkEx( const char* sUrl,int x,int y,int w,int h,bool bCloseWebview );

        void closeExternalLink();

        void callWebviewJS( const char* sFunctionName,const char* sJsonParam,const char* sCallbackFunction );

        void hideWebview();

        void showWebView();

		void regShaderDefine(const char* name, long long value);

        void exit();

    public:

        JCScriptRuntime*        m_pScrpitRuntime;

    };
}
//------------------------------------------------------------------------------


#endif //__JSRuntime_H__

//-----------------------------END FILE--------------------------------
