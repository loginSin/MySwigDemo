//
// Created by Qi on 2025/8/6.
//

#include "Transformer.h"

namespace rcim {
    std::string getStringFromJObject(JNIEnv *env, jobject jObj, std::string key) {
        if (env == nullptr || jObj == nullptr || key.empty()) {
            return "";
        }
        // 获取 Java 类的字段 ID
        jclass jClazz = env->GetObjectClass(jObj);
        if (!jClazz) {
            return "";
        }
        jfieldID valueField = env->GetFieldID(jClazz, key.c_str(), "Ljava/lang/String;");
        jstring value = (jstring) env->GetObjectField(jObj, valueField);
        const char *valueStr = value ? env->GetStringUTFChars(value, 0) : "";
        std::string result(valueStr);
        if (value) {
            env->ReleaseStringUTFChars(value, valueStr);
        }
        env->DeleteLocalRef(jClazz);
        return result;
    }

    std::map<std::string, std::string> getMapStringFromJObject(JNIEnv *env, jobject jObj, std::string key) {
        std::map<std::string, std::string> emptyMap;
        if (env == nullptr || jObj == nullptr || key.empty()) {
            return emptyMap;
        }
        // 获取 Java 类的字段 ID
        jclass jClazz = env->GetObjectClass(jObj);
        if (!jClazz) {
            return emptyMap;
        }
        // io.rong.imlib.swig.MapString.java
        jfieldID sdkVersionsField = env->GetFieldID(jClazz, key.c_str(), "Lio/rong/imlib/swig/MapString;");


        jobject mapStringObj = env->GetObjectField(jObj, sdkVersionsField);
        jclass cls = env->GetObjectClass(mapStringObj);
        jfieldID fid = env->GetFieldID(cls, "swigCPtr", "J"); // J 是 long 的签名
        jlong ptr = env->GetLongField(mapStringObj, fid);

        std::map< std::string,std::string > *arg1 = 0 ;
        std::map< std::string,std::string > *result = 0 ;
        arg1 = *(std::map< std::string,std::string > **)&ptr;
        if (!arg1) {
            return emptyMap;
        }
        result = (std::map< std::string,std::string > *)new std::map< std::string,std::string >((std::map< std::string,std::string > const &)*arg1);

        env->DeleteLocalRef(jClazz);
        return *result;
    }
} // namespace rcim