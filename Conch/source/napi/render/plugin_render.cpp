#include <stdint.h>
#include <unistd.h>

#include "plugin_render.h"
#include "../../napi/plugin_manager.h"
#include "../modules/TouchesNapi.h"
#include "../../napi/NAPIFun.cpp"

#include <assert.h>
#include "util/Log.h"

using namespace laya;

#ifdef __cplusplus
extern "C" {
#endif

PluginRender* PluginRender::instance_ = nullptr;
OH_NativeXComponent_Callback PluginRender::callback_;
uint64_t PluginRender::animationInterval_ = 16;
uint64_t PluginRender::lastTime = 0;


void OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceCreatedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CREATED, component, window);
}

void OnSurfaceChangedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceChangedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CHANGED, component, window);
}

void OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceDestroyedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_DESTROY, component, window);
}


void DispatchTouchEventCB(OH_NativeXComponent* component, void* window)
{
    OH_NativeXComponent_TouchEvent* touchEvent = new(std::nothrow) OH_NativeXComponent_TouchEvent();
    if (!touchEvent) {
        LOGI("DispatchTouchEventCB::touchEvent alloc failed");
        return;
    }
    int32_t ret = OH_NativeXComponent_GetTouchEvent(component, window, touchEvent);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_TOUCH_EVENT, component, window, touchEvent);
    } else {
        delete touchEvent;
    }
}

PluginRender::PluginRender() : component_(nullptr)
{
    auto renderCallback = PluginRender::GetNXComponentCallback();
    renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

PluginRender* PluginRender::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new PluginRender();
    }
    return instance_;
}

OH_NativeXComponent_Callback* PluginRender::GetNXComponentCallback()
{
    return &PluginRender::callback_;
}

// static
void PluginRender::onMessageCallback(const uv_async_t* /* req */) {
    void* window = nullptr;
    WorkerMessageData msgData;
    PluginRender* render = PluginRender::GetInstance();

    while (true) {
        //loop until all msg dispatch
        if (!render->dequeue(reinterpret_cast<WorkerMessageData*>(&msgData))) {
            // Queue has no data
            break;
        }

        if ((msgData.type >= MessageType::WM_XCOMPONENT_SURFACE_CREATED) && (msgData.type <= MessageType::WM_XCOMPONENT_SURFACE_DESTROY)) {
            OH_NativeXComponent* nativexcomponet = reinterpret_cast<OH_NativeXComponent*>(msgData.data);
            // CC_ASSERT(nativexcomponet != nullptr);

            if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CREATED) {
                render->OnSurfaceCreated(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_TOUCH_EVENT) {
                render->DispatchTouchEvent(nativexcomponet, msgData.window, msgData.touchEvent);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CHANGED) {
                render->OnSurfaceChanged(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_DESTROY) {
                render->OnSurfaceDestroyed(nativexcomponet, msgData.window);
            } else {
                // CC_ASSERT(false);
            }
            continue;
        }

        if (msgData.type == MessageType::WM_APP_SHOW) {
            render->OnShowNative();
        } else if (msgData.type == MessageType::WM_APP_HIDE) {
            render->OnHideNative();
        } else if (msgData.type == MessageType::WM_APP_DESTROY) {
            render->OnDestroyNative();
        }
        if(msgData.type == MessageType::WM_VSYNC) {
            // render->runTask();
        }
    }
}

static uint64_t getCurrentMillSecond() {
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    return stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec / 1000; //millseconds
}

// static
void PluginRender::timerCb(uv_timer_t* handle)
{
    // LOGI("PluginRender::timerCb, animationInterval_ is %{public}lu", animationInterval_);
    if (PluginRender::GetInstance()->eglCore_ != nullptr) {
        PluginRender::GetInstance()->eglCore_->Update();
        NAPIFun::ConchNAPI_onDrawFrame();
    }
}

void PluginRender::SetNativeXComponent(OH_NativeXComponent* component)
{
    component_ = component;
    OH_NativeXComponent_RegisterCallback(component_, &PluginRender::callback_);
}

void PluginRender::workerInit(napi_env env, uv_loop_t* loop) {
    LOGI("PluginRender::workerInit");
    workerLoop_ = loop;
    if (workerLoop_) {
        uv_async_init(workerLoop_, &messageSignal_, reinterpret_cast<uv_async_cb>(PluginRender::onMessageCallback));
        if (!messageQueue_.empty()) {
            triggerMessageSignal(); // trigger the signal to handle the pending message
        }
    }
}


void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window) {
    WorkerMessageData data{type, static_cast<void*>(component), window};
    enqueue(data);
}

void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window, OH_NativeXComponent_TouchEvent* touchEvent) {
    WorkerMessageData data{type, static_cast<void*>(component), window, touchEvent};
    enqueue(data);
}

void PluginRender::enqueue(const WorkerMessageData& msg) {
    messageQueue_.enqueue(msg);
    triggerMessageSignal();
}

bool PluginRender::dequeue(WorkerMessageData* msg) {
    return messageQueue_.dequeue(msg);
}

void PluginRender::triggerMessageSignal() {
    if(workerLoop_ != nullptr) {
        // It is possible that when the message is sent, the worker thread has not yet started.
        uv_async_send(&messageSignal_);
    }
}

void PluginRender::run() {
    LOGI("PluginRender::run");
    if (workerLoop_) {
        uv_timer_init(workerLoop_, &timerHandle_);
        timerInited_ = true;
    }
}

void PluginRender::changeFPS(uint64_t animationInterval) {
    LOGI("PluginRender::changeFPS, animationInterval from %{public}lu to %{public}lu", animationInterval_, animationInterval);
    animationInterval_ = animationInterval;
}

void PluginRender::OnSurfaceCreated(OH_NativeXComponent* component, void* window)
{
    LOGI("PluginRender::OnSurfaceCreated");
    eglCore_ = new EGLCore();
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        eglCore_->GLContextInit(window, width_, height_);
        NAPIFun::ConchNAPI_OnGLReady(width_,height_);
    }
}

void PluginRender::OnSurfaceChanged(OH_NativeXComponent* component, void* window)
{
    LOGI("PluginRender::OnSurfaceChanged");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        NAPIFun::ConchNAPI_OnGLReady(width_,height_);
    }
}

void PluginRender::OnSurfaceDestroyed(OH_NativeXComponent* component, void* window)
{
}

void PluginRender::DispatchTouchEvent(OH_NativeXComponent* component, void* window, OH_NativeXComponent_TouchEvent* touchEvent)
{
    intptr_t ids[touchEvent->numPoints];
    float xs[touchEvent->numPoints];
    float ys[touchEvent->numPoints];
    for (int i = 0; i < touchEvent->numPoints; i++) {
        ids[i] = touchEvent->touchPoints[i].id;
        xs[i] = touchEvent->touchPoints[i].x;
        ys[i] = touchEvent->touchPoints[i].y;
        LOGI("Touch Info : x = %{public}f, y = %{public}f", xs[i], ys[i]);
    }
    switch (touchEvent -> type) {
        case OH_NATIVEXCOMPONENT_DOWN:
            nativeHandleTouchDown(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_DOWN");
            break;
        case OH_NATIVEXCOMPONENT_UP:
            nativeHandleTouchUp(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_UP");
            break;
        case OH_NATIVEXCOMPONENT_MOVE:
            nativeHandleTouchMove(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_MOVE");
            break;
        case OH_NATIVEXCOMPONENT_CANCEL:
            nativeHandleTouchCancel(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_CANCEL");
            break;
        case OH_NATIVEXCOMPONENT_UNKNOWN:
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_UNKNOWN");
            break;
        default:
            LOGI("Touch Info : default");
            break;
    }
    delete touchEvent;
}

void PluginRender::OnCreateNative(napi_env env, uv_loop_t* loop) {
    LOGI("PluginRender::OnCreateNative");
}

void PluginRender::OnShowNative() {
    LOGI("PluginRender::OnShowNative");
    if (timerInited_) {
        uv_timer_start(&timerHandle_, &PluginRender::timerCb, 0, 1);
    }
    JCAudioManager* am = JCAudioManager::GetInstance();
    if(am->getMp3Mute() == false && am ->getMp3Stopped() == false)
    {
        auto pFunction = std::bind(&JCAudioManager::resumeMp3,am);
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
}

void PluginRender::OnHideNative() {
    LOGI("PluginRender::OnHideNative");
    JCAudioManager* am =JCAudioManager::GetInstance();
    if(am->getMp3Mute() == false && am->getMp3Stopped() == false)
    {
        auto pFunction = std::bind(&JCAudioManager::pauseMp3,am);
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();

    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
}

void PluginRender::OnDestroyNative() {
    LOGI("PluginRender::OnDestoryNative");
    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
    NAPIFun::ConchNAPI_ReleaseDLib();
}

napi_value PluginRender::Export(napi_env env, napi_value exports)
{
    LOGI("PluginRender::Export");
    // Register JS API
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("changeShape", PluginRender::NapiChangeShape),
        DECLARE_NAPI_FUNCTION("drawTriangle", PluginRender::NapiDrawTriangle),
        DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColor),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

napi_value PluginRender::NapiChangeShape(napi_env env, napi_callback_info info)
{
    LOGI("PluginRender::NapiChangeShape");
    PluginRender* instance = PluginRender::GetInstance();
    if (instance) {
        instance->eglCore_->Update();
    }
    return nullptr;
}

napi_value PluginRender::NapiDrawTriangle(napi_env env, napi_callback_info info)
{
    LOGI("NapiDrawTriangle");
    return nullptr;
}

napi_value PluginRender::NapiChangeColor(napi_env env, napi_callback_info info)
{
    LOGI("NapiChangeColor");
    return nullptr;
}
#ifdef __cplusplus
}
#endif