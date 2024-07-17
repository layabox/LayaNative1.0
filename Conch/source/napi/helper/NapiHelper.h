#include "../conch/JCConch.h"
#include <hilog/log.h>

using namespace laya;

class NapiHelper {
public:
    ~NapiHelper() {}
    static NapiHelper *GetInstance() { return &NapiHelper::help_; }
    std::string getDeviceInfo();
    int getNetworkType();
    void startVibration(float duration);
    void playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime);
    void pauseBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float p_nVolume);
    void resumeBackgroundMusic();
    void setCurrentTime(double nCurrentTime);
    double getCurrentTime();
    double getDuration();
    std::string getAppVersion();
    std::string getAppLocalVersion();

    void showDialog(const char *p_sBuffer);
    void setKeepScreenOn(bool value);
    void setPreferredOrientation(int orientation);
    float getScreenInch();
    int getAvalidMem();
    int getUsedMem();
    void exitGame();
    void enableAccelerometer();
    void disableAccelerometer();
    void enableOrientation();
    void disableOrientation();
    void handleEditBoxMsg(std::string name, int m_tag);
    void handleEditBoxMsgStr(std::string name, int m_tag, std::string value);
    std::string handleEditBoxMsgGetStr(std::string name, int m_tag);

    void handleEditBoxMsgBool(std::string name, int m_tag, bool value);
    void handleEditBoxMsgNum1(std::string name, int m_tag, float value1);
    void handleEditBoxMsgNum2(std::string name, int m_tag, float value1, float value2);

    std::string postMessageToUIThread(std::string eventName, std::string data);
    std::string postSyncMessageToUIThread(std::string eventName, std::string data);
    std::string postCmdToMain(std::string data);

private:
    std::string __getDeviceInfo();
    int __getNetworkType();
    void __startVibration(float duration);
    void __playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime);
    void __pauseBackgroundMusic();
    void __stopBackgroundMusic();
    void __setBackgroundMusicVolume(float p_nVolume);
    void __resumeBackgroundMusic();
    void __setCurrentTime(double nCurrentTime);
    double __getCurrentTime();
    double __getDuration();
    std::string __getAppVersion();
    std::string __getAppLocalVersion();

    void __showDialog(const char *p_sBuffer);
    void __setKeepScreenOn(bool value);
    void __setPreferredOrientation(int orientation);
    float __getScreenInch();
    int __getAvalidMem();
    int __getUsedMem();
    void __exitGame();
    void __enableAccelerometer();
    void __disableAccelerometer();
    void __enableOrientation();
    void __disableOrientation();
    void __handleEditBoxMsg(std::string name, int m_tag);
    void __handleEditBoxMsgStr(std::string name, int m_tag, std::string value);
    std::string __handleEditBoxMsgGetStr(std::string name, int m_tag);

    void __handleEditBoxMsgBool(std::string name, int m_tag, bool value);
    void __handleEditBoxMsgNum1(std::string name, int m_tag, float value1);
    void __handleEditBoxMsgNum2(std::string name, int m_tag, float value1, float value2);

    std::string __postMessageToUIThread(std::string eventName, std::string data);
    std::string __postSyncMessageToUIThread(std::string eventName, std::string data);
    std::string __postCmdToMain(std::string data);

    static NapiHelper help_;

private:
    std::string deviceInfo;
    std::string appVersion;
    std::string appLocalVersion;
    std::string eventResult;
    std::string syncEventResult;
    int networkType;
    int volume;
    double currentTime;
    double duration;
    float screenInch;
    int avalidMem;
    int usedMem;
    std::string inputInfoValue;
};