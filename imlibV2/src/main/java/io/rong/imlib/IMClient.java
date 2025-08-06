package io.rong.imlib;


import android.content.Context;
import android.util.Log;

import java.util.concurrent.atomic.AtomicLong;

import io.rong.imlib.base.callback.IData1Callback;
import io.rong.imlib.base.enums.EngineError;
import io.rong.imlib.connect.enums.ConnectionStatus;
import io.rong.imlib.connect.listener.ConnectionStatusListener;
import io.rong.imlib.swig.MapString;
import io.rong.imlib.swig.VectorLong;
import io.rong.imlib.swig.rc_adapter;
import io.rong.imlib.swig_helper.EngineBuilderParam;
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

        // 测试传递 Java 对象到 C++
        MapString sdkVersions = new MapString();
        sdkVersions.put("imlib", "1.0.0");

        EngineBuilderParam param = new EngineBuilderParam();
        param.appKey = appKey;
        param.deviceId = "test_device_id";
        param.packageName = context.getPackageName();
        param.imlibVersion = "1.0.0";
        param.deviceModel = "iPhone";
        param.deviceManufacturer = "Apple";
        param.osVersion = "iOS 15.0";
        param.sdkVersions = sdkVersions;
        param.appVersion = "1.2.3";

        // 创建空的 VectorInt 作为输出参数
        VectorLong builderPtrArray = new VectorLong();

        int code = rc_adapter.create_engine_builder(param, builderPtrArray);
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
}
