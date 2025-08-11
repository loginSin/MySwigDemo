#ifndef MYSWIGDEMO_RCCLIENT_H
#define MYSWIGDEMO_RCCLIENT_H

#include <cstdint>
#include <vector>

#include "libs/includes/rcim_client.h"
#include "RcimNativeFunctionDefine.h"

// 不能使用 long 、 long long，所有需要用long 、long long、指针的地方统一用 int64_t
// swig 会把 C 的 long 默认转成 int，64 位变 32 位，精度丢失
// swig 默认不支持 long long 类型

namespace rcim {

int createEngineBuilder(RcimEngineBuilderParam *param, int64_t *outBuilderPtr);

int64_t newSdkVersionArray(int size);

void insertSdkVersionArray(int64_t ptr, int64_t *ptrArr, int size);

void freeSdkVersionArray(int64_t ptr);

int engineBuilderSetStorePath(int64_t builderPtr, const char *storePath);
int engineBuilderSetAreaCode(int64_t builderPtr, int areaCode);
int engineBuilderSetNaviServer(int64_t builderPtr, std::vector<std::string> naviVec);
int engineBuilderSetStatisticServer(int64_t builderPtr, const char *StatisticServer);
int engineBuilderSetCloudType(int64_t builderPtr, int cloudType);
int engineBuilderSetDbEncrypted(int64_t builderPtr, bool encrypted);
int engineBuilderSetEnableGroupCall(int64_t builderPtr, bool enable);
int engineBuilderSetEnableReconnectKick(int64_t builderPtr, bool enable);
int engineBuilderSetFilePath(int64_t builderPtr, const char *filePath);
int engineBuilderSetNetworkEnv(int64_t builderPtr, const char *networkEnv);

int engineBuilderBuild(int64_t builderPtr, int64_t *outEnginePtr);

void engineConnect(int64_t enginePtr, const char *token, int timeout,
                   RcimNativeStringCallback *callback);

void engineSetConnectionStatusListener(int64_t enginePtr, RcimNativeIntListener *listener);

void engineSendMessage(int64_t enginePtr, RcimMessageBox *msgBox,
                       RcimNativeSendMessageCallback *sendMsgCallback);
} // namespace rcim
#endif //MYSWIGDEMO_RCCLIENT_H
