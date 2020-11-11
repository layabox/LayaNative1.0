//
//  LayaWebViewCacheManager.cpp
//  proj.ios
//
//  Created by layabox_mac on 16/10/11.
//  Copyright © 2016年 layabox_mac. All rights reserved.
//

#include "JNI-layaWebviewCacheManager.h"

layaWebviewCacheManager* layaWebviewCacheManager::m_pInstance = nullptr;

layaWebviewCacheManager::layaWebviewCacheManager()
{
    m_data_byte_array = nullptr;
}

layaWebviewCacheManager::~layaWebviewCacheManager()
{
}

layaWebviewCacheManager* layaWebviewCacheManager::getInstance(){
    if( m_pInstance == NULL){
        m_pInstance = new layaWebviewCacheManager();
    }
    return m_pInstance;
}

void layaWebviewCacheManager::delInstance(){
    if(m_pInstance){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void layaWebviewCacheManager::onHitCache(const char* buff, int length)
{
    if (length > 0 && buff!= nullptr){
        m_data_byte_array = m_env->NewByteArray(length);
        m_env->SetByteArrayRegion(m_data_byte_array, 0, length, reinterpret_cast<const jbyte*>(buff));
    }
    else{
            m_data_byte_array = nullptr;
    }
    assert(m_pCurrentCacheData);
    delete m_pCurrentCacheData;
    m_pCurrentCacheData =  nullptr;
}

void layaWebviewCacheManager::onMissCache(laya::JCDownloadMgr::onEndFunc onEnd)
{
    m_data_byte_array = nullptr;
    m_pCurrentCacheData->onEnd = onEnd;
    auto pCache = m_pCurrentCacheData->laya8Cache.lock();
    assert(pCache);
    pCache->add(m_currentUrl,m_pCurrentCacheData);
}

void layaWebviewCacheManager::setCurrentCacheData(JNIEnv * env,const std::string& url,laya::Laya8CacheData* pCacheData)
{
    m_env = env;
    m_currentUrl = url;
    m_pCurrentCacheData = pCacheData;
}
