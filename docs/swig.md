# SWIG 使用指南

[swig 官网](https://www.swig.org/)

# 1. 原理分析

jni 的难点是 java 和 C/CPP 的数据类型互转（例如 Java 的 Message 和 C struct MyMessageBox 互转），方法互调（连接方法、连接监听等）。

swig 自动生成了这两步，生成代码实现了 jni 。开发者只需要专注纯 java 代码和纯 C 代码即可，无需考虑 jni 的东西

原理：swig 扫描特定的 .h 生成对应的 jni 适配层(RcClient_wrap.h/cpp)和 java 类(Java swig 包)。

java 层的类持有 C 的指针，操作 java 对象会直接操作 C 的指针（例如 MyMessageBox.java ）

# 2. 提取 c-ffi .h 文件

rust c-ffi 生成了 .h 文件，swig 需要扫描该文件。.h 文件中包含三类内容：枚举值、结构体、函数 & 函数指针

> 1. 枚举值

不能使用 swig 扫描。需要将枚举值映射成 int。

原因：swig 会为枚举值生成对应的 jni 方法，但是当枚举值没有找到时会抛异常。必须避免新错误码遇到旧 sdk 导致崩溃

```text
%apply int { MyConversationType };
```

> 2. 结构体

可以使用 swig 扫描，swig 会为结构体生成同名的 java 类。

```text
参考 c-extract-structs.py 的实现，其作用是把 c-ffi .h 的结构体摘出来
```

> 3. 函数 & 函数指针

不能使用 swig 扫描。需要手动声明并实现。函数一般都带函数指针，swig 无法直接处理，需要转成 Callback 的形式处理

```c
// swig 无法处理

typedef void (*MyConnectCb)(const void *context, enum MyEngineError code, const char *user_id);

void my_engine_connect(const struct MyEngineSync *engine,
                         const char *token,
                         int32_t timeout_sec,
                         const void *context,
                         MyConnectCb callback);
```

```c
// swig 可以处理

    class NativeConnectCallback {
    public:
        NativeConnectCallback() = default;

        virtual ~NativeConnectCallback() = default;

        virtual void onResult(int code, const char *userId) = 0;
    };

    void nativeEngineConnect(int64_t enginePtr, const char *token, int timeout,
                             NativeConnectCallback *callback);

```

# 3. 封装 c-ffi 方法

> 1. 基本数据类型

bool，int，long， char* 等 swig 默认支持

> 2. 复杂数据类型

这里指 c-ffi 的 struct ，主要是需要做 c-ffi struct 和 Java class 的映射，比如 struct MyMessageBox 和 Message.java 的映射

swig 的做法是扫描 struct ，生成同名的 Java 类，然后再和普通 Java 类映射

```text
struct MyMessageBox <==> MyMessageBox.java <==> Message.java

struct MyMessageBox < swig 自动处理 > MyMessageBox.java

MyMessageBox.java < 代码手动处理 > Message.java
```

其他的 struct 均为该逻辑

> 3. Callback

本质是函数，只不过是有限次调用的，在 C++ 层需要释放（详细见后续`内存管理`），比如连接成功后，连接 Callback 需要释放

下面是 Callback 的完整实现流程，此处以发消息接口为例

```text
// 1. 根据实际情况定义 CPP 的 Callback 类

    class NativeSendMessageCallback {
    public:
        NativeSendMessageCallback() = default;

        virtual ~NativeSendMessageCallback() = default;

        virtual void onSave(const MyMessageBox *nativeMsg) = 0;

        virtual void onResult(int code, const MyMessageBox *nativeMsg) = 0;

    };

// 2. 在 swig  .i 配置文件中启用 director 功能，swig 会生成同名的 Java 类

%feature("director") NativeSendMessageCallback;

// 3. 在 cpp 中定义该方法，并实现

// RcClient.h

    void nativeEngineSendMessage(int64_t enginePtr, MyMessageBox *msgBox, NativeSendMessageCallback *sendMsgCallback);
                             
// RcClient.cpp

    void nativeEngineSendMessageSaved(const void *context, const struct MyMessageBox *msg_box) {
        auto *callback = static_cast<NativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onSave(msg_box);
        }
    }

    void nativeEngineSendMessageAdapter(const void *context,
                                        enum MyEngineError code, const struct MyMessageBox *msg_box) {
        auto *callback = static_cast<NativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onResult(code, msg_box);
            // 重要：需要释放 Callback，避免内存泄露
            delete callback;
        }
    }

    void nativeEngineSendMessage(int64_t enginePtr, MyMessageBox *msgBox, NativeSendMessageCallback *sendMsgCallback) {
        if (enginePtr == 0) {
            if (sendMsgCallback) {
                sendMsgCallback->onResult(MyEngineError_InvalidArgumentEngineSync, msgBox);
            }
            return;
        }
        auto *engine = reinterpret_cast<MyEngineSync *>(static_cast<uintptr_t>(enginePtr));
        my_engine_send_message(engine, msgBox, nullptr, sendMsgCallback,
                                 nativeEngineSendMessageAdapter,
                                 nativeEngineSendMessageSaved);
    }
```


> 4. Listener

本质是函数，只不过是无限次调用的，流程和 Callback 一样，只是`不能释放`。比如收消息监听，永远不知道什么时候会收到消息，所以永远不能释放

下面是 Listener 的完整实现流程，此处以收消息监听为例

```text
// 1. 根据实际情况定义 CPP 的 Listener 类

    class NativeMessageReceivedListener {
    public:
        NativeMessageReceivedListener() = default;

        virtual ~NativeMessageReceivedListener() = default;

        virtual void onChanged(const struct MyMessageBox *nativeMsgBox, const struct MyReceivedInfo *nativeInfo) = 0;
    };

// 2. 在 swig  .i 配置文件中启用 director 功能，swig 会生成同名的 Java 类

%feature("director") NativeMessageReceivedListener;

// 3. 在 cpp 中定义该方法，并实现

// RcClient.h

int nativeEngineSetMessageReceivedListener(int64_t enginePtr, NativeMessageReceivedListener *listener);
                             
// RcClient.cpp

    void nativeEngineSetMessageReceivedListenerAdapter(const void *context, const struct MyMessageBox *msg_box, const struct MyReceivedInfo *info) {
        auto *listener = static_cast<NativeMessageReceivedListener *>(const_cast<void *>(context));
        if (listener) {
            listener->onChanged(msg_box, info);
        }
    }

    int nativeEngineSetMessageReceivedListener(int64_t enginePtr,
                                               NativeMessageReceivedListener *listener) {
        if (enginePtr == 0) {
            return MyEngineError_InvalidArgumentEngineSync;
        }
        auto *engine = reinterpret_cast<MyEngineSync *>(static_cast<uintptr_t>(enginePtr));
        return my_engine_set_message_received_listener(engine, listener, nativeEngineSetMessageReceivedListenerAdapter);
    }
```

通过上面的例子，可以得到如下结论
1. Callback 和 Listener 都是函数，只是调用次数不同。Callback 有限次调用，用完需要删除。Listener 无限次调用，不能删除
2. swig 生成了 jni 的代码，在 RcClient.cpp 内部使用 c-ffi 结构体，避免了数据的转化
3. RcClient.cpp 里面没有任何 jni 的内容，直接使用即可，非常方便

# 4. swig 配置说明

见 `RcClient.i` 的描述

# 5. 内存管理

Java 的对象由 JVM 管理，不需要我们关心释放的问题

但是 Swig 生成的 C++ 对象需要手动释放，避免内存泄漏。

```text
SWIG 生成的 Java 类中有 finalize() 和 delete() 方法，常见于用 SWIG 包装的 C/C++ 原生资源管理。

这两个方法的作用 
    finalize() 是 Java 的垃圾回收机制在对象被回收时调用的回调方法。SWIG 生成的 finalize() 通常会调用 delete() 来释放底层 C++ 对象的内存，防止内存泄漏。
    delete() 是显式释放底层 C++ 对象资源的方法。调用后，该 Java 对象对应的 C++ 对象会被立即销毁。

调用建议
    推荐手动调用 delete() 释放资源
        尽量避免依赖 finalize()。Java 的垃圾回收器调用 finalize() 的时机不可控，且 finalize() 会拖延资源释放。
        手动调用 delete() 可以及时释放 C++ 对象，避免 native 内存泄漏。
    调用 delete() 后不要再使用该对象
        delete() 释放后，Java 对象虽然存在，但其底层指针已无效，再访问可能导致异常或崩溃。

如果不能手动管理，依赖 finalize() 作为兜底
    但这通常只适合测试或简单程序，生产环境推荐显式管理。

避免重复调用 delete()
    SWIG 生成的类通常有 swigCMemOwn 字段标记是否拥有 C++ 对象所有权，delete() 会检查该标志避免重复释放。

总结
推荐：程序中用完 native 对象后，尽快调用 delete() 释放。

finalize() 仅作为最后的安全网，不能依赖它保证及时释放。

delete() 方法名称太过普通，所以生成了 swigDelete() 方法
```

## 5.1. 创建 model 类

在 java 中所有出现 `new My` 的地方应该都需要进行手动释放，释放调用 `swigDelete()` 方法

最简单的规则是：java 遇到 `new My` 就需要考虑调用 `swigDelete()`

1. new MyXXX 是一个普通的 model 类，需要在用完该类的时候进行释放

```java
public void init() {
    // 出现 new MyEngineBuilderParam
    MyEngineBuilderParam param = new MyEngineBuilderParam();
    param.setApp_key(appKey);

    // 出现 new MySDKVersion
    MySDKVersion sdkVer1 = new MySDKVersion();
    sdkVer1.setName("imlib");

    // 出现 new MySDKVersion
    MySDKVersion sdkVer2 = new MySDKVersion();
    sdkVer2.setName("imkit");
    
    int code = rc_adapter.nativeCreateEngineBuilder(param, builderPtrArr);

    // 上述 new My 使用完成之后，手动调用 swigDelete() 进行删除
    sdkVer1.swigDelete();
    sdkVer2.swigDelete();
    param.swigDelete();
}

```

## 5.2. 创建 callback 类
new NativeXXXCallback 是一个原生的 callback 类，需要把所有权从 Java 转移至 C++ 层，等 callback 使用完成之后，在 C++ 层 delete 该 callback

> 为什么需要把所有权转给 C++ 层？

默认流程：

以 NativeSendMessageCallback.java 为例

Java 层 new NativeSendMessageCallback()，同时会在 C++ 层 new NativeSendMessageCallback()

在消息尚未发送成功时，Java NativeSendMessageCallback.java 对象可能会被调用 finalize() ，同步释放 C++ 的 NativeSendMessageCallback 指针。导致 C++ 使用被释放的 NativeSendMessageCallback 指针，从而发生野指针崩溃

> 如何把所有权从 Java 转移到 C++ 层？

调用 swigReleaseOwnership()

其本质是 C++ 调用 env->NewGlobalRef 变成全局引用，避免提前释放


`C++ 层使用完 callback 后需要 delete`

```java
        // java 层调用逻辑
        // 1. 创建原生监听
        NativeSendMessageCallback nativeSendMessageCallback = new NativeSendMessageCallback() {
            @Override
            public void onSave(MyMessageBox nativeMsg) {
                if (callback != null) {
                    Message msg = Transformer.messageFromNative(nativeMsg);
                    callback.onAttached(msg);
                }
            }

            @Override
            public void onResult(int code, MyMessageBox nativeMsg) {
                if (callback == null) {
                    return;
                }
                Message msg = Transformer.messageFromNative(nativeMsg);
                if (code == 0) {
                    callback.onSuccess(msg);
                }else {
                    callback.onError(msg, IRongCoreEnum.CoreErrorCode.valueOf(code));
                }
            }
        };
        // 2. 把所有权从 java 转移到 C++ 层，避免提前释放：等效于 C++ 调用 env->NewGlobalRef 变成全局引用，等 Callback 使用完成后，需要在 C++ 手动 delete
        nativeSendMessageCallback.swigReleaseOwnership();
        // 3. 把原生 Callback 设置给 jni
        RcClient.nativeEngineSendMessage(this.enginePtr.get(), inputMsgBox , nativeSendMessageCallback);
}
```

```cpp
    // cpp 层实现逻辑
    void nativeEngineSendMessageSaved(const void *context, const struct MyMessageBox *msg_box) {
        auto *callback = static_cast<NativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onSave(msg_box);
        }
    }

    void nativeEngineSendMessageAdapter(const void *context,
                                        enum MyEngineError code,
                                        const struct MyMessageBox *msg_box) {
        auto *callback = static_cast<NativeSendMessageCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onResult(code, msg_box);
            // Java 层调用 swigReleaseOwnership 方法后。C++ Callback 使用完需要删除 Callback ，避免内存泄露
            delete callback;
        }
    }

    void nativeEngineSendMessage(int64_t enginePtr, MyMessageBox *msgBox,
                                 NativeSendMessageCallback *sendMsgCallback) {
        if (enginePtr == 0) {
            if (sendMsgCallback) {
                sendMsgCallback->onResult(MyEngineError_InvalidArgumentEngineSync, msgBox);
            }
            return;
        }
        auto *engine = reinterpret_cast<MyEngineSync *>(static_cast<uintptr_t>(enginePtr));
        my_engine_send_message(engine, msgBox, nullptr, sendMsgCallback,
                                 nativeEngineSendMessageAdapter,
                                 nativeEngineSendMessageSaved);
    }
```

## 5.3. 创建 listener 类

流程和 Callback 一样，只是不需要在 C++ 释放

```java
        // java 层调用逻辑
        // 1. 创建原生监听
        NativeConnectionStatusListener nativeConnectListener = new NativeConnectionStatusListener() {
            @Override
            public void onChanged(int value) {
                // 4. 原生监听方法触发后调用 Java 监听方法
                for (IRongCoreListener.ConnectionStatusListener listener : ListenerHolder.connectionStatusListenerList) {
                    IRongCoreListener.ConnectionStatusListener.ConnectionStatus status = ListenerHolder.connectionStatusFromNative(value);
                    if (listener != null) {
                        listener.onChanged(status);
                    }
                }
            }
        };
        // 2. 把原生监听所有权转移至 C++ 层，避免提前释放：C++ 调用 env->NewGlobalRef 变成全局引用
        nativeConnectListener.swigReleaseOwnership();
        // 3. 把原生监听设置给 jni
        RcClient.nativeEngineSetConnectionStatusListener(enginePtr, nativeConnectListener);
```

```cpp
    // cpp 层实现逻辑
    void nativeEngineSetConnectionStatusListenerAdapter(const void *context,
                                                        enum MyConnectionStatus status) {
        auto *listener = static_cast<NativeConnectionStatusListener *>(const_cast<void *>(context));
        if (listener) {
            // listener 不能被删除
            listener->onChanged(status);
        }
    }

    void nativeEngineSetConnectionStatusListener(int64_t enginePtr,
                                                 NativeConnectionStatusListener *listener) {
        if (enginePtr == 0) {
            return;
        }
        auto *engine = reinterpret_cast<MyEngineSync *>(static_cast<uintptr_t>(enginePtr));
        my_engine_set_connection_status_listener(engine, listener,
                                                   nativeEngineSetConnectionStatusListenerAdapter);
    }
```