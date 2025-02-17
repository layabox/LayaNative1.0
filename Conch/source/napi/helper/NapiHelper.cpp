#include "NapiHelper.h"
#include "bundle/native_interface_bundle.h"

using namespace laya;

NapiHelper NapiHelper::help_;

std::string NapiHelper::getDeviceInfo()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getDeviceInfo, this));
    return deviceInfo;
}

std::string NapiHelper::__getDeviceInfo()
{
    if (auto getDeviceInfo = aki::JSBind::GetJSFunction("DeviceUtils.getDeviceInfo"))
    {
        deviceInfo = getDeviceInfo->Invoke<std::string>();
    }
    return deviceInfo;
}

int NapiHelper::getNetworkType()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getNetworkType, this));
    return networkType;
}

int NapiHelper::__getNetworkType()
{
    if (auto getNetworkType = aki::JSBind::GetJSFunction("NetworkUtils.getNetworkType"))
    {
        networkType = getNetworkType->Invoke<int>();
    }
    return networkType;
}

void NapiHelper::playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__playBackgroundMusic, this, p_sUrl, p_nTimes, nCurrentTime));
}

void NapiHelper::__playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime)
{
    if (auto playBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.playBackgroundMusic"))
    {
        playBackgroundMusic->Invoke<void>(p_sUrl, p_nTimes, (int)(nCurrentTime * 1000));
    }
}

void NapiHelper::pauseBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__pauseBackgroundMusic, this));
}

void NapiHelper::__pauseBackgroundMusic()
{
    if (auto pauseBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.pauseBackgroundMusic"))
    {
        pauseBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::stopBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__stopBackgroundMusic, this));
}

void NapiHelper::__stopBackgroundMusic()
{
    if (auto stopBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.stopBackgroundMusic"))
    {
        stopBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::resumeBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__resumeBackgroundMusic, this));
}

void NapiHelper::__resumeBackgroundMusic()
{
    if (auto resumeBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.resumeBackgroundMusic"))
    {
        resumeBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::setBackgroundMusicVolume(float p_nVolume)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setBackgroundMusicVolume, this, p_nVolume));
}

void NapiHelper::__setBackgroundMusicVolume(float p_nVolume)
{
    if (auto setBackgroundMusicVolume = aki::JSBind::GetJSFunction("SoundUtils.setBackgroundMusicVolume"))
    {
        setBackgroundMusicVolume->Invoke<void>(p_nVolume);
    }
}

void NapiHelper::setCurrentTime(double nCurrentTime)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setCurrentTime, this, nCurrentTime));
}

void NapiHelper::__setCurrentTime(double nCurrentTime)
{
    if (auto setCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.setCurrentTime"))
    {
        setCurrentTime->Invoke<void>(nCurrentTime);
    }
}

double NapiHelper::getCurrentTime()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getCurrentTime, this));
    return currentTime;
}

double NapiHelper::__getCurrentTime()
{
    currentTime = 0.0f;
    if (auto getCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.getCurrentTime"))
    {
        currentTime = getCurrentTime->Invoke<double>();
    }
    return currentTime;
}

double NapiHelper::getDuration()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getDuration, this));
    return duration;
}

double NapiHelper::__getDuration()
{
    duration = 0.0f;
    if (auto getMp3Duration = aki::JSBind::GetJSFunction("SoundUtils.getDuration"))
    {
        duration = getMp3Duration->Invoke<double>();
    }
    return duration;
}

void NapiHelper::startVibration(float duration)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__startVibration, this, duration));
}

void NapiHelper::__startVibration(float duration)
{
    if (auto startVibration = aki::JSBind::GetJSFunction("DeviceUtils.startVibration"))
    {
        startVibration->Invoke<void>(duration);
    }
}

std::string NapiHelper::getAppVersion()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAppVersion, this));
    return appVersion;
}

std::string NapiHelper::__getAppVersion()
{
    if (auto getAppVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppVersion"))
    {
        appVersion = getAppVersion->Invoke<std::string>();
    }
    return appVersion;
}

std::string NapiHelper::getAppLocalVersion()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAppLocalVersion, this));
    return appLocalVersion;
}

std::string NapiHelper::__getAppLocalVersion()
{
    if (auto getAppLocalVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppLocalVersion"))
    {
        appLocalVersion = getAppLocalVersion->Invoke<std::string>();
    }
    return appLocalVersion;
}

void NapiHelper::showDialog(const char *p_sBuffer)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__showDialog, this, p_sBuffer));
}

void NapiHelper::__showDialog(const char *p_sBuffer)
{
    std::string strBuffer = p_sBuffer;
    if (auto showDialog = aki::JSBind::GetJSFunction("Dialog.showDialog"))
    {
        showDialog->Invoke<void>(strBuffer.c_str());
    }
}

void NapiHelper::setKeepScreenOn(bool value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setKeepScreenOn, this, value));
}

void NapiHelper::__setKeepScreenOn(bool value)
{
    if (auto setKeepScreenOn = aki::JSBind::GetJSFunction("DeviceUtils.setKeepScreenOn"))
    {
        setKeepScreenOn->Invoke<void>(value);
    }
}

void NapiHelper::setPreferredOrientation(int orientation)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setPreferredOrientation, this, orientation));
}

void NapiHelper::__setPreferredOrientation(int orientation)
{
    if (auto setPreferredOrientation = aki::JSBind::GetJSFunction("DeviceUtils.setPreferredOrientation"))
    {
        setPreferredOrientation->Invoke<void>(orientation);
    }
}

float NapiHelper::getScreenInch()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getScreenInch, this));
    return screenInch;
}

float NapiHelper::__getScreenInch()
{
    std::promise<float> promise;
    std::function<void(float)> cb = [&promise](float message)
    { promise.set_value(message); };
    if (auto getScreenInch = aki::JSBind::GetJSFunction("DeviceUtils.getScreenInch"))
    {
        getScreenInch->Invoke<void>(cb);
    }
    screenInch = promise.get_future().get();
    return screenInch;
}

int NapiHelper::getAvalidMem()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAvalidMem, this));
    return avalidMem;
}

int NapiHelper::__getAvalidMem()
{
    if (auto getAvalidMem = aki::JSBind::GetJSFunction("DeviceUtils.getAvalidMem"))
    {
        avalidMem = getAvalidMem->Invoke<int>();
    }
    return avalidMem;
}

int NapiHelper::getUsedMem()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getUsedMem, this));
    return usedMem;
}

int NapiHelper::__getUsedMem()
{
    if (auto getUsedMem = aki::JSBind::GetJSFunction("DeviceUtils.getPrivateDirty"))
    {
        usedMem = getUsedMem->Invoke<int>();
    }
    return usedMem;
}

void NapiHelper::exitGame() { JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__exitGame, this)); }

void NapiHelper::__exitGame()
{
    if (auto exit = aki::JSBind::GetJSFunction("ApplicationManager.exit"))
    {
        exit->Invoke<void>();
    }
}

std::string NapiHelper::postMessageToUIThread(std::string eventName, std::string data)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__postMessageToUIThread, this, eventName, data));
    return eventResult;
}

std::string NapiHelper::__postMessageToUIThread(std::string eventName, std::string data)
{
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}

std::string NapiHelper::postSyncMessageToUIThread(std::string eventName, std::string data)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__postSyncMessageToUIThread, this, eventName, data));
    return syncEventResult;
}

std::string NapiHelper::__postSyncMessageToUIThread(std::string eventName, std::string data)
{
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message)
    { promise.set_value(message); };
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        post->Invoke<void>(eventName, data, cb);
    }
    syncEventResult = promise.get_future().get();
    return syncEventResult;
}

/**
 * 调用本地方法，目前只支持调用ets/ts脚本类型中的静态方法 示例是Test.ets下的test方法
 * @param isSyn 是否是同步方法
 * @param clsPath 脚本路径 示例：entry/src/main/Tests/Test
 * @param methodName 模块名称/静态方法名称 示例：entry/test(模块名称可省略，省略为clsPath第一个字符串)
 * @param paramStr 方法入参 示例：json字符串
 * @return 方法出参 示例：json字符串
 */
std::string NapiHelper::callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__callNativeMethod, this, isSyn, clsPath, methodName, paramStr)
    );
    return methodResult;
}

std::string NapiHelper::__callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr)
{
    std::string module_name;
    std::string method;
    
    std::string methodStr = methodName;
    std::string::size_type pos = methodStr.find("/");
    if (pos != std::string::npos) {
        module_name = methodStr.substr(0, pos);
        method = methodStr.substr(pos + 1);
    } else {
        method = methodName;
        std::string pathStr = clsPath;
        pos = pathStr.find("/");
        module_name = pos != std::string::npos ? pathStr.substr(0, pos) : "entry";
    }
    
    std::string module_info = __getModuleInfo(module_name.c_str());
    
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message)
    {
        promise.set_value(message);
    };
    AsyncCallParam *callParam = new AsyncCallParam{cb, paramStr, isSyn, clsPath, methodName, method, module_info};
    JSFunction::getFunction("HandleMessageUtils.executeNativeMethod").invokeAsync(callParam);
    
    methodResult = promise.get_future().get();
    return methodResult;
}

std::string NapiHelper::__getModuleInfo(const char* module_name)
{
    if (bundle_name.empty()) {
        OH_NativeBundle_ApplicationInfo info = OH_NativeBundle_GetCurrentApplicationInfo();
        bundle_name = info.bundleName;
    }
    
    std::string module_info = bundle_name + "/" + module_name;
    return module_info;
}

void NapiHelper::handleEditBoxMsg(std::string name, int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleEditBoxMsg, this, name, m_tag));
}

void NapiHelper::__handleEditBoxMsg(std::string name, int m_tag)
{
    if (auto handle = aki::JSBind::GetJSFunction("EditBox.handleMsgVoid"))
    {
        handle->Invoke<void>(name, m_tag);
    }
}

void NapiHelper::handleEditBoxMsgStr(std::string name, int m_tag, std::string value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleEditBoxMsgStr, this, name, m_tag, value));
}

void NapiHelper::__handleEditBoxMsgStr(std::string name, int m_tag, std::string value)
{
    if (auto handle = aki::JSBind::GetJSFunction("EditBox.handleMsgStr"))
    {
        handle->Invoke<void>(name, m_tag, value);
    }
}

std::string NapiHelper::handleEditBoxMsgGetStr(std::string name, int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleEditBoxMsgGetStr, this, name, m_tag));
    return inputInfoValue;
}

std::string NapiHelper::__handleEditBoxMsgGetStr(std::string name, int m_tag)
{
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message)
    { promise.set_value(message); };
    if (auto getStr = aki::JSBind::GetJSFunction("EditBox.handleMsgGetValue"))
    {
        getStr->Invoke<void>(name, m_tag, cb);
    }
    inputInfoValue = promise.get_future().get();
    return inputInfoValue;
}

void NapiHelper::handleEditBoxMsgBool(std::string name, int m_tag, bool value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleEditBoxMsgBool, this, name, m_tag, value));
}

void NapiHelper::__handleEditBoxMsgBool(std::string name, int m_tag, bool value)
{
    if (auto handle = aki::JSBind::GetJSFunction("EditBox.handleMsgBool"))
    {
        handle->Invoke<void>(name, m_tag, value);
    }
}

void NapiHelper::handleEditBoxMsgNum2(std::string name, int m_tag, float value1, float value2)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__handleEditBoxMsgNum2, this, name, m_tag, value1, value2));
}

void NapiHelper::__handleEditBoxMsgNum2(std::string name, int m_tag, float value1, float value2)
{
    if (auto handle = aki::JSBind::GetJSFunction("EditBox.handleMsgNum2"))
    {
        handle->Invoke<void>(name, m_tag, value1, value2);
    }
}

void NapiHelper::handleEditBoxMsgNum1(std::string name, int m_tag, float value1)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__handleEditBoxMsgNum1, this, name, m_tag, value1));
}

void NapiHelper::__handleEditBoxMsgNum1(std::string name, int m_tag, float value1)
{
    if (auto handle = aki::JSBind::GetJSFunction("EditBox.handleMsgNum1"))
    {
        handle->Invoke<void>(name, m_tag, value1);
    }
}

std::string NapiHelper::postCmdToMain(std::string data)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__postCmdToMain, this, data));
    return eventResult;
}

std::string NapiHelper::__postCmdToMain(std::string data)
{
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.postCmdToMain"))
    {
        eventResult = post->Invoke<std::string>(data);
    }
    return eventResult;
}

void NapiHelper::handleCloseWebview()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleCloseWebview, this));
}

void NapiHelper::__handleCloseWebview()
{
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.closeWebview")) {
        handle->Invoke<void>();
    }
}

void NapiHelper::handleCreateWebview(const char *sUrl, int x, int y, int w, int h, bool bCloseWebview) {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleCreateWebview, this, sUrl, x, y, w, h, bCloseWebview));
}

void NapiHelper::__handleCreateWebview(const char *sUrl, int x, int y, int w, int h, bool bCloseWebview) {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.createWebview")) {
        handle->Invoke<void>(sUrl, x, y, w, h, bCloseWebview);
    }
}

void NapiHelper::handleCallWebviewJS(const char *sFunctionName, const char *sJsonParam, const char *sCallbackFunction) {
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__handleCallWebviewJS, this, sFunctionName, sJsonParam, sCallbackFunction));
}

void NapiHelper::__handleCallWebviewJS(const char *sFunctionName, const char *sJsonParam, const char *sCallbackFunction) {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.callWebViewJS")) {
        handle->Invoke<void>(sFunctionName, sJsonParam, sCallbackFunction);
    }
}

void NapiHelper::handleHideWebview() {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleHideWebview, this));
}

void NapiHelper::__handleHideWebview() {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.hideWebview")) {
        handle->Invoke<void>();
    }
}

void NapiHelper::handleShowWebview() {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleShowWebview, this));
}

void NapiHelper::__handleShowWebview() {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.showWebView")) {
        handle->Invoke<void>();
    }
}
std::unordered_map<std::string, JSFunction> JSFunction::FUNCTION_MAP;  