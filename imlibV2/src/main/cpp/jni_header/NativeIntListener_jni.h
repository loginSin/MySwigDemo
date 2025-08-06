//
// Created by Qi on 2025/8/6.
//

#ifndef MYSWIGDEMO_NATIVEINTLISTENER_JNI_H
#define MYSWIGDEMO_NATIVEINTLISTENER_JNI_H

#include <jni.h>
#include "util/Context.h"
#include "jni_helper/jvm.h"

namespace rcim {
    void callNativeIntListener(const void *context, int value) {
        void *listener = getFuncFromContextByListener(context);
        jobject jListener = (jobject) listener;

        JNIEnv *env = RongCloud::jni::AttachCurrentThreadIfNeeded();

        // 获取 class
        jclass listenerClass = env->GetObjectClass(jListener);

        // 获取方法 ID
        jmethodID onCompleteMethod = env->GetMethodID(
                listenerClass, "onChanged", "(I)V");

        if (onCompleteMethod == nullptr) {
            // method not found
            env->DeleteLocalRef(listenerClass);
            return;
        }

        // 调用方法
        env->CallVoidMethod(jListener, onCompleteMethod, value);

        // 4. 清理局部引用
        env->DeleteLocalRef(listenerClass);
    }
} // namespace rcim

#endif //MYSWIGDEMO_NATIVEINTLISTENER_JNI_H
