#ifndef _MIRAI_FACE_MESSAGE_HPP_
#define _MIRAI_FACE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief QQ表情消息
 * 
 * 表情可以通过 `id` 或 `name` 发送，优先级 `id` > `name`
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FaceMessage::_id` | `-1`
 * `FaceMessage::_name` | `""`
 */
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

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual FaceMessage* Clone() const override { return new FaceMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const FaceMessage& rhs) { return (_id >= 0) ? this->_id == rhs._id : this->_name == rhs._name; }

	bool operator!=(const FaceMessage& rhs) { return !(*this == rhs); }

	/// 获取表情id
	int64_t GetId() const { return this->_id; }

	/// 获取表情名称
	std::string GetName() const { return this->_name; }

	/// 设置表情id
	FaceMessage& SetId(int64_t id)
	{
		this->_id = id;
		return *this;
	}

	/// 设置表情名称。这一操作会清除已设置的id。
	FaceMessage& SetName(const std::string& name)
	{
		this->_id = -1;
		this->_name = name;
		return *this;
	}
};

} // namespace Mirai


#endif