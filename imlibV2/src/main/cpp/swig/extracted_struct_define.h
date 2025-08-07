typedef struct RcimSDKVersion {
  /**
   * SDK 名称，字符串长度限制 1-64 字符
   */
  const char *name;
  /**
   * SDK 版本号，字符串长度限制 1-64 字符
   */
  const char *version;
} RcimSDKVersion;

typedef struct RcimEngineBuilderParam {
  /**
   * App 应用标识
   */
  const char *app_key;
  /**
   * 客户端平台类型
   */
  RcimPlatform platform;
  /**
   * 客户端设备标识，尽量在构建时一次性传入
   * 注：鸿蒙目前因为设备 ID 获取为异步接口，不能在引擎构建时第一时间传入
   * 可以暂时传 NULL，然后需要在创建 engine 后调用 set_device_id 设置
   */
  const char *device_id;
  /**
   * 客户端包名（如："cn.xxx.im"）
   */
  const char *package_name;
  /**
   * 调用此库的平台层 imlib 版本号，供大数据统计和服务确认连接版本
   */
  const char *imlib_version;
  /**
   * 设备型号（如：iPhoneX），供大数据统计
   */
  const char *device_model;
  /**
   * 设备厂商（如：Apple），供大数据统计
   */
  const char *device_manufacturer;
  /**
   * 操作系统版本号（如：iOS_14.0），供大数据统计
   */
  const char *os_version;
  /**
   * 当前环境中所有 SDK 版本号，需包含 imlib_ver 键值对，
   * 构建完成后会自动添加 rustlib_ver 键值对，供大数据统计
   * 如：{ "imlib": "4.0.0", "imkit": "2.0.0" }
   */
  const struct RcimSDKVersion *sdk_version_vec;
  /**
   * sdk_ver_vec 的数组长度
   */
  int32_t sdk_version_vec_len;
  /**
   * 调用此库的 App 版本号，供大数据统计
   */
  const char *app_version;
} RcimEngineBuilderParam;

typedef struct RcimReceivedStatus {
  bool is_read;
  bool is_listened;
  bool is_download;
  bool is_retrieved;
  bool is_multiple_received;
} RcimReceivedStatus;

typedef struct RcimReadReceiptUserInfo {
  /**
   * 发送已读回执用户 ID
   */
  const char *sender_id;
  /**
   * 已读回执时间戳
   */
  uint64_t timestamp;
  /**
   * 是否是被 AT 用户，仅已读 V5 版本有效
   */
  bool mentioned;
} RcimReadReceiptUserInfo;

typedef struct RcimReadReceiptInfo {
  /**
   * 是否为需要回执的消息（true: 需要回执的消息；false: 普通消息）
   */
  bool is_read_receipt_message;
  /**
   * 是否发送过消息回执响应，仅对消息接收方有效
   */
  bool has_respond;
  /**
   * 会话中响应过该消息回执的成员
   */
  const struct RcimReadReceiptUserInfo *respond_user_vec;
  int32_t respond_user_vec_len;
} RcimReadReceiptInfo;

typedef struct RcimReadReceiptInfoV2 {
  /**
   * 是否发送过消息回执响应，仅对消息接收方有效
   */
  bool has_respond;
  /**
   * 会话中响应过该消息回执的成员
   */
  const struct RcimReadReceiptUserInfo *respond_user_vec;
  int32_t respond_user_vec_len;
  /**
   * 已读成员数
   */
  uint32_t read_count;
  /**
   * 所有成员数
   */
  uint32_t total_count;
} RcimReadReceiptInfoV2;

typedef struct RcimIosConfig {
  /**
   * iOS 平台通知栏分组 ID
   * 相同的 thread-id 推送分为一组
   * iOS10 开始支持
   */
  const char *thread_id;
  /**
   * iOS 标识推送的类型
   * 如果不设置后台默认取消息类型字符串，如 RC:TxtMsg
   */
  const char *category;
  /**
   * iOS 平台通知覆盖 ID
   * apnsCollapseId 相同时，新收到的通知会覆盖老的通知，最大 64 字节
   * iOS10 开始支持
   */
  const char *apns_collapse_id;
  /**
   * iOS 富文本推送内容
   */
  const char *rich_media_uri;
  /**
   * iOS 推送级别。默认值 "active"
   * "passive" 被动通知：即并不需要及时关注的通知，类似餐馆推荐通知
   * "active"   主动通知（默认的）：默认的通知，即人们可能想知道的，类似最喜欢的体育比赛的最新比分
   * "time-sensitive" 时效性通知：需要人们立刻注意的通知，类似账户安全问题或快递动态
   * "critical" 重要通知：关于个人健康或直接影响到设备拥有者的公共安全事件且需要立刻关注的，这类通知很少，一般是来自公共政府机构或健康 App。
   */
  const char *interruption_level;
} RcimIosConfig;

typedef struct RcimAndroidConfig {
  /**
   * Android 平台 Push 唯一标识
   * 目前支持小米、华为推送平台，默认开发者不需要进行设置，当消息产生推送时，消息的 messageUId 作为 notificationId 使用。
   */
  const char *notification_id;
  /**
   * 小米的渠道 ID
   * 该条消息针对小米使用的推送渠道，如开发者集成了小米推送，需要指定 channelId 时，可向 Android 端研发人员获取，channelId 由开发者自行创建。
   */
  const char *mi_channel_id;
  /**
   * 小米 Large icon 链接
   * 由于小米官方已停止支持该能力，该字段/方法已失效
   * 华为的渠道 ID
   * 该条消息针对华为使用的推送渠道，如开发者集成了华为推送，需要指定 channelId 时，可向 Android 端研发人员获取，channelId 由开发者自行创建。
   */
  const char *hw_channel_id;
  /**
   * 华为推送消息级别
   * 消息级别包括 "NORMAL", "LOW"
   */
  const char *hw_importance;
  /**
   * 华为通知栏消息右侧大图标 URL
   */
  const char *hw_image_url;
  /**
   * 华为推送消息分类：
   * - 社交通讯：即时通讯 [IM],音频、视频通话 [VOIP]
   * - 服务提醒：订阅 [SUBSCRIPTION],出行 [TRAVEL],健康 [HEALTH],工作事项提醒 [WORK],帐号动态 [ACCOUNT],订单&物流 [EXPRESS],财务 [FINANCE],系统提示 [SYSTEM_REMINDER],邮件 [MAIL]
   * - 资讯营销类：内容资讯/新闻/财经动态/生活资讯/社交动态/调研/其他 [MARKETING]
   * - 营销活动：产品促销/功能推荐/运营活动/MARKETING
   *
   * 更多信息请参考华为消息分类标准文档：https://developer.huawei.com/consumer/cn/doc/development/HMSCore-Guides/message-classification-0000001149358835
   *
   * 默认值为 NULL，如果为 NULL，则以服务配置为准
   */
  const char *hw_category;
  /**
   * 荣耀推送消息级别
   * 消息级别包括 "NORMAL", "LOW"
   */
  const char *honor_importance;
  /**
   * 荣耀通知栏消息右侧大图标 URL
   */
  const char *honor_image_url;
  /**
   * OPPO 的渠道 ID
   * 该条消息针对 OPPO 使用的推送渠道，如开发者集成了 OPPO 推送，需要指定 channelId 时，可向 Android 端研发人员获取，channelId 由开发者自行创建。
   */
  const char *oppo_channel_id;
  /**
   * VIVO 推送消息分类
   *
   * 系统消息分类
   * 即时消息 [IM],账号与资产 [ACCOUNT],日程待办 [TODO],设备信息 [DEVICE_REMINDER],订单与物流 [ORDER],订阅提醒 [SUBSCRIPTION]
   * 运营消息分类
   * 新闻 [NEWS],内容推荐 [CONTENT],运营活动 [MARKETING],社交动态 [SOCIAL]
   * 更多信息请参考 Vivo 消息分类标准文档：https://dev.vivo.com.cn/documentCenter/doc/359
   *
   * 默认值为 NULL，如果为 NULL，则以服务配置为准
   */
  const char *vivo_category;
  /**
   * VIVO 推送通道类型
   * 开发者集成了 VIVO 推送，需要指定推送类型时，可进行设置。
   * 目前可选值 "0"(运营消息) 和  "1"(系统消息)
   */
  const char *vivo_type;
  /**
   * FCM 通知的频道 ID
   * 该应用程序必须使用此频道 ID 创建一个频道，然后才能收到带有该频道 ID 的任何通知。如果您未在请求中发送此频道 ID，或者如果应用尚未创建提供的频道 ID，则 FCM 使用应用清单中指定的频道 ID。
   */
  const char *fcm_channel_id;
  /**
   * FCM 通知类型推送时所使用的分组 ID
   */
  const char *fcm_collapse_key;
  /**
   * FCM 通知类型的推送所使用的通知图片 URL
   */
  const char *fcm_image_url;
} RcimAndroidConfig;

typedef struct RcimHarmonyConfig {
  /**
   * HarmonyOS 通知栏消息右侧大图标 URL
   * 通知栏右侧图片，格式支持 png、jpg、jpeg、heif、gif、bmp，图片长*宽 < 25000 像素，图片不满足要求的情况下，终端不能显示通知消息。
   */
  const char *image_url;
  /**
   * HarmonyOS 推送消息分类：
   * 社交通讯：消息自分类标识，默认为空。category 取值必须为大写字母
   * 默认值为 NULL，如果为 NULL，则以服务配置为准
   * 更多信息请参考鸿蒙通知消息分类标准文档：https://developer.huawei.com/consumer/cn/doc/development/HMSCore-Guides/message-classification-0000001149358835
   */
  const char *category;
} RcimHarmonyConfig;

typedef struct RcimPushConfig {
  /**
   * 通知栏是否屏蔽通知标题（true: 不显示通知标题；false: 显示通知标题）
   * 默认情况下融云单聊消息通知标题为用户名、群聊消息为群名称，设置后不会再显示通知标题。
   * 此属性只针目标用户为 iOS 平台时有效，Android 第三方推送平台的通知标题为必填项，所以暂不支持
   */
  bool disable_push_title;
  /**
   * 默认标题显示规则：内置消息：单聊通知标题显示为发送者名称，群聊通知标题显示为群名称。自定义消息：默认不显示标题。
   */
  const char *push_title;
  /**
   * 推送内容。
   */
  const char *push_content;
  /**
   * 远程推送附加信息。
   */
  const char *push_data;
  /**
   * 是否强制显示通知详情。推送越权设置，当目标用户设置推送不显示消息详情时，可通过此功能，强制设置该条消息显示推送详情。
   */
  bool force_show_detail_content;
  /**
   * iOS config
   *
   * <br>内部成员属性，传入空指针表示无效值，空串视为有效值；<br/>
   * 空串会被 encode 并传递给服务端<br/>且空串在业务上目前是无意义的，但以后有变为有意义的可能性<br/>
   * 比如：表示一些默认场景值，所以平台侧要保证不随意传入空串
   *
   */
  const struct RcimIosConfig *ios_config;
  /**
   * Android config
   *
   * <br>内部成员属性，传入空指针表示无效值，空串视为有效值；<br/>
   * 空串会被 encode 并传递给服务端<br/>且空串在业务上目前是无意义的，但以后有变为有意义的可能性<br/>
   * 比如：表示一些默认场景值，所以平台侧要保证不随意传入空串
   *
   */
  const struct RcimAndroidConfig *android_config;
  /**
   * Harmony config
   *
   * <br>内部成员属性，传入空指针表示无效值，空串视为有效值；<br/>
   * 空串会被 encode 并传递给服务端<br/>且空串在业务上目前是无意义的，但以后有变为有意义的可能性<br/>
   * 比如：表示一些默认场景值，所以平台侧要保证不随意传入空串
   *
   */
  const struct RcimHarmonyConfig *harmony_config;
} RcimPushConfig;

typedef struct RcimMessageBox {
  /**
   * 会话类型
   */
  RcimConversationType conv_type;
  /**
   * 目标 ID
   */
  const char *target_id;
  /**
   * 所属会话的业务标识，长度限制 20 字符
   */
  const char *channel_id;
  /**
   * 数据库消息表递增 ID
   */
  int64_t message_id;
  /**
   * 消息方向
   */
  RcimMessageDirection direction;
  /**
   * 发送者 ID
   */
  const char *sender_id;
  const struct RcimReceivedStatus *received_status;
  RcimSentStatus sent_status;
  int64_t received_time;
  int64_t sent_time;
  /**
   * 消息的类型名
   */
  const char *object_name;
  /**
   * 消息内容 内置消息大都是 json 字符串，自定义消息可能是其他格式
   */
  const char *content;
  /**
   * 可被搜索的内容，只做为入参，不做出参，返回的消息该字段为空
   */
  const char *searchable_words;
  /**
   * 服务端生产的消息唯一 ID（在同一个 AppKey 下全局唯一）
   */
  const char *uid;
  /**
   * 消息的附加字段
   * Android: extra
   * iOS: extra
   * C++: extra_content_
   */
  const char *extra;
  /**
   * 群阅读回执信息
   */
  const struct RcimReadReceiptInfo *read_receipt_info;
  /**
   * 群阅读回执信息 V2
   */
  const struct RcimReadReceiptInfoV2 *read_receipt_info_v2;
  /**
   * 是否在收到消息的时候发送通知
   * Android: messageConfig
   * iOS: messageConfig
   * C++: disable_notification_
   */
  bool is_notification_disabled;
  /**
   * 消息推送配置
   * Android: messagePushConfig
   * iOS: messagePushConfig
   *
   * 内部成员属性，传入空指针表示无效值，空串视为有效值；
   * 空串会被 encode 并传递给服务端<br/>且空串在业务上目前是无意义的，但以后有变为有意义的可能性
   * 比如：表示一些默认场景值，所以平台侧要保证不随意传入空串
   *
   */
  const struct RcimPushConfig *push_config;
  /**
   * 是否是离线消息，只在接收消息的回调方法中有效，如果消息为离线消息，则为 true，其他情况均为 false
   * Android: isOffLine
   * iOS: isOffLine
   */
  bool is_offline;
  /**
   * 消息是否可以包含扩展信息
   * 该属性在消息发送时确定，发送之后不能再做修改
   * 扩展信息只支持单聊、群聊、超级群，其它会话类型不能设置扩展信息
   *
   * Android: extSupport
   * iOS: canIncludeExpansion
   * C++: ext_support_
   */
  bool is_ext_supported;
  /**
   * 必须是 json 字符串，形如 "{"key1":"value1","key2":"value2"}"
   * 否则会被返回 Json 解析错误
   *
   * Android: extMsg
   * iOS: expansionDicEx
   * C++: ext_msg_
   */
  const char *ext_content;
  /**
   * 标识是否是一条已读回执 (V5) 消息
   */
  bool is_need_receipt;
  /**
   * 消息接收方使用，标识是否发送过已读回执 (V5)
   */
  bool has_sent_receipt;
} RcimMessageBox;

