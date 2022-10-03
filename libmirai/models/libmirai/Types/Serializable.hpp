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

#ifndef _MIRAI_TYPES_SERIALIZABLE_HPP_
#define _MIRAI_TYPES_SERIALIZABLE_HPP_

#include <nlohmann/json_fwd.hpp>

namespace Mirai
{

/**
 * @brief Base class for all json serializable type
 * 
 */

class Serializable
{
public:
	virtual void FromJson(const nlohmann::json&) = 0;
	virtual nlohmann::json ToJson() const = 0;

	virtual ~Serializable() = default;
};

/// Custom serialization to nlohmann::json type for all `Serializable` derived class
void to_json(nlohmann::json& j, const Serializable& p);

/// Custom serialization from nlohmann::json type for all `Serializable` derived class
void from_json(const nlohmann::json& j, Serializable& p);

} // namespace Mirai

#endif