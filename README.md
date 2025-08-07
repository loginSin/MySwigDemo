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