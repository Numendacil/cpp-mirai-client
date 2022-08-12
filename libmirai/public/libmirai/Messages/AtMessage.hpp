#ifndef _MIRAI_AT_MESSAGE_HPP_
#define _MIRAI_AT_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class AtMessage : public MessageBase
{
protected:
	QQ_t _target;
	std::string _display;

public:


	AtMessage() {}
	AtMessage(QQ_t target) : _target(target) {}
	AtMessage(const AtMessage&) = default;
	AtMessage& operator=(const AtMessage&) = default;
	AtMessage(AtMessage&&) noexcept = default;
	AtMessage& operator=(AtMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "At";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual AtMessage* Clone() const override
	{
		return new AtMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;


	bool operator==(const AtMessage& rhs)
	{
		return this->_target == rhs._target;
	}

	bool operator!=(const AtMessage& rhs)
	{
		return !(*this == rhs);
	}

	QQ_t GetTarget() const { return this->_target; }
	void SetTarget(QQ_t target) { this->_target = target; }
	std::string GetDisplayName() { return this->_display; }
};

}

#endif