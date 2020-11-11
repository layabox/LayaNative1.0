/**
 @file			MarketAppStore
 @brief         市场回调的类
 @author		wyw
 @version		1.0
 @date			2015_9_18
 @company       JoyChina
 */

#import "PlatformInterface/LayaPlatformInterface.h"
#import "../iap/IAPManager.h"

@interface MarketAppStore: NSObject<LayaPlatformInterface,JCIapProcessCtrlDelegate>
{
}
//初始化
-(MarketAppStore*)init;
@end