
/* RcClient.i */
%module(directors="1") RcClient  // 生成的 Java 类名


%include "swig/base.i"
%include "swig/struct_define.i"

%{
#include "RcClient.h"
#include <vector>
#include <string>
%}

%include <std_string.i>
%include <std_vector.i>

// 显式告诉 SWIG 你需要支持 std::vector<std::string>
namespace std {
        %template(RcimStringVector) vector<string>;
}

namespace rcim {
int createEngineBuilder(RcimEngineBuilderParam *param, int64_t *OUTPUT);
int engineBuilderBuild(int64_t builderPtr, int64_t *OUTPUT);
}

// 启用 director 功能，允许 Java 继承 C++ 类
%feature("director") RcimNativeStringCallback;
%feature("director") RcimNativeSendMessageCallback;

%feature("director") RcimNativeIntListener;
%feature("director") RcimNativeMessageReceivedListener;


%include "RcimNativeFunctionDefine.h"

// 包含头文件 - 必须在类型映射之后包含
%include "RcClient.h"
