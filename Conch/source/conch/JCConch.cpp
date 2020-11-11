/**
@file			JCConch.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_13
*/

#include "JCConch.h"
#include <algorithm>
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include <fileSystem/JCFileSystem.h>
#include <downloadCache/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "JCConch.h"
#include "JSWrapper/JSInterface/JSInterface.h"
#include "JSWrapper/LayaWrap/JSFileReader.h"
#include "JSWrapper/LayaWrap/JSGlobalExportCFun.h"
#include "JCScrpitRuntime.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "JCThreadCmdMgr.h"
#include "JCSystemConfig.h"

#ifdef JS_V8
    #include "JSWrapper/v8debug/debug-agent.h"
#endif
#ifdef ANDROID
    #include "CToJavaBridge.h"
#elif __APPLE__
    #include "CToObjectC.h"
    #include "pthread.h"
#endif

std::string gRedistPath = "";
std::string gResourcePath = "";
#ifdef __APPLE__
    std::string gAssetRootPath = "";
#endif
int g_nInnerWidth = 1024;
int g_nInnerHeight = 768;
bool g_bGLCanvasSizeChanged = false;
namespace laya
{
    extern JCWorkerThread* g_DecThread;
	JCConch* JCConch::s_pConch = NULL;
	int JCConch::s_nDebugPort = 0;
	int64_t JCConch::s_nUpdateTime = 0;
    JCFileSource* JCConch::s_pAssetsFiles = NULL;
	std::shared_ptr<JCConchRender> JCConch::s_pConchRender;

	/*
	void _global_onCreateFileError()
    {
		errorflog("空间已满。写文件失败！");
		LOGE("空间已满。写文件失败！");
		//先发给QQ吧
		const char* pMsg="{\"type\":\"ge_storage_full\"}";
#ifdef ANDROID
		std::vector<intptr_t> params;
		params.push_back( (intptr_t)pMsg );
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callStaticJavaMethod( JMN_sendMessageToPlatform,params, kRet);
#else
    
#endif
	}
	*/
	void _vibrate() 
    {
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrate", kRet);
#endif
	}
	JCConch::JCConch(int nDownloadThreadNum)
    {
        s_pConch = this;
#ifdef __APPLE__
        pthread_key_create(&JCWorkerThread::s_tls_curThread, NULL);
        pthread_key_create(&s_tls_curDataThread, NULL);
        pthread_key_create(&JSClassMgr::s_tls_curThread, NULL);
        pthread_key_create(&__TlsData::s_tls_curThread, NULL);
#endif
        m_sCachePath = gRedistPath + "/appCache";
        g_DecThread = new JCWorkerThread(true);
        g_DecThread->setThreadName("image decode");
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        LOGI("download thread num = %d", nDownloadThreadNum);
        pdmgr->init(nDownloadThreadNum);	//多加一个线程，可能要给优先级较低的任务用。

        m_pFileResMgr = new JCFileResManager(pdmgr);
        m_pFileResMgr->m_pCmdPoster = &m_ThreadCmdMgr;
		s_pConchRender.reset(new JCConchRender(m_pFileResMgr));
        s_pConchRender->setSize(g_nInnerWidth, g_nInnerHeight);
        m_pAssetsRes = JCConch::s_pAssetsFiles;
        s_pConchRender->setAssetRes(m_pAssetsRes);
        m_strStartJS = "scripts/apploader.js";
        m_pScrpitRuntime = new JCScriptRuntime();
        m_pScrpitRuntime->init(m_pFileResMgr, m_pAssetsRes, &m_ThreadCmdMgr,s_pConchRender->m_pFreeTypeRender);

#ifdef WIN32
        char* jsportfile = "d:/temp/sdcard/layabox/jsdebug.txt";
#elif ANDROID
        char* jsportfile = "/sdcard/layabox/jsdebug.txt";
#elif __APPLE__
        char* jsportfile = "";
#endif
        if (s_nDebugPort <= 0) {
            FILE* fp = fopen(jsportfile, "r");
            if (fp) {
                LOGI("DEBUG js enable!");
                char buf[32];
                int n = fread(buf, 1, 10, fp);
                if (n >= 4 && n<32) {
                    buf[n] = 0;
                    LOGI("debug port=%s", buf);
                    s_nDebugPort = atoi(buf);
                }
                else {
                    s_nDebugPort = 5858;
                }
                fclose(fp);
            }
        }
#ifdef JS_V8
        if (s_nDebugPort > 0) 
        {
            LOGI("open js debug port at %d", s_nDebugPort);
            m_pDbgAgent = new DebuggerAgent("layabox", s_nDebugPort);
            m_pDbgAgent->enableJSDebug();
            m_pScrpitRuntime->m_pDbgAgent = m_pDbgAgent;
        }
        else 
        {
            m_pDbgAgent = NULL;
            s_nDebugPort = 0;
        }
#endif
	}

	JCConch::~JCConch() 
    {
		s_pConchRender.reset();
		s_pConch = NULL;
        /*
        //TODO 这段代码有点恶心，日后再整理
        //不在这删除了，在JCConchRender中删除了
        if (m_pFileResMgr)
        {
            delete m_pFileResMgr;
            m_pFileResMgr = NULL;
        }
        */
        if (m_pScrpitRuntime) {
            delete m_pScrpitRuntime;
            m_pScrpitRuntime = NULL;
        }
#ifdef JS_V8
        if (m_pDbgAgent) {
            m_pDbgAgent->Shutdown();
            delete m_pDbgAgent;
            m_pDbgAgent = NULL;
        }
#endif
	}

	void JCConch::onAppStart() {
        m_strLocalStoragePath = gRedistPath + "/localstorage/";
       
        try
        {
            fs::create_directories(m_strLocalStoragePath);
        }
        catch (...)
        {
            if (global_onCreateFileError)
            {
                global_onCreateFileError();
            }
            return;
        }

        m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, &m_pScrpitRuntime->m_ScriptThread);
        m_pScrpitRuntime->start(m_strStartJS.c_str());
	}
    void JCConch::reload() {
        LOGI("JCConch::reload start...");
        //先通知消息管理器，关闭各个线程之间的post
        m_ThreadCmdMgr.stop();
        m_pScrpitRuntime->reload();
        m_ThreadCmdMgr.start();
        m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, &m_pScrpitRuntime->m_ScriptThread);
        LOGI("JCConch::reload end.");
    }
	void JCConch::DelAppRes() {
	}
	void JCConch::onClearMemory() {
	}
	void JCConch::onAppDestory() {
        //先通知消息管理器，关闭各个线程之间的post
        m_ThreadCmdMgr.stop();

        //渲染要继续
        JCConch::s_pConchRender->willExit();
        
        //关闭解码线程
        delete g_DecThread;
        
        if (m_pScrpitRuntime) {
            delete m_pScrpitRuntime;
            m_pScrpitRuntime = NULL;
        }
        
        //关闭下载线程
        //删除所有的下载任务
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
        JCDownloadMgr::delInstance();

    }
	void JCConch::postCmdToMainThread(int p_nCmd, int p_nParam1, int p_nParam2) {
#ifdef __APPLE__
		CToObjectCPostMainThread(p_nCmd, p_nParam1, p_nParam2);
#else
		m_funcPostMsgToMainThread(p_nCmd, p_nParam1, p_nParam2);
#endif
	}
    int JCConch::urlHistoryLength() {
        return m_vUrlHistory.size();
    }
    void JCConch::urlBack() {
        urlGo(-1);
    }

    void JCConch::urlGo(int s) {
#ifdef __APPLE__
        CToObjectCRunStopJSLoop();
#endif
        int sz = m_vUrlHistory.size();
        m_nUrlHistoryPos += s;
        if (m_nUrlHistoryPos >= sz) m_nUrlHistoryPos = sz - 1;
        if (m_nUrlHistoryPos < 0) m_nUrlHistoryPos = 0;
        if(m_nUrlHistoryPos<m_vUrlHistory.size())
            g_kSystemConfig.m_strStartURL = m_vUrlHistory[m_nUrlHistoryPos];
        postCmdToMainThread(CMD_ReloadProcess, 0, 0);
    }

    void JCConch::urlForward() {
        urlGo(1);
    }

    void JCConch::urlHistoryPush(const char* pUrl) {
        m_nUrlHistoryPos++; //位置
        int nsz = m_nUrlHistoryPos+1; //希望的大小
        m_vUrlHistory.resize(nsz);
        m_vUrlHistory[m_nUrlHistoryPos] = pUrl;
        //
        int sz = m_vUrlHistory.size();
        if (sz>1) {
            if (m_vUrlHistory[sz - 1] == m_vUrlHistory[sz - 2]) {
                m_vUrlHistory.resize(sz - 1);
                m_nUrlHistoryPos--;
            }
        }
    }

	void JCConch::onRunCmdInMainThread(int p_nCmd, int p_nParam1, int p_nParam2) {
		switch (p_nCmd) {
		case JCConch::CMD_ActiveProcess:
			break;
		case JCConch::CMD_DeactiveProcess:
			break;
		case JCConch::CMD_CloseProcess:
			break;
		case JCConch::CMD_ReloadProcess:
            reload();
			break;
		case JCConch::CMD_UrlBack:
            urlBack();
			break;
		case JCConch::CMD_UrlForward:
			break;
		case JCConch::CMD_onOrientationChanged:
			break;
		}
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
