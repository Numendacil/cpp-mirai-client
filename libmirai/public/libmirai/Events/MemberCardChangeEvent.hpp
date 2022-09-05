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

#ifndef _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员名片更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberCardChangeEvent::_member` | `GroupMember{}`
 * `MemberCardChangeEvent::_origin` | `""`
 * `MemberCardChangeEvent::_current` | `""`
 *
 * 由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播
 */
class MemberCardChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	std::string _origin;
	std::string _current;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberCardChangeEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// MemberCardChangeEvent* Clone() const override
	// {
	//	return new MemberCardChangeEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群名片
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群名片
	std::string GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif