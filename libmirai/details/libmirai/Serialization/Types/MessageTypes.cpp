#include "MessageTypes.hpp"

#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;

namespace
{

constexpr std::array<std::string_view, static_cast<std::size_t>(MessageTypes::ENUM_END)> names = 
{
	"App",
	"AtAll",
	"At",
	"Voice",
	"Dice",
	"Face",
	"File",
	"FlashImage",
	"Forward",
	"Image",
	"Json",
	"MarketFace",
	"MiraiCode",
	"MusicShare",
	"Plain",
	"Poke",
	"Quote",
	"Source",
	"Xml",
};

constexpr std::string_view enum_to_str(const MessageTypes& m)
{
	auto i = static_cast<std::size_t>(m);
	if (i < names.size()) return names.at(i);
	else
		return "";
}

constexpr MessageTypes str_to_enum(std::string_view s)
{
	for (std::size_t i = 0; i < names.size(); i++)
		if (names.at(i) == s) return static_cast<MessageTypes>(i);

	return MessageTypes::ENUM_END;
}

} // namespace

void from_json(const json& j, MessageTypes& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	p = str_to_enum(j.get<std::string>());

	MIRAI_PARSE_GUARD_END;
}
void to_json(json& j, const MessageTypes& p)
{
	j = enum_to_str(p);
}

std::string to_string(MessageTypes type)
{
	return std::string{enum_to_str(type)};
}

}