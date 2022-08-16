#ifndef _MIRAI_TYPES_GROUP_SETTINGS_HPP_
#define _MIRAI_TYPES_GROUP_SETTINGS_HPP_
#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


namespace Mirai 
{

/**
 * @brief 群设置
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupConfig::name`		| `""`
 * `GroupConfig::AllowConfessTalk`		| `false`
 * `GroupConfig::AllowMemberInvite` | `false`
 * `GroupConfig::AutoApprove` | `false`
 * `GroupConfig::AllowAnonymousChat` | `false`
 */
struct GroupConfig
{
	/// 群名
	std::string name;
	/// 是否允许坦白说
	bool AllowConfessTalk = false;
	/// 是否允许邀请入群
	bool AllowMemberInvite = false;
	/// 是否自动审批入群申请
	bool AutoApprove = false;
	/// 是否允许匿名聊天
	bool AllowAnonymousChat = false;

	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupConfig&);

/**
 * @brief 群公告
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAnnouncement::group`		| `Group{}`
 * `GroupAnnouncement::content`		| `""`
 * `GroupAnnouncement::SenderId` | `0_qq`
 * `GroupAnnouncement::fid` | `""`
 * `GroupAnnouncement::AllConfirmed` | `false`
 * `GroupAnnouncement::ConfirmedCount` | `0`
 * `GroupAnnouncement::PublicationTime` | `0`
 */
struct GroupAnnouncement
{
	/// 群聊
	Group group;
	/// 公告内容
	std::string content;
	/// 公告发送着QQ
	QQ_t SenderId;
	/// 公告id，唯一标识符
	std::string fid;
	/// 是否全员已确认
	bool AllConfirmed = false;
	/// 公告确认人数
	int ConfirmedCount = 0;
	/// 发布时间
	std::time_t PublicationTime = 0;
	
	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupAnnouncement&);

}

#endif