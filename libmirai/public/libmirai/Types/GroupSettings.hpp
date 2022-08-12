#ifndef _MIRAI_TYPES_GROUP_SETTINGS_HPP_
#define _MIRAI_TYPES_GROUP_SETTINGS_HPP_
#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


namespace Mirai 
{

struct GroupConfig
{
	std::string name;
	bool AllowConfessTalk = false;
	bool AllowMemberInvite = false;
	bool AutoApprove = false;
	bool AllowAnonymousChat = false;

	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupConfig&);

struct GroupAnnouncement
{
	Group group;
	std::string content;
	QQ_t SenderId;
	std::string fid;
	bool AllConfirmed = false;
	int ConfirmedCount = 0;
	std::time_t PublicationTime = 0;
	
	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupAnnouncement&);

}

#endif