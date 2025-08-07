//
// Created by Qi on 2025/8/6.
//

#ifndef MYSWIGDEMO_RC_ADAPTER_H
#define MYSWIGDEMO_RC_ADAPTER_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include "libs/includes/rcim_client.h"

int create_engine_builder(void *javaParam, std::vector<long long> &outBuilderPtrArray);

int engine_builder_set_store_path(long long builderPtr, std::string storePath);

int engine_builder_build(long long builderPtr, std::vector<long long> &outEngineArray);

void engine_connect(long long enginePtr, std::string token, int timeout, void *callback);

void engine_set_connection_status_listener(long long enginePtr, void *listener);

void engine_send_message(long long enginePtr, RcimMessageBox *msgBox, void *sendMsgCallback);

#endif //MYSWIGDEMO_RC_ADAPTER_H
