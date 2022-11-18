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

#ifndef _MIRAI_EVENT_BASE_HPP_
#define _MIRAI_EVENT_BASE_HPP_

#include <memory>
#include <string>

namespace Mirai
{

class MiraiClient;

/**
 * @brief Base class for all event types
 * 
 * 所有有效的派生类均需要定义 `_TYPE_`
 */
class EventBase
{
protected:
	/**
	 * @brief 指向 `MiraiClient` 的指针
	 * 
	 * 对应于接收该事件的 `MiraiClient` ，方便对事件作出响应
	 */
	MiraiClient* _client = nullptr;

	friend class MiraiClient;

	/// Sets the `MiraiClient` pointer
	EventBase(MiraiClient* client) : _client(client) {}

	virtual void Deserialize(const void*) = 0;

public:
	EventBase() = default;
	EventBase(const EventBase&) = delete;
	EventBase& operator=(const EventBase&) = delete;
	EventBase(EventBase&&) = default;
	EventBase& operator=(EventBase&&) = default;

	// static constexpr std::string_view _TYPE_ = "EventBase";

	virtual ~EventBase() = default;

	/// 获取接收该事件的 `MiraiClient` 的引用
	MiraiClient& GetMiraiClient() const { return *_client; }

	struct Serializable;;
};

} // namespace Mirai

#endif