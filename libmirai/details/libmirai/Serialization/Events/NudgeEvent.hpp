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

#ifndef _MIRAI_SERIALIZATION_NUDGE_EVENT_HPP_
#define _MIRAI_SERIALIZATION_NUDGE_EVENT_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/NudgeEvent.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct NudgeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, NudgeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == NudgeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("fromId").get_to(p._FromId);
		j.at("action").get_to(p._action);
		j.at("suffix").get_to(p._suffix);

		p._target = NudgeTarget{};
		p._target.SetTarget(j.at("target").get<QQ_t>());

		const auto& subject = j.at("subject");
		p._target.SetSubject(subject.at("id").get<int64_t>());
		p._target.SetNudgeType(subject.at("kind").get<NudgeType>());

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif