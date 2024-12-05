import hilog from '@ohos.hilog';
import { EditBoxWorkEntity } from '../../entity/WorkerMsgEntity';
import type { ThreadWorkerGlobalScope } from '@ohos.worker';

export class EditBoxWork {
  static MODULE_NAME: string = 'EditBoxWork';

  private static workerPort: ThreadWorkerGlobalScope;

  static init(workerPort: ThreadWorkerGlobalScope): void {
    EditBoxWork.workerPort = workerPort;
  }

  static handleMsgVoid(name: string, viewTag: number) {
    let editBoxWorkEntity: EditBoxWorkEntity = new EditBoxWorkEntity(EditBoxWork.MODULE_NAME, name);
    editBoxWorkEntity.viewTag = viewTag;
    EditBoxWork.workerPort.postMessage(editBoxWorkEntity);
  }

  static handleMsgBool(name: string, viewTag: number, value: boolean) {
    let editBoxWorkEntity: EditBoxWorkEntity = new EditBoxWorkEntity(EditBoxWork.MODULE_NAME, name);
    editBoxWorkEntity.viewTag = viewTag;
    editBoxWorkEntity.bool1 = value;
    EditBoxWork.workerPort.postMessage(editBoxWorkEntity);
  }

  static handleMsgNum1(name: string, viewTag: number, value1: number) {
    let editBoxWorkEntity: EditBoxWorkEntity = new EditBoxWorkEntity(EditBoxWork.MODULE_NAME, name);
    editBoxWorkEntity.viewTag = viewTag;
    editBoxWorkEntity.num1 = value1;
    EditBoxWork.workerPort.postMessage(editBoxWorkEntity);
  }

  static handleMsgNum2(name: string, viewTag: number, value1: number, value2: number) {
    let editBoxWorkEntity: EditBoxWorkEntity = new EditBoxWorkEntity(EditBoxWork.MODULE_NAME, name);
    editBoxWorkEntity.viewTag = viewTag;
    editBoxWorkEntity.num1 = value1;
    editBoxWorkEntity.num2 = value2;
    EditBoxWork.workerPort.postMessage(editBoxWorkEntity);
  }


  static handleMsgStr(name: string, viewTag: number, value: string) {
    let editBoxWorkEntity: EditBoxWorkEntity = new EditBoxWorkEntity(EditBoxWork.MODULE_NAME, name);
    editBoxWorkEntity.viewTag = viewTag;
    editBoxWorkEntity.str1 = value;
    EditBoxWork.workerPort.postMessage(editBoxWorkEntity);
  }
}
