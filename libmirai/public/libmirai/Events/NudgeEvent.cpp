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

#include "NudgeEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void NudgeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	this->_FromId = Utils::GetValue(data, "fromId", QQ_t{});
	this->_action = Utils::GetValue(data, "action", "");
	this->_suffix = Utils::GetValue(data, "suffix", "");

	this->_target = NudgeTarget{};
	this->_target.SetTarget(Utils::GetValue(data, "target", QQ_t{}));
	json subject = Utils::GetValue(data, "subject", json{});
	if (!subject.empty() && subject.is_object())
	{
		this->_target.SetSubject(Utils::GetValue(subject, "id", (int64_t)0));
		this->_target.SetTargetKind(Utils::GetValue(subject, "kind", ""));
	}
}

} // namespace Mirai