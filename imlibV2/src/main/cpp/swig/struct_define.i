
// 告诉 SWIG 把 RcimConversationType 当作 int 处理，注意，声明的枚举值不能带 enum 关键字
%apply int { RcimConversationType };
%apply int { RcimMessageDirection };
%apply int { RcimSentStatus };
%apply int { RcimPlatform };

%include "extracted_struct_define.h"

//%inline %{
//
//typedef struct RcimMessageBox {
//    /**
//     * 会话类型
//     */
//    RcimConversationType conv_type;
//    /**
//     * 目标 ID
//     */
//    const char *target_id;
//    /**
//     * 所属会话的业务标识，长度限制 20 字符
//     */
//    const char *channel_id;
//    /**
//     * 数据库消息表递增 ID
//     */
//    int64_t message_id;
//    /**
//     * 消息方向
//     */
//    RcimMessageDirection direction;
//    /**
//     * 发送者 ID
//     */
//    const char *sender_id;
//    const struct RcimReceivedStatus *received_status;
//    RcimSentStatus sent_status;
//    int64_t received_time;
//    int64_t sent_time;
//    /**
//     * 消息的类型名
//     */
//    const char *object_name;
//    /**
//     * 消息内容 内置消息大都是 json 字符串，自定义消息可能是其他格式
//     */
//    const char *content;
//    /**
//     * 可被搜索的内容，只做为入参，不做出参，返回的消息该字段为空
//     */
//    const char *searchable_words;
//    /**
//     * 服务端生产的消息唯一 ID（在同一个 AppKey 下全局唯一）
//     */
//    const char *uid;
//    /**
//     * 消息的附加字段
//     * Android: extra
//     * iOS: extra
//     * C++: extra_content_
//     */
//    const char *extra;
//    /**
//     * 群阅读回执信息
//     */
//    const struct RcimReadReceiptInfo *read_receipt_info;
//    /**
//     * 群阅读回执信息 V2
//     */
//    const struct RcimReadReceiptInfoV2 *read_receipt_info_v2;
//    /**
//     * 是否在收到消息的时候发送通知
//     * Android: messageConfig
//     * iOS: messageConfig
//     * C++: disable_notification_
//     */
//    bool is_notification_disabled;
//    /**
//     * 消息推送配置
//     * Android: messagePushConfig
//     * iOS: messagePushConfig
//     *
//     * 内部成员属性，传入空指针表示无效值，空串视为有效值；
//     * 空串会被 encode 并传递给服务端<br/>且空串在业务上目前是无意义的，但以后有变为有意义的可能性
//     * 比如：表示一些默认场景值，所以平台侧要保证不随意传入空串
//     *
//     */
//    const struct RcimPushConfig *push_config;
//    /**
//     * 是否是离线消息，只在接收消息的回调方法中有效，如果消息为离线消息，则为 true，其他情况均为 false
//     * Android: isOffLine
//     * iOS: isOffLine
//     */
//    bool is_offline;
//    /**
//     * 消息是否可以包含扩展信息
//     * 该属性在消息发送时确定，发送之后不能再做修改
//     * 扩展信息只支持单聊、群聊、超级群，其它会话类型不能设置扩展信息
//     *
//     * Android: extSupport
//     * iOS: canIncludeExpansion
//     * C++: ext_support_
//     */
//    bool is_ext_supported;
//    /**
//     * 必须是 json 字符串，形如 "{"key1":"value1","key2":"value2"}"
//     * 否则会被返回 Json 解析错误
//     *
//     * Android: extMsg
//     * iOS: expansionDicEx
//     * C++: ext_msg_
//     */
//    const char *ext_content;
//    /**
//     * 标识是否是一条已读回执 (V5) 消息
//     */
//    bool is_need_receipt;
//    /**
//     * 消息接收方使用，标识是否发送过已读回执 (V5)
//     */
//    bool has_sent_receipt;
//} RcimMessageBox;
//
//%}