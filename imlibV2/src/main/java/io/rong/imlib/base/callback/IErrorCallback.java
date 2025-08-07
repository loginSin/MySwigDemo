package io.rong.imlib.base.callback;

import io.rong.imlib.base.enums.EngineError;

public interface IErrorCallback {
    void onError(EngineError error);
}
