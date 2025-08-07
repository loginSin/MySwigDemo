//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_NATIVESTRINGCALLBACK_H
#define MYSWIGDEMO_NATIVESTRINGCALLBACK_H

// native callback ，固定返回 code 和 string
class NativeStringCallback {
public:
    NativeStringCallback() {};

    virtual ~NativeStringCallback() {}

    // 提供默认实现，而不是纯虚函数，这样SWIG可以生成完整的Java类
    virtual void onResult(int code, const char *value) = 0;
};

#endif //MYSWIGDEMO_NATIVESTRINGCALLBACK_H
