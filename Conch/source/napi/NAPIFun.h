#ifndef _NAPIFUN_
#define _NAPIFUN_

#include "JCConch.h"
#include "Audio/JCAudioManager.h"
#include "util/Log.h"
#include "aki/jsbind.h"
#include "JCSystemConfig.h"
#include "util/JCZipFile.h"
#include "../conch/JCScriptRuntime.h"
#include <rawfile/raw_file_manager.h>
#include "napi/plugin_manager.h"
#include "downloadCache/JCOHOSFileSource.h"
#include "../common/misc/JCLayaThreadPool.h"

using namespace laya;

extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern std::string gRedistPath;

extern NativeResourceManager *g_pAssetManager;
extern std::string gAssetRootPath;

class NAPIFun
{
public:
    ~NAPIFun() {}
    static NAPIFun *GetInstance()
    {
        return &NAPIFun::fun_;
    }

    static void postCmdToMainThread(int p_nCmd, int p_nParam1,int p_nParam2);
    static void ConchNAPI_configSetParamExt(std::string p_strParamExt);
    static void ConchNAPI_configSetURL(std::string p_strUrl);
    static void ConchNAPI_configSetIsPlug(bool p_bIsPlug);
    static void ConchNAPI_InitDLib(napi_value assetManager,int nThreadNum,std::string p_strAssetRootPath,std::string p_strCachePath);
    static void ConchNAPI_SetLocalStoragePath(std::string p_strLocalStorage);
    static void ConchNAPI_ReleaseDLib();
    static void ConchNAPI_OnAppDestroy();
    static void ConchNAPI_OnAppPause();
    static void ConchNAPI_OnAppResume();
    static void ConchNAPI_OnGLReady(int width,int height);
    static void ConchNAPI_OnAppStart();
    static void ConchNAPI_onDrawFrame();
    static void ConchNAPI_onVSyncCallback(long VSynctm);
    static void ConchNAPI_audioMusicPlayEnd();
    static void ConchNAPI_networkChanged(int nNetworkType);
    static void ConchNAPI_inputChange(int keycode);
    static void ConchNAPI_setLocalizable(bool p_bIsLocalPackage);
    static void ConchNAPI_captureScreenCallBack(int w,int h,std::string byteArray);
    static void ConchNAPI_onRunCmd(std::string cmd);
    static void ConchNAPI_gameMsgHandle(std::string key, std::string value);
    static void ConchNAPI_RunJS(const std::string &js);
private:
    bool g_bEngineInited =false;
    std::mutex g_kReadyLock;
    laya::JCConch *g_pConch= NULL;
    bool g_bInBKGround = false;
    int64_t g_nInitTime = 0;

private:
    static NAPIFun fun_;

public:
    static bool m_bTakeScreenshot;
};

#endif