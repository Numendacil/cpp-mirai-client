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

#include "HttpClientImpl.hpp"

#include <string>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai::Details
{

using json = nlohmann::json;
using std::string;

namespace
{

const char* const JSON_CONTENT_TYPE = "application/json;charset=UTF-8";

}

// json HttpClientImpl::Verify(const string& VerifyKey)
// {
// 	json body = {{"verifyKey", VerifyKey}};
// 	auto result = this->_client.Post("/verify", body.dump(), JSON_CONTENT_TYPE);
// 	json resp = Utils::ParseResponse(result);
// 	return resp;
// }

// json HttpClientImpl::Bind(const string& SessionKey, QQ_t qq)
// {
// 	json body = {{"sessionKey", SessionKey}, {"qq", int64_t(qq)}};
// 	auto result = this->_client.Post("/bind", body.dump(), JSON_CONTENT_TYPE);
// 	json resp = Utils::ParseResponse(result);
// 	return resp;
// }

json HttpClientImpl::SessionInfo(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->_client.Get("/sessionInfo", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

// json HttpClientImpl::Release(const string& SessionKey, QQ_t qq)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}, {"qq", qq.to_string()}};
// 	auto result = this->_client.Get("/release", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp;
// }


// json HttpClientImpl::CountMessage(const string& SessionKey)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}};
// 	auto result = this->_client.Get("/countMessage", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp.at("data");
// }

// json HttpClientImpl::FetchMessage(const string& SessionKey, int count)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}, {"count", std::to_string(count)}};
// 	auto result = this->_client.Get("/fetchMessage", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp.at("data");
// }

// json HttpClientImpl::FetchLatestMessage(const string& SessionKey, int count)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}, {"count", std::to_string(count)}};
// 	auto result = this->_client.Get("/fetchLatestMessage", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp.at("data");
// }

// json HttpClientImpl::PeekMessage(const string& SessionKey, int count)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}, {"count", std::to_string(count)}};
// 	auto result = this->_client.Get("/peekMessage", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp.at("data");
// }

// json HttpClientImpl::PeekLatestMessage(const string& SessionKey, int count)
// {
// 	httplib::Params params = {{"sessionKey", SessionKey}, {"count", std::to_string(count)}};
// 	auto result = this->_client.Get("/peekLatestMessage", params, httplib::Headers{});
// 	json resp = Utils::ParseResponse(result);
// 	return resp.at("data");
// }


json HttpClientImpl::About()
{
	auto result = this->_client.Get("/about");
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::BotList()
{
	auto result = this->_client.Get("/botList");
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}


json HttpClientImpl::MessageFromId(const string& SessionKey, MessageId_t id, UID_t target)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"messageId", std::to_string(id)}, {"target", target.to_string()}};
	auto result = this->_client.Get("/messageFromId", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}


json HttpClientImpl::FriendList(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->_client.Get("/friendList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::GroupList(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->_client.Get("/groupList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::MemberList(const string& SessionKey, GID_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->_client.Get("/memberList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}


json HttpClientImpl::BotProfile(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->_client.Get("/botProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::FriendProfile(const string& SessionKey, QQ_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->_client.Get("/friendProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::MemberProfile(const string& SessionKey, GID_t target, QQ_t MemberId)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"target", target.to_string()}, {"memberId", MemberId.to_string()}};
	auto result = this->_client.Get("/memberProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::UserProfile(const string& SessionKey, QQ_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->_client.Get("/userProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::SendFriendMessage(const string& SessionKey, QQ_t qq, const json& message,
                                       std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"qq", qq}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->_client.Post("/sendFriendMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::SendGroupMessage(const string& SessionKey, GID_t group, const json& message,
                                      std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"group", group}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->_client.Post("/sendGroupMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::SendTempMessage(const string& SessionKey, QQ_t qq, GID_t group, const json& message,
                                     std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"qq", qq}, {"group", group}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->_client.Post("/sendTempMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::SendNudge(const string& SessionKey, QQ_t target, UID_t subject, const string& kind)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"subject", subject}, {"kind", kind}};
	auto result = this->_client.Post("/sendNudge", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::Recall(const string& SessionKey, MessageId_t id, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"messageId", id}, {"target", target}};
	auto result = this->_client.Post("/recall", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::RoamingMessages(const string& SessionKey, std::time_t TimeStart, std::time_t TimeEnd, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"timeStart", TimeStart}, {"timeEnd", TimeEnd}, {"target", target}};
	auto result = this->_client.Post("/roamingMessages", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp["data"];
}


json HttpClientImpl::FileList(const string& SessionKey, const string& id, const string& path, UID_t target,
                              int64_t offset, int64_t size, bool withDownloadInfo)
{
	httplib::Params params = {{"sessionKey", SessionKey},
	                          {"target", target.to_string()},
	                          {"withDownloadInfo", withDownloadInfo ? "true" : "false"}};
	if (!path.empty()) params.emplace("path", path);
	else
		params.emplace("id", id);
	if (offset > 0) params.emplace("offset", std::to_string(offset));
	if (size > 0) params.emplace("size", std::to_string(size));
	auto result = this->_client.Get("/file/list", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::FileInfo(const string& SessionKey, const string& id, const string& path, UID_t target,
                              bool withDownloadInfo)
{
	httplib::Params params = {{"sessionKey", SessionKey},
	                          {"target", target.to_string()},
	                          {"withDownloadInfo", withDownloadInfo ? "true" : "false"}};
	if (!path.empty()) params.emplace("path", path);
	else
		params.emplace("id", id);
	auto result = this->_client.Get("/file/info", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::FileMkdir(const string& SessionKey, const string& id, const string& path, UID_t target,
                               const string& directory)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"directoryName", directory}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->_client.Post("/file/mkdir", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::FileDelete(const string& SessionKey, const string& id, const string& path, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->_client.Post("/file/delete", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::FileMove(const string& SessionKey, const string& id, const string& path, UID_t target,
                              const string& TargetDirId, const string& TargetDirPath)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	if (!TargetDirPath.empty()) body["moveToPath"] = TargetDirPath;
	else
		body["moveTo"] = TargetDirId;
	auto result = this->_client.Post("/file/move", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::FileRename(const string& SessionKey, const string& id, const string& path, UID_t target,
                                const string& name)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"renameTo", name}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->_client.Post("/file/rename", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::FileUpload(const string& SessionKey, const string& path, UID_t target, const string& type,
                                const string& name, const string& content)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""},
	                                         {"path", path, "", ""},
	                                         {"target", target.to_string(), "", ""},
	                                         {"type", type, "", ""},
	                                         {"file", content, name, "application/octet-stream"}};
	this->_client.set_compress(true);
	auto result = this->_client.Post("/file/upload", items);
	this->_client.set_compress(false);
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}


json HttpClientImpl::UploadImage(const string& SessionKey, const string& type, const string& image)
{
	httplib::MultipartFormDataItems items = {
		{"sessionKey", SessionKey, "", ""}, {"type", type, "", ""}, {"img", image, "", "application/octet-stream"}};
	this->_client.set_compress(true);
	auto result = this->_client.Post("/uploadImage", items);
	this->_client.set_compress(false);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::UploadAudio(const string& SessionKey, const string& type, const string& Audio)
{
	httplib::MultipartFormDataItems items = {
		{"sessionKey", SessionKey, "", ""}, {"type", type, "", ""}, {"img", Audio, "", "application/octet-stream"}};
	this->_client.set_compress(true);
	auto result = this->_client.Post("/uploadVoice", items);
	this->_client.set_compress(false);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::DeleteFriend(const string& SessionKey, QQ_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->_client.Post("/deleteFriend", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::Mute(const string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"time", time.count()}};
	auto result = this->_client.Post("/mute", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::Unmute(const string& SessionKey, GID_t target, QQ_t member)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}};
	auto result = this->_client.Post("/unmute", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::Kick(const string& SessionKey, GID_t target, QQ_t member, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"msg", message}};
	auto result = this->_client.Post("/kick", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::Quit(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->_client.Post("/quit", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::MuteAll(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->_client.Post("/muteAll", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::UnmuteAll(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->_client.Post("/unmuteAll", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::SetEssence(const string& SessionKey, MessageId_t id, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"messageId", id}, {"target", target}};
	auto result = this->_client.Post("/setEssence", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::GetGroupConfig(const string& SessionKey, GID_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->_client.Get("/groupConfig", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::PostGroupConfig(const string& SessionKey, GID_t target, const string& name,
                                     std::optional<bool> AllowConfessTalk, std::optional<bool> AllowMemberInvite,
                                     std::optional<bool> AutoApprove, std::optional<bool> AllowAnonymousChat)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"config", json::object()}};
	if (!name.empty()) body.at("config")["name"] = name;
	if (AllowConfessTalk.has_value()) body.at("config")["confessTalk"] = AllowConfessTalk.value();
	if (AllowMemberInvite.has_value()) body.at("config")["AllowMemberInvite"] = AllowMemberInvite.value();
	if (AutoApprove.has_value()) body.at("config")["autoApprove"] = AutoApprove.value();
	if (AllowAnonymousChat.has_value()) body.at("config")["anonymousChat"] = AllowAnonymousChat.value();
	auto result = this->_client.Post("/groupConfig", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::GetMemberInfo(const string& SessionKey, GID_t target, QQ_t member)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"target", target.to_string()}, {"memberId", member.to_string()}};
	auto result = this->_client.Get("/memberInfo", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::PostMemberInfo(const string& SessionKey, GID_t target, QQ_t member, const string& name,
                                    const string& title)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"info", json::object()}};
	if (!name.empty()) body["info"]["name"] = name;
	if (!title.empty()) body["info"]["specialTitle"] = title;
	auto result = this->_client.Post("/memberInfo", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::MemberAdmin(const string& SessionKey, GID_t target, QQ_t member, bool assign)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"assign", assign}};
	auto result = this->_client.Post("/memberAdmin", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::AnnoList(const string& SessionKey, GID_t target, int64_t offset, int64_t size)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"id", target.to_string()}};
	if (offset > 0) params.emplace("offset", std::to_string(offset));
	if (size > 0) params.emplace("size", std::to_string(size));
	auto result = this->_client.Get("/anno/list", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::AnnoPublish(const string& SessionKey, GID_t target, const string& content, const string& url,
                                 const string& path, const string& base64, bool ToNewMember, bool pinned,
                                 bool ShowEditCard, bool ShowPopup, bool RequireConfirm)
{
	json body = {{"sessionKey", SessionKey}, {"target", target},
	             {"content", content},       {"sendToNewMember", ToNewMember},
	             {"pinned", pinned},         {"showEditCard", ShowEditCard},
	             {"showPopup", ShowPopup},   {"requireConfirm", RequireConfirm}};
	if (!url.empty()) body["imageUrl"] = url;
	else if (!path.empty())
		body["imagePath"] = path;
	else if (!base64.empty())
		body["imageBase64"] = base64;
	auto result = this->_client.Post("/anno/publish", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp.at("data");
}

json HttpClientImpl::AnnoDelete(const string& SessionKey, GID_t target, const string& fid)
{
	json body = {{"sessionKey", SessionKey}, {"id", target}, {"fid", fid}};
	auto result = this->_client.Post("/anno/delete", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::RespNewFriendRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
                                               int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->_client.Post("/resp/newFriendRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::RespMemberJoinRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
                                                int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->_client.Post("/resp/memberJoinRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::RespBotInvitedJoinGroupRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId,
                                                         GID_t GroupId, int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->_client.Post("/resp/botInvitedJoinGroupRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::CmdExecute(const string& SessionKey, const json& command)
{
	json body = {{"sessionKey", SessionKey}, {"command", command}};
	auto result = this->_client.Post("/cmd/execute", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::CmdRegister(const string& SessionKey, const string& name, const std::vector<string>& alias,
                                 const string& usage, const string& description)
{
	json body = {{"sessionKey", SessionKey}, {"name", name}, {"usage", usage}, {"description", description}};
	if (!alias.empty()) body["alias"] = alias;
	auto result = this->_client.Post("/cmd/register", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	return resp;
}


json HttpClientImpl::PostRaw(const string& path, const string& content, const string& ContentType)
{
	auto result = this->_client.Post(path, content, ContentType);
	json resp = Utils::ParseResponse(result);
	return resp;
}

json HttpClientImpl::GetRaw(const string& path, const std::multimap<string, string> params)
{
	auto result = this->_client.Get(path, params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	return resp;
}

} // namespace Mirai::Details
