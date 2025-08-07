%include "stdint.i"
%include cpointer.i
%include typemaps.i

// 将 long *ptrArr 映射为 Java 的 long[] 数组
%apply long[] { long *ptrArr };

// 或者使用更明确的类型映射
%typemap(in) long *ptrArr {
        $1 = JCALL2(GetLongArrayElements, jenv, $input, 0);
}

%typemap(argout) long *ptrArr {
        JCALL3(ReleaseLongArrayElements, jenv, $input, $1, 0);
}

%typemap(jni) long *ptrArr "jlongArray"
%typemap(jtype) long *ptrArr "long[]"
%typemap(jstype) long *ptrArr "long[]"
%typemap(javain) long *ptrArr "$javainput"

// 为 Java EngineBuilderParam 对象添加类型映射
%apply jobject { void* };

//
//// 为 std::string 添加类型映射，确保在 Java 中正确处理
//%include "std_string.i"
//
//// 为 std::map 添加类型映射，映射为 Java 的 Map
//%include "std_map.i"
//
//// 为 std::vector 添加类型映射
//%include "std_vector.i"
//
//// 声明 std::map<std::string, std::string> 的类型映射
//%template(MapString) std::map<std::string, std::string>;
//
//// 声明 std::vector<int> 的类型映射
//%template(VectorInt) std::vector<int>;
//
//// 为 std::vector<long> 添加正确的类型映射
//%template(VectorLong) std::vector<long long>;