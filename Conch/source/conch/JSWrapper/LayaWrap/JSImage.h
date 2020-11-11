/**
@file			JSImage.h
@brief			
@author			James
@version		1.0
@date			2014_11_17
*/

#ifndef __JSImage_H__
#define __JSImage_H__

//包含头文件
#include <stdio.h>
#include "../JSInterface/jsobjbase.h"
#include "../JSInterface/JSInterface.h"
#include <Image/JCImage.h>
#include <resource/JCResource.h>

/** 
 * @brief 
*/
namespace laya
{
    class IConchThreadCmdMgr;
	class JSImage :public JsObjBase, public JSObjNode
	{
	public:
		enum { onloadid, onerrorid, thisid, objid };
		static JsObjClassInfo JSCLSINFO;
        static void exportJS();
    public:

		JSImage();

        ~JSImage();

        void onLoaded(std::weak_ptr<int> callbackref);

		void onError( int p_nError,std::weak_ptr<int> callbackref );

		void onLoadedCallJSFunction(std::weak_ptr<int> callbackref);

		void onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref);
        
        void setObj(JSValueAsParam p_pFunction);
        
        JsValue getObj();

		void SetOnload(JSValueAsParam p_pFunction );

		JsValue GetOnload();

		void SetOnError(JSValueAsParam p_pFunction );

		JsValue GetOnError();

		int GetWidth();

		int GetHeight();
		
		const char* getSrc();

		void setSrc( const char* p_sSrc );

        bool getComplete();

		JsValue getImageData( int p_nX,int p_nY,int p_nW,int p_nH );

		int getImageID();

        bool syncRestoreResource();

        void putBitmapData( char* pData,int width, int height);

        void putBitmapDataJS( JSValueAsParam pArrayBuffer, int width, int height );

        void destroy();

        void releaseTexture();

        void enableMerageInAtlas(bool bValue);

    private:

        void onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref);

        void onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref);

        void onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref);

        void onDownloadError(JCResStateDispatcher* p_pRes, int e, std::weak_ptr<int>& callbackref);

        bool downloadImage(bool p_bSyncDecode);

        void sendCreateImageMsgToRenderThread();

	public:
        
        JsObjHandle		                m_pOnLoad;		//4
        JsObjHandle		                m_pOnError;     //4
        JsObjHandle                     m_pObj;
        
        bool			                m_bComplete;	//3
        std::shared_ptr<int>	        m_CallbackRef;

    private:
        JCImage*		                m_pImage;       //256
        int                             m_nID;
        std::string                     m_sUrl;
        int                             m_nDownloadState;
	};
}

#endif //__JSImage_H__

//-----------------------------END FILE--------------------------------