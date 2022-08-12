#ifndef _MIRAI_UTILS_FACTORY_HPP_
#define _MIRAI_UTILS_FACTORY_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <cassert>

namespace Mirai::Utils
{

template <class Base>
class Factory
{
public:
	std::unique_ptr<Base> Make(const std::string& s) const
	{
		return (this->factory_map.at(s))();
	}

	bool Exist(const std::string& s) const
	{
		return this->factory_map.count(s);
	}

	std::vector<std::string> GetKeyList() const
	{
		std::vector<std::string> v;
		v.reserve(this->factory_map.size());
		for (const auto& p : this->factory_map)
			v.push_back(p.first);
		return v;
	}

	template <class T> 
	bool Register(const std::string& s)
	{
		static_assert(std::is_base_of<Base, T>::value, "T must be a derived class of Base");
		if (this->Exist(s))
			return false;
		this->factory_map[s] = []() -> std::unique_ptr<Base>
		{
			return std::make_unique<T>();
		};
		return true;
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Base>(*)()> factory_map;
};

}

#endif