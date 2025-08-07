
/* rc_adapter.i */
%module rc_adapter  // 生成的 Java 类名


%include "swig/base.i"
%include "swig/struct_define.i"

%{
#include "rc_adapter.h"
%}

int create_engine_builder(RcimEngineBuilderParam *param, long long *OUTPUT);
int engine_builder_build(long long builderPtr, long long *OUTPUT);

// 包含头文件 - 必须在类型映射之后包含
%include "rc_adapter.h"
