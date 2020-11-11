/**
@file			JSImage.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_17
*/

//包含头文件
#include "JSImage.h"
#include <util/Log.h>
#include <util/JCMemorySurvey.h>
#include "../JSInterface/JSInterface.h"
#include "../../JCScrpitRuntime.h"
#include <downloadCache/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include <WebGLRender/JCWebGLRender.h>
#include "../../JCConch.h"
#include "../../JCCmdDispatchManager.h"
#include "JSRuntime.h"
#include <fileSystem/JCFileSystem.h>

namespace laya
{

ADDJSCLSINFO(JSImage, JSObjNode);
JSImage::JSImage()
{
    m_pImage = new JCImage();
    m_pImage->setManager( JCConch::s_pConchRender->m_pAtlasManager, JCConch::s_pConchRender->m_pTextureManager,JCConch::s_pConchRender->m_pFileResManager,JCConch::s_pConchRender->m_pImageManager );
    m_CallbackRef.reset(new int(1));
    m_bComplete = false;
    m_pClsInfo = &JSImage::JSCLSINFO;
    m_nDownloadState = 0;
    m_nID = JCConch::s_pConchRender->m_pImageManager->getImageID();
}
JSImage::~JSImage()
{
	m_pOnLoad.Reset();
	m_pOnError.Reset();
    m_pObj.Reset();
	JCMemorySurvey::GetInstance()->releaseClass( "image",this );
    destroy();
}
void JSImage::destroy()
{
    if (m_pImage)
    {
        //从JS线程删除这个ID
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteImage);
        pRenderCmd->append(m_nID);
        m_pImage = NULL;
    }
}
void JSImage::releaseTexture()
{
    if (m_pImage)
    {
        //从JS线程删除这个ID
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_ImageReleaseTexture);
        pRenderCmd->append(m_nID);
    }
}
void JSImage::onLoaded(std::weak_ptr<int> callbackref)
{
	std::function<void(void)> pFunction = std::bind(&JSImage::onLoadedCallJSFunction,this, callbackref);
	JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
}
void JSImage::onError( int p_nError,std::weak_ptr<int> callbackref )
{
	std::function<void(void)> pFunction = std::bind(&JSImage::onErrorCallJSFunction,this,p_nError, callbackref);
	JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
}
void JSImage::onLoadedCallJSFunction(std::weak_ptr<int> callbackref)
{
	if (!callbackref.lock()) return;
    if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
    if (!IsMyJsEnv()) return;

    if (GetWidth() <= 0 || GetHeight() <= 0|| m_pImage->m_kBitmapData.m_pImageData==NULL) {
        m_pOnError.Call(500);
    }
    else {
        sendCreateImageMsgToRenderThread();
        if (m_pImage->m_kBitmapData.m_pUserData)
        {
            m_pOnLoad.Call((const char*)m_pImage->m_kBitmapData.m_pUserData);
        }
        else
        {
           m_pOnLoad.Call();
        }
    }
}
void JSImage::sendCreateImageMsgToRenderThread()
{
    int nMemSize = GetWidth() * GetHeight() * 4 + 272;
    AdjustAmountOfExternalAllocatedMemory(nMemSize);
    JCMemorySurvey::GetInstance()->newClass("image", 1024, this);
    m_pImage->m_sUrl = m_sUrl;
    //先把js提供的命令存一下。
    JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
    JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
    pRenderCmd->append(PUBLIC_CMD_DISPATH);
    pRenderCmd->append(PFT_createImage);
    pRenderCmd->append(m_nID);
    pRenderCmd->append((long)(m_pImage));
    m_bComplete = true;
}
void JSImage::onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref )
{
	if (!callbackref.lock())return;
    if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
	if (!IsMyJsEnv())return;
    LOGW("download image file error! %s\n", m_sUrl.c_str());
    m_pOnError.Call(p_nError);
}
bool JSImage::getComplete()
{
    return m_bComplete;
}
void JSImage::SetOnload(JSValueAsParam p_pFunction )
{
	m_pOnLoad.set(onloadid,this,p_pFunction);
}
JsValue JSImage::GetOnload()
{
	return m_pOnLoad.getJsObj();
}
void JSImage::SetOnError(JSValueAsParam p_pFunction )
{
	m_pOnError.set(onerrorid,this,p_pFunction);
}
JsValue JSImage::GetOnError()
{
    return m_pOnError.getJsObj();
}
JsValue JSImage::getObj()
{
    return m_pObj.getJsObj();
}
void JSImage::setObj(JSValueAsParam obj)
{
    m_pObj.set(objid,this,obj);
}
const char* JSImage::getSrc()
{
    return m_sUrl.c_str();
}
bool JSImage::syncRestoreResource()
{
    //if (!m_pImage || !m_pImage->m_pImageFile ) return false;
    //return m_pImage->m_pImageFile->downloadImage(true);
    return false;
}
void JSImage::setSrc( const char* p_sSrc )
{
	if (!p_sSrc) return;
    m_sUrl = p_sSrc;
	std::weak_ptr<int> cbref(m_CallbackRef);
    downloadImage( false );
}
void JSImage::onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref){
    if (!callbackref.lock()){
        LOGE("jsimg GCed");
        if (p_bmp.m_pImageData){
            p_bmp.releaseData();
        }
        return;
    }

    if (m_pImage && m_pImage->m_kBitmapData.m_pImageData){
        m_pImage->m_kBitmapData.releaseData();
        m_pImage->m_kBitmapData.m_pImageData = NULL;
    }

    m_pImage->m_kBitmapData = p_bmp;
    onLoaded(callbackref);
}
void JSImage::onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref)
{
	JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(&JSImage::onDecodeEnd, this, p_bmp, callbackref));
}
void JSImage::onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref)
{
    m_nDownloadState = 0;
    JCFileRes* pFileRes = (JCFileRes*)p_pRes;
    if (pFileRes->m_pBuffer.get())
    {
        //同步加载
        if (p_bDecodeSync){
        }
        else
        {
            imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, callbackref);
            loadImageMemASync(pFileRes->m_pBuffer, pFileRes->m_nLength, cb);
        }
    }
    else
    {
        onDownloadError(p_pRes, 1, callbackref);
    }
}
void JSImage::onDownloadError(JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref)
{
    if (!callbackref.lock())return;
    m_nDownloadState = 0;
    onError(e,callbackref);
    //TODO
    //setState(error);
}
bool JSImage::downloadImage(bool p_bSyncDecode)
{
    if (m_nDownloadState == 1)return false;
    m_nDownloadState = 1;
    //TODO 要处理本地文件的情况
    std::weak_ptr<int> cbref(m_CallbackRef);
    JCFileRes* pRes = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sUrl);
    pRes->setOnReadyCB(std::bind(&JSImage::onDownloadOK, this, std::placeholders::_1, false, cbref));
    pRes->setOnErrorCB(std::bind(&JSImage::onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
    return true;
}
int JSImage::GetWidth()
{
	return m_pImage->getWidth();
}
int JSImage::GetHeight()
{
	return m_pImage->getHeight();
}
void JSImage::putBitmapDataJS(JSValueAsParam pArrayBuffer, int width, int height)
{
    JsArrayBufferData ab(!isSupportTypedArrayAPI());
    bool bIsArrayBuffer = extractJSAB(pArrayBuffer, ab);
    if (bIsArrayBuffer)
    {
        if (ab.len >= width * height * 4)
        {
            putBitmapData(ab.data,width, height);
        }
        else
        {
            LOGE("JSImage::pushBitmapData array buffer size < width * height * 4");
        }
    }
    else 
    {
        LOGE("JSImage::pushBitmapData param is not an ArrayBuffer!");
    }
}
void JSImage::putBitmapData(char* pData, int width, int height )
{
    if (m_pImage && m_pImage->m_kBitmapData.m_bIsCompressed)
    {
        LOGE("JSImage::pushBitmapData compressed texture do not support putBitmapData!");
        return;
    }

    if (m_pImage && m_pImage->m_kBitmapData.m_pImageData) 
    {
        m_pImage->m_kBitmapData.releaseData();
        m_pImage->m_kBitmapData.m_pImageData = NULL;
    }
    m_pImage->m_kBitmapData.m_bPremultiplyAlpha = false;
    m_pImage->m_kBitmapData.m_nFormat = GL_RGBA;
    m_pImage->m_kBitmapData.m_nTextureWidth = m_pImage->m_kBitmapData.m_nWidth = width;
    m_pImage->m_kBitmapData.m_nTextureHeight = m_pImage->m_kBitmapData.m_nHeight = height;
    m_pImage->m_kBitmapData.m_pImageData = new char[width * height * 4];
    memcpy(m_pImage->m_kBitmapData.m_pImageData, pData, width*height * 4);
    //设置url的名字
    char sCachePath[1024];
    memset(sCachePath, 0, 1024);
    sprintf(sCachePath,"%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(),m_nID);
    //写入文件
    writeFileSync1(sCachePath, pData, width*height * 4);
    m_sUrl = sCachePath;
    m_pImage->m_bPushBitmapData = true;
    sendCreateImageMsgToRenderThread();
    //不需要回调
    //onLoaded(m_CallbackRef);
}
JsValue JSImage::getImageData( int p_nX,int p_nY,int p_nW,int p_nH )
{
	if( m_bComplete == false ) return JSP_TO_JS_NULL;
	if( m_pImage == NULL ) return JSP_TO_JS_NULL;
	BitmapData* pImg = &(m_pImage->m_kBitmapData);
	if( pImg  )
	{
        if (pImg->m_bIsCompressed)
            return JSP_TO_JS_NULL;
		if( p_nX < 0 || p_nY < 0 || p_nX >= pImg->m_nWidth || p_nY >= pImg->m_nHeight )return JSP_TO_JS_NULL;
		if( ( p_nX + p_nW ) > pImg->m_nWidth || ( p_nY + p_nH ) > pImg->m_nHeight  )return JSP_TO_JS_NULL;

        if (pImg->m_bPremultiplyAlpha)
        {
            pImg->releaseData();
        }

        if (pImg->m_pImageData != NULL || (pImg->m_pImageData == NULL && m_pImage->enableBitmapData()))
        {
            if (p_nX == 0 && p_nY == 0 && p_nW == pImg->m_nWidth && p_nH == pImg->m_nHeight)
            {
                return createJSAB(pImg->m_pImageData, pImg->m_nWidth * pImg->m_nHeight * 4);
            }
            else
            {
                unsigned char* pTemp = (unsigned char *)(pImg->m_pImageData);
                int nSize = p_nH * p_nW * 4;
                int nDstLine = p_nW * 4;
                int nSrcLine = pImg->m_nWidth * 4;
                unsigned char* pBuffer = new unsigned char[nSize];
                for (int i = 0; i < p_nH; i++)
                {
                    memcpy(&pBuffer[nDstLine*i], &pTemp[nSrcLine*(i + p_nY) + p_nX * 4], nDstLine);
                }
                return createJSAB((char*)pBuffer, nSize);
            }
        }
	}/*
	else
	{
		//TODO 缩略图怎么办 缩放比加入不是正好就SB了
	}
    */
	return JSP_TO_JS_NULL;
}
void JSImage::enableMerageInAtlas(bool bValue)
{
    if (m_pImage)
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_ImageEnableMerageInAtlas);
        pRenderCmd->append(m_nID);
        pRenderCmd->append(bValue==true?1:0);
    }
}
int JSImage::getImageID()
{
	return m_nID;
}
void JSImage::exportJS() 
{
    JSP_CLASS("image", JSImage);
    JSP_ADD_PROPERTY_RO(conchImgId, JSImage, getImageID);
    JSP_ADD_PROPERTY_RO(width, JSImage, GetWidth);
    JSP_ADD_PROPERTY_RO(height, JSImage, GetHeight);
    JSP_ADD_PROPERTY(src, JSImage, getSrc, setSrc);
    JSP_ADD_PROPERTY(_onload, JSImage, GetOnload, SetOnload);
    JSP_ADD_PROPERTY(onload, JSImage, GetOnload, SetOnload);
    JSP_ADD_PROPERTY(onerror, JSImage, GetOnError, SetOnError);
    JSP_ADD_PROPERTY(obj, JSImage, getObj, setObj);
    JSP_ADD_PROPERTY_RO(complete, JSImage, getComplete);
    JSP_ADD_METHOD("setSrc", JSImage::setSrc);
    JSP_ADD_METHOD("getImageData", JSImage::getImageData);
    JSP_ADD_METHOD("putBitmapData", JSImage::putBitmapDataJS);
    JSP_ADD_METHOD("syncRestoreResource", JSImage::syncRestoreResource);
    JSP_ADD_METHOD("destroy", JSImage::destroy);
    JSP_ADD_METHOD("releaseTexture", JSImage::releaseTexture);
    JSP_ADD_METHOD("enableMerageInAtlas",JSImage::enableMerageInAtlas);
    JSP_INSTALL_CLASS("image", JSImage);
}

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
