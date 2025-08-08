//
// Created by Qi on 2025/8/7.
//

#ifndef MYSWIGDEMO_RCIMNATIVESENDMESSAGECALLBACK_H
#define MYSWIGDEMO_RCIMNATIVESENDMESSAGECALLBACK_H

#include "../libs/includes/rcim_client.h"

class RcimNativeSendMessageCallback {
public:
    RcimNativeSendMessageCallback() {};

    virtual ~RcimNativeSendMessageCallback() {}

    virtual void onSave(const RcimMessageBox *msg) = 0;

    virtual void onResult(RcimNativeSendMessageCallback *deleteThis, int code, const RcimMessageBox *msg) = 0;
};

#endif //MYSWIGDEMO_RCIMNATIVESENDMESSAGECALLBACK_H
