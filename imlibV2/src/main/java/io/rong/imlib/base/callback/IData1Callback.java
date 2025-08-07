package io.rong.imlib.base.callback;

public interface IData1Callback<T> extends IErrorCallback {
    void onSuccess(T value);
}
