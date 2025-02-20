import { GlobalContext, GlobalContextConstants } from "../common/GlobalContext";
import laya from "liblaya.so";

export class HandleMessageUtils {
    private static context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);

    private static workerPort;

    static init(workerPort): void {
        HandleMessageUtils.workerPort = workerPort;
    }

    static postCmdToMain(data: string): string {
        HandleMessageUtils.workerPort.postMessage({module:"reload", function:data});
        return "true";
    }
    static RunJS(data: string): void {
        laya.ConchNAPI_RunJS(data);
    }
    /**
    * 异步事件
    * @param eventName 事件名称
    * @param data 数据
    */
    static handleMessage(eventName: string, data: string): string {
        // 发送给游戏js在conch.setGameJsOnMessage((key,value)=>{});注册的函数中接受回调
        // let worker = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WORKER);
        // worker.postMessage({type:"注册的key",text:"返回的value"});
        return "true";
    }

    /**
    * 同步事件
    * @param eventName 事件名称
    * @param data 数据
    * @param cb callback
    */
    static handleSyncMessage(eventName: string, data: string, cb: Function): void {
        cb("");
    }

    /**
     * 游戏侧通过PlatformClass反射调用本地自定义方法
     * @param isSyn 是否同步方法
     * @param cName 脚本文件路径
     * @param mName 方法名称
     * @param nativeFunc 本地自定义方法
     * @param paramStr 传输数据
     */
    static executeNativeMethod(isSyn: boolean, cName: string, mName: string, nativeFunc: Function, paramStr: string): string {

        let jParam = JSON.parse(paramStr || "[]");

        let result = null;
        if (isSyn) {
            result = nativeFunc && nativeFunc.apply(this, jParam);
        } else {
            let paramCb = (call_re: string) => {
                let rsJson = {
                    objId: -1,
                    cName: cName,
                    mName: mName,
                    v: call_re
                }
                laya.ConchNAPI_RunJS("conch.platCallBack(" + JSON.stringify(rsJson) + ")");
            };
            jParam[jParam.length] = paramCb;
            result = nativeFunc && nativeFunc.apply(this, jParam);
        }

        let rsJson = {
            v: result
        }
        return JSON.stringify(rsJson);
    }
}