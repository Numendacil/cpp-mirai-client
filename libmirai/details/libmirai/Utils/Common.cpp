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

#include "Common.hpp"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>


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
			if (result.contains("msg")) message = result["msg"].get<std::string>();
			switch (code)
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
				if (message == "指定操作不支持") throw NoOperateSupport(message);
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

json ParseResponse(const std::string& result)
{
	json res;
	try
	{
		res = json::parse(result);
	}
	catch (const json::parse_error& e)
	{
		throw ParseError(e.what(), result);
	}
	return ParseResponse(res);
}

json ParseResponse(const httplib::Result& result)
{
	if (!result || result.error() != httplib::Error::Success)
		throw NetworkException(-1, httplib::to_string(result.error()));
	if (result->status < 200 || result->status > 299) throw NetworkException(result->status, result->body);

	return ParseResponse(result->body);
}

} // namespace Mirai::Utils