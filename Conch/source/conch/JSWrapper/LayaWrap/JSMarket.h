/**
@file			JSMarket.h
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

#ifndef __JSMarket_H__
#define __JSMarket_H__

//包含头文件
#include <stdio.h>
#include <string>
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
    class IConchThreadCmdMgr;
class JSMarket :public JsObjBase, public JSObjNode
{
public:
	static JsObjClassInfo JSCLSINFO;
	static void exportJS();

	JSMarket();

	~JSMarket();
public:

	//获得marketName
	std::string getMarketName();

	//登陆模式
	int getLoginType();

	//获得进入平台的类型
	int getEnterPlatformType();

	//能否pushicon
	int canSendToDesktop(const char* p_sJsonParam);

	//设置token目录
	static void onMarketInit( std::string p_sTokenDir );

	//是否已经登陆了，如果登陆了，第二次进入直接刷新token就可以了
	bool getIsLogined();

    //初始化Market
    void init( const char* p_sJsonParam,JSValueAsParam p_pCallback );
    void onInitCallback( std::string p_sJsonParam );
    void onInitCallbackCallJS( std::string p_sJsonParam );
    
    
	//登陆，传入的参数是登陆完成后的回调
	void login( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onLoginCallback( std::string p_sJsonParam );
	void onLoginCallbackCallJS( std::string p_sJsonParam );

	//注销
	void logout( const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onLogoutCallback( std::string p_sJsonParam );
	void onLogoutCallbackCallJS( std::string p_sJsonParam );

	//切换用户
	void switchUser(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onSwitchUser(std::string p_sJsonParam);
	void onSwitchUserCallJS(std::string p_sJsonParam);

	//进入第三方的平台
	void enterPlatform(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onEnterPlatform(std::string p_sJsonParam);
	void onEnterPlatformCallJS(std::string p_sJsonParam);

	//进入bbs
	void enterBBS(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onEnterBBS(std::string p_sJsonParam);
	void onEnterBBSCallJS(std::string p_sJsonParam);

	//反馈
	void enterFeedback(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onEnterFeedback(std::string p_sJsonParam);
	void onEnterFeedbackCallJS(std::string p_sJsonParam);

	//进入账户管理
	void enterAccountMgr(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onEnterAccountMgr(std::string p_sJsonParam);
	void onEnterAccountMgrCallJS(std::string p_sJsonParam);

	//充值
	void CZ( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onCZEvent( std::string p_sJsonParam );
	void onCZEventCallJS( const std::string p_sJsonParam );

	//购买道具
	void buyProps( const char* p_sJsonParam,JSValueAsParam p_sCallback );
	void onBuyPropsCallback( std::string p_sJsonParam );
	void onBuyPropsCallbackCallJS( std::string p_sJsonParam );		
	
	//设置充值信息
	void setCZInfo( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onSetCZInfo(std::string p_sJsonParam);
	void onSetCZInfoCallJS(std::string p_sJsonParam);
    
	//分享
	void enterShareAndFeed( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onShareAndFeedCallback( std::string p_sJsonParam );
	void onShareAndFeedCallbackCallJS( std::string p_sJsonParam );   

	//邀请 p_sMessage,p_sFilters,p_sData
	void enterInvite( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onInviteCallback( std::string p_sJsonParam );
	void onInviteCallbackCallJS( std::string p_sJsonParam );

	//打开话题圈
	void openTopicCircle(const char* p_sJsonParam,JSValueAsParam p_pCallback);
	void onTopicCircle(std::string p_sJsonParam);
	void onTopicCircleCallJS(std::string p_sJsonParam);
	
	//授权
	void authorize( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onAuthorizeCallback( std::string p_sJsonParam );
	void onAuthorizeCallbackCallJS( std::string p_sJsonParam);

	//刷新tooken
	void refreshToken( const char* p_sJsonParam,JSValueAsParam p_pCallback );
    void onRefreshTokenCallback( std::string p_sJsonParam);
    void onRefreshTokenCallbackCallJS( std::string p_sJsonParam);
    
    //获得好友列表
    void getGameFriends( const char* p_sJsonParam,JSValueAsParam p_pCallback );
    void onGetFameFriendsCallback( std::string p_sJsonParam);
    void onGetFameFriendsCallbackCallJS( std::string p_sJsonParam);
    
    //sendToDesktop
    void sendToDesktop( const char* p_sJsonParam,JSValueAsParam p_pCallback );
    void onSendToDesktopCallback( std::string p_sJsonParam);
    void onSendToDesktopCallbackCallJS( std::string p_sJsonParam);

	//发送消息
	void sendMessageToPlatform( const char* p_sJsonParam,JSValueAsParam p_pCallback );
	void onSendMessageToPlatform( std::string p_sJsonParam );
	void onSendMessageToPlatformCallJS( std::string p_sJsonParam);

	//新增getUserInfo getAvailableLoginType
	void getUserInfo(const char* p_sJsonParam, JSValueAsParam p_pCallback);
	void onGetUserInfo(std::string p_sJsonParam);
	void onGetUserInfoCallJS(std::string p_sJsonParam);
	void getAvailableLoginType(const char* p_sJsonParam, JSValueAsParam p_pCallback);
	void onGetAvailableLoginType(std::string p_sJsonParam);
	void onGetAvailableLoginTypeCallJS(std::string p_sJsonParam);

	//新增getVaule setValue
	std::string getValue(const char* key);
	void setValue(const char*key, const char* value);
    
    void openAppStoreUrl( const char* p_sAppID );
    
public:
	
    std::string  getLocalInfo();

	void writeLocalInfo(const char* p_sJsonParam);

    std::string getLocalToken();
    
    std::string getLocalOpenid();
    
    void writeLocalToken( std::string p_sToken );
    
    void writeLocalOpenid( std::string p_sOpenid );

	void setIsClearConchEngineMemory( bool p_bClear );

private:

	void clearConchEngineMemory();

public:
	enum{initjsid,loginid,logoutid,feedandshareid,inviteid,rechargeid,buypropsid,authorizeid,refreshtokenid,
	getfriendsid,sendtodesktopid,topiccircleid,enterfeedbackid,enterbbsid,enterplatformid,
	switchuserid,enteraccountmgrid,setrechargeinfoid,sendmessagetoplatformid,
	iosiapid,getuserinfoid,getavailablelogintypeid};

    JsObjHandle             m_pInitJSFunction;
	JsObjHandle				m_pLoginJSFunction;
    JsObjHandle				m_pLogoutJSFunction;
	JsObjHandle				m_pFeedAndShareJSFunction;
	JsObjHandle				m_pInviteJSFunction;
	JsObjHandle				m_pCZJSFunction;
	JsObjHandle				m_pBuyPropsJSFunction;
	JsObjHandle				m_pAuthorizeJSFunction;
	JsObjHandle				m_pRefreshTokenJSFunction;
	JsObjHandle             m_pGetFriendsJSFunction;
	JsObjHandle             m_pSendToDesktopJSFunction;
	JsObjHandle             m_pTopicCircleFunction;
	JsObjHandle             m_pEnterFeedbackJSFunction;
	JsObjHandle             m_pEnterBBSJSFunction;
	JsObjHandle             m_pEnterPlatformJSFunction;
	JsObjHandle             m_pSwitchUserJSFunction;
	JsObjHandle				m_pEnterAccountMgrJSFunction;
	JsObjHandle				m_pSetCZInfoJSFunction;
    JsObjHandle				m_pSendMessageToPlatformFunction;
	JsObjHandle				m_pGetUserInfoJSFunction;
	JsObjHandle				m_pGetAvailableLoginType;
    
public:
    static std::string      ms_sCacheDir;
	bool					m_bClearConchMemory;
    std::string             m_sMarketName;
    IConchThreadCmdMgr*     m_pCmdPoster;
};
}

#endif //__JSMarket_H__

//-----------------------------END FILE--------------------------------
