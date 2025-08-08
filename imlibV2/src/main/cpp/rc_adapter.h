#ifndef MYSWIGDEMO_RC_ADAPTER_H
#define MYSWIGDEMO_RC_ADAPTER_H

#include <stdint.h>

#include "libs/includes/rcim_client.h"
#include "RcimNativeFunctionDefine.h"

// 不能使用 long 、 long long，所有需要用long 、long long、指针的地方统一用 int64_t
// swig 会把 C 的 long 默认转成 int，64 位变 32 位，精度丢失
// swig 默认不支持 long long 类型

int createEngineBuilder(RcimEngineBuilderParam *param, int64_t *outBuilderPtr);

int64_t newSdkVersionArray(int size);

void insertSdkVersionArray(int64_t ptr, int64_t *ptrArr, int size);

void freeSdkVersionArray(int64_t ptr);

int engineBuilderSetStorePath(int64_t builderPtr, const char *storePath);

int engineBuilderBuild(int64_t builderPtr, int64_t *outEnginePtr);

void engineConnect(int64_t enginePtr, const char *token, int timeout,
                   RcimNativeStringCallback *callback);

void engineSetConnectionStatusListener(int64_t enginePtr, RcimNativeIntListener *listener);

void engineSendMessage(int64_t enginePtr, RcimMessageBox *msgBox,
                       RcimNativeSendMessageCallback *sendMsgCallback);

#endif //MYSWIGDEMO_RC_ADAPTER_H
