package io.rong.imlib;


import android.content.Context;

import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import io.rong.imlib.base.callback.IData1Callback;
import io.rong.imlib.base.enums.EngineError;
import io.rong.imlib.connect.enums.ConnectionStatus;
import io.rong.imlib.connect.listener.ConnectionStatusListener;
import io.rong.imlib.message.Message;
import io.rong.imlib.message.callback.ISendMessageCallback;
import io.rong.imlib.swig.RcimNativeStringCallback;
import io.rong.imlib.swig.RcimNativeIntListener;
import io.rong.imlib.swig.RcimNativeSendMessageCallback;
import io.rong.imlib.swig.RcimEngineBuilderParam;
import io.rong.imlib.swig.RcimMessageBox;
import io.rong.imlib.swig.RcimSDKVersion;
import io.rong.imlib.swig.rc_adapter;

/**
 * @author rongcloud
 * @date 2025/8/4
 */
public class IMClient {
    private static final String TAG = "IMClient";
    private final AtomicLong enginePtr = new AtomicLong();
    // NativeXXXListener 需要在 java 层被持有，否则会被销毁造成野指针
    private final AtomicReference<RcimNativeIntListener> conStatusListenerRef = new AtomicReference<>();

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

    public void init(Context context, String appKey) {

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
        long longArr = rc_adapter.rcim_sdk_version_array_new(sdkVersionSize);
        long[] longPtr = {sdkVer1.getCPtr(), sdkVer2.getCPtr()};
        rc_adapter.rcim_sdk_version_array_insert(longArr, longPtr, longPtr.length);

        RcimSDKVersion totalVer = RcimSDKVersion.fromPointer(longArr);
        param.setSdk_version_vec(totalVer);
        param.setSdk_version_vec_len(sdkVersionSize);

        long[] builderPtrArr = {0};
        int code = rc_adapter.create_engine_builder(param, builderPtrArr);
        long builderPtr = 0;
        if (builderPtrArr.length > 0) {
            builderPtr = builderPtrArr[0];
        }
        sdkVer1.swigDelete();
        sdkVer2.swigDelete();
        param.swigDelete();

        rc_adapter.rcim_sdk_version_array_free(longArr);

        String storePath = context.getFilesDir().getPath();
        code = rc_adapter.engine_builder_set_store_path(builderPtr, storePath);

        long[] enginePtrArray = {0};
        code = rc_adapter.engine_builder_build(builderPtr, enginePtrArray);

        if (enginePtrArray.length > 0) {
            this.enginePtr.set(enginePtrArray[0]);
        }
    }

    public void connect(String token, int timeout, IData1Callback<String> callback) {
        rc_adapter.engine_connect(this.enginePtr.get(), token, timeout, new RcimNativeStringCallback() {
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
                deleteThis.swigDelete();
            }
        });
    }

    public void setConnectionStatusListener(ConnectionStatusListener listener) {
        RcimNativeIntListener cachedNativeListener = this.conStatusListenerRef.get();
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
        this.conStatusListenerRef.set(nativeListener);
        rc_adapter.engine_set_connection_status_listener(this.enginePtr.get(), nativeListener);
    }

    public void sendMessage(Message msg, ISendMessageCallback<Message> sendMessageCallback) {

        RcimMessageBox inputMsgBox = new RcimMessageBox();
        inputMsgBox.setConv_type(msg.getConversationType().getValue());
        inputMsgBox.setTarget_id(msg.getTargetId());
        inputMsgBox.setChannel_id(msg.getChannelId());
        inputMsgBox.setObject_name(msg.getObjectName());
        inputMsgBox.setContent(msg.getContentJson());
        rc_adapter.engine_send_message(this.enginePtr.get(), inputMsgBox, new RcimNativeSendMessageCallback() {

            @Override
            public void onSave(RcimMessageBox msg) {
                String content = msg.getContent();
                if (sendMessageCallback != null) {
//                    sendMessageCallback.onAttached(message);
                }
            }

            @Override
            public void onResult(RcimNativeSendMessageCallback deleteThis,int code, RcimMessageBox msg) {
                String content = msg.getContent();
                if (sendMessageCallback == null) {
                    return;
                }
                if (EngineError.Success.getCode() == code) {
//                    sendMessageCallback.onSuccess(message);
                } else {

                }
                inputMsgBox.swigDelete();
                deleteThis.swigDelete();
            }
        });
    }
}
