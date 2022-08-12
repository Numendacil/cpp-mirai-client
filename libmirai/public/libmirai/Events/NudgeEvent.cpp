#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "NudgeEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void NudgeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_FromId = Utils::GetValue(data, "fromId", QQ_t{});
	this->_action = Utils::GetValue(data, "action", "");
	this->_suffix = Utils::GetValue(data, "suffix", "");

	this->_target = NudgeTarget{};
	this->_target.SetTarget(Utils::GetValue(data, "target", QQ_t{}));
	json subject = Utils::GetValue(data, "subject", json{});
	if (!subject.empty() && subject.is_object())
	{
		this->_target.SetSubject(Utils::GetValue(subject, "id", (int64_t)0));
		this->_target.SetTargetKind(Utils::GetValue(subject, "kind", ""));
	}
}

}