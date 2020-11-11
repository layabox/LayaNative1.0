/**
@file			JCConch.h
@brief			
@author			guo
@version		1.0
@date			2016_5_13
*/

#ifndef _LAYA_CONCH_H_
#define _LAYA_CONCH_H_
#include <vector>
#include <util/JCCommonMethod.h>
#include "JCConchRender.h"
#include "JSWrapper/JSInterface/JSInterface.h"
#include "JCThreadCmdMgr.h"

namespace laya
{
    class JCFileResManager;
	class JCFileSource;
	class DebuggerAgent;
    class JCScriptRuntime;
	class JCConch
	{
	public:
		enum CMDMSG 
        {
			CMD_ActiveProcess = 0x400 + 3333 + 100,	        //0x400=WM_USER
			CMD_DeactiveProcess,
			CMD_ReloadProcess,
			CMD_CloseProcess,
			CMD_UrlBack,
			CMD_UrlForward,
			CMD_onOrientationChanged = 0x400 + 3333 + 110,	//屏幕切换已经完成。
		};
	public:

		JCConch( int nDownloadThreadNum );

		~JCConch();

		void DelAppRes();

		void onRunCmdInMainThread(int p_nCmd, int p_nParam1, int p_nParam2);

		void onAppStart();

        void reload();

        int  urlHistoryLength();
        void urlBack();
        void urlGo(int s);
        void urlForward();
        void urlHistoryPush(const char* pUrl);

		void exit();

		void onClearMemory();

		void onAppDestory();

		void postCmdToMainThread(int p_nCmd, int p_nParam1, int p_nParam2);

		const char* getLocalStoragePath() { return m_strLocalStoragePath.c_str(); }

    public:

        static JCConch*			                s_pConch;
        static int64_t			                s_nUpdateTime;
        static JCFileSource*	                s_pAssetsFiles;
        std::function<void(int, int, int)>      m_funcPostMsgToMainThread;
        static std::shared_ptr<JCConchRender>	s_pConchRender;
        JCFileSource*			                m_pAssetsRes;
        std::string				                m_strStartJS;
        std::string                             m_sCachePath;
        JCScriptRuntime*                        m_pScrpitRuntime;
        JCThreadCmdMgr                          m_ThreadCmdMgr;
#ifdef JS_V8
        DebuggerAgent*			                m_pDbgAgent;
#endif
        static int				                s_nDebugPort;
        std::string				                m_strLocalStoragePath;
        JCFileResManager*	                    m_pFileResMgr;
    protected:
        bool					                m_bDestroying;
        std::vector<std::string>                m_vUrlHistory;
        int                                     m_nUrlHistoryPos=-1;//-1表示还不存在
	};
};
#endif //_LAYA_CONCH_H_

//-----------------------------END FILE--------------------------------