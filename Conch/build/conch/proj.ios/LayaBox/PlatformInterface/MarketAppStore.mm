/**
 @file			MarketAppStroe.mm
 @brief         AppStroe版本的充值
 @author		wyw
 @version		1.0
 @date			2015_9_18
 @company       JoyChina
 */
#import "MarketAppStore.h"
#import "PlatformInterface/LayaPlatformCallback.h"
#import "PlatformInterface/LayaPlatformFactory.h"
#import "../ViewController.h"
#import "conchConfig.h"

@implementation MarketAppStore

-(MarketAppStore*)init
{
    self = [super init];
    NSString* sRootCachePath = [[conchRuntime GetIOSConchRuntime] getRootCachePath];
    NSString* sMarketCacheDir = [NSString stringWithFormat:@"%@/marketInfo/",sRootCachePath];
    [[LayaPlatformFactory GetInstance] setCacheDir:[sMarketCacheDir cStringUsingEncoding:NSUTF8StringEncoding]];
    return self;
}
-(void)	LP_Init
{
    [[IAPManager getInstance] initWithGameID:[conchConfig GetInstance]->m_sGameID andDelegate:self];
}
-(void)	LP_Init:(NSString*)jsonParam
{
}
-(NSString*)getMarketName
{
    return [conchConfig GetInstance]->m_sPlatformClassName;
}
-(int)getLoginType
{
    return [conchConfig GetInstance]->m_nLoginType;
}
-(int)getEnterPlatformType
{
    return [conchConfig GetInstance]->m_nEnterPlatformType;
}
-(void)LP_Login:(NSString*)jsonParam
{
	NSLog(@"this is MarketAppStore method login");
}
-(void)LP_SwitchUser:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method SwitchUser");
}
-(void)LP_Logout:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method logout");
}
-(void)LP_CZ:(NSString*) jsonParam
{
    [[IAPManager getInstance] recharge:jsonParam];
}
-(void)LP_EnterPlatform:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method EnterPlatform");
}
-(void)LP_LeavePlatform:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method LeavePlatform");
}
-(void)LP_onGameEvent:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method onGameEvent");
}
-(void)LP_enterBBS:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method enterBBS");
}
-(void)LP_enterFeedback:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method enterFeedback");
}
-(void)LP_onPause:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method onPause");
}
-(bool) LP_onExit:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method onExit");
    return true;
}
-(void)LP_setCZInfo:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method setRechargetInfo");
}
-(void)LP_enterAccountMgr:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method enterAccountMgr");
}
-(void)LP_gmProps:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method BuyProps");
}
-(void)LP_enterShareAndFeed:(NSString*) jsonParam
{
}
-(void)LP_enterInvite:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method enterInvite");
}
-(void)LP_authorize:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method authorize");
}
-(void)LP_RefreshToken:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method refreshToken");
}
-(void)LP_getGameFriends:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method getGameFriends");
}
-(void)LP_sendToDesktop:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method sendToDesktop");
}
-(int)LP_canSendToDesktop:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method canSendToDesktop");
    return 1;
}
-(void)LP_sendMessageToPlatform:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method sendMessageToPlatform");
}
-(void)LP_openTopicCircle:(NSString*) jsonParam
{
	NSLog(@"this is MarketAppStore method openTopicCircle");
}

-(void)LP_onResp:(id)resp;
{
}
-(void)LP_setMarketState:(MarketState)p_nState
{
    
}
- (void)onSuccess:(NSString *)p_pProductionIdentifier quantity:(NSInteger)p_iNumber orderID:(NSString *)p_sOrderID
{
    NSString* pJsonString = @"{  \"order_id\" : \"error\",  \"amount\" : \"0\",  \"code\" : \"-2\", \"desc\" : \"error\",  \"product_id\" : \"error\"}";
    NSString* sNum = [NSString stringWithFormat:@"%d",p_iNumber ];
    NSDictionary* pDictionary = [NSDictionary dictionaryWithObjectsAndKeys:@"0",@"code",p_pProductionIdentifier?p_pProductionIdentifier:@"error",@"product_id",sNum,@"amount",p_sOrderID?p_sOrderID:@"error", @"order_id",@"Success",@"desc",nil];
    NSError* pError = nil;
    NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:pDictionary options:NSJSONWritingPrettyPrinted error:&pError];
    if( !pError )
    {
        pJsonString = [[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding];
    }
    [[LayaPlatformCallback GetInstance]LP_onCZCallback:pJsonString];
    [[LayaPlatformCallback GetInstance]LP_gmPropsCallback:pJsonString];
}

- (void)onFailed:(NSString *)p_pProductionIdentifier quantity:(NSInteger)p_iNumber orderID:(NSString *)p_sOrderID
{
    NSString* pJsonString = @"{  \"order_id\" : \"error\",  \"amount\" : \"0\",  \"code\" : \"-2\", \"desc\" : \"error\",  \"product_id\" : \"error\"}";
    
    NSString* sNum = [NSString stringWithFormat:@"%d",p_iNumber ];
    NSDictionary* pDictionary = [NSDictionary dictionaryWithObjectsAndKeys:@"-1",@"code",p_pProductionIdentifier?p_pProductionIdentifier:@"error",@"product_id",sNum,@"amount",p_sOrderID?p_sOrderID:@"error", @"order_id",@"error",@"desc",nil];
    NSError* pError = nil;
    NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:pDictionary options:NSJSONWritingPrettyPrinted error:&pError];
    if( !pError )
    {
        pJsonString = [[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding];
    }
    [[LayaPlatformCallback GetInstance]LP_onCZCallback:pJsonString];
    [[LayaPlatformCallback GetInstance]LP_gmPropsCallback:pJsonString];
}
@end
