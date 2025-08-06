
/* native_engine.i */
%module native_engine2  // 生成的 Java 类名

%{
/* 包含原始 C 头文件（如有） */
#include "../libs/includes/rcim_client.h"
%}

%include "stdint.i"
%include "cpointer.i"

// 为 int64_t 添加正确的类型映射
%apply long long { int64_t };

/* 声明要包装的函数 */
%include "../libs/includes/rcim_client.h"
