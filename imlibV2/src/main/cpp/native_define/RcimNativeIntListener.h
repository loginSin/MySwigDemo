//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_RCIMNATIVEINTLISTENER_H
#define MYSWIGDEMO_RCIMNATIVEINTLISTENER_H


class RcimNativeIntListener {
public:
    RcimNativeIntListener() {};

    virtual ~RcimNativeIntListener() {}

    virtual void onChanged(int value) = 0;
};


#endif //MYSWIGDEMO_RCIMNATIVEINTLISTENER_H
