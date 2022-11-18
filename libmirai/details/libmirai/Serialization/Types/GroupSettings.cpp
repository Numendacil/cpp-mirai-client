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

#include "GroupSettings.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Types/BasicTypes.hpp>

namespace Mirai
{

using json = nlohmann::json;

void from_json(const json& j, GroupConfig& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	j.at("name").get_to(p.name);
	j.at("confessTalk").get_to(p.AllowConfessTalk);
	j.at("allowMemberInvite").get_to(p.AllowMemberInvite);
	j.at("autoApprove").get_to(p.AutoApprove);
	j.at("anonymousChat").get_to(p.AllowAnonymousChat);

	MIRAI_PARSE_GUARD_END;
}

void from_json(const json& j, GroupAnnouncement& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	j.at("group").get_to(p.group);
	j.at("content").get_to(p.content);
	j.at("senderId").get_to(p.SenderId);
	j.at("fid").get_to(p.fid);
	j.at("allConfirmed").get_to(p.AllConfirmed);
	j.at("confirmedMembersCount").get_to(p.ConfirmedCount);
	j.at("publicationTime").get_to(p.PublicationTime);

	MIRAI_PARSE_GUARD_END;
}


} // namespace Mirai