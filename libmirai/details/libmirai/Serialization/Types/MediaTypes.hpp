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

#ifndef _MIRAI_SERIALIZATION_TYPES_MEDIA_TYPES_HPP_
#define _MIRAI_SERIALIZATION_TYPES_MEDIA_TYPES_HPP_

#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <libmirai/Types/MediaTypes.hpp>

#include <libmirai/Serialization/Types/Serializable.hpp>


namespace Mirai
{

MIRAI_DECLARE_FROM_TO_JSON(FileInfo);
MIRAI_DECLARE_FROM_TO_JSON(GroupFileInfo);
MIRAI_DECLARE_FROM_TO_JSON(MiraiImage);
MIRAI_DECLARE_FROM_TO_JSON(MiraiAudio);

} // namespace Mirai

#endif