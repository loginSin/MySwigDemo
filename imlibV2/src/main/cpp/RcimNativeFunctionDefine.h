#ifndef MYSWIGDEMO_RCIMNATIVEFUNCTIONDEFINE_H
#define MYSWIGDEMO_RCIMNATIVEFUNCTIONDEFINE_H

#include "libs/includes/rcim_client.h"

namespace rcim {

/// ---------------------------------------------Callback--------------------------------------------------
    /**
    * native string callback ，固定返回 code 和 string
    */
    class RcimNativeStringCallback {
    public:
        RcimNativeStringCallback() = default;

        virtual ~RcimNativeStringCallback() = default;

        // callback 使用完成了，把当前 callback 指针传给 Java 层，让 Java 释放
        virtual void
        onResult(RcimNativeStringCallback *deleteThis, int code, const char *value) = 0;
    };

    /**
    * native 发消息 callback
    */
    class RcimNativeSendMessageCallback {
    public:
        RcimNativeSendMessageCallback() = default;

        virtual ~RcimNativeSendMessageCallback() = default;

        virtual void onSave(const RcimMessageBox *nativeMsg) = 0;

        // callback 使用完成了，把当前 callback 指针传给 Java 层，让 Java 释放
        virtual void onResult(RcimNativeSendMessageCallback *deleteThis, int code,
                              const RcimMessageBox *nativeMsg) = 0;

    };

/// ---------------------------------------------Listener--------------------------------------------------
    /**
    * native int listener，固定给 Java 层返回数字，可以用来做一般的状态监听，如连接状态监听
    *
    * Listener 需要避免被释放，就必须要求被 Java 类持有，具体参考连接状态监听
    */
    class RcimNativeIntListener {
    public:
        RcimNativeIntListener() = default;

        virtual ~RcimNativeIntListener() = default;

        virtual void onChanged(int value) = 0;
    };

    class RcimNativeMessageReceivedListener {
    public:
        RcimNativeMessageReceivedListener() = default;

        virtual ~RcimNativeMessageReceivedListener() = default;

        virtual void onChanged(const struct RcimMessageBox *nativeMsgBox, const struct RcimReceivedInfo *nativeInfo) = 0;
    };

} // namespace rcim

#endif //MYSWIGDEMO_RCIMNATIVEFUNCTIONDEFINE_H
