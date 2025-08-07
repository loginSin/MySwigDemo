//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_NATIVEINTLISTENER_H
#define MYSWIGDEMO_NATIVEINTLISTENER_H


class NativeIntListener {
public:
    NativeIntListener() {};

    virtual ~NativeIntListener() {}

    virtual void onChanged(int value) = 0;
};


#endif //MYSWIGDEMO_NATIVEINTLISTENER_H
