//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_RCIMNATIVESTRINGCALLBACK_H
#define MYSWIGDEMO_RCIMNATIVESTRINGCALLBACK_H

// native callback ，固定返回 code 和 string
class RcimNativeStringCallback {
public:
    RcimNativeStringCallback() {};

    virtual ~RcimNativeStringCallback() {}

    virtual void onResult(RcimNativeStringCallback *deleteThis, int code, const char *value) = 0;
};

#endif //MYSWIGDEMO_RCIMNATIVESTRINGCALLBACK_H
