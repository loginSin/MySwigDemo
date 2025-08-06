
/* rc_adapter.i */
%module rc_adapter  // 生成的 Java 类名

%{
#include "rc_adapter.h"
%}

// 为 std::string 添加类型映射，确保在 Java 中正确处理
%include "std_string.i"

// 为 std::map 添加类型映射，映射为 Java 的 Map
%include "std_map.i"

// 为 std::vector 添加类型映射
%include "std_vector.i"

// 为 long 类型添加正确的映射
%apply long long { long };

// 为 void* 添加类型映射，在 Java 中映射为 long
// %apply long long { void* };

// 为 Java EngineBuilderParam 对象添加类型映射
%apply jobject { void* };

// 声明 std::map<std::string, std::string> 的类型映射
%template(MapString) std::map<std::string, std::string>;

// 声明 std::vector<int> 的类型映射
%template(VectorInt) std::vector<int>;

// 为 std::vector<long> 添加正确的类型映射
%template(VectorLong) std::vector<long long>;

// 包含头文件 - 必须在类型映射之后包含
%include "rc_adapter.h"

// 使用 %native 指令生成指定的 Java 类
//%native (get_native_param_from_java) long get_native_param_from_java("io.rong.imlib.swig_helper.EngineBuilderParam");
