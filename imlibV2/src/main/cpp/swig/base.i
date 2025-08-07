%include "stdint.i"
%include cpointer.i
%include typemaps.i

///// ----------------支持 int64_t - 开始 ----------------------------------
//// 将 int64_t *ptrArr 映射为 Java 的 long[] 数组
%apply long[] { int64_t *ptrArr };

// 或者使用更明确的类型映射
%typemap(in) int64_t *ptrArr {
        $1 = JCALL2(GetLongArrayElements, jenv, $input, 0);
}

%typemap(argout) int64_t *ptrArr {
        JCALL3(ReleaseLongArrayElements, jenv, $input, $1, 0);
}

%typemap(jni) int64_t *ptrArr "jlongArray"
%typemap(jtype) int64_t *ptrArr "long[]"
%typemap(jstype) int64_t *ptrArr "long[]"
%typemap(javain) int64_t *ptrArr "$javainput"
//// 将 int64_t *ptrArr 映射为 Java 的 long[] 数组
///// ----------------支持 int64_t - 结束 ----------------------------------


%apply jobject { void* };
