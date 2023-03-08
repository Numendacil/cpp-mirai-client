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

#ifndef MIRAI_MUSIC_SHARE_MESSAGE_HPP_
#define MIRAI_MUSIC_SHARE_MESSAGE_HPP_

#include <array>
#include <string>
#include <utility>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 音乐分享卡片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `MusicShareMessage::kind_` | `MusicShareType::ENUM_END`
 * `MusicShareMessage::title_` | `""`
 * `MusicShareMessage::summary_` | `""`
 * `MusicShareMessage::JumpUrl_` | `""`
 * `MusicShareMessage::PictureUrl_` | `""`
 * `MusicShareMessage::MusicUrl_` | `""`
 * `MusicShareMessage::brief_` | `""`
 */
class MusicShareMessage final : public IMessageImpl<MusicShareMessage>
{
	friend IMessageImpl<MusicShareMessage>;

protected:
	MusicShareType kind_ = MusicShareType::ENUM_END;
	std::string title_{};
	std::string summary_{};
	std::string JumpUrl_{};
	std::string PictureUrl_{};
	std::string MusicUrl_{};
	std::string brief_{};

	static constexpr MessageTypes TYPE_ = MessageTypes::MUSIC_SHARE;
	static constexpr bool SUPPORT_SEND_ = true;

	bool isValid_() const final
	{
		return !(this->kind_ == MusicShareType::ENUM_END || this->title_.empty() || this->summary_.empty()
		         || this->JumpUrl_.empty() || this->PictureUrl_.empty() || this->MusicUrl_.empty()
		         || this->brief_.empty());
	}

public:
	MusicShareMessage() = default;

	MusicShareMessage(MusicShareType kind, std::string title, std::string summary, std::string JumpUrl,
	                  std::string PictureUrl, std::string MusicUrl, std::string brief)
		: kind_(kind)
		, title_(std::move(title))
		, summary_(std::move(summary))
		, JumpUrl_(std::move(JumpUrl))
		, PictureUrl_(std::move(PictureUrl))
		, MusicUrl_(std::move(MusicUrl))
		, brief_(std::move(brief))
	{
	}

	/// 获取分享种类
	MusicShareType GetKind() const { return this->kind_; }
	/// 获取标题
	std::string GetTitle() const { return this->title_; }
	/// 获取介绍
	std::string GetSummary() const { return this->summary_; }
	/// 获取转跳链接
	std::string GetJumpUrl() const { return this->JumpUrl_; }
	/// 获取封面图片连接
	std::string GetPictureUrl() const { return this->PictureUrl_; }
	/// 获取音乐链接
	std::string GetMusicUrl() const { return this->MusicUrl_; }
	/**
	 * @brief 获取简介
	 * 
	 * 简介为未打开会话窗口时显示的简要文字消息
	 */
	std::string GetBrief() const { return this->brief_; }

	/// 设置分享种类
	MusicShareMessage& SetKind(MusicShareType kind)
	{
		this->kind_ = kind;
		return *this;
	}
	/// 设置标题
	MusicShareMessage& SetTitle(std::string title)
	{
		this->title_ = std::move(title);
		return *this;
	}
	/// 设置介绍
	MusicShareMessage& SetSummary(std::string summary)
	{
		this->summary_ = std::move(summary);
		return *this;
	}
	/// 设置转跳链接
	MusicShareMessage& SetJumpUrl(std::string JumpUrl)
	{
		this->JumpUrl_ = std::move(JumpUrl);
		return *this;
	}
	/// 设置封面图片连接
	MusicShareMessage& SetPictureUrl(std::string PictureUrl)
	{
		this->PictureUrl_ = std::move(PictureUrl);
		return *this;
	}
	/// 设置音乐链接
	MusicShareMessage& SetMusicUrl(std::string MusicUrl)
	{
		this->MusicUrl_ = std::move(MusicUrl);
		return *this;
	}
	/// 设置简介
	MusicShareMessage& SetBrief(std::string brief)
	{
		this->brief_ = std::move(brief);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<MusicShareMessage::GetType()>
{
	using type = MusicShareMessage;
};


} // namespace Mirai


#endif