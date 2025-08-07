package io.rong.imlib;


import android.content.Context;
import android.util.Log;

import java.util.concurrent.atomic.AtomicLong;

import io.rong.imlib.base.callback.IData1Callback;
import io.rong.imlib.base.enums.EngineError;
import io.rong.imlib.connect.enums.ConnectionStatus;
import io.rong.imlib.connect.listener.ConnectionStatusListener;
import io.rong.imlib.message.Message;
import io.rong.imlib.message.callback.ISendMessageCallback;
import io.rong.imlib.swig.RcimEngineBuilderParam;
import io.rong.imlib.swig.RcimMessageBox;
import io.rong.imlib.swig.RcimSDKVersion;
import io.rong.imlib.swig.VectorLong;
import io.rong.imlib.swig.rc_adapter;
import io.rong.imlib.swig_helper.callback.NativeSendMessageCallback;
import io.rong.imlib.swig_helper.callback.NativeStringCallback;
import io.rong.imlib.swig_helper.listener.NativeIntListener;

/**
 * @author rongcloud
 * @date 2025/8/4
 */
public class IMClient {
    private static final String TAG = "IMClient";
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
        VectorLong longVec = new VectorLong();
        longVec.add(sdkVer1.getCPtr());
        longVec.add(sdkVer2.getCPtr());
        rc_adapter.rcim_sdk_version_array_insert(longArr,longVec);

        RcimSDKVersion totalVer = RcimSDKVersion.fromPointer(longArr);
        param.setSdk_version_vec(totalVer);
        param.setSdk_version_vec_len(sdkVersionSize);


        // 创建空的 VectorInt 作为输出参数
        VectorLong builderPtrArray = new VectorLong();

        int code = rc_adapter.create_engine_builder(param, builderPtrArray);

        rc_adapter.rcim_sdk_version_array_free(longArr);
        long builderPtr = 0;
        if (builderPtrArray.size() > 0) {
            builderPtr = builderPtrArray.get(0);
            Log.d(TAG, "init: " + builderPtrArray.get(0));
        }

        String storePath = context.getFilesDir().getPath();
        code = rc_adapter.engine_builder_set_store_path(builderPtr, storePath);

        VectorLong enginePtrArray = new VectorLong();
        code = rc_adapter.engine_builder_build(builderPtr, enginePtrArray);

        if (enginePtrArray.size() > 0) {
            this.enginePtr.set(enginePtrArray.get(0));
        }
    }

    public void connect(String token, int timeout, IData1Callback<String> callback) {
        rc_adapter.engine_connect(this.enginePtr.get(), token, timeout, new NativeStringCallback() {
            @Override
            public void onComplete(int code, String value) {
                if (callback == null) {
                    return;
                }
                if (EngineError.Success.getCode() == code) {
                    callback.onSuccess(value);
                } else {
                    callback.onError(EngineError.codeOf(code));
                }
            }
        });
    }

    public void setConnectionStatusListener(ConnectionStatusListener listener) {
        rc_adapter.engine_set_connection_status_listener(this.enginePtr.get(), new NativeIntListener() {
            @Override
            public void onChanged(int status) {
                if (listener != null) {
                    listener.onConnectionStatusChanged(ConnectionStatus.codeOf(status));
                }
            }
        });
    }

    public void sendMessage(Message msg, ISendMessageCallback<Message> sendMessageCallback) {
        RcimMessageBox msgBox = new RcimMessageBox();
        msgBox.setConv_type(msg.getConversationType().getValue());
        msgBox.setTarget_id(msg.getTargetId());
        msgBox.setChannel_id(msg.getChannelId());
        msgBox.setObject_name(msg.getObjectName());
        msgBox.setContent(msg.getContentJson());
        rc_adapter.engine_send_message(this.enginePtr.get(), msgBox, new NativeSendMessageCallback() {

            @Override
            public void onAttached(Message message) {
                if (sendMessageCallback != null) {
                    sendMessageCallback.onAttached(message);
                }
            }

            @Override
            public void onSuccess(Message message) {
                if (sendMessageCallback != null) {
                    sendMessageCallback.onSuccess(message);
                }
            }

            @Override
            public void onError(EngineError error, Message data) {
                if (sendMessageCallback != null) {
                    sendMessageCallback.onError(error, data);
                }
            }
        });
    }
}
