package io.rong.imlib;

import android.content.Context;

import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import io.rong.imlib.base.callback.IData1Callback;
import io.rong.imlib.base.enums.EngineError;
import io.rong.imlib.connect.enums.ConnectionStatus;
import io.rong.imlib.connect.listener.ConnectionStatusListener;
import io.rong.imlib.internal.swig.RcimNativeMessageReceivedListener;
import io.rong.imlib.internal.swig.RcimReceivedInfo;
import io.rong.imlib.internal.swig.RcimStringVector;
import io.rong.imlib.internal.utils.CallbackHolder;
import io.rong.imlib.internal.utils.ListenerHolder;
import io.rong.imlib.message.Message;
import io.rong.imlib.message.callback.ISendMessageCallback;
import io.rong.imlib.internal.swig.RcClient;
import io.rong.imlib.internal.swig.RcimNativeStringCallback;
import io.rong.imlib.internal.swig.RcimNativeIntListener;
import io.rong.imlib.internal.swig.RcimNativeSendMessageCallback;
import io.rong.imlib.internal.swig.RcimEngineBuilderParam;
import io.rong.imlib.internal.swig.RcimMessageBox;
import io.rong.imlib.internal.swig.RcimSDKVersion;
import io.rong.imlib.internal.utils.Transformer;
import io.rong.imlib.message.listener.MessageReceivedListener;
import io.rong.imlib.message.model.ReceivedInfo;

public class IMClient {
    private final AtomicLong enginePtr = new AtomicLong();

    static {
        try {
            System.loadLibrary("imlibv2");
        } catch (UnsatisfiedLinkError e) {
            e.printStackTrace();
        }
    }

    private IMClient() {

    }

    private static class SingletonHolder {
        static IMClient sInstance = new IMClient();
    }

    public static IMClient getInstance() {
        return SingletonHolder.sInstance;
    }

    public void init(
            Context context,
            String appKey
    ) {

        resetData();

        RcimEngineBuilderParam param = new RcimEngineBuilderParam();
        param.setApp_key(appKey);
        param.setDevice_id("test_device_id");
        param.setPackage_name(context.getPackageName());
        param.setImlib_version("1.0.0");
        param.setDevice_model("iPhone");
        param.setDevice_manufacturer("Apple");
        param.setOs_version("iOS 15.0");
        param.setApp_version("1.2.3");

        RcimSDKVersion sdkVer1 = new RcimSDKVersion();
        sdkVer1.setName("imlib");
        sdkVer1.setVersion("1.0.0");
        RcimSDKVersion sdkVer2 = new RcimSDKVersion();
        sdkVer2.setName("imkit");
        sdkVer2.setVersion("1.1.1");

        int sdkVersionSize = 2;
        long longArr = RcClient.newSdkVersionArray(sdkVersionSize);
        long[] longPtr = {sdkVer1.getCPtr(), sdkVer2.getCPtr()};
        RcClient.insertSdkVersionArray(longArr, longPtr, longPtr.length);

        RcimSDKVersion totalVer = RcimSDKVersion.fromPointer(longArr);
        param.setSdk_version_vec(totalVer);
        param.setSdk_version_vec_len(sdkVersionSize);

        long[] builderPtrArr = {0};
        int code = RcClient.createEngineBuilder(param, builderPtrArr);
        long builderPtr = builderPtrArr[0];
        sdkVer1.swigDelete();
        sdkVer2.swigDelete();
        param.swigDelete();

        RcClient.freeSdkVersionArray(longArr);

        String storePath = context.getFilesDir().getPath();
        code = RcClient.engineBuilderSetStorePath(builderPtr, storePath);
        code = RcClient.engineBuilderSetAreaCode(builderPtr, 1);
        RcimStringVector stringVector = new RcimStringVector();
        code = RcClient.engineBuilderSetNaviServer(builderPtr, stringVector);
        stringVector.swigDelete();
        code = RcClient.engineBuilderSetStatisticServer(builderPtr, "");
        code = RcClient.engineBuilderSetCloudType(builderPtr, 108);
        code = RcClient.engineBuilderSetDbEncrypted(builderPtr, false);
        code = RcClient.engineBuilderSetEnableGroupCall(builderPtr, false);
        code = RcClient.engineBuilderSetEnableReconnectKick(builderPtr, false);
        code = RcClient.engineBuilderSetFilePath(builderPtr, "RongCloud");
        code = RcClient.engineBuilderSetNetworkEnv(builderPtr, "");

        long[] enginePtrArray = {0};
        code = RcClient.engineBuilderBuild(builderPtr, enginePtrArray);

        this.enginePtr.set(enginePtrArray[0]);
    }

    private void resetData() {
        this.enginePtr.set(0);
        ListenerHolder.clearAllNativeListener();
        CallbackHolder.clearAllNativeCallback();
    }

    public void connect(String token, int timeout, IData1Callback<String> callback) {
        // 1. 创建 NativeCallback
        RcimNativeStringCallback nativeCallback = new RcimNativeStringCallback() {
            @Override
            public void onResult(RcimNativeStringCallback deleteThis, int code, String value) {
                if (callback == null) {
                    return;
                }
                if (EngineError.Success.getCode() == code) {
                    callback.onSuccess(value);
                } else {
                    callback.onError(EngineError.codeOf(code));
                }
                // 4. NativeCallback 使用完成之后，从 Java 层移除并且释放该 NativeCallback
                if (deleteThis != null) {
                    CallbackHolder.removeNativeCallback(deleteThis.getCPtr());
                    deleteThis.swigDelete();
                }
            }
        };

        // 2. Java 层保存 NativeCallback
        CallbackHolder.saveNativeCallback(nativeCallback.getCPtr(), nativeCallback);
        // 3. 将 NativeCallback 传给 jni
        RcClient.engineConnect(this.enginePtr.get(), token, timeout, nativeCallback);
    }

    public void setConnectionStatusListener(ConnectionStatusListener listener) {
        RcimNativeIntListener cachedNativeListener = ListenerHolder.getNativeConnectListener();
        // 原生监听已存在说明已经设置过了，就不再设置
        if (cachedNativeListener != null) {
            return;
        }
        RcimNativeIntListener nativeListener = new RcimNativeIntListener() {
            @Override
            public void onChanged(int value) {
                if (listener != null) {
                    ConnectionStatus status = ConnectionStatus.codeOf(value);
                    listener.onConnectionStatusChanged(status);
                }
            }
        };
        ListenerHolder.saveNativeConnectListener(nativeListener);
        RcClient.engineSetConnectionStatusListener(this.enginePtr.get(), nativeListener);
    }

    public void sendMessage(Message msg, ISendMessageCallback<Message> sendMessageCallback) {

        RcimMessageBox inputMsgBox = new RcimMessageBox();
        Transformer.messageToNative(msg, inputMsgBox);
        RcimNativeSendMessageCallback nativeCallback = new RcimNativeSendMessageCallback() {
            @Override
            public void onSave(RcimMessageBox nativeMsg) {
                Message msg = Transformer.messageFromNative(nativeMsg);
                if (sendMessageCallback != null) {
                    sendMessageCallback.onAttached(msg);
                }
            }

            @Override
            public void onResult(RcimNativeSendMessageCallback deleteThis, int code, RcimMessageBox nativeMsg) {
                Message msg = Transformer.messageFromNative(nativeMsg);
                if (sendMessageCallback == null) {
                    return;
                }
                if (EngineError.Success.getCode() == code) {
                    sendMessageCallback.onSuccess(msg);
                } else {
                    sendMessageCallback.onError(EngineError.codeOf(code), msg);
                }

                inputMsgBox.swigDelete();
                if (deleteThis != null) {
                    CallbackHolder.removeNativeCallback(deleteThis.getCPtr());
                    deleteThis.swigDelete();
                }
            }
        };
        CallbackHolder.saveNativeCallback(nativeCallback.getCPtr(), nativeCallback);
        RcClient.engineSendMessage(this.enginePtr.get(), inputMsgBox, nativeCallback);
    }

    public void setMessageReceivedListener(MessageReceivedListener listener) {
        RcimNativeMessageReceivedListener cachedMsgRevListener = ListenerHolder.getNativeMessageReceivedListener();
        // 原生监听已存在说明已经设置过了，就不再设置
        if (cachedMsgRevListener != null) {
            return;
        }

        RcimNativeMessageReceivedListener nativeListener = new RcimNativeMessageReceivedListener() {
            @Override
            public void onChanged(RcimMessageBox nativeMsgBox, RcimReceivedInfo nativeInfo) {
                if (listener != null) {
                    Message msg = Transformer.messageFromNative(nativeMsgBox);
                    ReceivedInfo info = Transformer.receivedInfoFromNative(nativeInfo);
                    listener.onReceived(msg, info);
                }
            }
        };
        ListenerHolder.saveNativeMessageReceivedListener(nativeListener);
        RcClient.engineSetMessageReceivedListener(this.enginePtr.get(), nativeListener);
    }
}
