//
// Created by Qi on 2025/8/6.
//

#include "rc_adapter.h"
#include "libs/includes/rcim_client.h"
#include <jni.h>
#include "jni_helper/jvm.h"
#include "util/Transformer.h"
#include "util/Context.h"
#include "jni_header/NativeStringCallback_jni.h"
#include "jni_header/NativeIntListener_jni.h"

int create_engine_builder(RcimEngineBuilderParam *param, int64_t *outBuilderPtr) {
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

int64_t rcim_sdk_version_array_new(int size) {
    RcimSDKVersion *array = (RcimSDKVersion *) malloc(sizeof(RcimSDKVersion) * size);
    if (!array) return 0; // 内存分配失败
    return (int64_t) (uintptr_t) array; // 转成 int64_t 类型安全返回
}

void rcim_sdk_version_array_insert(int64_t ptr, int64_t *ptrArr, int size) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) ptr;
    for (size_t i = 0; i < size; ++i) {
        int64_t ptr = ptrArr[i];
        RcimSDKVersion *ver = (RcimSDKVersion *) ptr;
        array[i].name = ver->name;
        array[i].version = ver->version;
    }
}


void rcim_sdk_version_array_free(int64_t ptr) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) (intptr_t) ptr;
    free(array);
}

int engine_builder_set_store_path(int64_t builderPtr, const char *storePath) {
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

int engine_builder_build(int64_t builderPtr, int64_t *outEnginePtr) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
    RcimEngineSync *engine_ = nullptr;
    RcimEngineError code = rcim_engine_builder_build(builder, &engine_);

    // 向输出参数添加数据
    *outEnginePtr = static_cast<int64_t>(reinterpret_cast<uintptr_t>(engine_));
    return code;
}

void engine_connect_adapter(const void *context, enum RcimEngineError code, const char *user_id) {
    printf("qxb %s", user_id);
    rcim::callNativeStringCallback(context, code, user_id);
    releaseContextByCallback(context);
}

void engine_connect(int64_t enginePtr, const char *token, int timeout, void *callback) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    void *context = genContextByCallback(engine, callback);
    rcim_engine_connect(engine, token, timeout, context, engine_connect_adapter);
}

void engine_set_connection_status_listener_adapter(const void *context,
                                                   enum RcimConnectionStatus status) {
    rcim::callNativeIntListener(context, status);
}

void engine_set_connection_status_listener(int64_t enginePtr, void *listener) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    void *context = genContextByListener(engine, listener);
    rcim_engine_set_connection_status_listener(engine, context,
                                               engine_set_connection_status_listener_adapter);
}

void engine_send_message_saved(const void *context, const struct RcimMessageBox *msg_box) {
    int a = 2;
}

void engine_send_message_adapter(const void *context,
                                 enum RcimEngineError code,
                                 const struct RcimMessageBox *msg_box) {
    int a = 2;
}

void engine_send_message(int64_t enginePtr, RcimMessageBox *msgBox, void *sendMsgCallback) {
    // todo qixinbing
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_send_message(engine, msgBox, nullptr, nullptr, engine_send_message_adapter,
                             engine_send_message_saved);
}