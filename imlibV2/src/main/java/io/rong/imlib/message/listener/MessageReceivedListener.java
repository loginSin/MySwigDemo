package io.rong.imlib.message.listener;

import io.rong.imlib.message.Message;
import io.rong.imlib.message.model.ReceivedInfo;

public interface MessageReceivedListener {
    void onReceived(Message msg, ReceivedInfo info);
}
