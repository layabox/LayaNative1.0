//
//  Test.h
//  LayaBox
//
//  Created by layabox_mac on 16/8/4.
//  Copyright © 2016年 LayaBox. All rights reserved.
//
#import <Foundation/NSObject.h>

@interface ScriptBridge: NSObject
+(void) scan;
+(void) onScanResult:(NSString*)result;
@end

