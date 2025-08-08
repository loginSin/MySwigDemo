
/* rc_adapter.i */
%module(directors="1") rc_adapter  // 生成的 Java 类名


%include "swig/base.i"
%include "swig/struct_define.i"

%{
#include "rc_adapter.h"
%}


int create_engine_builder(RcimEngineBuilderParam *param, int64_t *OUTPUT);
int engine_builder_build(int64_t builderPtr, int64_t *OUTPUT);

// 启用 director 功能，允许 Java 继承 C++ 类
%feature("director") RcimNativeStringCallback;
%include "native_define/RcimNativeStringCallback.h"

%feature("director") RcimNativeSendMessageCallback;
%include "native_define/RcimNativeSendMessageCallback.h"

%feature("director") RcimNativeIntListener;
%include "native_define/RcimNativeIntListener.h"

// 包含头文件 - 必须在类型映射之后包含
%include "rc_adapter.h"
