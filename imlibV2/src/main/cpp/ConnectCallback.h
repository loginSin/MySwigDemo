//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_CONNECTCALLBACK_H
#define MYSWIGDEMO_CONNECTCALLBACK_H

// 为SWIG提供完整的类定义，这样会生成ConnectCallback.java
class ConnectCallback {
public:
    ConnectCallback() {};

    virtual ~ConnectCallback() {}

    // 提供默认实现，而不是纯虚函数，这样SWIG可以生成完整的Java类
    virtual void onConnect(int code, const char *user_id) = 0;
};

#endif //MYSWIGDEMO_CONNECTCALLBACK_H
