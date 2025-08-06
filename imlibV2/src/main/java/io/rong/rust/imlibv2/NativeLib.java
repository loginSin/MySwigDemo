package io.rong.rust.imlibv2;

import io.rong.rust.imlibv2.jni.example;

public class NativeLib {

    // Used to load the 'imlibv2' library on application startup.
    static {
        System.loadLibrary("imlibv2");
    }

    /**
     * A native method that is implemented by the 'imlibv2' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public int add(int a ,int b) {
        return example.add(a, b);
    }
}