package io.rong.imlib.message.callback;

import io.rong.imlib.base.callback.IErrorDataCallback;
import io.rong.imlib.message.Message;

// todo qixinbing onError 和 IBaseCallback 不一样
public interface ISendMessageCallback<T> extends IErrorDataCallback<T> {
    void onAttached(Message message);
    void onSuccess(Message message);
}
