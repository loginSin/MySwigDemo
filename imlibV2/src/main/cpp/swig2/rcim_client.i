
/* native_engine.i */
%module native_engine2  // 生成的 Java 类名

%{
/* 包含原始 C 头文件（如有） */
#include "../libs/includes/rcim_client.h"
%}

%include "stdint.i"
%include "cpointer.i"

// 为输出参数定义正确的 typemap
//%typemap(in) struct RcimEngineBuilder **out_builder (struct RcimEngineBuilder *temp) {
//    temp = NULL;
//    $1 = &temp;
//}
//
//%typemap(argout) struct RcimEngineBuilder **out_builder {
//    if (*$1) {
//        $result = SWIG_NewPointerObj(*$1, RcimEngineBuilder, 0);
//    } else {
//        $result = SWIG_Py_Void();
//    }
//}
//
//%typemap(jni) struct RcimEngineBuilder **out_builder "jobject"
//%typemap(jtype) struct RcimEngineBuilder **out_builder "RcimEngineBuilder"
//%typemap(jstype) struct RcimEngineBuilder **out_builder "RcimEngineBuilder"
//%typemap(javaout) struct RcimEngineBuilder **out_builder {
//    return new RcimEngineBuilder($jnicall, true);
//}
//
//// 为其他输出参数也添加类似的 typemap
//%typemap(in) struct RcimEngineSync **out_engine (struct RcimEngineSync *temp) {
//    temp = NULL;
//    $1 = &temp;
//}
//
//%typemap(argout) struct RcimEngineSync **out_engine {
//    if (*$1) {
//        $result = SWIG_NewPointerObj(*$1, SWIGTYPE_p_RcimEngineSync, 0);
//    } else {
//        $result = SWIG_Py_Void();
//    }
//}
//
//%typemap(jni) struct RcimEngineSync **out_engine "jobject"
//%typemap(jtype) struct RcimEngineSync **out_engine "RcimEngineSync"
//%typemap(jstype) struct RcimEngineSync **out_engine "RcimEngineSync"
//%typemap(javaout) struct RcimEngineSync **out_engine {
//    return new RcimEngineSync($jnicall, true);
//}
//
//// 为 char** 输出参数添加 typemap
//%typemap(in) char **out_ver (char *temp) {
//    temp = NULL;
//    $1 = &temp;
//}
//
//%typemap(argout) char **out_ver {
//    if (*$1) {
//        $result = SWIG_FromCharPtr(*$1);
//    } else {
//        $result = SWIG_Py_Void();
//    }
//}
//
//%typemap(jni) char **out_ver "jstring"
//%typemap(jtype) char **out_ver "String"
//%typemap(jstype) char **out_ver "String"
//%typemap(javaout) char **out_ver {
//    return $jnicall;
//}
//
//// 为 char** out_user_id 输出参数添加 typemap
//%typemap(in) char **out_user_id (char *temp) {
//    temp = NULL;
//    $1 = &temp;
//}
//
//%typemap(argout) char **out_user_id {
//    if (*$1) {
//        $result = SWIG_FromCharPtr(*$1);
//    } else {
//        $result = SWIG_Py_Void();
//    }
//}
//
//%typemap(jni) char **out_user_id "jstring"
//%typemap(jtype) char **out_user_id "String"
//%typemap(jstype) char **out_user_id "String"
//%typemap(javaout) char **out_user_id {
//    return $jnicall;
//}
//
//// 为 int64_t* out_delta_time 输出参数添加 typemap
//%typemap(in) int64_t *out_delta_time (int64_t temp) {
//    temp = 0;
//    $1 = &temp;
//}
//
//%typemap(argout) int64_t *out_delta_time {
//    $result = SWIG_From_long_long(*$1);
//}
//
//%typemap(jni) int64_t *out_delta_time "jlong"
//%typemap(jtype) int64_t *out_delta_time "long"
//%typemap(jstype) int64_t *out_delta_time "long"
//%typemap(javaout) int64_t *out_delta_time {
//    return $jnicall;
//}
//
//// 为 enum RcimConnectionStatus *out_status 输出参数添加 typemap
//%typemap(in) enum RcimConnectionStatus *out_status (enum RcimConnectionStatus temp) {
//    temp = (enum RcimConnectionStatus)0;
//    $1 = &temp;
//}
//
//%typemap(argout) enum RcimConnectionStatus *out_status {
//    $result = SWIG_From_int(*$1);
//}
//
//%typemap(jni) enum RcimConnectionStatus *out_status "jint"
//%typemap(jtype) enum RcimConnectionStatus *out_status "RcimConnectionStatus"
//%typemap(jstype) enum RcimConnectionStatus *out_status "RcimConnectionStatus"
//%typemap(javaout) enum RcimConnectionStatus *out_status {
//    return RcimConnectionStatus.swigToEnum($jnicall);
//}

/* 声明要包装的函数 */
%include "../libs/includes/rcim_client.h"
