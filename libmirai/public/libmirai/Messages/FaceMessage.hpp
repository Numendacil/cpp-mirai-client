#ifndef _MIRAI_FACE_MESSAGE_HPP_
#define _MIRAI_FACE_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class FaceMessage : public MessageBase
{
protected:
	int64_t _id;
	std::string _name;


public:
	FaceMessage() : _id(-1) {}
	FaceMessage(int64_t id) : _id(id) {}
	FaceMessage(const std::string& name) : _name(name) {}
	FaceMessage(const FaceMessage&) = default;
	FaceMessage& operator=(const FaceMessage&) = default;
	FaceMessage(FaceMessage&&) noexcept = default;
	FaceMessage& operator=(FaceMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Face";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FaceMessage* Clone() const override
	{
		return new FaceMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const FaceMessage& rhs)
	{
		return (_id >= 0)? this->_id == rhs._id : this->_name == rhs._name;
	}

	bool operator!=(const FaceMessage& rhs)
	{
		return !(*this == rhs);
	}

	int64_t GetId() const { return this->_id; }
	void SetId(int64_t id) { this->_id = id; }
	std::string GetName() const { return this->_name; }
	void SetName(const std::string& name) { this->_name = name; }

};

}


#endif