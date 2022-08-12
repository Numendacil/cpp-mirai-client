#ifndef _MIRAI_MESSAGE_BASE_HPP_
#define _MIRAI_MESSAGE_BASE_HPP_


#include <string>
#include <memory>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/Serializable.hpp>

namespace Mirai
{

class MessageBase : public Serializable
{
public:

	// static constexpr std::string_view _TYPE_ = "MessageBase";

	virtual std::string_view GetType() const = 0;

	virtual MessageBase* Clone() const = 0;

	virtual std::shared_ptr<MessageBase> CloneShared() const
	{
		return std::shared_ptr<MessageBase>(this->Clone());
	}

	virtual std::unique_ptr<MessageBase> CloneUnique() const
	{
		return std::unique_ptr<MessageBase>(this->Clone());
	}

	virtual bool isValid() const = 0;

	virtual ~MessageBase() = default;
};

}


#endif