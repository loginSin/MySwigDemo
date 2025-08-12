package io.rong.imlib.internal.utils;

import androidx.annotation.Nullable;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * <h2> Callback 谁来持有？Java？C++ ？</h2>
 * <h3>Java C++ 都不持有 callback  会报这个崩溃</h3>
 * <pre>
 * 2025-08-11 17:31:39.569 17426-18554 QXB-Java                io.rong.rust.myswigdemo              I  RcimNativeStringCallback delete: swigCMemOwn=true swigCPtr=-5476376626353525232 <p>
 * 2025-08-11 17:31:39.570 17426-18554 QXB-Java                io.rong.rust.myswigdemo              I  RcimNativeStringCallback delete: swigCMemOwn=false swigCPtr=-5476376626353525232 <p>
 * 2025-08-11 17:31:39.685 17426-18638 QXB-CPP                 io.rong.rust.myswigdemo              I  engineConnectAdapter callback=0xb400007931f22e10， user_id=0xb400007911d855f0， user_id_value=xYnNRqU94 <p>
 * 2025-08-11 17:31:43.611 17426-18638 libc                    io.rong.rust.myswigdemo              A  Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x3fd6 in tid 18638 (rust.myswigdemo), pid 17426 (rust.myswigdemo) <p>
 * <p>
 * 第一行 log 意思是 Java 层的 RcimNativeStringCallback 被调用了 delete ，所以释放了 <p>
 * 第二行 log 意思是 Java 层的 RcimNativeStringCallback 被调用了 delete，但是已经被释放过了，所以什么也没有做 <p>
 * 第三行 log 意思是 C++ 层调用 engineConnectAdapter 调用 RcimNativeStringCallback <p>
 * 第四行 log 意思是 崩溃了 <p>
 * 根据日志来看，原因是在 Java 层提前释放了 RcimNativeStringCallback，导致 C++ 层使用了野指针 <p>
 * </pre>
 *
 * <h3>Java 持有 callback</h3>
 * 解决思路是在 Java 层持有 callback： 调用 saveCallback <p>
 * 等 callback 使用完成后再移除：调用 removeCallback
 *
 * <h3>C++ 持有 callback</h3>
 * <pre>
 *     void engineConnectAdapter(const void *context, enum RcimEngineError code, const char *user_id) {
 *         if (!context) {
 *             return;
 *         }
 *         // context 是 NewGlobalRef 出来的 jobject
 *         JNIEnv *env = RongCloud::jni::AttachCurrentThreadIfNeeded();
 *         jobject callbackObj = reinterpret_cast<jobject>(const_cast<void *>(context));
 *
 *         // 将 jobject 转成你的 C++ RcimNativeStringCallback* 包装类
 *         auto *callback = reinterpret_cast<RcimNativeStringCallback *>(callbackObj);
 *
 *         callback->onResult(callback, code, user_id);
 *         env->DeleteGlobalRef(callbackObj);
 *     }
 *     void engineConnect(int64_t enginePtr, const char *token, int timeout,
 *                        RcimNativeStringCallback *callback) {
 *         if (enginePtr == 0) {
 *             if (callback) {
 *                 callback->onResult(callback, RcimEngineError_InvalidArgumentEngineSync, nullptr);
 *             }
 *             return;
 *         }
 *
 *         JNIEnv *env = RongCloud::jni::AttachCurrentThreadIfNeeded();
 *         jobject callbackObj = reinterpret_cast<jobject>(callback);
 *         jobject globalCallbackObj = env->NewGlobalRef(callbackObj); // 该行必现崩溃
 *
 *         auto *engine = reinterpret_cast<RcimEngineSync *>(static_cast<uintptr_t>(enginePtr));
 *         rcim_engine_connect(engine, token, timeout, globalCallbackObj, engineConnectAdapter);
 *     }
 * </pre>
 *
 * <pre>
 * java_vm_ext.cc:577] JNI DETECTED ERROR IN APPLICATION: use of invalid jobject 0xb400007931f25010
 * java_vm_ext.cc:577]     from void io.rong.imlib.internal.swig.RcClientJNI.engineConnect(long, java.lang.String, int, long, io.rong.imlib.internal.swig.RcimNativeStringCallback)
 * runtime.cc:655] Runtime aborting...
 *
 * 崩溃的原因是 env->NewGlobalRef 需要一个 Java 的对象，但 RcimNativeStringCallback *callback 是 C++ 的对象
 * </pre>
 *
 * @version todo
 */
public class CallbackHolder {
    // 使用 ConcurrentHashMap 是考虑线程安全，因为不确定 save & remove 方法是否在同一个线程
    private static final Map<Long, Object> cachedCallbacks = new ConcurrentHashMap<>();

    private CallbackHolder() {

    }

    /**
     * 保存 callback
     *
     * @param ptr            callback 的指针
     * @param nativeCallback callback
     */
    public static void saveNativeCallback(long ptr, @Nullable Object nativeCallback) {
        if (nativeCallback != null) {
            cachedCallbacks.put(ptr, nativeCallback);
        }
    }

    /**
     * 移除 callback
     * 只能在初始化、重新初始化的地方调用该方法
     *
     * @param ptr callback 的指针
     */
    public static void removeNativeCallback(long ptr) {
        cachedCallbacks.remove(ptr);
    }

    /**
     * 清空所有的缓存的 NativeCallback
     */
    public static void clearAllNativeCallback() {
        cachedCallbacks.clear();
    }
}
