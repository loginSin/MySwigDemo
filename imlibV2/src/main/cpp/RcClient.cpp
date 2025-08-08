
#include "RcClient.h"
#include "libs/includes/rcim_client.h"
#include <jni.h>

namespace rcim {

int createEngineBuilder(RcimEngineBuilderParam *param, int64_t *outBuilderPtr) {
    if (!param) {
        return 0;
    }
    param->platform = RcimPlatform_Android;

    RcimEngineBuilder *builder;
    RcimEngineError code = rcim_create_engine_builder(param, &builder);
    // 向输出参数添加数据
    *outBuilderPtr = static_cast<int64_t>(reinterpret_cast<uintptr_t>(builder));
    return code;
}

int64_t newSdkVersionArray(int size) {
    RcimSDKVersion *array = (RcimSDKVersion *) malloc(sizeof(RcimSDKVersion) * size);
    if (!array) return 0; // 内存分配失败
    // 转成 int64_t 类型安全返回
    return (int64_t)(uintptr_t) array;
}

void insertSdkVersionArray(int64_t ptr, int64_t *ptrArr, int size) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) ptr;
    for (size_t i = 0; i < size; ++i) {
        int64_t ptr = ptrArr[i];
        RcimSDKVersion *ver = (RcimSDKVersion *) ptr;
        array[i].name = ver->name;
        array[i].version = ver->version;
    }
}


void freeSdkVersionArray(int64_t ptr) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) (intptr_t) ptr;
    free(array);
}

int engineBuilderSetStorePath(int64_t builderPtr, const char *storePath) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));

    RcimEngineError code = rcim_engine_builder_set_store_path(builder, storePath);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_area_code(builder, RcimAreaCode_Bj);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_navi_server(builder, nullptr, 0);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_statistic_server(builder, nullptr);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_cloud_type(builder, RcimCloudType_PrivateCloud);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_db_encrypted(builder, false);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_enable_group_call(builder, false);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_enable_reconnect_kick(builder, false);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_file_path(builder, nullptr);
    if (RcimEngineError_Success != code) {
        return code;
    }
    code = rcim_engine_builder_set_network_env(builder, nullptr);
    return code;
}

int engineBuilderBuild(int64_t builderPtr, int64_t *outEnginePtr) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
    RcimEngineSync *engine_ = nullptr;
    RcimEngineError code = rcim_engine_builder_build(builder, &engine_);

    // 向输出参数添加数据
    *outEnginePtr = static_cast<int64_t>(reinterpret_cast<uintptr_t>(engine_));
    return code;
}

void engineConnectAdapter(const void *context, enum RcimEngineError code, const char *user_id) {
    RcimNativeStringCallback *callback = static_cast<RcimNativeStringCallback *>(const_cast<void *>(context));
    if (callback) {
        callback->onResult(callback, code, user_id);
    }
}

void engineConnect(int64_t enginePtr, const char *token, int timeout,
                   RcimNativeStringCallback *callback) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_connect(engine, token, timeout, callback, engineConnectAdapter);
}

void engineSetConnectionStatusListenerAdapter(const void *context,
                                              enum RcimConnectionStatus status) {
    RcimNativeIntListener *listener = static_cast<RcimNativeIntListener *>(const_cast<void *>(context));
    if (listener) {
        listener->onChanged(status);
    }
}

void engineSetConnectionStatusListener(int64_t enginePtr, RcimNativeIntListener *listener) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_set_connection_status_listener(engine, listener,
                                               engineSetConnectionStatusListenerAdapter);
}

void engineSendMessageSaved(const void *context, const struct RcimMessageBox *msg_box) {
    RcimNativeSendMessageCallback *callback = static_cast<RcimNativeSendMessageCallback *>(const_cast<void *>(context));
    if (callback) {
        callback->onSave(msg_box);
    }
}

void engineSendMessageAdapter(const void *context,
                              enum RcimEngineError code,
                              const struct RcimMessageBox *msg_box) {
    RcimNativeSendMessageCallback *callback = static_cast<RcimNativeSendMessageCallback *>(const_cast<void *>(context));
    if (callback) {
        callback->onResult(callback, code, msg_box);
    }
}

void engineSendMessage(int64_t enginePtr, RcimMessageBox *msgBox,
                       RcimNativeSendMessageCallback *sendMsgCallback) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_send_message(engine, msgBox, nullptr, sendMsgCallback, engineSendMessageAdapter,
                             engineSendMessageSaved);
}

} // namespace rcim
