//
// Created by Qi on 2025/8/6.
//

#ifndef MYSWIGDEMO_NATIVESTRINGCALLBACK_JNI_H
#define MYSWIGDEMO_NATIVESTRINGCALLBACK_JNI_H

#include <jni.h>
#include "util/Context.h"
#include "jni_helper/jvm.h"

namespace rcim {
    void callNativeStringCallback(const void *context, enum RcimEngineError code, const char *str) {
        void *callback = getFuncFromContextByCallback(context);
        jobject jCallback = (jobject) callback;

        JNIEnv *env = RongCloud::jni::AttachCurrentThreadIfNeeded();

        // 获取 class
        jclass callbackClass = env->GetObjectClass(jCallback);

        // 获取方法 ID
        jmethodID onCompleteMethod = env->GetMethodID(
                callbackClass, "onComplete", "(ILjava/lang/String;)V");

        if (onCompleteMethod == nullptr) {
            // method not found
            env->DeleteLocalRef(callbackClass);
            return;
        }

        // 将 std::string 转为 jstring
        jstring jValue = env->NewStringUTF(str);

        // 调用方法
        env->CallVoidMethod(jCallback, onCompleteMethod, code, jValue);

        // 清理
        env->DeleteLocalRef(jValue);
    }
} // namespace rcim
#endif //MYSWIGDEMO_NATIVESTRINGCALLBACK_JNI_H
