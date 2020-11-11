/**
 @file			LayaPlatformInterface
 @brief         Interface
 @author		James
 @version		1.0
 @date			2013_10_11
 @company       LayaBox
 */

#import "UIKit/UIKit.h"

enum MarketState
{
    MS_NORMAL,              //无效状态
    MS_QQ_REFRESH_TOKEN,    //QQ刷新token
    MS_QQ_AUTHORIZE,        //QQ授权
    MS_QQ_SHARE,            //QQ分享
    MS_WX_REFRESH_TOKEN,    //WX刷新token
    MS_WX_AUTHORIZE,        //WX授权
    MS_WX_SHARE,            //WX分享
};

@protocol LayaPlatformInterface

-(void)LP_Init;
//平台初始化
-(void)LP_Init:(NSString*)jsonParam;
//获得marketName
-(NSString*)getMarketName;
//获得登陆方式 0为自己做的登录  1为第三方平台登录   2为授权的方式
-(int)getLoginType;
//获得平台的类型
-(int)getEnterPlatformType;
//平台登录
-(void)LP_Login:(NSString*)jsonParam;
//平台切换用户
-(void)LP_SwitchUser:(NSString*) jsonParam;
//平台登录退出
-(void)LP_Logout:(NSString*) jsonParam;
//平台充值
-(void)LP_CZ:(NSString*) jsonParam;
//进入社区
-(void)LP_EnterPlatform:(NSString*) jsonParam;
//退出社区
-(void)LP_LeavePlatform:(NSString*) jsonParam;
//进入游戏，或者进入大厅
-(void)LP_onGameEvent:(NSString*) jsonParam;
//进入BBS
-(void)LP_enterBBS:(NSString*) jsonParam;
//进入游戏反馈
-(void)LP_enterFeedback:(NSString*) jsonParam;
//暂停的处理，当从后台转为前台的时候处理
-(void)LP_onPause:(NSString*) jsonParam;
//退出的处理函数  如果没有处理一定返回false 否则无法退出
-(bool) LP_onExit:(NSString*) jsonParam;
//充值的时候设置的信息
-(void)LP_setCZInfo:(NSString*) jsonParam;
//账户管理
-(void)LP_enterAccountMgr:(NSString*) jsonParam;
//购买道具
-(void)LP_gmProps:(NSString*) jsonParam;
//分享
-(void)LP_enterShareAndFeed:(NSString*) jsonParam;
//的邀请
-(void)LP_enterInvite:(NSString*) jsonParam;
//授权
-(void)LP_authorize:(NSString*) jsonParam;
//刷新token
-(void)LP_RefreshToken:(NSString*) jsonParam;
//获得好友列表
-(void)LP_getGameFriends:(NSString*) jsonParam;
//发送桌面图标
-(void)LP_sendToDesktop:(NSString*) jsonParam;
//能否发送到桌面
-(int)LP_canSendToDesktop:(NSString*) jsonParam;
//发送消息给平台
-(void)LP_sendMessageToPlatform:(NSString*) jsonParam;
//打开话题圈
-(void)LP_openTopicCircle:(NSString*) jsonParam;
//回调
-(void)LP_onResp:(id)resp;
//设置状态
-(void)LP_setMarketState:(MarketState)p_nState;
//获得是否安装了微信
-(bool)LP_getIsInstalledWX;

@end
