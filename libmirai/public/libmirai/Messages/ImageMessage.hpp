#ifndef _MIRAI_IMAGE_MESSAGE_HPP_
#define _MIRAI_IMAGE_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/MediaTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class ImageMessage : public MessageBase
{
protected:
	MiraiImage _image;

	void _clear() noexcept
	{
		this->_image = {};
	}

public:
	ImageMessage(const MiraiImage& image = {}) : _image(image) {}
	ImageMessage(std::string ImageId, std::string url, std::string path, std::string base64)
	: _image(ImageId, url, path, base64) {}
	ImageMessage(const ImageMessage&) = default;
	ImageMessage& operator=(const ImageMessage&) = default;
	ImageMessage(ImageMessage&&) noexcept = default;
	ImageMessage& operator=(ImageMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Image";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual ImageMessage* Clone() const override
	{
		return new ImageMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	MiraiImage GetImage() const { return this->_image; }

	void SetImageId(const std::string& id) { this->_clear(); this->_image.id = id; }
	void SetUrl(const std::string& url) { this->_clear(); this->_image.url = url; }
	void SetPath(const std::string& path) { this->_clear(); this->_image.path = path; }
	void SetBase64(const std::string& base64) { this->_clear(); this->_image.base64 = base64; }
	void SetImage(const MiraiImage& image) { this->_clear(); this->_image = image; }
};

}


#endif