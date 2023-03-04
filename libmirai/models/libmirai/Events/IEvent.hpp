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

#ifndef _MIRAI_EVENT_INTERFACE_HPP_
#define _MIRAI_EVENT_INTERFACE_HPP_

#include <memory>
#include <string>
#include <libmirai/Types/EventTypes.hpp>

namespace Mirai
{

class MiraiClient;

/**
 * @brief Common interface for all event types
 * 
 */
template <class Event>
class IEvent
{
private:
	friend class MiraiClient;
	
	/**
	 * @brief 指向 `MiraiClient` 的指针
	 * 
	 * 对应于接收该事件的 `MiraiClient` ，方便对事件作出响应
	 */
	MiraiClient* _client = nullptr;

	/// Sets the `MiraiClient` pointer
	void _SetClient(MiraiClient* client) { this->_client = client; }

	Event& _top() { return *static_cast<Event*>(this); }
	const Event& _top() const { return *static_cast<const Event*>(this); }

protected:
	IEvent() = default;
	~IEvent() = default;

public:
	IEvent(const IEvent&) = default;
	IEvent& operator=(const IEvent&) = default;
	IEvent(IEvent&&) = default;
	IEvent& operator=(IEvent&&) = default;

	/// 获取事件类型
	static constexpr EventTypes GetType() { return Event::_TYPE_; }

	/// 获取接收该事件的 `MiraiClient` 的引用
	MiraiClient& GetMiraiClient() const { return *_client; } 
};

} // namespace Mirai

#endif