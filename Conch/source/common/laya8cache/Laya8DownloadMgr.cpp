#include "Laya8DownloadMgr.h"
#include "Laya8CacheManager.h"
#include "../resource/JCFileResManager.h"

namespace laya {
    Laya8DownloadMgr::Laya8DownloadMgr(Laya8CacheManager* cacheMgr):m_cacheMgr(cacheMgr)
    {
    }
    void Laya8DownloadMgr::download(const char* p_pszURL, int p_nPriority,
                                          onProgressFunc p_ProgCb,
                                          onEndFunc p_CompleteCb,
                                          int p_nOptTimeout) {
        m_bCancelTask = false;
        if (0 != p_pszURL) {
            if (strlen(p_pszURL) <= 0) {
                return;
            }
            m_cacheMgr->onMissCache(p_CompleteCb);
        }
    }
}
