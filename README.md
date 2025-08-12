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
└── rcim_client.h

```

# 2. 通过 swig 生成 jni 辅助代码

```shell
bash swig.sh
```

# 3. 正常运行 app

# 4. 内存释放

## 4.1. Java 层
在 java 中所有出现 `new Rcim` 的地方应该都需要进行手动释放，释放调用 `swigDelete()` 方法

最简单的规则是：java 遇到 `new Rcim` 就需要考虑调用 `swigDelete()`

1. new RcimXXX 是一个普通的 model 类，需要在用完该类的时候进行释放

```java
public void init() {
    // 出现 new RcimEngineBuilderParam
    RcimEngineBuilderParam param = new RcimEngineBuilderParam();
    param.setApp_key(appKey);

    // 出现 new RcimSDKVersion
    RcimSDKVersion sdkVer1 = new RcimSDKVersion();
    sdkVer1.setName("imlib");

    // 出现 new RcimSDKVersion
    RcimSDKVersion sdkVer2 = new RcimSDKVersion();
    sdkVer2.setName("imkit");
    
    int code = rc_adapter.createEngineBuilder(param, builderPtrArr);

    // 上述 new Rcim 使用完成之后，手动调用 swigDelete() 进行删除
    sdkVer1.swigDelete();
    sdkVer2.swigDelete();
    param.swigDelete();
}

```
2. new RcimXXXCallback 是一个 callback 类，需要在 Callback 用完的时候进行释放

Callback 只能有限次数的调用，用完之后就得销毁，比如连接 callback ，发送消息 callback

```java
    public void connect(String token, int timeout, IData1Callback<String> callback) {
    // 1. 创建 NativeCallback
    RcimNativeStringCallback nativeCallback = new RcimNativeStringCallback() {
        @Override
        public void onResult(RcimNativeStringCallback deleteThis, int code, String value) {
            if (callback == null) {
                return;
            }
            if (EngineError.Success.getCode() == code) {
                callback.onSuccess(value);
            } else {
                callback.onError(EngineError.codeOf(code));
            }
            // 4. NativeCallback 使用完成之后，从 Java 层移除并且释放该 NativeCallback
            CallbackHolder.removeCallback(deleteThis.getCPtr());
            deleteThis.swigDelete();
        }
    };

    // 2. Java 层保存 NativeCallback
    CallbackHolder.saveCallback(nativeCallback.getCPtr(), nativeCallback);
    // 3. 将 NativeCallback 传给 jni
    RcClient.engineConnect(this.enginePtr.get(), token, timeout, nativeCallback);
}
```

> 为什么不能在 C++ 层通过 jobject pObj = env->NewGlobalRef(j_callback); 来持久存储？

见 CallbackHolder.java 的注释

3. new RcimXXXListener 是一个 listener 类，不能释放

Listener 是监听，需要触发无数次，不能释放。所以需要被 Java 类持有，避免被释放，参考 conStatusListenerRef 的使用