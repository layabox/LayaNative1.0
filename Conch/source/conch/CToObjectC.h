/**
@file			CToObjectC.h
@brief			
@author			wyw
@version		1.0
@date			2014_8_26
*/

#ifndef _CToObjectC_H_
#define _CToObjectC_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <JavaScriptCore/JSBase.h>
#include "resource/Audio/JCAudioInterface.h"
#include "resource/Audio/JCWaveInfo.h"
#include <functional>

void CToObjectCPostMainThread(int cmd,int param1,int param2);

char* CToObjectCGetFontBuffer(long& dataSize);

void CToObjectCPostEditBox(int cmd,int param1,int param2=0,bool bparam2=true,const char* sparam3=NULL);

void ObjectCOperateEditBox(int cmd,int param1,int param2=0,bool bparam2=true,std::string sparam3="");

void CToObjectCPostMarket(int cmd,const char* sparam );

void ObjectCOperateMarket(int cmd,const std::string sparam );

void CToObjectCAlert(const char* message);

//以下是EditBox相关的
//------------------------------------------------------------------------------
enum IOSEditBoxOperator
{
    IOS_EDITBOX_SETPOSX = 1,
    IOS_EDITBOX_SETPOSY,
    IOS_EDITBOX_SETWIDTH,
    IOS_EDITBOX_SETHEIGHT,
    IOS_EDITBOX_SETVALUE,
    IOS_EDITBOX_SETSTYLE,
    IOS_EDITBOX_SETVISIBLE,
    IOS_EDITBOX_SETFOCUS,
    IOS_EDITBOX_SETBLUR,
    IOS_EDITBOX_SETCOLOR,
    IOS_EDITBOX_SETFONTSIZE,
    IOS_EDITBOX_SETFONTPOS,
    IOS_EDITBOX_SETFONTSIZE2,
    IOS_EDITBOX_SETCURSORPOSITION,
    IOS_EDITBOX_SETMAXLENGTH,
    IOS_EDITBOX_SETPASSWORD,
    IOS_EDITBOX_SETREGULAR,
    IOS_EDITBOX_SETNUMBERONLY,
    IOS_EDITBOX_SETMULTIABLE,
    IOS_EDITBOX_SETFORBIDEDIT,
};
enum IOSMarketOperator
{
    IOS_MARKET_INIT = 1,
    IOS_MARKET_LOGIN,
    IOS_MARKET_SWITCHUSER,
    IOS_MARKET_LOGOUT,
    IOS_MARKET_PAY,
    IOS_MARKET_ENTERPLATFORM,
    IOS_MARKET_LEAVEPLATFORM,
    IOS_MARKET_ONGAMEEVENT,
    IOS_MARKET_ENTERBBS,
    IOS_MARKET_ENTERFREEBACK,
    IOS_MARKET_ONPAUSE,
    IOS_MARKET_ONEXIT,
    IOS_MARKET_SETRECHARGEINFO,
    IOS_MARKET_ENTERACCOUNTMGR,
    IOS_MARKET_BUYPROPS,
    IOS_MARKET_ENTERSHAREANDFEED,
    IOS_MARKET_ENTERINVITE,
    IOS_MARKET_AUTHORIZE,
    IOS_MARKET_REFRESHTOKEN,
    IOS_MARKET_GETGAMEFRIENDS,
    IOS_MARKET_SENDTODESKTOP,
    IOS_MARKET_SENDMESSAGETOPLATFORM,
    IOS_MARKET_OPENTOPICCIRCLE,
};
static const int IOS_EDITBOX_POSX = 1;

void IOS_SetEditBoxStyle(const char* p_sType);

void CToObjectCSetEditBoxX( int p_nX );

void CToObjectCSetEditBoxY( int p_nY );

void CToObjectCSetEditBoxWidth( int p_nWidth );

void CToObjectCSetEditBoxHeight( int p_nHeight );

void CToObjectCSetEditBoxValue( const char* p_sValue );

void CToObjectCSetEditBoxStyle( const char* p_sType );

void CToObjectCSetEditBoxVisible( bool p_bVisible );

void CToObjectCSetEditBoxFocus();

void CToObjectCSetEditBoxBlur();

void CToObjectCSetEditBoxColor( int p_nColor );

void CToObjectCSetEditBoxFontSize( int p_nFontSize );

void CToObjectCSetEditBoxFontPos( int p_nX,int p_nY );

void CToObjectCSetEditBoxFontSize( int p_nWidth,int p_nHeight );

void CToObjectCSetEditBoxCursorPosition( int p_nPos );

void CToObjectCSetEditBoxMaxLength( int p_nMaxLength );

void CToObjectCSetEditBoxPassword( bool p_bPassword );

void CToObjectCSetEditBoxRegular( const char* p_sRegular );

void CToObjectCSetEditBoxNumberOnly( bool p_bNumberOnly );

void CToObjectCSetEditBoxForbidEdit( bool p_bForbidEdit );

const char* CToObjectCGetEditBoxValue();

void CToObjectCSetEditBoxMultiAble(bool p_bMultiAble);

//以下是Market相关的
//------------------------------------------------------------------------------
//获得marketName
std::string CToObjectCGetMarketName();
//获得登陆方式
int CToObjectCGetLoginType();
//获得平台的类型
int CToObjectCGetEnterPlatformType();
//平台初始化
void CToObjectCMarketInit( const char* jsonParam );
//平台登录
void CToObjectCMarketLogin( const char* jsonParam );
//平台切换用户
void CToObjectCMarketSwitchUser( const char* jsonParam );
//平台登录退出
void CToObjectCMarketLogout( const char* jsonParam );
//平台代币充值 sProjectName根据自己需要填写
void CToObjectCMarketCZ( const char* jsonParam );
//社区
void CToObjectCMarketEnterPlatform( const char* jsonParam );
//退出社区
void CToObjectCMarketLeavePlatform( const char* jsonParam );
//进入游戏，或者进入大厅
void CToObjectCMarketOnGameEvent( const char* jsonParam );
//进入BBS
void CToObjectCMarketEnterBBS( const char* jsonParam );
//进入游戏反馈
void CToObjectCMarketEnterFeedback( const char* jsonParam );
//暂停的处理，当从后台转为前台的时候处理
void CToObjectCMarketOnPause( const char* jsonParam );
//退出的处理函数  如果没有处理一定返回false 否则无法退出
bool CToObjectCMarketOnExit( const char* jsonParam );
//充值的时候设置的信息
void CToObjectCMarketSetCZInfo( const char* jsonParam );
//账户管理
void CToObjectCMarketEnterAccountMgr( const char* jsonParam );
//购买道具
void CToObjectCMarketBuyProps( const char* jsonParam );
//分享
void CToObjectCMarketEnterShareAndFeed( const char* jsonParam );
//facebook的邀请
void CToObjectCMarketEnterInvite( const char* jsonParam );
//授权
void CToObjectCMarketAuthorize( const char* jsonParam );
//刷新token
void CToObjectCMarketRefreshToken( const char* jsonParam );
//获得好友列表
void CToObjectCMarketGetGameFriends( const char* jsonParam );
//发送桌面图标
void CToObjectCMarketSendToDesktop( const char* jsonParam );
//能否发送到桌面
int CToObjectCMarketCanSendToDesktop( const char* jsonParam );
//发送消息给平台
void CToObjectCMarketSendMessageToPlatform( const char* jsonParam );
//打开话题圈
void CToObjectCMarketOpenTopicCircle( const char* jsonParam );

//------------------------------------------------------------------------------
//以下是声音相关的
//------------------------------------------------------------------------------
void CToObjectCPlayMp3Audio( const char* p_sUrl,int p_nTimes,float nCurrentTime );
void CToObjectCSetMp3Volume( float p_nVolume );
void CToObjectCStopMp3();
void CToObjectCResumeMp3();
void CToObjectCPauseMp3();

//------------------------------------------------------------------------------
//JSC比较恶心，必须在ObjectC层开启和关闭js线程的循环
void CToObjectCRunJSLoop();
void CToObjectCRunStopJSLoop();
void CToObjectCPostFunc(std::function<void(void)> func);
//------------------------------------------------------------------------------
//本地推送
void CToObjectCSetRepeatNotify( int p_nID,long p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
void CToObjectCSetOnceNotify( int p_nID,long p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
void CToObjectCDeleteOnceNotify( int p_nID );
void CToObjectCDeleteAllNotify();

//------------------------------------------------------------------------------
//以下是一些杂项
//------------------------------------------------------------------------------
//获得总的内存数
long CToObjectCGetTotalMem();
long CToObjectCGetUsedMem();
long CToObjectCGetAvalidMem();
float CToObjectCGetScreenInch();
void CToObjectCSetScreenOrientation( int p_nType );
long CToObjectCGetScreenOrientation();
int CToObjectCGetNetworkType();
std::string CToObjectCGetGUID();
std::string CToObjectCGetDeviceModel();
std::string CToObjectCGetDeviceInfo();
float CToObjectCGetDeviceSystemVersion();
std::string CToObjectCGetAppVersion();
std::string CToObjectCGetAppLocalVersion();
void CToObjectCSetExternalLink( const char* p_sUrl,int x,int y,int w,int h,bool bShowCloseButton );
void CToObjectCCloseExternalLink();
void CToObjectCSetScreenWakeLock( bool p_bWakeLock );
void CToObjectCShowAssistantTouch( bool p_bShow );
void CToObjectCShowToast( const char* p_sInfo );
void CToObjectCOpenAppStoreUrl( const char* p_sAppID );
bool CToObjectCGetIsInstalledWx();
void CToObjectCCallMethod(int objid,bool isSync, const char*clsName, const char* methodName, const char* paramStr);//
void CToObjectCCallWebviewJS(const char* functionName, const char* jsonParam, const char* callback);
void CToObjectCShowWebView();
void CToObjectCHideWebView();
void CToObjectCSetSensorAble(bool p_bAble);
void CToObjectCRunJS(const std::string& script);
void CToObjectCCaptureScreen();
#endif //_CToObjectC_H_

//-----------------------------END FILE--------------------------------
