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

#ifndef MIRAI_SERIALIZATION_FORWARD_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_FORWARD_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/ForwardMessage.hpp>
#include <libmirai/Serialization/Messages/ForwardMessageNode.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include "libmirai/Utils/Common.hpp"

namespace Mirai
{

struct ForwardMessage::Serializable
{

	static void from_json(const nlohmann::json& j, ForwardMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == ForwardMessage::GetType()); // NOLINT(*-array-to-pointer-decay)
		assert(j.at("nodeList").is_array()); // NOLINT(*-array-to-pointer-decay)

		j.at("nodeList").get_to(p.NodeList_);

		if (!Utils::HasValue(j, "display"))
		{
			p.title_ = std::nullopt;
			p.brief_ = std::nullopt;
			p.source_ = std::nullopt;
			p.preview_ = std::nullopt;
			p.summary_ = std::nullopt;
		}
		else
		{
			const auto& display = j.at("display");
			Utils::GetOptional(display, "title", p.title_);
			Utils::GetOptional(display, "brief", p.brief_);
			Utils::GetOptional(display, "source", p.source_);
			Utils::GetOptional(display, "preview", p.preview_);
			Utils::GetOptional(display, "summary", p.summary_);
		}

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, ForwardMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == ForwardMessage::GetType()); // NOLINT(*-array-to-pointer-decay)
		assert(j.at("nodeList").is_array()); // NOLINT(*-array-to-pointer-decay)

		p.NodeList_.resize(j.at("nodeList").size());
		auto it1 = p.NodeList_.begin();
		for (auto it2 = j.at("nodeList").begin(); it2 != j.at("nodeList").end(); it1++, it2++)
		{
			::Mirai::from_json(std::move(*it2), *it1);
		}

		if (!Utils::HasValue(j, "display"))
		{
			p.title_ = std::nullopt;
			p.brief_ = std::nullopt;
			p.source_ = std::nullopt;
			p.preview_ = std::nullopt;
			p.summary_ = std::nullopt;
		}
		else
		{
			auto&& display = std::move(j.at("display"));
			Utils::GetOptional(std::move(display), "title", p.title_);
			Utils::GetOptional(std::move(display), "brief", p.brief_);
			Utils::GetOptional(std::move(display), "source", p.source_);
			{
				auto it = display.find("preview");
				if (it == display.end() || it.value().is_null())
					p.preview_ = std::nullopt;
				else
				{
					p.preview_ = decltype(p.preview_){};
					p.preview_->resize(display.at("preview").size());
					auto it1 = p.preview_->begin();
					for (auto it2 = display.at("preview").begin(); it2 != display.at("preview").end(); it1++, it2++)
					{
						::Mirai::from_json(std::move(*it2), *it1);
					}
				}
			}

			Utils::GetOptional(std::move(display), "summary", p.summary_);
		}

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const ForwardMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = ForwardMessage::GetType();
		j["nodeList"] = p.NodeList_;

		if (p.title_)
			j["display"]["title"] = *p.title_;
		if (p.brief_)
			j["display"]["brief"] = *p.brief_;
		if (p.source_)
			j["display"]["source"] = *p.source_;
		if (p.preview_)
			j["display"]["preview"] = *p.preview_;
		if (p.summary_)
			j["display"]["summary"] = *p.summary_;
	}

	static void to_json(nlohmann::json& j, ForwardMessage&& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = ForwardMessage::GetType();
		j["nodeList"] = std::move(p.NodeList_);

		if (p.title_)
			j["display"]["title"] = std::move(*p.title_);
		if (p.brief_)
			j["display"]["brief"] = std::move(*p.brief_);
		if (p.source_)
			j["display"]["source"] = std::move(*p.source_);
		if (p.preview_)
			j["display"]["preview"] = std::move(*p.preview_);
		if (p.summary_)
			j["display"]["summary"] = std::move(*p.summary_);
	}

};

} // namespace Mirai

#endif