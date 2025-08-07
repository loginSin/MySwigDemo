package io.rong.imlib.conversation.enums;

public enum ConversationType {
    /**
     * NONE
     *
     * @note 边界值，不能使用。
     */
    NONE(0, "none"),
    /** 私聊。 */
    PRIVATE(1, "private"),

    /** 讨论组。 */
    DISCUSSION(2, "discussion"),

    /** 群组。 */
    GROUP(3, "group"),

    /** 聊天室。 */
    CHATROOM(4, "chatroom"),

    /** 客服。 */
    CUSTOMER_SERVICE(5, "customer_service"),

    /** 系统。 */
    SYSTEM(6, "system"),

    /** 应用公众服务。 */
    APP_PUBLIC_SERVICE(7, "app_public_service"),

    /** 公众服务平台。 */
    PUBLIC_SERVICE(8, "public_service"),

    /** push 推送。 */
    PUSH_SERVICE(9, "push_service"),

    /** 超级群 */
    ULTRA_GROUP(10, "ULTRA_GROUP"),

    /** 加密会话（仅对部分私有云用户开放，公有云用户不适用）。 */
    ENCRYPTED(11, "encrypted"),

    /** rtc 音视频通话类型。 */
    RTC_ROOM(12, "rtc_room");

    private final int value;
    private final String name;

    /**
     * 构造函数。
     *
     * @param value 会话类型的值。
     */
    ConversationType(int value, String name) {
        this.value = value;
        this.name = name;
    }

    /**
     * 获取会话类型的值。
     *
     * @return 会话类型的值。
     */
    public int getValue() {
        return this.value;
    }

    /**
     * 获取会话类型名称。
     *
     * @return 会话类型名称。
     */
    public String getName() {
        return this.name;
    }

    /**
     * 设置会话类型。
     *
     * @param code 会话类型的值。
     * @return 会话类型枚举。
     */
    public static ConversationType setValue(int code) {
        for (ConversationType c : ConversationType.values()) {
            if (code == c.getValue()) {
                return c;
            }
        }
        return PRIVATE;
    }
}
