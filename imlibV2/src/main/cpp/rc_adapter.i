
/* rc_adapter.i */
%module(directors="1") rc_adapter  // 生成的 Java 类名


%include "swig/base.i"
%include "swig/struct_define.i"

%{
#include "ConnectCallback.h"
#include "NativeSendMessageCallback.h"
#include "NativeIntListener.h"
#include "rc_adapter.h"
%}


int create_engine_builder(RcimEngineBuilderParam *param, int64_t *OUTPUT);
int engine_builder_build(int64_t builderPtr, int64_t *OUTPUT);

// 启用 director 功能，允许 Java 继承 C++ 类
%feature("director") ConnectCallback;
%feature("director") NativeSendMessageCallback;
%feature("director") NativeIntListener;

// 包含头文件 - 必须在类型映射之后包含
%include "ConnectCallback.h"
%include "NativeSendMessageCallback.h"
%include "NativeIntListener.h"
%include "rc_adapter.h"
