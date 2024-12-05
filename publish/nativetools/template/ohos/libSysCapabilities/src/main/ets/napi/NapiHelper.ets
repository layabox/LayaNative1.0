import { Dialog } from '../components/dialog/DialogWorker'
import { JumpManager } from '../system/appJump/JumpManager'
import { DeviceUtils } from '../system/device/DeviceUtils'
import { NetworkUtils } from '../system/device/NetworkUtils'
import ApplicationManager from '../system/application/ApplicationManager'
import { WebView } from '../components/webview/WebView'
import SoundUtils from '../system/sound/SoundUtils'
import laya from "liblaya.so"
import { EditBox } from '../components/editBox/EditBox'
import { EditBoxWork } from '../components/editBox/EditBoxWork'
import { HandleMessageUtils } from '../Event/HandleMessageUtils'

export class NapiHelper {
  // 需要将JS代码执行在worker线程的在此处注册方法
  static registerWorkerFunctions() {
    NapiHelper.registerLayaWorkerFunc();
    NapiHelper.registerSound();
    NapiHelper.registerWebView();
  }

  // 需要将JS代码执行在UI线程的在此处注册方法
  static registerUIFunctions(registerFunction: Function) {
    laya.JSBind.bindFunction("DeviceUtils.getScreenInch", DeviceUtils.getScreenInch);
    laya.JSBind.bindFunction('DeviceUtils.setKeepScreenOn', DeviceUtils.setKeepScreenOn);
    laya.JSBind.bindFunction('DeviceUtils.setPreferredOrientation', DeviceUtils.setPreferredOrientation);
    laya.JSBind.bindFunction("ApplicationManager.exit", ApplicationManager.exit);
    laya.JSBind.bindFunction("HandleMessageUtils.handleMessage", HandleMessageUtils.handleMessage);
    laya.JSBind.bindFunction("HandleMessageUtils.handleSyncMessage", HandleMessageUtils.handleSyncMessage);
    laya.JSBind.bindFunction('EditBox.handleMsgGetValue', EditBox.handleMsgGetValue);
    registerFunction("HandleMessageUtils.executeNativeMethod", HandleMessageUtils.executeNativeMethod);
  }

  private static registerLayaWorkerFunc() {
    laya.JSBind.bindFunction('EditBox.handleMsgVoid', EditBoxWork.handleMsgVoid);
    laya.JSBind.bindFunction('EditBox.handleMsgStr', EditBoxWork.handleMsgStr);
    laya.JSBind.bindFunction('EditBox.handleMsgBool', EditBoxWork.handleMsgBool);
    laya.JSBind.bindFunction('EditBox.handleMsgNum1', EditBoxWork.handleMsgNum1);
    laya.JSBind.bindFunction('EditBox.handleMsgNum2', EditBoxWork.handleMsgNum2);

    laya.JSBind.bindFunction("DeviceUtils.startVibration", DeviceUtils.startVibration);
    laya.JSBind.bindFunction("DeviceUtils.getDeviceInfo", DeviceUtils.getDeviceInfo);
    laya.JSBind.bindFunction("ApplicationManager.getApplicationInfo", ApplicationManager.getApplicationInfo);
    laya.JSBind.bindFunction("DeviceUtils.getPrivateDirty", DeviceUtils.getPrivateDirty);
    laya.JSBind.bindFunction("DeviceUtils.getAvalidMem", DeviceUtils.getAvalidMem);
    laya.JSBind.bindFunction('Dialog.showDialog', Dialog.showDialog);
    laya.JSBind.bindFunction("NetworkUtils.getNetworkType", NetworkUtils.getNetworkType);
    laya.JSBind.bindFunction("HandleMessageUtils.postCmdToMain", HandleMessageUtils.postCmdToMain);
  }

  private static registerSound() {
    laya.JSBind.bindFunction('SoundUtils.playBackgroundMusic', SoundUtils.playBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.pauseBackgroundMusic', SoundUtils.pauseBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.stopBackgroundMusic', SoundUtils.stopBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.resumeBackgroundMusic', SoundUtils.resumeBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.setBackgroundMusicVolume', SoundUtils.setBackgroundMusicVolume);
    laya.JSBind.bindFunction('SoundUtils.setCurrentTime', SoundUtils.setCurrentTime);
    laya.JSBind.bindFunction('SoundUtils.getCurrentTime', SoundUtils.getCurrentTime);
    laya.JSBind.bindFunction('SoundUtils.getDuration', SoundUtils.getDuration);
  }


  private static registerWebView() {
    console.log("register createWebView");
    laya.JSBind.bindFunction('WebUtils.createWebview', WebView.createWebView);
    laya.JSBind.bindFunction('WebUtils.closeWebview', WebView.closeWebview);
    laya.JSBind.bindFunction('WebUtils.callWebviewJS', WebView.callWebviewJS);
    laya.JSBind.bindFunction('WebUtils.hideWebview', WebView.hideWebview);
    laya.JSBind.bindFunction('WebUtils.showWebView', WebView.showWebView);
  }
}


