
/* rc_adapter.i */
%module rc_adapter  // 生成的 Java 类名


%include "swig/base.i"
%include "swig/struct_define.i"

%{
#include "rc_adapter.h"
%}

int create_engine_builder(RcimEngineBuilderParam *param, int64_t *OUTPUT);
int engine_builder_build(int64_t builderPtr, int64_t *OUTPUT);

// 包含头文件 - 必须在类型映射之后包含
%include "rc_adapter.h"
