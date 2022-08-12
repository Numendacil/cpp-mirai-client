#include <nlohmann/json.hpp>
#include <httplib.h>
#include <libmirai/Exceptions/Exceptions.hpp>

#include "Common.hpp"


namespace Mirai::Utils
{
using json = nlohmann::json;

json ParseResponse(const json& result)
{
	if (result.contains("code"))
	{
		int code = result.at("code").get<int>();
		if (code != 0)
		{
			std::string message;
			if (result.contains("msg"))
				message = result["msg"].get<std::string>();
			switch(code)
			{
			case 1:
				throw AuthKeyFail(message);
			case 2:
				throw NoBot(message);
			case 3:
				throw IllegalSession(message);
			case 4:
				throw NotVerifySession(message);
			case 5:
				throw NoElement(message);
			case 6:
				if (message == "指定操作不支持")
					throw NoOperateSupport(message);
				else
					throw NoSuchFile(message);
			case 10:
				throw PermissionDenied(message);
			case 20:
				throw BotMuted(message);
			case 30:
				throw MessageTooLarge(message);
			default:
				throw MiraiApiHttpException(code, message);
			}
		}
	}

	return result;
}

json ParseResponse(const std::string &result)
{
	json res;
	try
	{
		res = json::parse(result);
	}
	catch(const json::parse_error& e)
	{
		throw ParseError(e.what(), result);
	}
	return ParseResponse(res);
}

json ParseResponse(const httplib::Result &result)
{
	if (!result || result.error() != httplib::Error::Success)
		throw NetworkException(-1, httplib::to_string(result.error()));
	if (result->status < 200 || result->status > 299)
		throw NetworkException(result->status, result->body);

	return ParseResponse(result->body);
}

}