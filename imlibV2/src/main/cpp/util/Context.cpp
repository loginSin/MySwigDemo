//
// Created by Qi on 2025/8/6.
//

#include "Context.h"
#include "jni_helper/jvm.h"

struct FunctionContext {
    const RcimEngineSync *engine_ = nullptr;
    jobject funcPtr = nullptr;
};

///  context 基础方法 ---------
void *genContextByFunction(const RcimEngineSync *engine, void *funcPtr) {
    auto *context = new FunctionContext();
    context->engine_ = engine;

    jobject jFunction = (jobject) funcPtr;
    JNIEnv *env = RongCloud::jni::GetEnv();
    jobject jFunctionRef = env->NewGlobalRef(jFunction);

    context->funcPtr = jFunctionRef;

    printf("qxb genContextByFunction context:%p,engine:%p,env:%p,funcPtr:%p", context,
           context->engine_, context->engine_, context->funcPtr);
    return static_cast<void *>(context);
}

void *getFuncFromContextByFunction(const void *context) {
    auto *ctx = static_cast<const FunctionContext *>(context);
    void *functionPtr = ctx->funcPtr;
    printf("qxb getFuncFromContextByFunction context:%p,engine:%p,env:%p,funcPtr:%p",
           context,
           ctx->engine_, ctx->engine_, ctx->funcPtr);
    return functionPtr;
}

const RcimEngineSync *getEngineFromContextByFunction(const void *context) {
    auto *ctx = (FunctionContext *) context;
    printf("qxb getEngineFromContextByFunction context:%p,engine:%p,env:%p,funcPtr:%p",
           context,
           ctx->engine_, ctx->engine_, ctx->funcPtr);
    return ctx->engine_;
}

void releaseContextByFunction(const void *context) {
    if (context == nullptr) {
        return;
    }
    auto *ctx = static_cast<const FunctionContext *>(context);
    if (ctx->funcPtr != nullptr) {
        JNIEnv *env = RongCloud::jni::GetEnv();
        env->DeleteGlobalRef(ctx->funcPtr);
    }
    delete context;
}

/// callback 一系列 context 方法 ---------
void *genContextByCallback(const RcimEngineSync *engine, void *callback) {
    return genContextByFunction(engine, callback);
}

void *getFuncFromContextByCallback(const void *context) {
    return getFuncFromContextByFunction(context);
}

const RcimEngineSync *getEngineFromContextByCallback(const void *context) {
    return getEngineFromContextByFunction(context);
}

void releaseContextByCallback(const void *context) {
    releaseContextByFunction(context);
}


/// listener 一系列 context 方法 ---------
void *genContextByListener(const RcimEngineSync *engine, void *listener) {
    return genContextByFunction(engine, listener);
}

void *getFuncFromContextByListener(const void *context) {
    return getFuncFromContextByFunction(context);
}

const RcimEngineSync *getEngineFromContextByListener(const void *context) {
    return getEngineFromContextByFunction(context);
}

/// 发送消息 一系列 context 方法 ---------
void *genContextBySendMessageCallback(const RcimEngineSync *engine, void *callback) {
    return genContextByFunction(engine, callback);
}

void *getFuncFromContextBySendMessageCallback(const void *context) {
    return getFuncFromContextByFunction(context);
}

const RcimEngineSync *getEngineFromContextBySendMessageCallback(const void *context) {
    return getEngineFromContextByFunction(context);
}

void releaseContextBySendMessageCallback(const void *context) {
    releaseContextByFunction(context);
}