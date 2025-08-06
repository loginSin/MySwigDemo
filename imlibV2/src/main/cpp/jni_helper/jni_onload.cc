
#include <jni.h>
#include "jvm.h"

#undef JNIEXPORT
#define JNIEXPORT __attribute__((visibility("default")))
static JavaVM *g_jvm = nullptr;

namespace RongCloud {
namespace jni {

extern "C" jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
  g_jvm = jvm;
  jint ret = InitGlobalJniVariables(jvm);
  if (ret < 0)
    return -1;
  return ret;
}

extern "C" void JNIEXPORT JNICALL JNI_OnUnLoad(JavaVM* jvm, void* reserved) {}

}  // namespace jni
}  // namespace RongCloud
