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

int create_engine_builder(RcimEngineBuilderParam *param,
                          std::vector<long long> &outBuilderPtrArray) {
    if (!param) {
        return 0;
    }
    param->platform = RcimPlatform_Android;

    RcimEngineBuilder *builder;
    RcimEngineError code = rcim_create_engine_builder(param, &builder);
    // 向输出参数添加数据
    auto builderPtr = static_cast<long long>(reinterpret_cast<uintptr_t>(builder));
    outBuilderPtrArray.push_back(builderPtr);
    return code;
}

long long rcim_sdk_version_array_new(int size) {
    RcimSDKVersion *array = (RcimSDKVersion *) malloc(sizeof(RcimSDKVersion) * size);
    if (!array) return 0; // 内存分配失败
    return (long long)(uintptr_t) array; // 转成 long long 类型安全返回
}

void rcim_sdk_version_array_insert(long long ptr, std::vector<long long> longVec) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) ptr;
    for (size_t i = 0; i < longVec.size(); ++i) {
        long ptr = longVec[i];
        RcimSDKVersion *ver = (RcimSDKVersion *)ptr;
        array[i].name = ver->name;
        array[i].version = ver->version;
    }
}

void rcim_sdk_version_array_free(long long ptr) {
    if (ptr == 0) return;
    RcimSDKVersion *array = (RcimSDKVersion *) (intptr_t) ptr;
    free(array);
}

int engine_builder_set_store_path(long long builderPtr, std::string storePath) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));

    RcimEngineError code = rcim_engine_builder_set_store_path(builder, storePath.c_str());
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

int engine_builder_build(long long builderPtr, std::vector<long long> &outEngineArray) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
    RcimEngineSync *engine_ = nullptr;
    RcimEngineError code = rcim_engine_builder_build(builder, &engine_);

    // 向输出参数添加数据
    auto enginePtr = static_cast<long long>(reinterpret_cast<uintptr_t>(engine_));
    outEngineArray.push_back(enginePtr);

    return code;
}

void engine_connect_adapter(const void *context, enum RcimEngineError code, const char *user_id) {
    printf("qxb %s", user_id);
    rcim::callNativeStringCallback(context, code, user_id);
//    releaseContextByCallback(context);
}

void engine_connect(long long enginePtr, std::string token, int timeout, void *callback) {
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    void *context = genContextByCallback(engine, callback);
    rcim_engine_connect(engine, token.c_str(), timeout, context, engine_connect_adapter);
}

void engine_set_connection_status_listener_adapter(const void *context,
                                                   enum RcimConnectionStatus status) {
    rcim::callNativeIntListener(context, status);
}

void engine_set_connection_status_listener(long long enginePtr, void *listener) {
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

void engine_send_message(long long enginePtr, RcimMessageBox *msgBox, void *sendMsgCallback) {
    // todo qixinbing
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_send_message(engine, msgBox, nullptr, nullptr, engine_send_message_adapter,
                             engine_send_message_saved);
}