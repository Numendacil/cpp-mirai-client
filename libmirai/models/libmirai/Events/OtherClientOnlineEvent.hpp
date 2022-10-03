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

#ifndef _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 其它客户端上线事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientOnlineEvent::_client` | `ClientDevice{}`
 * `OtherClientOnlineEvent::_kind` | `std::nullopt`
 */
class OtherClientOnlineEvent : public EventBase
{

protected:
	ClientDevice _client;
	std::optional<int64_t> _kind = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "OtherClientOnlineEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// OtherClientOnlineEvent* Clone() const override
	// {
	//	return new OtherClientOnlineEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取设备信息
	ClientDevice GetClient() const { return this->_client; }
	/// 获取详细设备类型，无则返回 `std::nullopt`
	std::optional<int64_t> GetKind() const { return this->_kind; }

	// TODO: replace kind with enum, see https://github.com/mamoe/mirai/blob/dev/mirai-core-api/src/commonMain/kotlin/contact/OtherClient.kt#L113
};

} // namespace Mirai


#endif