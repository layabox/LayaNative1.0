#import "JSBridge.h"
#import "AppDelegate.h"
@implementation JSBridge

+(void)hideSplash
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    [appDelegate.launchView hide];
}
+(void)setTips:(NSArray*)tips
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    appDelegate.launchView.tips = tips;
}
+(void)setFontColor:(NSString*)color
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    [appDelegate.launchView setFontColor:color];
}
+(void)bgColor:(NSString*)color
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    [appDelegate.launchView setBackgroundColor:color];
}
+(void)loading:(NSNumber*)percent
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    [appDelegate.launchView setPercent:percent.integerValue];
}
+(void)showTextInfo:(NSNumber*)show
{
    AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
    [appDelegate.launchView showTextInfo:show.intValue > 0];
}
@end

