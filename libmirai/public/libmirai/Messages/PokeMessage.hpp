#ifndef _MIRAI_POKE_MESSAGE_HPP_
#define _MIRAI_POKE_MESSAGE_HPP_

#include <string>
#include <array>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class PokeMessage : public MessageBase
{
public:
	enum PokeKind : std::size_t 
	{ 
		CHUOYICHUO = 0,		// 戳一戳
		BIXIN,			// 比心
		DIANZAN,		// 点赞
		XINSUI,			// 心碎
		LIULIULIU,		// 666
		FANGDAZHAO,		// 放大招
		GOUYIN,			// 勾引
		BAOBEIQIU,		// 宝贝球 (SVIP)
		ROSE,			// 玫瑰花 (SVIP)
		ZHAOHUANSHU,		// 召唤术 (SVIP)
		RANGNIPI,		// 让你皮 (SVIP)
		JIEYIN,			// 结印 (SVIP)
		SHOULEI,		// 手雷 (SVIP)
		ZHUAYIXIA,		// 抓一下 (SVIP)
		SUIPING,		// 碎屏 (SVIP)
		QIAOMEN,		// 敲门 (SVIP)
		UNKNOWN
	};

protected:
	PokeKind _kind;

	
	static constexpr std::array<std::string_view, static_cast<std::size_t>(PokeKind::UNKNOWN)> _PokeKindStr = 
	{
		"ChuoYiChuo",
		"BiXin",
		"DianZan",
		"XinSui",
		"LiuLiuLiu",
		"FangDaZhao",
		"GouYin",
		"BaoBeiQiu",
		"Rose",
		"ZhaoHuanShu",
		"RangNiPi",
		"JieYin",
		"ShouLei",
		"ZhuaYiXia",
		"SuiPing",
		"QiaoMen"
	};

	static constexpr std::string_view _to_string(const PokeKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _PokeKindStr.size())
			return _PokeKindStr[i];
		else
			return "";
	}

	static constexpr PokeKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _PokeKindStr.size(); i++)
			if (_PokeKindStr[i] == s)
				return static_cast<PokeKind>(i);

		return PokeKind::UNKNOWN;
	}
public:
	PokeMessage() {}
	PokeMessage(PokeKind kind) : _kind(kind) {}
	PokeMessage(const PokeMessage&) = default;
	PokeMessage& operator=(const PokeMessage&) = default;
	PokeMessage(PokeMessage&&) noexcept = default;
	PokeMessage& operator=(PokeMessage&&) noexcept = default;
	

	static constexpr std::string_view _TYPE_ = "Poke";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual PokeMessage* Clone() const override
	{
		return new PokeMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const PokeMessage& rhs)
	{
		return this->_kind == rhs._kind;
	}

	bool operator!=(const PokeMessage& rhs)
	{
		return !(*this == rhs);
	}

	PokeKind GetPokeKind() const { return this->_kind; }
	void SetPokeKind(PokeKind kind) { this->_kind = kind; }
};

}


#endif