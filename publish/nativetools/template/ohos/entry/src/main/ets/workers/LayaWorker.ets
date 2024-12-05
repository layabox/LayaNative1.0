import worker, { MessageEvents, ThreadWorkerGlobalScope } from '@ohos.worker';
import laya from "liblaya.so";
import { ContextType } from "@ohos/libSysCapabilities"
import { Dialog } from "@ohos/libSysCapabilities"
import { WebView } from "@ohos/libSysCapabilities"
import { JumpManager } from "@ohos/libSysCapabilities"
import { EditBoxWork} from "@ohos/libSysCapabilities"
import { HandleMessageUtils } from "@ohos/libSysCapabilities"
import { NapiHelper } from "@ohos/libSysCapabilities/src/main/ets/napi/NapiHelper"
import { GlobalContext, GlobalContextConstants } from "@ohos/libSysCapabilities"
import { WorkerDataEntity } from './WorkerDataEntity';

const appLifecycle: laya.CPPFunctions = laya.getContext(ContextType.APP_LIFECYCLE);
const workerContext: laya.CPPFunctions = laya.getContext(ContextType.WORKER_INIT);
const napiContext: laya.CPPFunctions = laya.getContext(ContextType.NATIVE_API);
workerContext.workerInit();

napiContext.nativeEngineStart();
NapiHelper.registerWorkerFunctions()

const workerPort: ThreadWorkerGlobalScope = worker.workerPort;

workerPort.onmessage = (e: MessageEvents): void => {
  let data: WorkerDataEntity = e.data;
  switch (data.type) {
    case "onXCLoad":
      console.log("[laya] onXCLoad Callback");
      Dialog.init(workerPort);
      JumpManager.init(workerPort);
      EditBoxWork.init(workerPort);
      WebView.init(workerPort);
      HandleMessageUtils.init(workerPort);
      laya.ConchNAPI_OnAppStart();
      break;
    case "abilityContextInit":
      GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT, data.context);
      break;
    case "exit":
      appLifecycle.onBackPress();
      break;
    default:
      laya.ConchNAPI_gameMsgHandle(data.type,data.text);
      console.error("laya worker: message type unknown");
  }
}


