#ifndef _MIRAI_FILE_MESSAGE_HPP_
#define _MIRAI_FILE_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

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

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FileMessage* Clone() const override
	{
		return new FileMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const FileMessage& rhs)
	{
		return this->_id == rhs._id;
	}

	bool operator!=(const FileMessage& rhs)
	{
		return !(*this == rhs);
	}

	std::string GetId() const { return this->_id; }
	std::string GetName() const { return this->_name; }
	int64_t GetSize() const { return this->_size; }
};

}

#endif