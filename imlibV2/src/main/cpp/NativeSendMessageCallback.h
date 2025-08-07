//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_NATIVESENDMESSAGECALLBACK_H
#define MYSWIGDEMO_NATIVESENDMESSAGECALLBACK_H

#include "libs/includes/rcim_client.h"

class NativeSendMessageCallback {
public:
    NativeSendMessageCallback() {};

    virtual ~NativeSendMessageCallback() {}

    virtual void onSave(const RcimMessageBox *msg) = 0;

    virtual void onResult(int code, const RcimMessageBox *msg) = 0;
};

#endif //MYSWIGDEMO_NATIVESENDMESSAGECALLBACK_H
