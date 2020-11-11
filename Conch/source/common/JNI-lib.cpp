#include <jni.h>
#include <string>
#include <sstream>
#include "util/Log.h"
#include "JNI-layaWebviewCacheManager.h"
#include "resource/JCFileResManager.h"
#include "downloadCache/JCServerFileCache.h"
#include <android/asset_manager.h>
#include "downloadCache/JCAndroidFileSource.h"
#include <android/asset_manager_jni.h>

std::string gRedistPath = "";
extern std::string gAssetRootPath ;
extern AAssetManager* g_pAssetManager;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,jstring p_strAssetRootPath, jstring p_strCachePath, jstring strHallUrl);
    JNIEXPORT jbyteArray JNICALL Java_com_layabox_browser_nativeInterface_JNILib_interceptLoadRequest(JNIEnv * env, jobject obj, jstring strUrl);
    JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_saveCache(JNIEnv * env, jobject obj,jstring strUrl,jbyteArray data);
    JNIEXPORT jstring JNICALL Java_com_layabox_browser_nativeInterface_JNILib_preUpdateDcc(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain);
    JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_updateDccClearAssetsid(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain);
    JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_doUpdateDcc(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain,jstring txtdcc,jstring assetsidStr);
}
JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,jstring p_strAssetRootPath, jstring p_strCachePath, jstring strHallUrl)
{
    char* pAssetRootPath =(char*) env->GetStringUTFChars( p_strAssetRootPath, NULL );
    char* pCachePath = (char*)env->GetStringUTFChars( p_strCachePath, NULL);
    char* pstrUrl =(char*) env->GetStringUTFChars( strHallUrl, NULL );

    gRedistPath = pCachePath;
    gRedistPath +="/";
    gAssetRootPath = pAssetRootPath;

    g_pAssetManager = AAssetManager_fromJava(env,assetManager);

    LOGI( "[jni] InitDLib CachePath=%s, AssetRoot=%s", gRedistPath.c_str(), gAssetRootPath.c_str());
    layaWebviewCacheManager::getInstance()->init(pstrUrl);


    env->ReleaseStringUTFChars(p_strAssetRootPath, pAssetRootPath);
    env->ReleaseStringUTFChars(p_strCachePath, pCachePath);
    env->ReleaseStringUTFChars(strHallUrl, pstrUrl);
}

JNIEXPORT jbyteArray JNICALL Java_com_layabox_browser_nativeInterface_JNILib_interceptLoadRequest(JNIEnv * env, jobject obj,jstring strUrl)
{
    std::shared_ptr<laya::Laya8Cache> pGameCache = layaWebviewCacheManager::getInstance()->getGameCache();
    std::shared_ptr<laya::Laya8Cache> pHallCache = layaWebviewCacheManager::getInstance()->getHallCache();

    char* pstrUrl =(char*) env->GetStringUTFChars( strUrl, NULL );
    LOGI("[jni] interceptLoadRequest：%s", pstrUrl);

    if (pHallCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheData* pCacheData = new laya::Laya8CacheData;
        pCacheData->laya8Cache = pHallCache;
        layaWebviewCacheManager::getInstance()->setCurrentCacheData(env,pstrUrl,pCacheData);
        laya::JCFileRes* pRes = pHallCache->getFileResManager()->getRes(pstrUrl);
        layaWebviewCacheManager::getInstance()->setOnHitCacheCallBack(pRes);
    }
    else if(pGameCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheData* pCacheData = new laya::Laya8CacheData;
        pCacheData->laya8Cache = pGameCache;
        layaWebviewCacheManager::getInstance()->setCurrentCacheData(env,pstrUrl,pCacheData);
        laya::JCFileRes* pRes = pGameCache->getFileResManager()->getRes(pstrUrl);
        layaWebviewCacheManager::getInstance()->setOnHitCacheCallBack(pRes);
    }
    else{
        layaWebviewCacheManager::getInstance()->m_data_byte_array = nullptr;
    }

    env->ReleaseStringUTFChars(strUrl, pstrUrl);

    return layaWebviewCacheManager::getInstance()->m_data_byte_array;
}

JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_saveCache(JNIEnv * env, jobject obj,jstring strUrl,jbyteArray data)
{
    std::shared_ptr<laya::Laya8Cache> pGameCache = layaWebviewCacheManager::getInstance()->getGameCache();
    std::shared_ptr<laya::Laya8Cache> pHallCache = layaWebviewCacheManager::getInstance()->getHallCache();

    char* pstrUrl =(char*) env->GetStringUTFChars( strUrl, NULL );
    LOGI("[jni] saveCache：%s", pstrUrl);

    jbyte * array = env->GetByteArrayElements(data,0);
    jsize len = env->GetArrayLength(data);

    if (pHallCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheData* pData = pHallCache->get(pstrUrl);
        if (pData != nullptr) {
            if (array != nullptr && len > 0) {
                pData->call((const char *) array, len, 200);
            }
            pHallCache->remove(pstrUrl);
        }
    }
    else if(pGameCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheData* pData = pGameCache->get(pstrUrl);
        if (pData != nullptr){
            if (array != nullptr && len > 0) {
                pData->call((const char *) array, len, 200);
            }
            pGameCache->remove(pstrUrl);
        }
    }
    env->ReleaseStringUTFChars(strUrl, pstrUrl);
}

JNIEXPORT jstring JNICALL Java_com_layabox_browser_nativeInterface_JNILib_preUpdateDcc(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain)
{
    std::shared_ptr<laya::Laya8Cache> pGameCache = layaWebviewCacheManager::getInstance()->getGameCache();
    std::shared_ptr<laya::Laya8Cache> pHallCache = layaWebviewCacheManager::getInstance()->getHallCache();

    char* pstrUrl =(char*) env->GetStringUTFChars( strUrl, NULL );
    char* pstrUrlDomain =(char*) env->GetStringUTFChars( strUrlDomain, NULL );

    LOGI("[jni] preUpdateDcc：%s", pstrUrl);

    std::string curassets = "";
    if (pHallCache->isMyRequest(pstrUrl)){
        curassets = laya::Laya8CacheManager::preUpdateDcc(pHallCache.get(),gRedistPath,pstrUrlDomain);
    }
    else{
        layaWebviewCacheManager::getInstance()->openGame(pstrUrl);
        curassets = laya::Laya8CacheManager::preUpdateDcc(pGameCache.get(),gRedistPath,pstrUrlDomain);
    }

    env->ReleaseStringUTFChars(strUrl, pstrUrl);
    env->ReleaseStringUTFChars(strUrlDomain, pstrUrlDomain);

    return env->NewStringUTF(curassets.c_str());
}

JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_updateDccClearAssetsid(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain)
{
    std::shared_ptr<laya::Laya8Cache> pGameCache = layaWebviewCacheManager::getInstance()->getGameCache();
    std::shared_ptr<laya::Laya8Cache> pHallCache = layaWebviewCacheManager::getInstance()->getHallCache();

    char* pstrUrl =(char*) env->GetStringUTFChars( strUrl, NULL );
    char* pstrUrlDomain =(char*) env->GetStringUTFChars( strUrlDomain, NULL );

    LOGI("[jni] updateDccClearAssetsid：%s", pstrUrl);

    if (pHallCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheManager::updateDccClearAssetsid(pHallCache.get(),gRedistPath,pstrUrlDomain);
    }
    else{
        laya::Laya8CacheManager::updateDccClearAssetsid(pGameCache.get(),gRedistPath,pstrUrlDomain);
    }

    env->ReleaseStringUTFChars(strUrl, pstrUrl);
    env->ReleaseStringUTFChars(strUrlDomain, pstrUrlDomain);
}

JNIEXPORT void JNICALL Java_com_layabox_browser_nativeInterface_JNILib_doUpdateDcc(JNIEnv * env, jobject obj,jstring strUrl,jstring strUrlDomain,jstring txtdcc,jstring assetsidStr)
{
    std::shared_ptr<laya::Laya8Cache> pGameCache = layaWebviewCacheManager::getInstance()->getGameCache();
    std::shared_ptr<laya::Laya8Cache> pHallCache = layaWebviewCacheManager::getInstance()->getHallCache();

    char* pstrUrl =(char*) env->GetStringUTFChars( strUrl, NULL );
    char* pstrUrlDomain =(char*) env->GetStringUTFChars( strUrlDomain, NULL );
    char* pstrTxtdcc =(char*) env->GetStringUTFChars( txtdcc, NULL );
    char* pstrAssetsidStr =(char*) env->GetStringUTFChars( assetsidStr, NULL );

    LOGI("[jni] doUpdateDcc：%s", pstrUrl);

    if (pHallCache->isMyRequest(pstrUrl)){
        laya::Laya8CacheManager::doUpdateDcc(pHallCache.get(),gRedistPath,pstrUrlDomain,pstrTxtdcc,pstrAssetsidStr);
    }
    else{
        laya::Laya8CacheManager::doUpdateDcc(pGameCache.get(),gRedistPath,pstrUrlDomain,pstrTxtdcc,pstrAssetsidStr);
    }

    env->ReleaseStringUTFChars(strUrl, pstrUrl);
    env->ReleaseStringUTFChars(strUrlDomain, pstrUrlDomain);
    env->ReleaseStringUTFChars(txtdcc,pstrTxtdcc);
    env->ReleaseStringUTFChars(assetsidStr,pstrAssetsidStr);
}