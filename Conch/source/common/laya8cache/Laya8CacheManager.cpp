

#include "Laya8CacheManager.h"
#include "../resource/JCFileResManager.h"
#include "../downloadCache/JCServerFileCache.h"
#include <cmath>
namespace laya {

    Laya8Cache::Laya8Cache(Laya8CacheManager* pCacheMgr,const std::string& url):m_url(url.c_str())
    {
        m_pSvFileCache = nullptr;
        m_pDownloadMgr = new Laya8DownloadMgr(pCacheMgr);
        m_pFileResMgr = new JCFileResManager(m_pDownloadMgr);
    }
    
    Laya8Cache::Laya8Cache(Laya8CacheManager* pCacheMgr)
    {
        m_pSvFileCache = nullptr;
        m_pDownloadMgr = new Laya8DownloadMgr(pCacheMgr);
        m_pFileResMgr = new JCFileResManager(m_pDownloadMgr);
    }

    Laya8Cache::~Laya8Cache()
    {
        if (m_pFileResMgr){
            delete m_pFileResMgr;
            m_pFileResMgr = nullptr;
        }
        if (m_pSvFileCache){
            delete m_pSvFileCache;
            m_pSvFileCache = nullptr;
        }
        for(CacheDataMapItr it = m_CacheDataMap.begin(); it != m_CacheDataMap.end(); it++){
            delete it->second;
        }
        m_CacheDataMap.clear();
    }

    bool Laya8Cache::isMyRequest(const std::string& url)const
    {
        JCUrl jcurl(url.c_str());
        return (jcurl.m_nProto == this->m_url.m_nProto) &&
                (jcurl.m_Host == this->m_url.m_Host) &&
                (jcurl.m_Port == this->m_url.m_Port);
    }

    void Laya8Cache::add(const std::string& url,Laya8CacheData* pCacheData)
    {
        CacheDataMapItr it = m_CacheDataMap.find(url);
        if (it == m_CacheDataMap.end()){
            m_CacheDataMap.insert(std::make_pair(url,pCacheData));
        }
    }

    Laya8CacheData* Laya8Cache::get(const std::string& url)
    {
        CacheDataMapItr it = m_CacheDataMap.find(url);
        if (it != m_CacheDataMap.end()){
            return it->second;
        }
        return nullptr;
    }

    void Laya8Cache::remove(const std::string& url)
    {
        CacheDataMapItr it = m_CacheDataMap.find(url);
        if (it != m_CacheDataMap.end()){
            delete it->second;
            m_CacheDataMap.erase(it);
        }
    }

    Laya8CacheManager::Laya8CacheManager()
    {
    }
    
    Laya8CacheManager::~Laya8CacheManager()
    {
        m_pHallCache.reset();
        m_pGameCache.reset();
    }

    void Laya8CacheManager::init(const std::string& hallUrl)
    {
        m_pHallCache.reset(new Laya8Cache(this,hallUrl));
        m_pGameCache.reset(new Laya8Cache(this));
    }
    
    Laya8Cache* Laya8CacheManager::openGame(const std::string& gameUrl)
    {
        m_pGameCache->m_url.parse(gameUrl.c_str());
        return m_pGameCache.get();
    }
    
    void Laya8CacheManager::closeGame()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pGameCache.reset(new Laya8Cache(this,""));
    }
    
    std::shared_ptr<Laya8Cache> Laya8CacheManager::getGameCache()const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pGameCache;
    }
    
    std::shared_ptr<Laya8Cache> Laya8CacheManager::getHallCache()const
    {
        return m_pHallCache;
    }
    
    void Laya8CacheManager::setOnHitCacheCallBack(JCFileRes* pRes)
    {
        m_CallbackRef.reset(new int(1));
        std::weak_ptr<int> cbref(m_CallbackRef);
        pRes->setOnReadyCB( std::bind(&Laya8CacheManager::onCacheHit,this, std::placeholders::_1,cbref));
    }
    
    bool Laya8CacheManager::onCacheHit(void* p_pRes, std::weak_ptr<int> callbackref)
    {
        if( !callbackref.lock() )
            return false;
        
        laya::JCResStateDispatcher* pRes = (laya::JCResStateDispatcher*)p_pRes;
        laya::JCFileRes* pFileRes = (laya::JCFileRes*)pRes;
        
        if( pFileRes->m_pBuffer.get()==NULL || pFileRes->m_nLength==0 ){
            return false;
        }
        
        if (pFileRes-> m_nLastAction != laya::JCFileRes::LOADFROMCACHE)
            return false;
        
        this->onHitCache(pFileRes->m_pBuffer.get(), pFileRes->m_nLength);
        return true;
    }

    std::string Laya8CacheManager::preUpdateDcc(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain)
    {
        
        if (pCache->m_pSvFileCache != nullptr){
            delete pCache->m_pSvFileCache;
        }
        pCache->m_pSvFileCache = new laya::JCServerFileCache();
        pCache->m_pFileResMgr->setFileCache(pCache->m_pSvFileCache);
        pCache->m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        pCache->m_pSvFileCache->switchToApp(domain.c_str());
        
        pCache->m_pSvFileCache->setResourceID("appurl", domain.c_str());
        return pCache->m_pSvFileCache->getResourceID("netassetsid");
    }
    
    void Laya8CacheManager::updateDccClearAssetsid(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain)
    {
        pCache->m_pSvFileCache->saveFileTable("");
        pCache->m_pSvFileCache->setResourceID("netassetsid", "");
        
        
        if (pCache->m_pSvFileCache != NULL){
            delete pCache->m_pSvFileCache;
        }
        pCache->m_pSvFileCache = new laya::JCServerFileCache();
        pCache->m_pFileResMgr->setFileCache(pCache->m_pSvFileCache);
        pCache->m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        pCache->m_pSvFileCache->switchToApp(domain.c_str());
    }
    
    void Laya8CacheManager::doUpdateDcc(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain,const std::string& txtdcc,const std::string&  assetsid)
    {
        pCache->m_pSvFileCache->saveFileTable(txtdcc.c_str());
        
        if (pCache->m_pSvFileCache != NULL){
            delete pCache->m_pSvFileCache;
        }
        pCache->m_pSvFileCache = new laya::JCServerFileCache();
        pCache->m_pFileResMgr->setFileCache(pCache->m_pSvFileCache);
        pCache->m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        pCache->m_pSvFileCache->switchToApp(domain.c_str());
    
        pCache->m_pSvFileCache->setResourceID("netassetsid", assetsid.c_str());
    }
    
    void Laya8CacheData::call(const char* buff, int length, int statusCode)
    {
        laya::JCBuffer buffer;
        buffer.create(length);
        memcpy(buffer.m_pPtr, buff, length);
        if (onEnd){
            this->onEnd(buffer, "", "",0,statusCode,"");
        }
    }
}
