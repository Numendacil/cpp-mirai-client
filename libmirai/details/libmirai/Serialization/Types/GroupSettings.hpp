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

#ifndef _MIRAI_SERIALIZABLE_TYPES_GROUP_SETTINGS_HPP_
#define _MIRAI_SERIALIZABLE_TYPES_GROUP_SETTINGS_HPP_
#include <ctime>
#include <string>

#include <libmirai/Types/GroupSettings.hpp>

#include <libmirai/Serialization/Types/Serializable.hpp>


namespace Mirai
{

MIRAI_DECLARE_FROM_TO_JSON(GroupConfig);
MIRAI_DECLARE_FROM_TO_JSON(GroupAnnouncement);

} // namespace Mirai

#endif