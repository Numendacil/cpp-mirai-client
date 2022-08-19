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

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void GroupConfig::FromJson(const json& data)
{
	this->name = Utils::GetValue(data, "name", "");
	this->AllowConfessTalk = Utils::GetValue(data, "confessTalk", false);
	this->AllowMemberInvite = Utils::GetValue(data, "allowMemberInvite", false);
	this->AutoApprove = Utils::GetValue(data, "autoApprove", false);
	this->AllowAnonymousChat = Utils::GetValue(data, "anonymousChat", false);
}

void from_json(const json& j, GroupConfig& p)
{
	p.FromJson(j);
}


void GroupAnnouncement::FromJson(const json& data)
{
	this->group = Utils::GetValue(data, "group", Group{});
	this->content = Utils::GetValue(data, "content", "");
	this->SenderId = Utils::GetValue(data, "senderId", QQ_t{});
	this->fid = Utils::GetValue(data, "fid", "");
	this->AllConfirmed = Utils::GetValue(data, "allConfirmed", false);
	this->ConfirmedCount = Utils::GetValue(data, "confirmedMembersCount", 0);
	this->PublicationTime = Utils::GetValue(data, "publicationTime", 0);
}

void from_json(const json& j, GroupAnnouncement& p)
{
	p.FromJson(j);
}


} // namespace Mirai