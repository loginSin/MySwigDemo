//
// Created by Qi on 2025/8/6.
//

#ifndef MYSWIGDEMO_CONTEXT_H
#define MYSWIGDEMO_CONTEXT_H
#include "libs/includes/rcim_client.h"
#include <jni.h>

/// callback 一系列 context 方法 ---------
void *genContextByCallback(const RcimEngineSync *engine, void *callback);
void *getFuncFromContextByCallback(const void *context);
const RcimEngineSync *getEngineFromContextByCallback(const void *context);
void releaseContextByCallback(const void *context);

/// listener 一系列 context 方法 ---------
void *genContextByListener(const RcimEngineSync *engine, void *listener);
void *getFuncFromContextByListener(const void *context);
const RcimEngineSync *getEngineFromContextByListener(const void *context);
//void releaseContextByListener(const void *context); // listener 的上下文不能释放

/// 发送消息 一系列 context 方法 ---------
void *genContextBySendMessageCallback(const RcimEngineSync *engine, void *callback);
void *getFuncFromContextBySendMessageCallback(const void *context);
const RcimEngineSync *getEngineFromContextBySendMessageCallback(const void *context);
void releaseContextBySendMessageCallback(const void *context);


#endif //MYSWIGDEMO_CONTEXT_H
