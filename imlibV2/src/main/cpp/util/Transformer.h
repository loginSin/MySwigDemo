#ifndef MYSWIGDEMO_TRANSFORMER_H
#define MYSWIGDEMO_TRANSFORMER_H

#include <string>
#include <map>
#include <jni.h>

namespace rcim {
    std::string getStringFromJObject(JNIEnv *env, jobject jObj, std::string key);
    std::map<std::string, std::string> getMapStringFromJObject(JNIEnv *env, jobject jObj, std::string key);
} // namespace rcim


#endif //MYSWIGDEMO_TRANSFORMER_H
