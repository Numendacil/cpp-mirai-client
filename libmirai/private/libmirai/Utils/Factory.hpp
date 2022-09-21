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

#ifndef _MIRAI_UTILS_FACTORY_HPP_
#define _MIRAI_UTILS_FACTORY_HPP_

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace Mirai::Utils
{

template<class Base> class Factory
{
public:
	std::unique_ptr<Base> Make(const std::string& s) const { return (this->factory_map.at(s))(); }

	bool Exist(const std::string& s) const { return this->factory_map.count(s); }

	std::vector<std::string> GetKeyList() const
	{
		std::vector<std::string> v;
		v.reserve(this->factory_map.size());
		for (const auto& p : this->factory_map)
			v.push_back(p.first);
		return v;
	}

	template<class T> bool Register(const std::string& s)
	{
		static_assert(std::is_base_of<Base, T>::value, "T must be a derived class of Base");
		if (this->Exist(s)) return false;
		this->factory_map[s] = []() -> std::unique_ptr<Base> { return std::make_unique<T>(); };
		return true;
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Base> (*)()> factory_map;
};

} // namespace Mirai::Utils

#endif