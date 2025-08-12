package io.rong.imlib.internal.utils;

import androidx.annotation.Nullable;

import java.util.concurrent.atomic.AtomicReference;

import io.rong.imlib.internal.swig.RcimNativeIntListener;
import io.rong.imlib.internal.swig.RcimNativeMessageReceivedListener;

/**
 * Java 层持有 NativeListener 避免提前释放
 * <p>
 * 如果 Java 层不持有 NativeListener， NativeListener 提前释放，C++ 调用该 Listener 会发生野指针崩溃
 *
 * @version todo
 */
public class ListenerHolder {
    private static final AtomicReference<RcimNativeIntListener> conStatusListenerRef = new AtomicReference<>();
    private static final AtomicReference<RcimNativeMessageReceivedListener> msgReceivedListenerRef = new AtomicReference<>();


    private ListenerHolder() {

    }

    /// ------------------------------------------清空所有的缓存监听----------------------------------------------
    // <editor-fold desc="清空所有的缓存监听">
    /**
     * 清空所有的原生监听
     * 只能在初始化、重新初始化的地方调用该方法
     */
    public static void clearAllNativeListener() {
        conStatusListenerRef.set(null);
        msgReceivedListenerRef.set(null);
    }
    // </editor-fold>


    /// ------------------------------------------连接----------------------------------------------
    // <editor-fold desc="连接">
    /**
     * 保存原生连接监听
     *
     * @param nativeListener 原生连接监听
     */
    public static void saveNativeConnectListener(@Nullable RcimNativeIntListener nativeListener) {
        if (nativeListener != null) {
            conStatusListenerRef.set(nativeListener);
        }
    }

    /**
     * 获取原生连接监听
     *
     * @return 原生连接监听，可能为 null
     */
    @Nullable
    public static RcimNativeIntListener getNativeConnectListener() {
        return conStatusListenerRef.get();
    }
    // </editor-fold>

    /// ------------------------------------------消息----------------------------------------------
    // <editor-fold desc="消息">
    /**
     * 保存原生收消息监听
     *
     * @param nativeListener 原生收消息监听
     */
    public static void saveNativeMessageReceivedListener(@Nullable RcimNativeMessageReceivedListener nativeListener) {
        if (nativeListener != null) {
            msgReceivedListenerRef.set(nativeListener);
        }
    }

    /**
     * 获取原生收消息监听
     *
     * @return 原生收消息监听，可能为 null
     */
    @Nullable
    public static RcimNativeMessageReceivedListener getNativeMessageReceivedListener() {
        return msgReceivedListenerRef.get();
    }
    // </editor-fold>
}
