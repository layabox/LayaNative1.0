
#ifndef Laya8DownloadMgr_hpp
#define Laya8DownloadMgr_hpp

#include "../downloadMgr/JCDownloadMgr.h"

namespace laya
{
    class Laya8CacheManager;
    class Laya8DownloadMgr : public laya::JCDownloadMgr {
    public:
        Laya8DownloadMgr(Laya8CacheManager* cacheMgr);
        virtual void download(const char* p_pszURL, int p_nPriority,onProgressFunc p_ProgCb,onEndFunc p_CompleteCb,int p_nOptTimeout);
protected:
        void load(onEndFunc p_CompleteCb);
        Laya8CacheManager* m_cacheMgr;
    };

}
#endif /* Laya8DownloadMgr_hpp */
