package io.rong.imlib.swig_helper.callback;

import io.rong.imlib.base.enums.EngineError;
import io.rong.imlib.message.Message;

public interface NativeSendMessageCallback {
    void onAttached(Message message);
    void onSuccess(Message message);
    void onError(EngineError error, Message data);
}
