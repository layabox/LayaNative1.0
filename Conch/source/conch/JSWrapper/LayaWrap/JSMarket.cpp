/**
@file			JSMarket.cpp
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

//包含头文件
#include "JSMarket.h"
#include "util/Log.h"
#include "util/JCLayaUrl.h"
#include "util/JCMemorySurvey.h"
//#include "util/util.h"
#include <fileSystem/JCFileSystem.h>
#ifdef ANDROID
	#include <jni.h>
	#include "../../CToJavaBridge.h"
#elif __APPLE__
    #include "../../CToObjectC.h"
#endif
#include "JSConchConfig.h"
#include "../../JCConch.h"
#include "../../JCScriptRuntime.h"


namespace laya 
{
	ADDJSCLSINFO(JSMarket, JSObjNode);
std::string JSMarket::ms_sCacheDir = "";
//------------------------------------------------------------------------------
JSMarket::JSMarket()
{
	AdjustAmountOfExternalAllocatedMemory( 512 );
	JCMemorySurvey::GetInstance()->newClass( "conchMarket",512,this );
	m_pLoginJSFunction.Reset();
	m_pFeedAndShareJSFunction.Reset();
	m_pInviteJSFunction.Reset();
	m_pCZJSFunction.Reset();
	m_pBuyPropsJSFunction.Reset();
	m_pAuthorizeJSFunction.Reset();
	m_pRefreshTokenJSFunction.Reset();
	m_pGetFriendsJSFunction.Reset();
    m_pSendToDesktopJSFunction.Reset();
	m_pTopicCircleFunction.Reset();
	m_pLogoutJSFunction.Reset();
	m_pEnterFeedbackJSFunction.Reset();
	m_pEnterBBSJSFunction.Reset();
	m_pEnterPlatformJSFunction.Reset();
	m_pSwitchUserJSFunction.Reset();
	m_pEnterAccountMgrJSFunction.Reset();
	m_pSetCZInfoJSFunction.Reset();
	m_pSendMessageToPlatformFunction.Reset();
    m_pInitJSFunction.Reset();
	m_bClearConchMemory = true;
    m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
}
//------------------------------------------------------------------------------
JSMarket::~JSMarket()
{
	JCMemorySurvey::GetInstance()->releaseClass( "conchMarket",this );
}
void JSMarket::init( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::init value = [%s]",p_sJsonParam );
    m_pInitJSFunction.set(initjsid,this, p_pCallback);
    #ifdef ANDROID
        /*
        std::vector<intptr_t> params;
        params.push_back((int)p_sJsonParam);
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callStaticJavaMethod( JMN_MarketInit,params, kRet);
        */
    #elif __APPLE__
        CToObjectCMarketInit( p_sJsonParam );
    #elif WIN32
    #endif
}
void JSMarket::onInitCallback( std::string p_sJsonParam )
{
    std::function<void(void)> pFunction = std::bind(&JSMarket::onInitCallbackCallJS, this,p_sJsonParam);
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onInitCallbackCallJS( std::string p_sJsonParam )
{
	if(!m_pInitJSFunction.Empty())
		m_pInitJSFunction.Call( p_sJsonParam.c_str() );
}
//------------------------------------------------------------------------------

void JSMarket::login( const char* p_sJsonParam,JSValueAsParam p_pFunction )
{
    LOGI(">>>>>>>>>>JSMarket::login value = [%s]",p_sJsonParam);
	m_pLoginJSFunction.set(loginid,this,p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "login", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketLogin( p_sJsonParam );
#elif WIN32
     std::string s = p_sJsonParam;
     onLoginCallback(s);
#endif
}
//------------------------------------------------------------------------------
void JSMarket::onLoginCallback( std::string jsonParam )
{
	std::function<void(void)> pFunction = std::bind(&JSMarket::onLoginCallbackCallJS, this,jsonParam);
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onLoginCallbackCallJS( std::string jsonParam )
{
    //回调为了和onAuthorizeCallback 统一，因为js层想用同一个回调
	if(!m_pLoginJSFunction.Empty())
		m_pLoginJSFunction.Call( jsonParam );
}
//------------------------------------------------------------------------------
//获得marketName
std::string JSMarket::getMarketName()
{
    LOGI(">>>>>>>>>>JSMarket::getMarketName");
#ifdef ANDROID
	std::vector<intptr_t> params;
	CToJavaBridge::JavaRet kRet;
	if( CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(),"getMarketName", kRet) )
    {
	    m_sMarketName = CToJavaBridge::GetInstance()->getJavaString( kRet.pJNI,kRet.strRet );
	    LOGI("getMarketName:=%s",m_sMarketName.c_str() );
	    return m_sMarketName;
    }
    return "";
#elif __APPLE__
    m_sMarketName = CToObjectCGetMarketName();
    return m_sMarketName;
#elif WIN32
#endif
	return "";
}
//------------------------------------------------------------------------------
//是否有登陆
int JSMarket::getLoginType()
{
    LOGI(">>>>>>>>>>JSMarket::getLoginType");
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getLoginType", kRet, CToJavaBridge::JavaRet::RT_Int))
    {
	    return kRet.intRet;
    }
    return 0;
#elif __APPLE__
    return CToObjectCGetLoginType();
#elif WIN32
#endif
	return false;
}
//------------------------------------------------------------------------------
//注销
void JSMarket::logout( const char* p_sJsonParam,JSValueAsParam p_pFunction )
{
    LOGI(">>>>>>>>>>JSMarket::logout");
	m_pLogoutJSFunction.set(logoutid,this,p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "logout", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketLogout( p_sJsonParam );
#elif WIN32
#endif
}
//------------------------------------------------------------------------------
void JSMarket::onLogoutCallback( std::string jsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onLogoutCallback p_sMsg=%s",jsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onLogoutCallbackCallJS, this,jsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onLogoutCallbackCallJS( std::string jsonParam)
{
	LOGI(">>>>>>>>>>JSMarket::onLogoutCallbackCallJS p_sMsg=%s",jsonParam.c_str() );
	if (!m_pLogoutJSFunction.Empty())
		m_pLogoutJSFunction.Call( jsonParam );
}
//------------------------------------------------------------------------------
//切换用户
void JSMarket::switchUser(const char* p_sJsonParam,JSValueAsParam p_pFunction)
{
	LOGI(">>>>>>>>>>JSMarket::switchUser");
    m_pSwitchUserJSFunction.set(switchuserid,this, p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "switchUser", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketSwitchUser( p_sJsonParam );
#elif WIN32
#endif
}

void JSMarket::onSwitchUser( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onSwitchUser p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onSwitchUserCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}

void JSMarket::onSwitchUserCallJS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onSwitchUserCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if (!m_pSwitchUserJSFunction.Empty())
		m_pSwitchUserJSFunction.Call( p_sJsonParam );
}

//------------------------------------------------------------------------------
//进入第三方的平台
void JSMarket::enterPlatform(const char* p_sJsonParam,JSValueAsParam p_pFunction)
{
	LOGI(">>>>>>>>>>JSMarket::enterPlatform");
	clearConchEngineMemory();
    m_pEnterPlatformJSFunction.set(enterplatformid,this,p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterPlatform", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterPlatform( p_sJsonParam );
#elif WIN32
#endif
}

void JSMarket::onEnterPlatform( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterPlatform p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onEnterPlatformCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}

void JSMarket::onEnterPlatformCallJS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterPlatformCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if(!m_pEnterPlatformJSFunction.Empty())
		m_pEnterPlatformJSFunction.Call( p_sJsonParam );
}
//------------------------------------------------------------------------------
//进入bbs
void JSMarket::enterBBS(const char* p_sJsonParam,JSValueAsParam p_pFunction)
{
	LOGI(">>>>>>>>>>JSMarket::enterBBS");
	clearConchEngineMemory();
    m_pEnterBBSJSFunction.set(enterbbsid,this, p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterBBS", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterBBS( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onEnterBBS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterBBS p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onEnterBBSCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onEnterBBSCallJS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterBBSCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if( !m_pEnterBBSJSFunction.Empty())
		m_pEnterBBSJSFunction.Call( p_sJsonParam );
}
//------------------------------------------------------------------------------
//反馈
void JSMarket::enterFeedback(const char* p_sJsonParam,JSValueAsParam p_pFunction)
{
	LOGI(">>>>>>>>>>JSMarket::enterFeedback");
	clearConchEngineMemory();
    m_pEnterFeedbackJSFunction.set(enterfeedbackid,this,p_pFunction);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterFeedback", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterFeedback( p_sJsonParam );
#elif WIN32

#endif
}
void JSMarket::onEnterFeedback( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterFeedback p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onEnterFeedbackCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onEnterFeedbackCallJS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterFeedbackCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if(!m_pEnterFeedbackJSFunction.Empty())
		m_pEnterFeedbackJSFunction.Call( p_sJsonParam );
}
//------------------------------------------------------------------------------
//进入账户管理
void JSMarket::enterAccountMgr(const char* p_sJsonParam,JSValueAsParam p_pCallback)
{
	LOGI(">>>>>>>>>>JSMarket::enterAccountMgr");
	clearConchEngineMemory();
    m_pEnterAccountMgrJSFunction.set(enteraccountmgrid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterAccountMgr", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterAccountMgr( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onEnterAccountMgr( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterAccountMgr p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onEnterAccountMgrCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onEnterAccountMgrCallJS( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onEnterAccountMgrCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if(!m_pEnterAccountMgrJSFunction.Empty())
		m_pEnterAccountMgrJSFunction.Call( p_sJsonParam );
}
//------------------------------------------------------------------------------
//充值
void JSMarket::CZ( const char* jsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::CZ %s",jsonParam);
	clearConchEngineMemory();
    m_pCZJSFunction.set(rechargeid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "recharge", jsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketCZ( jsonParam );
#elif WIN32
#endif
}
void JSMarket::onCZEvent( std::string jsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onCZEvent desc=%s",jsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onCZEventCallJS, this,jsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onCZEventCallJS( std::string p_sDesc )
{
    LOGI(">>>>>>>>>>JSMarket::onCZEventCallJS desc=%s",p_sDesc.c_str() );
	if(!m_pCZJSFunction.Empty())
		m_pCZJSFunction.Call( p_sDesc );
}
//------------------------------------------------------------------------------
//购买道具
void JSMarket::buyProps( const char* p_sJsonParam, JSValueAsParam p_sCallback )
{
    LOGI(">>>>>>>>>>JSMarket::buyProps");
	clearConchEngineMemory();
    m_pBuyPropsJSFunction.set(buypropsid,this, p_sCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "buyProps", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketBuyProps( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onBuyPropsCallback( std::string p_sDesc )
{
	std::function<void(void)> pFunction = std::bind(&JSMarket::onBuyPropsCallbackCallJS, this,p_sDesc );
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onBuyPropsCallbackCallJS( std::string p_sDesc )
{
	if(!m_pBuyPropsJSFunction.Empty())
		m_pBuyPropsJSFunction.Call( p_sDesc );
}
//------------------------------------------------------------------------------
//获得进入平台的类型
int JSMarket::getEnterPlatformType()
{
    LOGI(">>>>>>>>>>JSMarket::getEnterPlatformType");
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getEnterPlatformType", kRet, CToJavaBridge::JavaRet::RT_Int))
    {
	    return kRet.intRet;
    }
    return 0;
#elif __APPLE__
    return CToObjectCGetEnterPlatformType();
#elif WIN32
#endif
	return -1;
}
//------------------------------------------------------------------------------
//设置充值信息
void JSMarket::setCZInfo( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::setCZInfo");
    m_pSetCZInfoJSFunction.set(setrechargeinfoid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setRechargeInfo",p_sJsonParam ,kRet);
#elif __APPLE__
    CToObjectCMarketSetCZInfo( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onSetCZInfo( std::string p_sJsonParam )
{
	std::function<void(void)> pFunction = std::bind(&JSMarket::onSetCZInfoCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onSetCZInfoCallJS( std::string p_sJsonParam )
{
	if(!m_pSetCZInfoJSFunction.Empty())
		m_pSetCZInfoJSFunction.Call( p_sJsonParam );
}
//-----------------------------------------------------------------------------
//分享
void JSMarket::enterShareAndFeed( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::enterShareAndFeedextInfo=%s",p_sJsonParam);
	clearConchEngineMemory();
	m_pFeedAndShareJSFunction.set(feedandshareid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterShareAndFeed", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterShareAndFeed( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onShareAndFeedCallback( std::string jsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onShareAndFeedCallback sMsg=%s",jsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onShareAndFeedCallbackCallJS, this,jsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onShareAndFeedCallbackCallJS( std::string jsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onShareAndFeedCallbackCallJS sMsg=%s",jsonParam.c_str() );
	if(!m_pFeedAndShareJSFunction.Empty())
		m_pFeedAndShareJSFunction.Call( jsonParam );
}
//------------------------------------------------------------------------------
//邀请
void JSMarket::enterInvite( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::enterInvite");
	clearConchEngineMemory();
    m_pInviteJSFunction.set(inviteid,this, p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "enterInvite", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketEnterInvite( p_sJsonParam );
#elif WIN32
#endif
}
//邀请的回调
void JSMarket::onInviteCallback( std::string jsonParam )
{
	std::function<void(void)> pFunction = std::bind(&JSMarket::onInviteCallbackCallJS, this,jsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onInviteCallbackCallJS( std::string jsonParam )
{
	if(!m_pInviteJSFunction.Empty())
		m_pInviteJSFunction.Call( jsonParam );
}
//------------------------------------------------------------------------------
//分享的回调
//------------------------------------------------------------------------------
void JSMarket::authorize( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::authorize");
    m_pAuthorizeJSFunction.set(authorizeid,this, p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "authorize", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketAuthorize( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onAuthorizeCallback( std::string jsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onAuthorizeCallback params=%s",jsonParam.c_str());
	std::function<void(void)> pFunction = std::bind(&JSMarket::onAuthorizeCallbackCallJS, this,jsonParam);
    m_pCmdPoster->postToJS( pFunction );
}
//------------------------------------------------------------------------------
void JSMarket::onAuthorizeCallbackCallJS( std::string jsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onAuthorizeCallbackCallJS params=%s",jsonParam.c_str());
	if(!m_pAuthorizeJSFunction.Empty())
		m_pAuthorizeJSFunction.Call( jsonParam);
}
//------------------------------------------------------------------------------
void JSMarket::refreshToken( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::refreshToken" );
    m_pRefreshTokenJSFunction.set(refreshtokenid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "refreshToken", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketRefreshToken( p_sJsonParam );
#elif WIN32
#endif
}
void JSMarket::onRefreshTokenCallback( std::string p_sJsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onRefreshTokenCallback token=%s",p_sJsonParam.c_str());
	std::function<void(void)> pFunction = std::bind(&JSMarket::onRefreshTokenCallbackCallJS, this,p_sJsonParam);
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onRefreshTokenCallbackCallJS( std::string p_sJsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onRefreshTokenCallbackCallJS token=%s",p_sJsonParam.c_str());
	if (!m_pRefreshTokenJSFunction.Empty())
		m_pRefreshTokenJSFunction.Call( p_sJsonParam);
}
//------------------------------------------------------------------------------
void JSMarket::getGameFriends( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::getGameFriends params=%s",p_sJsonParam);
    m_pGetFriendsJSFunction.set(getfriendsid,this,p_pCallback);
#ifdef ANDROID
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getGameFriends", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketGetGameFriends( p_sJsonParam );
#elif WIN32
    
#endif
}
void JSMarket::onGetFameFriendsCallback( std::string p_sJsonParam)
{
    LOGI(">>>>>>>>>>JSMarket::onGetFameFriendsCallback p_sMsg=%s",p_sJsonParam.c_str());
    std::function<void(void)> pFunction = std::bind(&JSMarket::onGetFameFriendsCallbackCallJS, this,p_sJsonParam);
    m_pCmdPoster->postToJS( pFunction );
    
}
void JSMarket::onGetFameFriendsCallbackCallJS( std::string p_sJsonParam)
{
    LOGI(">>>>>>>>>>JSMarket::onGetFameFriendsCallbackCallJS p_sMsg=%s",p_sJsonParam.c_str());
	if(!m_pGetFriendsJSFunction.Empty())
		m_pGetFriendsJSFunction.Call( p_sJsonParam);
}
//------------------------------------------------------------------------------
int JSMarket::canSendToDesktop(const char* p_sJsonParam)
{
	LOGI(">>>>>>>>>>JSMarket::canSendToDesktop=[%s]",p_sJsonParam);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "canSendToDesktop",p_sJsonParam, kRet, CToJavaBridge::JavaRet::RT_Int))
    {
	    return kRet.intRet;
    }
    return 1;
#elif __APPLE__
    return CToObjectCMarketCanSendToDesktop( p_sJsonParam );
#elif WIN32

#endif
	return 0;
}
void JSMarket::sendToDesktop( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
    LOGI(">>>>>>>>>>JSMarket::sendToDesktop extInfo=%s",p_sJsonParam );
    m_pSendToDesktopJSFunction.set(sendtodesktopid,this,p_pCallback);
#ifdef ANDROID
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "sendToDesktop", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketSendToDesktop( p_sJsonParam );
#elif WIN32
    
#endif
}
void JSMarket::onSendToDesktopCallback( std::string p_sJsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onSendToDesktopCallback p_sMsg=%s",p_sJsonParam.c_str() );
    std::function<void(void)> pFunction = std::bind(&JSMarket::onSendToDesktopCallbackCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onSendToDesktopCallbackCallJS( std::string p_sJsonParam )
{
    LOGI(">>>>>>>>>>JSMarket::onSendToDesktopCallbackCallJS p_sMsg=%s",p_sJsonParam.c_str() );
	if(!m_pSendToDesktopJSFunction.Empty())
		m_pSendToDesktopJSFunction.Call( p_sJsonParam );
}
//------------------------------------------------------------------------------
void urltopathfile(char* p_pszURL){
	char* pdt = p_pszURL;
	while(*pdt!=NULL){
		switch(*pdt){
		case '/':*pdt='_';break;
		case ':':*pdt='_';break;
		case '@':*pdt='_';break;
		case'\\':*pdt='_';break;
		case '?':*pdt='_';break;
		}
		pdt++;
	}
}
bool writeMarketData(const char* p_pszDir, const char* p_pszType, const char* p_pszContent ){
	if( JCScriptRuntime::s_JSRT &&  JCScriptRuntime::s_JSRT->m_pUrl){
		std::string url = JCScriptRuntime::s_JSRT->m_pUrl->m_strPath;
		url.at(0)=url.at(0);	//在android下，如果不修改url，则url和pProc->m_pUrl->m_strPath使用的是同一个指针
		urltopathfile((char*)url.c_str());
		url = std::string(p_pszDir)+"/"+url;
		url+=".";url+=p_pszType;
		try{
			FILE* pf = fopen(url.c_str(),"wb");
			if(pf){
				int len = strlen(p_pszContent);
				if(sizeof(int)==fwrite(&len,1,sizeof(int),pf)){
					if( len!=fwrite(p_pszContent,1,len,pf)){
						LOGE("writeMarketData_%s error:%s,%s", p_pszType, url.c_str(),p_pszContent);
						fclose(pf);
						throw -6;
						return false;
					}
					fclose(pf);
					return true;
				}
				fclose(pf);
			}else{
				throw -6;
			}
		}catch(int e){
			if( e==-6 && global_onCreateFileError){
				global_onCreateFileError();
			}
		}
	}
	return false;
}
std::string readMarketData(const char* p_pszDir, const char* p_pszType ){
	std::string ret = "";
	if(JCScriptRuntime::s_JSRT&&  JCScriptRuntime::s_JSRT->m_pUrl){
		std::string url = JCScriptRuntime::s_JSRT->m_pUrl->m_strPath;
		url.at(0)=url.at(0);	//在android下，如果不修改url，则url和pProc->m_pUrl->m_strPath使用的是同一个指针
		urltopathfile((char*)url.c_str());
		url = std::string(p_pszDir)+"/"+url;
		url+=".";url+=p_pszType;
		FILE* pf = fopen(url.c_str(),"rb");
		if(pf){
			int len = 0;
			if( fread(&len,1,sizeof(int),pf)==sizeof(int)){
				char buff[1024];
				memset( buff,0,1024 );
				if(len<1024){
					if( len==fread(buff,1,len,pf)){
						ret.append((char*)buff,len);
						fclose(pf);
						return ret;
					}
				}else{
					LOGW("Error:readMarketData 数据太大.");
				}
			}
			fclose(pf);
		}
	}
	return ret;
}
bool JSMarket::getIsLogined()
{
	std::string openid = getLocalOpenid();
	std::string token = getLocalToken();
    LOGI(">>>>>>>>>>JSMarket::getIsLogined Openid=%s,localToken=%s",openid.c_str(),token.c_str());
	return openid.length()&&token.length();
}
std::string  JSMarket::getLocalInfo()
{
#ifdef ANDROID
	std::string szResult = readMarketData(ms_sCacheDir.c_str(),"cpinfo");
	LOGI(">>>>>>>>>>JSMarket::getLocalInfo = [%s]",szResult.c_str());
	return szResult;
#elif __APPLE__
    std::string szResult = readMarketData(ms_sCacheDir.c_str(),"cpinfo");
    LOGI(">>>>>>>>>>JSMarket::getLocalInfo = [%s]",szResult.c_str());
    return szResult;
#elif WIN32

#endif
	return "";
}
void JSMarket::writeLocalInfo( const char* p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::writeLocalInfo=%s,dir=%s",p_sJsonParam,ms_sCacheDir.c_str());
#ifdef ANDROID
	writeMarketData( ms_sCacheDir.c_str(), "cpinfo",p_sJsonParam);
#elif __APPLE__
    writeMarketData( ms_sCacheDir.c_str(), "cpinfo",p_sJsonParam);
#elif WIN32
#endif
}
//------------------------------------------------------------------------------
std::string JSMarket::getLocalToken()
{
	return readMarketData(ms_sCacheDir.c_str(),"token");
}
//------------------------------------------------------------------------------
std::string JSMarket::getLocalOpenid()
{
	return readMarketData(ms_sCacheDir.c_str(),"openid");
}
//------------------------------------------------------------------------------
void JSMarket::writeLocalToken( std::string p_sToken )
{
    LOGI(">>>>>>>>>>JSMarket::writeLocalToken=%s,dir=%s",p_sToken.c_str(),ms_sCacheDir.c_str());
	writeMarketData( ms_sCacheDir.c_str(), "token",p_sToken.c_str());
}
//------------------------------------------------------------------------------
void JSMarket::writeLocalOpenid( std::string p_sOpenid )
{
    LOGI(">>>>>>>>>>JSMarket::writeLocalOpenid=%s,dir=%s",p_sOpenid.c_str(),ms_sCacheDir.c_str() );
    writeMarketData(ms_sCacheDir.c_str(), "openid", p_sOpenid.c_str());
}
void JSMarket::onMarketInit( std::string p_sTokenDir )
{
	LOGI(">>>>>>>>>>JSMarket::onMarketInit=%s",p_sTokenDir.c_str() );
    ms_sCacheDir = p_sTokenDir;
	if( ms_sCacheDir.length() <= 0 )
	{
		ms_sCacheDir = JSConchConfig::getInstance()->getLocalStoragePath();
	}
}
//------------------------------------------------------------------------------
void JSMarket::sendMessageToPlatform( const char* p_sJsonParam,JSValueAsParam p_pCallback  )
{
	LOGI(">>>>>>>>>>JSMarket::sendMessageToMarket p_sJsonParam=%s",p_sJsonParam );
    m_pSendMessageToPlatformFunction.set(sendmessagetoplatformid,this,p_pCallback);
#ifdef ANDROID
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "sendMessageToPlatform", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketSendMessageToPlatform( p_sJsonParam );
#elif WIN32
    
#endif
}
void JSMarket::onSendMessageToPlatform( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onSendMessageToPlatform p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onSendMessageToPlatformCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onSendMessageToPlatformCallJS( std::string p_sJsonParam )
{
	if (!m_pSendMessageToPlatformFunction.Empty())
		m_pSendMessageToPlatformFunction.Call(p_sJsonParam);
}
//------------------------------------------------------------------------------
void JSMarket::openTopicCircle( const char* p_sJsonParam,JSValueAsParam p_pCallback )
{
	clearConchEngineMemory();
    m_pTopicCircleFunction.set(topiccircleid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "openTopicCircle", p_sJsonParam, kRet);
#elif __APPLE__
    CToObjectCMarketOpenTopicCircle( p_sJsonParam );
#elif WIN32

#endif
}
void JSMarket::onTopicCircle( std::string p_sJsonParam )
{
	LOGI(">>>>>>>>>>JSMarket::onTopicCircle p_sMsg=%s",p_sJsonParam.c_str() );
	std::function<void(void)> pFunction = std::bind(&JSMarket::onTopicCircleCallJS, this,p_sJsonParam );
    m_pCmdPoster->postToJS( pFunction );
}
void JSMarket::onTopicCircleCallJS( std::string p_sJsonParam )
{
	if(!m_pTopicCircleFunction.Empty())
		m_pTopicCircleFunction.Call(p_sJsonParam);
}
//------------------------------------------------------------------------------------
void JSMarket::getUserInfo(const char* p_sJsonParam, JSValueAsParam p_pCallback)
{
    m_pGetUserInfoJSFunction.set(getuserinfoid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getUserInfo", p_sJsonParam, kRet);
#elif __APPLE__
	//CToObjectCMarketOpenTopicCircle(p_sJsonParam);
#elif WIN32

#endif
}
void JSMarket::onGetUserInfo(std::string p_sJsonParam)
{
	LOGI(">>>>>>>>>>JSMarket::onGetUserInfo p_sMsg=%s", p_sJsonParam.c_str());
	std::function<void(void)> pFunction = std::bind(&JSMarket::onGetUserInfoCallJS, this, p_sJsonParam);
    m_pCmdPoster->postToJS(pFunction);
}
void JSMarket::onGetUserInfoCallJS(std::string p_sJsonParam)
{
	if(!m_pGetUserInfoJSFunction.Empty())
		m_pGetUserInfoJSFunction.Call(p_sJsonParam);
}
//------------------------------------------------------------------------------------
void JSMarket::getAvailableLoginType(const char* p_sJsonParam, JSValueAsParam p_pCallback)
{
    m_pGetAvailableLoginType.set(getavailablelogintypeid,this,p_pCallback);
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAvailableLoginType", p_sJsonParam, kRet);
#elif __APPLE__
	//CToObjectCMarketOpenTopicCircle(p_sJsonParam);
#elif WIN32

#endif
}
void JSMarket::onGetAvailableLoginType(std::string p_sJsonParam)
{
	LOGI(">>>>>>>>>>JSMarket::onGetAvailableLoginType p_sMsg=%s", p_sJsonParam.c_str());
	std::function<void(void)> pFunction = std::bind(&JSMarket::onGetAvailableLoginTypeCallJS, this, p_sJsonParam);
    m_pCmdPoster->postToJS(pFunction);
}
void JSMarket::onGetAvailableLoginTypeCallJS(std::string p_sJsonParam)
{
	if(!m_pGetAvailableLoginType.Empty())
		m_pGetAvailableLoginType.Call(p_sJsonParam);
}
//--------------------------------------------------------------------------------------
std::string JSMarket::getValue(const char* key)
{
	LOGI(">>>>>>>>>>JSMarket::getValue");
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    if(CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getMarketValue", key, kRet))
	{
		std::string value = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
		LOGI("getValue:=%s", value.c_str());
		return value;
	}
	return "";
#elif __APPLE__
#elif WIN32
#endif
	return "";
}
void JSMarket::setValue(const char* key, const char* value)
{
#ifdef ANDROID
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setMarketValue", key, value, kRet);
#elif __APPLE__
#elif WIN32
#endif
}
//--------------------------------------------------------------------------------------
void JSMarket::setIsClearConchEngineMemory( bool p_bClear )
{
	m_bClearConchMemory = p_bClear;
}
void JSMarket::clearConchEngineMemory()
{
	if( m_bClearConchMemory )
	{
		LOGI(">>>>>>>>>>JSMarket::clearConchEngineMemory()");
//#ifdef JS_V8
//                JSP_RUN_SCRIPT("gc();");
//#elif JS_JSVM
                JSP_RUN_SCRIPT("gc();",nullptr);
//#endif
		if( JCConch::s_pConch ) 
		{
            JCConch::s_pConch->onClearMemory();
		}
	}
}
void JSMarket::openAppStoreUrl( const char* p_sAppID )
{
#ifdef __APPLE__
    CToObjectCOpenAppStoreUrl( p_sAppID );
#elif ANDROID
#elif WIN32
#endif
}
void JSMarket::exportJS()
{
    JSP_GLOBAL_CLASS("conchMarket", JSMarket, JCScriptRuntime::s_JSRT->m_pMarket);
    JSP_GLOBAL_ADD_METHOD("getMarketName", JSMarket::getMarketName);
    JSP_GLOBAL_ADD_METHOD("getLoginType", JSMarket::getLoginType);
    JSP_GLOBAL_ADD_METHOD("setIsClearConchEngineMemory", JSMarket::setIsClearConchEngineMemory);
    JSP_GLOBAL_ADD_METHOD("getEnterPlatformType", JSMarket::getEnterPlatformType);
    JSP_GLOBAL_ADD_METHOD("getIsLogined", JSMarket::getIsLogined);
    JSP_GLOBAL_ADD_METHOD("init", JSMarket::init);
    JSP_GLOBAL_ADD_METHOD("login", JSMarket::login);
    JSP_GLOBAL_ADD_METHOD("logout", JSMarket::logout);
    JSP_GLOBAL_ADD_METHOD("switchUser", JSMarket::switchUser);
    JSP_GLOBAL_ADD_METHOD("enterPlatform", JSMarket::enterPlatform);
    JSP_GLOBAL_ADD_METHOD("enterBBS", JSMarket::enterBBS);
    JSP_GLOBAL_ADD_METHOD("enterFeedback", JSMarket::enterFeedback);
    JSP_GLOBAL_ADD_METHOD("enterAccountMgr", JSMarket::enterAccountMgr);
    JSP_GLOBAL_ADD_METHOD("authorize", JSMarket::authorize);
    JSP_GLOBAL_ADD_METHOD("refreshToken", JSMarket::refreshToken);
    JSP_GLOBAL_ADD_METHOD("cz", JSMarket::CZ);
    JSP_GLOBAL_ADD_METHOD("buyProps", JSMarket::buyProps);
    JSP_GLOBAL_ADD_METHOD("setCZInfo", JSMarket::setCZInfo);
    JSP_GLOBAL_ADD_METHOD("enterShareAndFeed", JSMarket::enterShareAndFeed);
    JSP_GLOBAL_ADD_METHOD("enterInvite", JSMarket::enterInvite);
    JSP_GLOBAL_ADD_METHOD("getGameFriends", JSMarket::getGameFriends);
    JSP_GLOBAL_ADD_METHOD("sendToDesktop", JSMarket::sendToDesktop);
    JSP_GLOBAL_ADD_METHOD("sendMessageToPlatform", JSMarket::sendMessageToPlatform);
    JSP_GLOBAL_ADD_METHOD("canSendToDesktop", JSMarket::canSendToDesktop);
    JSP_GLOBAL_ADD_METHOD("openTopicCircle", JSMarket::openTopicCircle);
    JSP_GLOBAL_ADD_METHOD("getUserInfo", JSMarket::getUserInfo);
    JSP_GLOBAL_ADD_METHOD("getAvailableLoginType", JSMarket::getAvailableLoginType);
    JSP_GLOBAL_ADD_METHOD("getValue", JSMarket::getValue);
    JSP_GLOBAL_ADD_METHOD("setValue", JSMarket::setValue);
    JSP_GLOBAL_ADD_METHOD("writeLocalInfo", JSMarket::writeLocalInfo);
    JSP_GLOBAL_ADD_METHOD("getLocalInfo", JSMarket::getLocalInfo);
    JSP_GLOBAL_ADD_METHOD("openAppStoreUrl", JSMarket::openAppStoreUrl);
    JSP_INSTALL_GLOBAL_CLASS("conchMarket", JSMarket, JCScriptRuntime::s_JSRT->m_pMarket);
}

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
