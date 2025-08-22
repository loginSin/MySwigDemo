# 1. 把编译好的 Android 库放到指定路径

```shell
# 查看 android 库路径
tree imlibV2/src/main/cpp/libs/
```

```text

imlibV2/src/main/cpp/libs/
├── arm64-v8a
│     ├── libcrypto.a
│     └── librust_universal_imsdk.a
└── includes
└── my_client.h

```

# 2. 封装 c-ffi

**重要**： [swig 使用指南](docs/swig.md)

需要`自下而上`处理。

1. 根据 c-ffi .h 文件，配置 swig .i 文件
2. 根据 c-ffi .h 文件，封装对应的函数
3. 使用 swig 生成代码
4. Java 层调用生成的代码

## 2.1. 根据 c-ffi .h 文件，配置 swig .i 文件

> 枚举值

在 `struct_define.i` 将枚举值映射成 int

```text
%apply int { MyConversationType };
```

> 结构体

将需要转化的结构体配置在 `c-extract-structs.py` 中，脚本会自动生成对应的 c-ffi 结构体定义。


## 2.2. 根据 c-ffi .h 文件，封装对应的函数

```c
// c-ffi 方法定义

typedef void (*MyConnectCb)(const void *context, enum MyEngineError code, const char *user_id);

void my_engine_connect(const struct MyEngineSync *engine,
                         const char *token,
                         int32_t timeout_sec,
                         const void *context,
                         MyConnectCb callback);
```

```c
// cpp 封装

    // 1. 把 c-ffi context 和函数指针封装成 cpp 类，并配置到 struct_define.i
    // context 是 cpp 类本身
    class NativeConnectCallback {
    public:
        NativeConnectCallback() = default;

        virtual ~NativeConnectCallback() = default;

        virtual void onResult(int code, const char *userId) = 0;
    };

    // 2. 将 c-ffi 方法重命名，my 改为 native，并改为驼峰命名
    void nativeEngineConnect(int64_t enginePtr, const char *token, int timeout,
                             NativeConnectCallback *callback);

    // 3. 实现 native 方法
    void nativeEngineConnectAdapter(const void *context, enum MyEngineError code, const char *user_id) {
        auto *callback = static_cast<NativeConnectCallback *>(const_cast<void *>(context));
        if (callback) {
            callback->onResult(code, user_id);
            delete callback;
        }
    }

    void nativeEngineConnect(int64_t enginePtr, const char *token, int timeout, NativeConnectCallback *callback) {
        if (enginePtr == 0) {
            if (callback) {
                callback->onResult(MyEngineError_InvalidArgumentEngineSync, nullptr);
            }
            return;
        }
        auto *engine = reinterpret_cast<MyEngineSync *>(static_cast<uintptr_t>(enginePtr));
        my_engine_connect(engine, token, timeout, callback, nativeEngineConnectAdapter);
    }
```


## 2.3. swig 生成 jni 辅助代码

```shell
bash swig.sh
```

## 2.4. Java 调用生成的代码

```java
        // 1. 创建 Callback
        NativeConnectCallback nativeConnectCallback = new NativeConnectCallback() {
            @Override
            public void onResult(int errorCode, String userId) {
                if (connectCallback == null) {
                    return;
                }
                FwLog.write(FwLog.I, FwLog.IM, A_CONNECT_R, "code|user_id", 0, userId);
                if (errorCode == 0) {
                    connectCallback.onSuccess(userId);
                } else {
                    connectCallback.onError(IRongCoreEnum.ConnectionErrorCode.valueOf(errorCode));
                }


                // todo 待处理 onDatabaseOpened
                // FwLog.write(FwLog.I, FwLog.IM, "A-connect-S", "code", code.getValue());
            }
        };
        // 2. 把所有权从 java 转移到 C++ 层，避免提前释放：等效于 C++ 调用 env->NewGlobalRef 变成全局引用，等 Callback 使用完成后，需要在 C++ 手动 delete
        nativeConnectCallback.swigReleaseOwnership();
        // 3. 把原生 Callback 设置给 jni
        RcClient.nativeEngineConnect(impl.enginePtr.get(), option.getToken(), option.getTimeLimit(), nativeConnectCallback);
```

# 3. 正常运行 app
