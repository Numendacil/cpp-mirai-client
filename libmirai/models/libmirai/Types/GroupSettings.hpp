// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MIRAI_TYPES_GROUP_SETTINGS_HPP_
#define MIRAI_TYPES_GROUP_SETTINGS_HPP_
#include <ctime>
#include <string>

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
	/// 是否开启全员禁言
	bool MuteAll = false;
};

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
};

} // namespace Mirai

#endif