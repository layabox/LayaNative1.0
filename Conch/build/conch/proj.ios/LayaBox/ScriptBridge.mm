//
//  Test.m
//  LayaBox
//
//  Created by layabox_mac on 16/8/4.
//  Copyright © 2016年 LayaBox. All rights reserved.
//

#import "ScriptBridge.h"
#import <conchRuntime.h>
#import <Foundation/NSString.h>
#import "ViewController.h"
#import "ScanViewController.h"

@implementation ScriptBridge

+ (void) scan
{
    if ([ViewController GetIOSViewController])
    {
        [[ViewController GetIOSViewController].navigationController presentViewController:[[ScanViewController alloc] init] animated:NO completion:nil];
    }
}
+(void) onScanResult:(NSString*)result
{
    [[conchRuntime GetIOSConchRuntime] callbackToJSWithClass:self methodName:@"scan" ret:result];
}
@end

