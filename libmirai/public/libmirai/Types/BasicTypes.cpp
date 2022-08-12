#include <cstdint>
#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

#include "BasicTypes.hpp"

namespace Mirai
{

using json = nlohmann::json;

void to_json(json& j, const UID_t& p)
{
	j = int64_t(p);
}

void from_json(const json& j, UID_t& p)
{
	p = j.get<int64_t>();
}

void to_json(nlohmann::json& j, const SEX& p)
{
	j = SEX::_to_string(p);
}
void from_json(const nlohmann::json& j, SEX& p)
{
	p = SEX::_to_enum(j.get<std::string>());
}

void to_json(nlohmann::json& j, const PERMISSION& p)
{
	j = PERMISSION::_to_string(p);
}
void from_json(const nlohmann::json& j, PERMISSION& p)
{
	p = PERMISSION::_to_enum(j.get<std::string>());
}


void User::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "id", QQ_t{});
	this->nickname = Utils::GetValue(data, "nickname", "");
	this->remark = Utils::GetValue(data, "remark", "");
}

json User::ToJson() const
{
	json data = json::object();
	data["id"] = id;
	data["nickname"] = this->nickname;
	data["remark"] = this->remark;
	return data;
}


void Group::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "id", GID_t{});
	this->name = Utils::GetValue(data, "name", "");
	this->permission = Utils::GetValue(data, "permission", PERMISSION::UNKNOWN);
}

json Group::ToJson() const
{
	json data = json::object();
	data["id"] = id;
	data["name"] = this->name;
	data["permission"] = this->permission;
	return data;
}


void GroupMember::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "id", QQ_t{});
	this->MemberName = Utils::GetValue(data, "memberName", "");
	this->permission = Utils::GetValue(data, "permission", PERMISSION::UNKNOWN);
	this->SpecialTitle = Utils::GetValue(data, "specialTitle", "");
	this->JoinTimestamp = Utils::GetValue(data, "joinTimestamp", (std::time_t)0);
	this->LastSpeakTimestamp = Utils::GetValue(data, "lastSpeakTimestamp", (std::time_t)0);
	this->MuteTimeRemaining = Utils::GetValue(data, "muteTimeRemaining", (std::time_t)0);
	this->group = Utils::GetValue(data, "group", Group{});
}

json GroupMember::ToJson() const
{
	json data = json::object();
	data["id"] = id;
	data["memberName"] = this->MemberName;
	data["permission"] = this->permission;
	data["specialTitle"] = this->SpecialTitle;
	data["joinTimestamp"] = this->JoinTimestamp;
	data["lastSpeakTimestamp"] = this->LastSpeakTimestamp;
	data["muteTimeRemaining"] = this->MuteTimeRemaining;
	data["group"] = this->group.ToJson();
	return data;
}


void UserProfile::FromJson(const json& data)
{
	this->nickname = Utils::GetValue(data, "nickname", "");
	this->email = Utils::GetValue(data, "email", "");
	this->age = Utils::GetValue(data, "age", 0);
	this->level = Utils::GetValue(data, "level", 0);
	this->sign = Utils::GetValue(data, "sign", "");
	this->sex = Utils::GetValue(data, "sex", SEX::UNKNOWN);
}

json UserProfile::ToJson() const
{
	json data = json::object();
	data["nickname"] = this->nickname;
	data["email"] = this->email;
	data["age"] = this->age;
	data["level"] = this->level;
	data["sign"] = this->sign;
	data["sex"] = this->sex;
	return data;
}


void ClientDevice::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "id", 0);
	this->platform = Utils::GetValue(data, "platform", "");
}

json ClientDevice::ToJson() const
{
	json data = json::object();
	data["id"] = this->id;
	data["platform"] = this->platform;
	return data;
}

}