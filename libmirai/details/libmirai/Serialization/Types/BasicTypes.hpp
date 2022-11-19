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

#ifndef _MIRAI_SERIALIZATION_TYPES_BASIC_TYPES_HPP_
#define _MIRAI_SERIALIZATION_TYPES_BASIC_TYPES_HPP_

#include <libmirai/Serialization/Types/Serializable.hpp>
#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

MIRAI_DECLARE_SERIALIZABLE_JSON(UID_t);
MIRAI_DECLARE_SERIALIZABLE_JSON(User);
MIRAI_DECLARE_SERIALIZABLE_JSON(Group);
MIRAI_DECLARE_SERIALIZABLE_JSON(GroupMember);
MIRAI_DECLARE_SERIALIZABLE_JSON(UserProfile);
MIRAI_DECLARE_SERIALIZABLE_JSON(ClientDevice);

MIRAI_DECLARE_FROM_TO_JSON(SEX);
MIRAI_DECLARE_FROM_TO_JSON(PERMISSION);
MIRAI_DECLARE_FROM_TO_JSON(NudgeType);
MIRAI_DECLARE_FROM_TO_JSON(MusicShareType);
MIRAI_DECLARE_FROM_TO_JSON(PokeType);
MIRAI_DECLARE_FROM_TO_JSON(HonorChangeType);

} // namespace Mirai

#endif