//
// Created by Qi on 2025/8/6.
//

#ifndef MYSWIGDEMO_RC_ADAPTER_H
#define MYSWIGDEMO_RC_ADAPTER_H

#include <string>

void *create_engine_builder(
        std::string appKey,
        std::string deviceId,
        std::string packageName,
        std::string imlibVersion,
        std::string deviceModel,
        std::string deviceManufacturer,
        std::string osVersion,
        std::string appVersion
);


#endif //MYSWIGDEMO_RC_ADAPTER_H
