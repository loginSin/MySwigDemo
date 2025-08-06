package io.rong;

import io.rong.imlib.swig.RcimEngineBuilder;

/**
 * @author rongcloud
 * @date 2025/8/4
 */
public class NativeClient {
    public static void init() {
        RcimEngineBuilderParam param = new RcimEngineBuilderParam();
        param.setApp_key("1234");
        param.setDevice_id("test_device_id");
        param.setPackage_name("com.rongcloud");
        RcimSDKVersion version = new RcimSDKVersion();
        param.setImlib_version("1.0.0");
        version.setName("imlib");
        version.setVersion("1.0.0");
        param.setSdk_version_vec(version);
        param.setSdk_version_vec_len(1);
        param.setDevice_model("oppo");
        param.setPlatform(RcimPlatform.RcimPlatform_Android);
        param.setDevice_manufacturer("oppo");
        param.setOs_version("1.2.3");
        param.setApp_version("2.2.2");
        RcimEngineBuilder[] builder = new RcimEngineBuilder[1];
        native_engine2.rcim_create_engine_builder(param, builder);
        int a = 3;
    }
}
