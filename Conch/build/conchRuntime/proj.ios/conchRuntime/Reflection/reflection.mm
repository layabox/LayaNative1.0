#import "refection.h"
//#import "../../../../source/conch/CToObjectC.h"
//#import "CToObjectCIOS.h"
#import "../../../../source/conch/JCScrpitRuntime.h"
#import "JCThreadCmdMgr.h"

extern void reflectionCallback(const std::string& jsonret);

@implementation Reflection
{
    NSMutableDictionary*        m_pIDToObjectDic;
    NSMutableDictionary*        m_pObjectToIDDic;
}
-(id)init
{
    self = [super init];
    if( self != nil )
    {
        m_pIDToObjectDic = [NSMutableDictionary dictionary];
        m_pObjectToIDDic = [NSMutableDictionary dictionary];
        return self;
    }
    return nil;
}
-(void)clearReflectionObjects
{
    [m_pIDToObjectDic removeAllObjects];
    [m_pObjectToIDDic removeAllObjects];
}
-(void)invoke: (Class) objc_class isStatic:(BOOL) isStatic target:(id)target select:(SEL)select Param:(NSString*)param
{
    NSMethodSignature* signature = isStatic ? [objc_class methodSignatureForSelector:select] : [objc_class instanceMethodSignatureForSelector:select];
    if (signature == nil){
        NSLog(@"reflection error: can not find method signature");
        return;
    }
    
    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
    if (invocation == nil){
        NSLog(@"reflection error: can not find invocation");
        return;
    }
    
    [invocation setTarget:target];
    [invocation setSelector:select];
    
    
    NSUInteger numArgs = [signature numberOfArguments];
    
    NSData* jsonData = [param dataUsingEncoding:NSUTF8StringEncoding];
    if (jsonData == nil){
        NSLog(@"reflection error");
        return;
    }
    
    NSError* error = nil;
    id idPara = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
    NSLog(@"%@   [%@]",[idPara class],param);
    if (idPara == nil || ![idPara isKindOfClass:[NSArray class]]){
        NSLog(@"reflection error");
        return;
    }
    
    NSArray* paraArray = idPara;
    NSUInteger num = [paraArray count];
    
    if (numArgs != num + 2){
        NSLog(@"reflection error: argument number is [%lu] but need [%lu]",(unsigned long)num,(unsigned long)(numArgs-2));
        return;
    }
    
    
    for(int i = 2; i < num + 2; i++){
        NSObject* obj = [paraArray objectAtIndex:i-2];
        [invocation setArgument:&obj atIndex:i];
    }
    
    [invocation invoke];

}

-(void)callMethod:(int)objid className:(NSString*)cls methodName:(NSString*)method param:(NSString*)param
{
    NSLog(@"2 [%i] [%@] [%@] [%@]",objid,cls,method,param);
    
    Class objc_class = NSClassFromString(cls);
    if (objc_class == nil){ 
        NSLog(@"reflection error : can not find class [%@]",cls);
        return;
    }
    
    if (objid == -1){
            
            SEL select = NSSelectorFromString(method);
            if (select == 0){
                NSLog(@"reflection error");
                return;
            }
            [self invoke:objc_class isStatic:TRUE target:objc_class select:select Param:param];
    }
    else{
        
        if ([method isEqualToString:@"<init>"]){//构造函数
            NSObject* object = [[objc_class alloc] init];
            if (object == nil){
                NSLog(@"reflection error : alloc init class [%@] object failed",cls);
                return;
            }
            [m_pIDToObjectDic setObject:object forKey:[NSNumber numberWithInt:objid]];
            [m_pObjectToIDDic setObject:[NSNumber numberWithInt:objid] forKey:[NSNumber numberWithInteger:(NSInteger)object]];
        }
        else{
            NSObject* object = [m_pIDToObjectDic objectForKey:[NSNumber numberWithInt:objid]];
        
            if (object == nil){
                NSLog(@"reflection error : can not find object id [%i]",objid);
                return;
            }
            
            if (![object isKindOfClass:objc_class]){
                NSLog(@"reflection error : object of id [%i] is not king of class [%@]",objid,cls);
                return;
            }
            
            SEL select = NSSelectorFromString(method);
            if (select == 0){
                NSLog(@"reflection error");
                return;
            }
            [self invoke:objc_class isStatic:FALSE target:object select:select Param:param];
        }
    }
    return;
}

-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    [self callbackToJSWithClassName:NSStringFromClass(cls) methodName:name ret:retObj];
}

-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    NSMutableDictionary* dic = [NSMutableDictionary dictionary];
    
    [dic setObject:@-1 forKey:@"objId"];
    [dic setObject:cls forKey:@"cName"];
    [dic setObject:name forKey:@"mName"];
    if (retObj == nil)
        [dic setObject:@"" forKey:@"v"];
    else
        [dic setObject:retObj forKey:@"v"];
    
    NSError* error = nil;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&error];
    if (error != nil){
        NSLog(@"callbackToJS error");
        return;
    }
    const char* jason = [[[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding] UTF8String];
    auto pFunction = std::bind(&reflectionCallback,std::string(jason));
    
    if (JCScriptRuntime::s_JSRT->m_pPoster){
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
}
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj
{
    NSMutableDictionary* dic = [NSMutableDictionary dictionary];
    
    [dic setObject:[m_pObjectToIDDic objectForKey:[NSNumber numberWithInteger:(NSInteger)obj]] forKey:@"objId"];
    [dic setObject:@"" forKey:@"cName"];
    [dic setObject:name forKey:@"mName"];
    if (retObj == nil)
        [dic setObject:@"" forKey:@"v"];
    else
        [dic setObject:retObj forKey:@"v"];
    
    NSError* error = nil;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&error];
    if (error != nil){
        NSLog(@"callbackToJS error");
        return;
    }
    const char* jason = [[[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding] UTF8String];
    auto pFunction = std::bind(&reflectionCallback,std::string(jason));
    
    if (JCScriptRuntime::s_JSRT->m_pPoster){
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
}
@end