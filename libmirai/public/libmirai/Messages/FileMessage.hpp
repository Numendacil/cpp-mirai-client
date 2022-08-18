#ifndef _MIRAI_FILE_MESSAGE_HPP_
#define _MIRAI_FILE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 文件消息
 * 
 * 仅用于接收，发送时将会被无视
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FileMessage::_id` | `""`
 * `FileMessage::_name` | `""`
 * `FileMessage::_size` | `0`
 */
class FileMessage : public MessageBase
{
protected:
	std::string _id;
	std::string _name;
	int64_t _size;

public:
	FileMessage() : _size(0) {}
	FileMessage(const FileMessage&) = default;
	FileMessage& operator=(const FileMessage&) = default;
	FileMessage(FileMessage&&) noexcept = default;
	FileMessage& operator=(FileMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "File";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual FileMessage* Clone() const override { return new FileMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const FileMessage& rhs) { return this->_id == rhs._id; }

	bool operator!=(const FileMessage& rhs) { return !(*this == rhs); }

	/// 获取文件id
	std::string GetId() const { return this->_id; }
	/// 获取文件名
	std::string GetName() const { return this->_name; }
	/// 获取文件大小
	int64_t GetSize() const { return this->_size; }
};

} // namespace Mirai

#endif