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

#ifndef MIRAI_TYPES_NUDGE_TARGET_HPP_
#define MIRAI_TYPES_NUDGE_TARGET_HPP_

#include <array>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

/**
 * @brief QQ移动端头像戳一戳动作的对象
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NudgeTarget::kind_` | `NudgeType::ENUM_END`
 * `NudgeTarget::target_` | `0_qq`
 * `NudgeTarget::subject_` | `0`
 */
class NudgeTarget
{
protected:
	NudgeType kind_ = NudgeType::ENUM_END;
	QQ_t target_{};
	int64_t subject_ = 0;

public:
	/**
	 * @brief 构造函数
	 * 
	 */
	NudgeTarget() = default;

	/**
	 * @brief 构造函数
	 * 
	 * @param kind 戳一戳的类型
	 * @param target 戳一戳的对象
	 * @param subject 戳一戳的发送主体，群聊消息时为群号，私聊时为对方QQ号码
	 */
	NudgeTarget(NudgeType kind, QQ_t target, UID_t subject) : kind_(kind), target_(target), subject_((int64_t)subject)
	{
	}

	/**
	 * @brief 返回戳一戳类型
	 * 
	 * @return 戳一戳类型 `enum`
	 */
	NudgeType GetNudgeType() const { return this->kind_; }

	/**
	 * @brief 返回戳一戳的对象
	 * 
	 * @return 戳一戳对象的QQ号
	 */
	QQ_t GetTarget() const { return this->target_; }

	/**
	 * @brief 返回戳一戳所在的群聊
	 * 
	 * 仅在类型为 `NudgeType::GROUP` 时有效，否则返回 `0`
	 * @return 戳一戳的群号
	 */
	GID_t GetGroup() const { return (this->kind_ == NudgeType::GROUP) ? (GID_t)this->subject_ : (GID_t)0; }

	/**
	 * @brief 设置戳一戳类型
	 * 
	 * @param kind 戳一戳类型 `enum`
	 * @return Reference to *this
	 */
	NudgeTarget& SetNudgeType(NudgeType kind)
	{
		this->kind_ = kind;
		return *this;
	}

	/**
	 * @brief 设置戳一戳对象
	 * 
	 * @param target 戳一戳对象QQ
	 * @return Reference to *this
	 */
	NudgeTarget& SetTarget(QQ_t target)
	{
		this->target_ = target;
		return *this;
	}

	/**
	 * @brief 设置戳一戳的发送主体
	 * 
	 * 不推荐，考虑使用 `NudgeFriend()`, `NudgeGroupMember()` 与 `NudgeStranger()`
	 * @param subject 戳一戳的发送主体
	 * @return Reference to *this
	 */
	NudgeTarget& SetSubject(int64_t subject)
	{
		this->subject_ = subject;
		return *this;
	}


	/**
	 * @brief 设置为好友戳一戳消息
	 * 
	 * @param target 好友QQ 
	 * @return Reference to *this
	 */
	NudgeTarget& NudgeFriend(QQ_t target)
	{
		this->kind_ = NudgeType::FRIEND;
		this->target_ = target;
		return *this;
	}

	/**
	 * @brief 设置为群聊戳一戳消息
	 * 
	 * @param target 群友QQ
	 * @param group 群聊号码
	 * @return Reference to *this
	 */
	NudgeTarget& NudgeGroupMember(QQ_t target, GID_t group)
	{
		this->kind_ = NudgeType::GROUP;
		this->target_ = target;
		this->subject_ = (int64_t)group;
		return *this;
	}

	/**
	 * @brief 设置为单向好友戳一戳消息
	 * 
	 * @param target 陌生人QQ
	 * @return Reference to *this
	 */
	NudgeTarget& NudgeStranger(QQ_t target)
	{
		this->kind_ = NudgeType::STRANGER;
		this->target_ = target;
		return *this;
	}
};

} // namespace Mirai

#endif