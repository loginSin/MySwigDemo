package io.rong.imlib.base.callback;

import io.rong.imlib.base.enums.EngineError;

public interface IErrorDataCallback<T> {
    void onError(EngineError error, T data);
}
