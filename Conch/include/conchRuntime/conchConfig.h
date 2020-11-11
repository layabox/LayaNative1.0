/**
 @file			conchConfig.h
 @brief         配置用到的，比如版本号 或者描述信息
 @author		James
 @version		1.0
 @date			2013_7_5
 @company       LayaBox
 */
#import "UIKit/UIKit.h"
#import <GLKit/GLKit.h>
#import <string>

@interface conchConfig : NSObject
{
@public
    
    NSString*       m_sPlatformClassName;   //第三方平台的名字
    NSString*       m_sAppVersion;          //对外版本号
    NSString*       m_sAppLocalVersion;     //对内版本号
    NSString*       m_sLaya8Url;            //如果是Laya8启动，启动的url
    std::string     m_sBackgroundcolor;     //背景色
    NSString*       m_sGameID;              //appStroe用到的
    bool            m_bCheckNetwork;        //是否检查网络
    bool            m_bNotification;        //是否打开推送
    bool            m_bShowAssistantTouch;  //是否显示AssitantTouch
    /*
     UIInterfaceOrientationMaskPortrait,             ===2
     UIInterfaceOrientationMaskPortraitUpsideDown,   ===4
     UIInterfaceOrientationMaskLandscapeLeft,        ===8
     UIInterfaceOrientationMaskLandscapeRight,       ===16
     */
    int             m_nOrientationType;     //游戏的方向
    int             m_nLoginType;           //登陆类型 0是没有  1是登陆 2是授权
    int             m_nEnterPlatformType;   //第三方平台的类型
}
+(conchConfig*)GetInstance;
-(bool)readIni;
-(conchConfig*)init;
@end
