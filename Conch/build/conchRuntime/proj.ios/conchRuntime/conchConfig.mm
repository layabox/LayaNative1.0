/**
 @file			conchConfig.h
 @brief         配置用到的，比如版本号 或者描述信息
 @author		wyw
 @version		1.0
 @date			2013_7_5
 @company       JoyChina
 */

#import "conchConfig.h"
#import <util/JCIniFile.h>
#import <string>
#import "conchRuntime.h"

//-------------------------------------------------------------------------------
static conchConfig* g_pConchConfig = nil;
//-------------------------------------------------------------------------------
@implementation conchConfig
//-------------------------------------------------------------------------------
+(conchConfig*)GetInstance
{
    if( g_pConchConfig == nil )
    {
        g_pConchConfig = [[conchConfig alloc] init];
    }
    return g_pConchConfig;
}
//-------------------------------------------------------------------------------
-(conchConfig*)init
{
    self = [super init];
    m_sPlatformClassName=nil;   //第三方平台的名字
    m_sLaya8Url=nil;            //如果是Laya8启动，启动的url
    m_sBackgroundcolor="#FFFFFF";//背景色
    m_sGameID=nil;              //appStroe用到的
    m_bCheckNetwork=true;       //是否检查网络
    m_bNotification=false;      //是否打开消息推送
    m_nOrientationType = 30;   //屏幕的方向
    m_nLoginType=0;            //登陆类型 0是没有  1是登陆 2是授权
    m_nEnterPlatformType=0;    //平台的类型
    m_bShowAssistantTouch = false;
    [self readIni];
    m_sAppVersion=nil;          //版本号
    m_sAppLocalVersion = nil;   //对内版本号
    NSDictionary* infoDictionary =  [[NSBundle mainBundle] infoDictionary];
    // 当前应用软件版本 Bundle versions string, short
    m_sAppVersion = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    NSLog(@"当前应用软件版本:%@",m_sAppVersion);
    // 当前应用版本号码 Bundle versions
    m_sAppLocalVersion = [infoDictionary objectForKey:@"CFBundleVersion"];
    NSLog(@"当前应用Local版本号码：%@",m_sAppLocalVersion);
    return self;
}
//-------------------------------------------------------------------------------
-(bool) readIni
{
    std::string sIniFileName = [[self getResourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
    sIniFileName += "/config.ini";
    // 初始化 IAP
    laya::JCIniFile *pConfigFile = laya::JCIniFile::loadFile( sIniFileName.c_str() );
    
    if( 0 == pConfigFile )
    {
        return false;
    }
    else
    {
        const char* sGameID=pConfigFile->GetValue("gameID");
        const char* sPlatformClassName=pConfigFile->GetValue("platformClassName");
        const char* sBackgroundColor=pConfigFile->GetValue("backgroundColor");
        const char* sCheckNetwork=pConfigFile->GetValue("checkNetwork");
        const char* sOrientation=pConfigFile->GetValue("orientation");
        const char* sLoginType = pConfigFile->GetValue("loginType");
        const char* sEnterPlatformType = pConfigFile->GetValue("enterPlatformType");
        const char* sLaya8Url = pConfigFile->GetValue("Laya8Url");
        const char* sNotification = pConfigFile->GetValue("notification");
        const char* sAssistantTouch = pConfigFile->GetValue("assistantTouch");
        if( sGameID )
        {
            m_sGameID = [[NSString alloc] initWithUTF8String:sGameID ];
        }
        else
        {
            NSLog(@"读取ini gameID 错误");
        }
        if( sPlatformClassName )
        {
            m_sPlatformClassName = [[NSString alloc]initWithUTF8String:sPlatformClassName];
        }
        else
        {
            NSLog(@"读取ini PlatformClassName 错误");
        }
        if( sBackgroundColor )
        {
            m_sBackgroundcolor = sBackgroundColor;
        }
        else
        {
            NSLog(@"读取ini backgroundColor 错误");
        }
        if( sCheckNetwork )
        {
            m_bCheckNetwork = atoi(sCheckNetwork)>0;
        }
        else
        {
            NSLog(@"读取ini checkNetworkd 错误");
        }
        if( sOrientation )
        {
            m_nOrientationType = atoi(sOrientation);
            if( m_nOrientationType < 1 )
            {
                NSLog(@"读取ini orientation 错误");
            }
        }
        else
        {
            NSLog(@"读取ini orientation错误");
        }
        
        if( sLoginType )
        {
            m_nLoginType = atoi(sLoginType);
        }
        else
        {
            NSLog(@"读取ini loginType错误");
        }
        if( sEnterPlatformType )
        {
            m_nEnterPlatformType = atoi(sEnterPlatformType);
        }
        else
        {
            NSLog(@"读取ini enterPlatformType错误");
        }
       
        if( sNotification )
        {
            m_bNotification = atoi(sNotification)>0;
        }
        else
        {
            NSLog(@"读取ini notification 错误");
        }
        if( sAssistantTouch )
        {
            m_bShowAssistantTouch = atoi(sAssistantTouch)>0;
        }
        else
        {
            NSLog(@"读取ini assistantTouch 错误");
        }
        if(sLaya8Url)
        {
            m_sLaya8Url = [[NSString alloc]initWithUTF8String:sLaya8Url];
        }
        else
        {
            NSLog(@"读取ini Laya8Url错误");
        }
        delete pConfigFile;
        pConfigFile = NULL;
    }
    return true;
}
//------------------------------------------------------------------------------
-(NSString*) getResourcePath
{
    return [[NSBundle mainBundle] resourcePath];
}
@end
