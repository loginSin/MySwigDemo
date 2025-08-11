package io.rong.imlib.internal.utils;


import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import io.rong.imlib.conversation.enums.ConversationType;
import io.rong.imlib.message.Message;
import io.rong.imlib.internal.swig.RcimMessageBox;

public class Transformer {
    private Transformer() {

    }

    public static void messageFromNative(@NonNull Message outMsg, @Nullable RcimMessageBox inNativeMsg) {
        if (inNativeMsg == null) {
            return;
        }
        outMsg.setMessageId(inNativeMsg.getMessage_id());
        outMsg.setConversationType(ConversationType.setValue(inNativeMsg.getConv_type()));
        outMsg.setTargetId(inNativeMsg.getTarget_id());
        outMsg.setChannelId(inNativeMsg.getChannel_id());
        outMsg.setObjectName(inNativeMsg.getObject_name());
        outMsg.setContentJson(inNativeMsg.getContent());
    }

    public static void messageToNative(@Nullable Message inMsg, @NonNull RcimMessageBox outNativeMsg) {
        if (inMsg == null) {
            return;
        }
        outNativeMsg.setMessage_id(inMsg.getMessageId());
        outNativeMsg.setConv_type(inMsg.getConversationType().getValue());
        outNativeMsg.setTarget_id(inMsg.getTargetId());
        outNativeMsg.setChannel_id(inMsg.getChannelId());
        outNativeMsg.setObject_name(inMsg.getObjectName());
        outNativeMsg.setContent(inMsg.getContentJson());
    }
}
