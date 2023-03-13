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

#ifndef MIRAI_SERIALIZATION_IMAGE_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_IMAGE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/ImageMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

template <class  MessageImpl>
struct ImageMessageImpl<MessageImpl>::Serializable
{

	static void from_json(const nlohmann::json& j, ImageMessageImpl<MessageImpl>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MessageImpl::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.get_to(p.image_);

		MIRAI_PARSE_GUARD_END(j);
	}

	// https://stackoverflow.com/questions/58812977/enable-if-to-disable-static-member-function-declaration-of-templated-class
	template <typename T = MessageImpl>
	static auto to_json(nlohmann::json& j, const ImageMessageImpl<MessageImpl>& p)
	-> std::enable_if_t<T::isSendSupported()>
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MessageImpl::GetType();
		j.update(p.image_);
	}

};

} // namespace Mirai

#endif