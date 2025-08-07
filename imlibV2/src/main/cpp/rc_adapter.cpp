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

int create_engine_builder(void *javaParam, std::vector<long long> &outBuilderPtrArray) {
    if (!javaParam) {
        return 0;
    }

    // 注意：在实际应用中，应该通过正确的方式获取 JNIEnv
    // 这里假设 g_jni_env 是可用的
    JNIEnv *env = RongCloud::jni::GetEnv();
    if (!env) {
        return 0;
    }

    auto param = (jobject) javaParam;

    std::string appKey = rcim::getStringFromJObject(env, param, "appKey");
    std::string deviceId = rcim::getStringFromJObject(env, param, "deviceId");
    std::string packageName = rcim::getStringFromJObject(env, param, "packageName");
    std::string imlibVersion = rcim::getStringFromJObject(env, param, "imlibVersion");
    std::string deviceModel = rcim::getStringFromJObject(env, param, "deviceModel");
    std::string deviceManufacturer = rcim::getStringFromJObject(env, param, "deviceManufacturer");
    std::string osVersion = rcim::getStringFromJObject(env, param, "osVersion");
    std::string appVersion = rcim::getStringFromJObject(env, param, "appVersion");
    std::map <std::string, std::string> sdkVersions = rcim::getMapStringFromJObject(env, param,
                                                                                    "sdkVersions");


    RcimEngineBuilderParam c_param;
    c_param.app_key = appKey.c_str();
    c_param.platform = RcimPlatform_Android;
    c_param.device_id = deviceId.c_str();
    c_param.package_name = packageName.c_str();
    c_param.imlib_version = imlibVersion.c_str();
    c_param.device_model = deviceModel.c_str();
    c_param.device_manufacturer = deviceManufacturer.c_str();
    c_param.os_version = osVersion.c_str();

    const size_t size = sdkVersions.size();
    RcimSDKVersion c_sdkVersions[size];
    int index = 0;
    for (const auto &[key, value]: sdkVersions) {
        c_sdkVersions[index].name = key.c_str();
        c_sdkVersions[index].version = value.c_str();
    }

    c_param.sdk_version_vec = c_sdkVersions;
    c_param.sdk_version_vec_len = size;
    c_param.app_version = appVersion.c_str();
    RcimEngineBuilder *builder;
    RcimEngineError code = rcim_create_engine_builder(&c_param, &builder);

    // 向输出参数添加数据
    auto builderPtr = static_cast<long long>(reinterpret_cast<uintptr_t>(builder));
    outBuilderPtrArray.push_back(builderPtr);

    return code;
}

int engine_builder_set_store_path(long long builderPtr, std::string storePath) {
    auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
//    RcimEngineError code = rcim_engine_builder_set_store_path(builder,
//                                              "/data/user/0/io.rong.rust.android_rust_imsdk/files");

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
    releaseContextByCallback(context);
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

}

void engine_send_message_adapter(const void *context,
                                 enum RcimEngineError code,
                                 const struct RcimMessageBox *msg_box) {

}

void engine_send_message(long long enginePtr, RcimMessageBox *msgBox, void *sendMsgCallback) {
    // todo qixinbing
    auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
    rcim_engine_send_message(engine, msgBox, nullptr, nullptr, engine_send_message_adapter,
                             engine_send_message_saved);
}