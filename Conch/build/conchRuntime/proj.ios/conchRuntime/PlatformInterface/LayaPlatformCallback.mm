/**
 @file			LayaPlatformCallback.mm
 @brief         市场的回调类
 @author		James
 @version		1.0
 @date			2013_10_11
 @company       LayaBox
 */

#import "LayaPlatformCallback.h"
#import "../../../../../source/conch/JSWrapper/LayaWrap/JSMarket.h"
#import "../../../../../source/conch/JCScrpitRuntime.h"
#import <util/JCCommonMethod.h>

//-------------------------------------------------------------------------------
static LayaPlatformCallback* g_pPlatformCallback = nil;
//-------------------------------------------------------------------------------
@implementation LayaPlatformCallback
//-------------------------------------------------------------------------------
-(id) init
{
    self = [super init];
    return self;
}
//-------------------------------------------------------------------------------
+(id) GetInstance
{
    if( g_pPlatformCallback == nil )
    {
        g_pPlatformCallback = [[LayaPlatformCallback alloc] init];
    }
    return g_pPlatformCallback;
}
//-------------------------------------------------------------------------------
-(void) LP_onMarketInitCallback:(NSString*)jsonParam tokenDir:(NSString*)p_sTokenDir;
{
    const char* sValue = [p_sTokenDir cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JSMarket::onMarketInit( sValue );
    if( jsonParam )
    {
        std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
        laya::JCScriptRuntime::s_JSRT->m_pMarket->onInitCallback( sBuffer.c_str() );
    }
}
-(void) LP_LoginCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onLoginCallback( sBuffer.c_str() );
}
-(void) LP_onCZCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onCZEvent( sBuffer.c_str() );
}
-(void) LP_ShareAndFeedCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onShareAndFeedCallback( sBuffer.c_str() );
}
-(void) LP_onAuthorizeCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onAuthorizeCallback( sBuffer.c_str() );
}
-(void) LP_onRefreshTokenCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onRefreshTokenCallback( sBuffer.c_str() );
}
-(void) LP_onGetGameFriendsCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onGetFameFriendsCallback( sBuffer.c_str() );
}
-(void) LP_onSendToDesktopCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onSendToDesktopCallback( sBuffer.c_str() );
}
-(void) LP_onLogoutCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onLogoutCallback( sBuffer.c_str() );
}
-(void) LP_onTopicCircleCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onLogoutCallback( sBuffer.c_str() );
}
-(void) LP_InviteCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onInviteCallback( sBuffer.c_str() );
}
-(void) LP_SwitchUserCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onSwitchUser( sBuffer.c_str() );
}
-(void) LP_EnterPlatformCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onEnterPlatform( sBuffer.c_str() );
}
-(void) LP_EnterBBSCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onEnterBBS( sBuffer.c_str() );
}
-(void) LP_EnterFeedbackCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onEnterFeedback( sBuffer.c_str() );
}
-(void) LP_EnterAccountMgrCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onEnterAccountMgr( sBuffer.c_str() );
}
-(void) LP_gmPropsCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onBuyPropsCallback( sBuffer.c_str() );
}
-(void) LP_SetCZInfoCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onSetCZInfo( sBuffer.c_str() );
}
-(void) LP_SendMessageToPlatformCallback:(NSString*) jsonParam
{
    std::string sBuffer = [jsonParam cStringUsingEncoding:NSUTF8StringEncoding];
    laya::JCScriptRuntime::s_JSRT->m_pMarket->onSendMessageToPlatform( sBuffer.c_str() );
}

@end
