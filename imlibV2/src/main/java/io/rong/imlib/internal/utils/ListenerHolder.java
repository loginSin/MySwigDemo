package io.rong.imlib.internal.utils;

import androidx.annotation.Nullable;

import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicReference;

import io.rong.imlib.connect.enums.ConnectionStatus;
import io.rong.imlib.connect.listener.ConnectionStatusListener;
import io.rong.imlib.internal.swig.RcClient;
import io.rong.imlib.internal.swig.RcimMessageBox;
import io.rong.imlib.internal.swig.RcimNativeIntListener;
import io.rong.imlib.internal.swig.RcimNativeMessageReceivedListener;
import io.rong.imlib.internal.swig.RcimReceivedInfo;
import io.rong.imlib.message.Message;
import io.rong.imlib.message.listener.MessageReceivedListener;
import io.rong.imlib.message.model.ReceivedInfo;

/**
 * Java 层持有 NativeListener 避免提前释放
 * <p>
 * 如果 Java 层不持有 NativeListener， NativeListener 提前释放，C++ 调用该 Listener 会发生野指针崩溃
 *
 * @version todo
 */
public class ListenerHolder {
    // 所有变量使用 final 避免出现 null
    private final AtomicReference<RcimNativeIntListener> nativeConStatusListenerRef = new AtomicReference<>();
    private static final CopyOnWriteArrayList<ConnectionStatusListener> connectionStatusListenerList = new CopyOnWriteArrayList<>();

    private final AtomicReference<RcimNativeMessageReceivedListener> nativeMsgReceivedListenerRef = new AtomicReference<>();
    private static final CopyOnWriteArrayList<MessageReceivedListener> messageReceivedListenerList = new CopyOnWriteArrayList<>();

    public ListenerHolder() {
        // do nothing
    }

    /// ------------------------------------------注册所有的原生监听----------------------------------------------
    // <editor-fold desc="注册所有的原生监听">
    public void registerAllNativeListeners(long enginePtr) {
        // 1. 创建原生监听
        RcimNativeIntListener nativeConnectListener = new RcimNativeIntListener() {
            @Override
            public void onChanged(int value) {
                // 4. 原生监听方法触发后调用 Java 监听方法
                for (ConnectionStatusListener listener : ListenerHolder.connectionStatusListenerList) {
                    ConnectionStatus status = ConnectionStatus.codeOf(value);
                    if (listener != null) {
                        listener.onConnectionStatusChanged(status);
                    }
                }
            }
        };
        // 2. 把原生监听设置给 jni
        RcClient.engineSetConnectionStatusListener(enginePtr, nativeConnectListener);
        // 3. 把原生监听保存在 Java 层，避免提前释放
        this.nativeConStatusListenerRef.set(nativeConnectListener);


        // 1. 创建原生监听
        RcimNativeMessageReceivedListener nativeMessageReceiveListener = new RcimNativeMessageReceivedListener() {
            @Override
            public void onChanged(RcimMessageBox nativeMsgBox, RcimReceivedInfo nativeInfo) {
                // 4. 原生监听方法触发后调用 Java 监听方法
                for (MessageReceivedListener listener : ListenerHolder.messageReceivedListenerList) {
                    Message msg = Transformer.messageFromNative(nativeMsgBox);
                    ReceivedInfo info = Transformer.receivedInfoFromNative(nativeInfo);
                    if (listener != null) {
                        listener.onReceived(msg, info);
                    }

                }
            }
        };
        // 2. 把原生监听设置给 jni
        RcClient.engineSetMessageReceivedListener(enginePtr, nativeMessageReceiveListener);
        // 3. 把原生监听保存在 Java 层，避免提前释放
        this.nativeMsgReceivedListenerRef.set(nativeMessageReceiveListener);


    }

    // </editor-fold>

    /// ------------------------------------------清空所有的缓存监听----------------------------------------------
    // <editor-fold desc="清空所有的缓存监听">

    /**
     * 清空所有的原生监听
     * 只能在初始化、重新初始化的地方调用该方法
     */
    public void clearAllNativeListener() {
        nativeConStatusListenerRef.set(null);
        nativeMsgReceivedListenerRef.set(null);
    }
    // </editor-fold>


    /// ------------------------------------------连接----------------------------------------------
    // <editor-fold desc="连接">

    /**
     * 增加连接监听
     *
     * @param listener 连接监听
     */
    public void addConnectListener(@Nullable ConnectionStatusListener listener) {
        if (listener == null) {
            return;
        }
        if (ListenerHolder.connectionStatusListenerList.contains(listener)) {
            return;
        }
        ListenerHolder.connectionStatusListenerList.add(listener);
    }

    /**
     * 移除连接监听
     *
     * @param listener 连接监听
     */
    public void removeConnectListener(@Nullable ConnectionStatusListener listener) {
        if (listener == null) {
            return;
        }
        ListenerHolder.connectionStatusListenerList.remove(listener);
    }
    // </editor-fold>

    /// ------------------------------------------消息----------------------------------------------
    // <editor-fold desc="消息">

    /**
     * 增加消息接收监听
     *
     * @param listener 消息接收监听
     */
    public void addMessageReceivedListener(@Nullable MessageReceivedListener listener) {
        if (listener == null) {
            return;
        }
        if (ListenerHolder.messageReceivedListenerList.contains(listener)) {
            return;
        }
        ListenerHolder.messageReceivedListenerList.add(listener);
    }

    /**
     * 移除消息接收监听
     *
     * @param listener 消息接收监听
     */
    public void removeMessageReceivedListener(@Nullable MessageReceivedListener listener) {
        if (listener == null) {
            return;
        }
        ListenerHolder.messageReceivedListenerList.remove(listener);
    }
    // </editor-fold>
}
