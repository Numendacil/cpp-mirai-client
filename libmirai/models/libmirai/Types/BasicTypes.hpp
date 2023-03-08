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

#ifndef MIRAI_TYPES_BASIC_TYPES_HPP_
#define MIRAI_TYPES_BASIC_TYPES_HPP_

#include <chrono>
#include <ctime>
#include <string>
#include <utility>

#ifdef IGNORE // Oh well, Windows.h again
#undef IGNORE
#endif

namespace Mirai
{

/**
 * @brief 消息id类型，用于撤回消息和引用消息
 * 
 */
using MessageId_t = int64_t;

/**
 * @brief Base class for QQ_t and GID_t
 * 
 * 不允许实例化，仅作为基类提供基本功能
 */
class UID_t
{
protected:
	int64_t number_ = 0;

	UID_t() = default;
	explicit UID_t(int64_t num) : number_{num} {}

public:
	explicit operator int64_t() const { return this->number_; }
	std::string to_string() const { return std::to_string(this->number_); }

	bool operator<(const UID_t& uid) const { return this->number_ < uid.number_; }
	bool operator==(const UID_t& uid) const { return this->number_ == uid.number_; }

	bool operator!=(const UID_t& uid) const { return !(*this == uid); }
	bool operator>(const UID_t& uid) const { return uid < *this; }
	bool operator>=(const UID_t& uid) const { return !(*this < uid); }
	bool operator<=(const UID_t& uid) const { return !(uid < *this); }

	struct Serializable;
};

/**
 * @brief QQ号码类型
 * 
 * 本质上为 `int64_t` ，但禁止隐式转换
 */
class QQ_t : public UID_t
{
public:
	QQ_t() : UID_t() {}
	explicit QQ_t(int64_t num) : UID_t(num) {}
};

inline QQ_t operator""_qq(unsigned long long num)
{
	return QQ_t(static_cast<int64_t>(num));
}

/**
 * @brief 群聊号码类型
 * 
 * 本质上为 `int64_t` ，但禁止隐式转换
 */
class GID_t : public UID_t
{
public:
	GID_t() : UID_t() {}
	explicit GID_t(int64_t num) : UID_t(num) {}
};

inline GID_t operator""_gid(unsigned long long num)
{
	return GID_t(static_cast<int64_t>(num));
}


// *********************************************
// ************ ENUM DECLARATIONS **************
// *********************************************


/**
 * @brief 处理好友申请的操作
 * 
 * Value	| Operation
 * ---------|----------
 * `ACCEPT` | 同意好友申请
 * `REFUSE` | 拒绝好友申请
 * `BLACKLIST` | 拒绝并拉黑，不再接收此人的申请
 */
enum class NewFriendRequestOp
{
	ACCEPT = 0,
	REFUSE,
	BLACKLIST
};

/**
 * @brief 处理用户入群申请的操作
 * 
 * Value	| Operation
 * ---------|----------
 * `ACCEPT` | 同意入群申请
 * `REFUSE` | 拒绝入群申请
 * `IGNORE` | 忽略入群申请
 * `REFUSE_BLACKLIST` | 拒绝并拉黑，不再接收此人的申请
 * `IGNORE_BLACKLIST` | 忽略并拉黑，不再接收此人的申请
 */
enum class MemberJoinRequestOp
{
	ACCEPT = 0,
	REFUSE,
	IGNORE,
	REFUSE_BLACKLIST,
	IGNORE_BLACKLIST
};

/**
 * @brief 处理被邀请入群的操作
 * 
 * Value	| Operation
 * ---------|----------
 * `ACCEPT` | 同意邀请
 * `REFUSE` | 拒绝拒绝邀请
 */
enum class BotInvitedJoinGroupRequestOp
{
	ACCEPT = 0,
	REFUSE
};

/**
 * @brief QQ用户性别
 * 
 *  Value	| Operation
 * ---------|----------
 * `MALE` | 男性
 * `FEMALE` | 女性
 * `UNKNOWN` | 未知性别
 */
enum class SEX
{
	MALE = 0,
	FEMALE,
	UNKNOWN,

	// Used for counting
	ENUM_END
};

/**
 * @brief 群成员权限
 * 
 * `ENUM_END` 为保留字段，使用时出现说明数据不合法
 *   Value	| Operation
 * ---------|----------
 * `OWNER` | 群主
 * `ADMINISTRATOR` | 管理员
 * `MEMBER` | 普通成员
 */
enum class PERMISSION
{
	OWNER = 0,
	ADMINISTRATOR,
	MEMBER,

	// Used for counting
	ENUM_END
};

/**
 * @brief 头像戳一戳消息的发送环境
 * 
 * `ENUM_END` 为保留字段，使用时出现说明数据不合法
 *   Value	| Operation
 * ---------|----------
 * `FRIEND` | 好友发送
 * `GROUP` | 群聊发送
 * `STRANGER` | 陌生人发送
 */
enum class NudgeType
{
	FRIEND = 0,
	GROUP,
	STRANGER,

	// Used for counting
	ENUM_END
};

/**
 * @brief 音乐卡片类型
 * 
 * `ENUM_END` 为保留字段，使用时出现说明数据不合法
 */
enum class MusicShareType
{
	NETEASECLOUDMUSIC = 0,
	QQMUSIC,
	MIGUMUSIC,
	KUGOUMUSIC,
	KUWOMUSIC,

	// Used for counting
	ENUM_END
};

/**
 * @brief 戳一戳（原窗口抖动）类型
 * 
 * `ENUM_END` 为保留字段，使用时出现说明数据不合法
 */
enum class PokeType
{
	CHUOYICHUO = 0, // 戳一戳
	BIXIN,          // 比心
	DIANZAN,        // 点赞
	XINSUI,         // 心碎
	LIULIULIU,      // 666
	FANGDAZHAO,     // 放大招
	GOUYIN,         // 勾引
	BAOBEIQIU,      // 宝贝球 (SVIP)
	ROSE,           // 玫瑰花 (SVIP)
	ZHAOHUANSHU,    // 召唤术 (SVIP)
	RANGNIPI,       // 让你皮 (SVIP)
	JIEYIN,         // 结印 (SVIP)
	SHOULEI,        // 手雷 (SVIP)
	ZHUAYIXIA,      // 抓一下 (SVIP)
	SUIPING,        // 碎屏 (SVIP)
	QIAOMEN,        // 敲门 (SVIP)

	// Used for counting
	ENUM_END
};

/**
 * @brief 群称号改变类型
 * 
 * `ENUM_END` 为保留字段，使用时出现说明数据不合法
 *   Value	| Operation
 * ---------|----------
 * `ACHIEVE` | 获得称号
 * `LOSE` | 失去称号
 */
enum class HonorChangeType
{
	ACHIEVE = 0,
	LOSE,

	// Used for counting
	ENUM_END
};

/**
 * @brief 指令发送者类型
 * 
 *   Value	| Operation
 * ---------|----------
 * `FRIEND` | 好友
 * `MEMBER` | 群聊
 * `CONSOLE` | 控制台 (MCL)
 */
enum class CommandSender
{
	FRIEND = 0,
	MEMBER,
	CONSOLE
};


// *********************************************
// ********** ENUM DECLARATIONS END ************
// *********************************************


/**
 * @brief QQ用户
 * 
 * 最基础的用户资料，需要更详细的内容请使用 `UserProfile` 类
 */
struct User
{
	/// 用户QQ号
	QQ_t id;
	/// 用户昵称
	std::string nickname;
	/// 用户备注（仅对好友有效）
	std::string remark;

	User(QQ_t id = 0_qq, std::string nickname = "", std::string remark = "")
		: id(id), nickname(std::move(nickname)), remark(std::move(remark))
	{
	}

	/**
	 * @brief 判定两用户是否为同一人
	 * 
	 * 仅判断QQ号
	 */
	bool operator==(const User& rhs) const { return this->id == rhs.id; }

	struct Serializable;
};

/**
 * @brief 群聊资料
 * 
 */
struct Group
{
	/// 群聊号码
	GID_t id;
	/// 群聊名称
	std::string name;
	/// Bot在群聊中的权限
	PERMISSION permission = PERMISSION::ENUM_END;

	Group(GID_t id = 0_gid, std::string name = "", PERMISSION permission = PERMISSION::ENUM_END)
		: id(id), name(std::move(name)), permission(permission)
	{
	}

	/**
	 * @brief 判定两群聊是否为同一群
	 * 
	 * 仅判断群号
	 */
	bool operator==(const Group& rhs) const { return this->id == rhs.id; }

	struct Serializable;
};

/**
 * @brief 群员资料
 * 
 * 仅含有与群聊相关的部分，个人资料请使用 `User` 与 `UserProfile` 类
 */
struct GroupMember
{
	/// 群员id
	QQ_t id;
	/// 群员名片
	std::string MemberName;
	/// 群员权限
	PERMISSION permission = PERMISSION::ENUM_END;
	/// 群头衔
	std::string SpecialTitle;
	/// 入群时间
	std::time_t JoinTimestamp = 0;
	/// 最后一次发言时间
	std::time_t LastSpeakTimestamp = 0;
	/// 剩余禁言时间
	std::chrono::seconds MuteTimeRemaining = std::chrono::seconds(0);
	/// 群聊资料
	Group group;

	GroupMember(QQ_t id = 0_qq, std::string MemberName = "", PERMISSION permission = PERMISSION::ENUM_END,
	            std::string SpecialTitle = "", std::time_t JoinTimestamp = 0, std::time_t LastSpeakTimestamp = 0,
	            std::chrono::seconds MuteTimeRemaining = std::chrono::seconds(0), Group group = {})
		: id(id)
		, MemberName(std::move(MemberName))
		, permission(permission)
		, SpecialTitle(std::move(SpecialTitle))
		, JoinTimestamp(JoinTimestamp)
		, LastSpeakTimestamp(LastSpeakTimestamp)
		, MuteTimeRemaining(MuteTimeRemaining)
		, group(std::move(group))
	{
	}

	/**
	 * @brief 判定两群员是否为同一人
	 * 
	 * 仅判断群号与QQ号
	 */
	bool operator==(const GroupMember& rhs) const { return this->id == rhs.id && this->group == rhs.group; }

	struct Serializable;
};

/**
 * @brief QQ用户资料
 * 
 */
struct UserProfile
{
	/// QQ昵称
	std::string nickname;
	/// QQ邮箱
	std::string email;
	/// 年龄
	int age = 0;
	/// QQ等级
	int level = 0;
	/// QQ签名
	std::string sign;
	/// 性别
	SEX sex = SEX::ENUM_END;

	UserProfile(std::string nickname = "", std::string email = "", int age = 0, int level = 0, std::string sign = "",
	            SEX sex = SEX::ENUM_END)
		: nickname(std::move(nickname))
		, email(std::move(email))
		, age(age)
		, level(level)
		, sign(std::move(sign))
		, sex(sex)
	{
	}

	/**
	 * @brief 比较用户资料是否相同
	 * 
	 * 等价与所有成员变量分别判断相等。由于用户资料可能巧合，因此不保证真的为同一人
	 */
	bool operator==(const UserProfile& rhs) const
	{
		return this->nickname == rhs.nickname && this->email == rhs.email && this->age == rhs.age
			&& this->level == rhs.level && this->sign == rhs.sign && this->sex == rhs.sex;
	}

	struct Serializable;
};

/**
 * @brief 设备类型
 * 
 */
struct ClientDevice
{
	/// 设备id，唯一标识符
	int64_t id = 0;
	/// 设备平台
	std::string platform; // TODO：replace with enum

	ClientDevice(int64_t id = 0, std::string platform = "") : id(id), platform(std::move(platform)) {}

	/**
	 * @brief 判断设备是否相同
	 * 
	 */
	bool operator==(const ClientDevice& rhs) const { return this->id == rhs.id && this->platform == rhs.platform; }

	struct Serializable;
};

} // namespace Mirai

namespace std
{

template<> struct hash<Mirai::GID_t>
{
	std::size_t operator()(const Mirai::GID_t& t) const { return hash<int64_t>{}((int64_t)t); }
};

template<> struct hash<Mirai::QQ_t>
{
	std::size_t operator()(const Mirai::QQ_t& t) const { return hash<int64_t>{}((int64_t)t); }
};

} // namespace std


#endif