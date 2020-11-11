
/**
@file			JCConchThreadMgr.h
@brief			
@author			guo
@version		1.0
@date			2016_5_18
*/

#ifndef __JCConchThreadMgr_H__
#define __JCConchThreadMgr_H__
#include <functional>
#include <vector>
#include <mutex>
#include <util/JCIThreadCmdMgr.h>

using namespace std;
namespace laya {
    class JCWorkerThread;
    class JCThreadCmdMgr:public IConchThreadCmdMgr {
    public:
        enum ThreadID{
            JS,
            DOWNLOADER,
            DECODER,
            NUM
        };
        JCWorkerThread* m_Threads[NUM];
        bool            m_bStop;
        recursive_mutex m_Lock;
    public:
        JCThreadCmdMgr();
        ~JCThreadCmdMgr();
        void postTo(int tid, const function<void(void)>& f);
        virtual void postToJS(const function<void(void)>& f);
        virtual void postToDownload(const function<void(void)>& f);
        virtual void postToDecoder(const function<void(void)>& f);
        void regThread(int id, JCWorkerThread* pTh);
        void stop();
        void start();
    };
}


#endif //__JCConchThreadMgr_H__
