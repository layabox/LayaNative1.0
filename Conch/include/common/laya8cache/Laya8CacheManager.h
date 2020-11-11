
#ifndef Laya8CacheManager_hpp
#define Laya8CacheManager_hpp

#include <string>
#include <stdio.h>
#include <mutex>
#include "Laya8DownloadMgr.h"
#include "../util/JCLayaUrl.h"
#include <unordered_map>
namespace laya {
    
    class JCFileResManager;
    class JCServerFileCache;
    class JCFileRes;
    class Laya8CacheManager;
    class Laya8CacheData;

    class Laya8Cache
    {
    public:
        Laya8Cache(Laya8CacheManager* pCacheMgr,const std::string& url);
        Laya8Cache(Laya8CacheManager* pCacheMgr);
        ~Laya8Cache();
        bool isMyRequest(const std::string& url)const;
        void add(const std::string& url,Laya8CacheData* pCacheData);
        Laya8CacheData* get(const std::string& url);
        void remove(const std::string& url);
        JCFileResManager* getFileResManager()const{return m_pFileResMgr;}
    private:
        friend class Laya8CacheManager;
        //for thread safty do not save datas below 
        JCFileResManager* m_pFileResMgr;
        JCServerFileCache* m_pSvFileCache;
        Laya8DownloadMgr* m_pDownloadMgr;
        JCUrl m_url;
        typedef std::unordered_map<std::string,Laya8CacheData*> CacheDataMap;
        typedef std::unordered_map<std::string,Laya8CacheData*>::iterator CacheDataMapItr;
        CacheDataMap m_CacheDataMap;
    };
    struct Laya8CacheData
    {
        std::weak_ptr<Laya8Cache> laya8Cache;
        laya::JCDownloadMgr::onEndFunc onEnd;
        void call(const char* buff, int length, int statusCode);
    };
    class Laya8CacheManager
    {
    public:
        Laya8CacheManager();
        virtual ~Laya8CacheManager();
        
        void init(const std::string& hallUrl);
        
        void closeGame();
        Laya8Cache* openGame(const std::string& gameUrl);
        
        std::shared_ptr<Laya8Cache> getHallCache()const;
        std::shared_ptr<Laya8Cache> getGameCache()const;
        
        void setOnHitCacheCallBack(JCFileRes* pRes);
        
        static std::string preUpdateDcc(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain);
        static void updateDccClearAssetsid(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain);
        static void doUpdateDcc(Laya8Cache* pCache,const std::string& redistPath,const std::string& domain,const std::string& txtdcc,const std::string&  assetsid);
    protected:
        friend class Laya8DownloadMgr;
        virtual void onHitCache(const char* buff, int length) = 0;
        virtual void onMissCache(JCDownloadMgr::onEndFunc onEnd) = 0;
        bool onCacheHit(void* p_pRes, std::weak_ptr<int> callbackref);
    protected:
        std::shared_ptr<Laya8Cache> m_pHallCache;
        std::shared_ptr<Laya8Cache> m_pGameCache;
        std::shared_ptr<int>	m_CallbackRef;
        mutable std::mutex m_mutex;
        
    };
}
#endif /* Laya8CacheManager_hpp */
