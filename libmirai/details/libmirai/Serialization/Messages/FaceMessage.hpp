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

#ifndef _MIRAI_SERIALIZATION_FACE_MESSAGE_HPP_
#define _MIRAI_SERIALIZATION_FACE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/FaceMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

template <class MessageImpl>
struct FaceMessageImpl<MessageImpl>::Serializable
{

	static void from_json(const nlohmann::json& j, FaceMessageImpl<MessageImpl>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MessageImpl::GetType()); // NOLINT(*-array-to-pointer-decay)

		p._id = Utils::GetValue(j, "faceId", (int64_t)-1);
		p._name = Utils::GetValue(j, "name", "");

		MIRAI_PARSE_GUARD_END(j);
	}

	// https://stackoverflow.com/questions/58812977/enable-if-to-disable-static-member-function-declaration-of-templated-class
	template <typename T = MessageImpl>
	static auto to_json(nlohmann::json& j, const FaceMessageImpl<MessageImpl>& p)
	-> std::enable_if_t<T::isSendSupported()>
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MessageImpl::GetType();
		if (p._id != -1) 
			j["faceId"] = p._id;
		else
			j["name"] = p._name;
	}

};

template <>
struct FaceMessageImpl<MarketFaceMessage>::Serializable
{
	static void from_json(const nlohmann::json& j, FaceMessageImpl<MarketFaceMessage>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MarketFaceMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		p._id = Utils::GetValue(j, "id", (int64_t)-1);
		p._name = Utils::GetValue(j, "name", "");

		MIRAI_PARSE_GUARD_END(j);
	}
};


} // namespace Mirai

#endif