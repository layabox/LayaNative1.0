//
//  LayaWebViewCacheManager.hpp
//  proj.ios
//
//  Created by layabox_mac on 16/10/11.
//  Copyright © 2016年 layabox_mac. All rights reserved.
//

#ifndef layaWebviewCacheManager_hpp
#define layaWebviewCacheManager_hpp

#include <string>
#include <stdio.h>
#include <jni.h>
#include "laya8cache/Laya8CacheManager.h"
#include "laya8cache/Laya8DownloadMgr.h"

class layaWebviewCacheManager:public laya::Laya8CacheManager
{
public:
    layaWebviewCacheManager();
    virtual ~layaWebviewCacheManager();
    virtual void onHitCache(const char* buff, int length) override;
    virtual void onMissCache(laya::JCDownloadMgr::onEndFunc onEnd) override;
    void setCurrentCacheData(JNIEnv * env,const std::string& url,laya::Laya8CacheData* pCacheData);
    static layaWebviewCacheManager* getInstance();
    static void delInstance();
    jbyteArray m_data_byte_array;
private:
    JNIEnv * m_env;
    laya::Laya8CacheData* m_pCurrentCacheData;
    std::string m_currentUrl;
    static layaWebviewCacheManager* m_pInstance;
};
#endif /* layaWebviewCacheManager_hpp */
