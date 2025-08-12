
#include "RcClient.h"
#include "libs/includes/rcim_client.h"
#include <jni.h>

namespace rcim {
//// ---------------------------------------创建引擎----------------------------------------------------
    int createEngineBuilder(RcimEngineBuilderParam *param, int64_t *outBuilderPtr) {
        if (param == nullptr) {
            return RcimEngineError_InvalidArgumentEngineBuilderParam;
        }
        param->platform = RcimPlatform_Android;

        RcimEngineBuilder *builder;
        RcimEngineError code = rcim_create_engine_builder(param, &builder);
        // 向输出参数添加数据
        *outBuilderPtr = static_cast<int64_t>(reinterpret_cast<uintptr_t>(builder));
        return code;
    }

    int64_t newSdkVersionArray(int size) {
        auto *array = (RcimSDKVersion *) malloc(sizeof(RcimSDKVersion) * size);
        if (!array) return 0; // 内存分配失败
        // 转成 int64_t 类型安全返回
        return (int64_t) (uintptr_t) array;
    }

    void insertSdkVersionArray(int64_t ptr, int64_t *ptrArr, int size) {
        if (ptr == 0) return;
        auto *array = (RcimSDKVersion *) ptr;
        for (size_t i = 0; i < size; ++i) {
            int64_t ptr = ptrArr[i];
            auto *ver = (RcimSDKVersion *) ptr;
            array[i].name = ver->name;
            array[i].version = ver->version;
        }
    }


    void freeSdkVersionArray(int64_t ptr) {
        if (ptr == 0) return;
        auto *array = (RcimSDKVersion *) (intptr_t) ptr;
        free(array);
    }

    int engineBuilderSetStorePath(int64_t builderPtr, const char *storePath) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_store_path(builder, storePath);
    }

    int engineBuilderSetAreaCode(int64_t builderPtr, int areaCode) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        RcimAreaCode code = RcimAreaCode_Bj;
        if (RcimAreaCode_Sg == areaCode) {
            code = RcimAreaCode_Sg;
        } else if (RcimAreaCode_Na == areaCode) {
            code = RcimAreaCode_Na;
        } else if (RcimAreaCode_SgB == areaCode) {
            code = RcimAreaCode_SgB;
        } else if (RcimAreaCode_Sa == areaCode) {
            code = RcimAreaCode_Sa;
        }
        return rcim_engine_builder_set_area_code(builder, code);
    }

    int engineBuilderSetNaviServer(int64_t builderPtr, std::vector<std::string> naviVec) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));

        if (naviVec.empty()) {
            return rcim_engine_builder_set_navi_server(builder, nullptr, 0);
        } else {
            int32_t size = naviVec.size();
            char **arr = new char *[size];
            for (size_t i = 0; i < size; ++i) {
                arr[i] = new char[naviVec[i].size() + 1];
                std::strcpy(arr[i], naviVec[i].c_str());
            }
            rcim_engine_builder_set_navi_server(builder, arr, size);

            for (size_t i = 0; i < size; ++i) {
                delete[] arr[i];
            }
            delete[] arr;
            return rcim_engine_builder_set_navi_server(builder, arr, size);
        }
    }

    int engineBuilderSetStatisticServer(int64_t builderPtr, const char *statisticServer) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_statistic_server(builder, statisticServer);
    }

    int engineBuilderSetCloudType(int64_t builderPtr, int cloudType) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));

        RcimCloudType c_cloudType = RcimCloudType_PublicCloud;
        if (106 == cloudType) {
            c_cloudType = RcimCloudType_PrivateCloud;
        } else if (104 == cloudType) {
            c_cloudType = RcimCloudType_PrivateCloud104;
        }
        return rcim_engine_builder_set_cloud_type(builder, c_cloudType);
    }

    int engineBuilderSetDbEncrypted(int64_t builderPtr, bool encrypted) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_db_encrypted(builder, encrypted);
    }

    int engineBuilderSetEnableGroupCall(int64_t builderPtr, bool enable) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_enable_group_call(builder, enable);
    }

    int engineBuilderSetEnableReconnectKick(int64_t builderPtr, bool enable) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_enable_reconnect_kick(builder, enable);
    }

    int engineBuilderSetFilePath(int64_t builderPtr, const char *filePath) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_file_path(builder, filePath);
    }

    int engineBuilderSetNetworkEnv(int64_t builderPtr, const char *networkEnv) {
        if (builderPtr == 0) {
            return RcimEngineError_InvalidArgumentEngineBuilder;
        }

        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        return rcim_engine_builder_set_network_env(builder, networkEnv);
    }

    int engineBuilderBuild(int64_t builderPtr, int64_t *outEnginePtr) {
        auto *builder = reinterpret_cast<RcimEngineBuilder *>(static_cast<uintptr_t>(builderPtr));
        RcimEngineSync *engine_ = nullptr;
        RcimEngineError code = rcim_engine_builder_build(builder, &engine_);

        // 向输出参数添加数据
        *outEnginePtr = static_cast<int64_t>(reinterpret_cast<uintptr_t>(engine_));
        return code;
    }

//// ---------------------------------------连接----------------------------------------------------

    void engineConnectAdapter(const void *context, enum RcimEngineError code, const char *user_id) {
        auto *callback = static_cast<RcimNativeStringCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onResult(callback, code, user_id);
        }
    }

    void engineConnect(int64_t enginePtr, const char *token, int timeout,
                       RcimNativeStringCallback *callback) {
        if (enginePtr == 0) {
            if (callback) {
                callback->onResult(callback, RcimEngineError_InvalidArgumentEngineSync, nullptr);
            }
            return;
        }
        auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
        rcim_engine_connect(engine, token, timeout, callback, engineConnectAdapter);
    }

    void engineSetConnectionStatusListenerAdapter(const void *context,
                                                  enum RcimConnectionStatus status) {
        auto *listener = static_cast<RcimNativeIntListener *>(const_cast<void *>(context));
        if (listener) {
            listener->onChanged(status);
        }
    }

    void engineSetConnectionStatusListener(int64_t enginePtr, RcimNativeIntListener *listener) {
        if (enginePtr == 0) {
            return;
        }
        auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
        rcim_engine_set_connection_status_listener(engine, listener,
                                                   engineSetConnectionStatusListenerAdapter);
    }

//// ---------------------------------------发送消息----------------------------------------------------

    void engineSendMessageSaved(const void *context, const struct RcimMessageBox *msg_box) {
        auto *callback = static_cast<RcimNativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onSave(msg_box);
        }
    }

    void engineSendMessageAdapter(const void *context,
                                  enum RcimEngineError code,
                                  const struct RcimMessageBox *msg_box) {
        auto *callback = static_cast<RcimNativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onResult(callback, code, msg_box);
        }
    }

    void engineSendMessage(int64_t enginePtr, RcimMessageBox *msgBox,
                           RcimNativeSendMessageCallback *sendMsgCallback) {
        if (enginePtr == 0) {
            if (sendMsgCallback) {
                sendMsgCallback->onResult(sendMsgCallback,
                                          RcimEngineError_InvalidArgumentEngineSync, msgBox);
            }
            return;
        }
        auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
        rcim_engine_send_message(engine, msgBox, nullptr, sendMsgCallback, engineSendMessageAdapter,
                                 engineSendMessageSaved);
    }

    void engineSetMessageReceivedListenerAdapter(const void *context,
                                                 const struct RcimMessageBox *msg_box,
                                                 const struct RcimReceivedInfo *info) {
        auto *listener = static_cast<RcimNativeMessageReceivedListener *>(const_cast<void *>(context));
        if (listener) {
            listener->onChanged(msg_box, info);
        }
    }

    int engineSetMessageReceivedListener(int64_t enginePtr,
                                         RcimNativeMessageReceivedListener *listener) {
        if (enginePtr == 0) {
            return RcimEngineError_InvalidArgumentEngineSync;
        }
        auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
        return rcim_engine_set_message_received_listener(engine, listener,
                                                         engineSetMessageReceivedListenerAdapter);
    }

} // namespace rcim
