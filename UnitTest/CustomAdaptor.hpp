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

#ifndef MIRAI_CUSTOM_ADAPTOR_HPP_
#define MIRAI_CUSTOM_ADAPTOR_HPP_

#include <memory>
#include <string_view>

#include <libmirai/Adaptors/IAdaptor.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Serialization/Messages/Messages.hpp>
#include <libmirai/Serialization/Events/Events.hpp>

#include <nlohmann/json.hpp>

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

class CustomAdaptor final : public IAdaptor
{
	static constexpr std::string_view SESSION_KEY = "SessionKey";

public:
	template<typename... Args> using Callback = std::function<void(Args...)>;
	using string = std::string;

	CustomAdaptor() = default;
	~CustomAdaptor() override = default;

	Callback<ClientConnectionEstablishedEvent> ConnectionEstablishedCallback;
	Callback<const string&> TextCallback;
	Callback<ClientConnectionErrorEvent> ConnectionErrorCallback;
	Callback<ClientConnectionClosedEvent> ConnectionClosedCallback;


	// *********************************************
	// ******* Connection & Event related **********
	// *********************************************

	/**
	 * @brief Register callback for ConnectionEstablished event
	 * 
	 */
	void OnConnectionEstablished(Callback<ClientConnectionEstablishedEvent> callback) override
	{
		this->ConnectionEstablishedCallback = callback;
	}

	/**
	 * @brief Register callback for receiving content
	 * 
	 */
	void OnRecv(Callback<const string&> callback) override { this->TextCallback = callback; }

	/**
	 * @brief Register callback for ConnectionError event
	 * 
	 */
	void OnConnectionError(Callback<ClientConnectionErrorEvent> callback) override
	{
		this->ConnectionErrorCallback = callback;
	}

	/**
	 * @brief Register callback for ConnectionClosed event
	 * 
	 */
	void OnConnectionClosed(Callback<ClientConnectionClosedEvent> callback) override
	{
		this->ConnectionClosedCallback = callback;
	}

	/**
	 * @brief Called for opening a session to mirai-api-http
	 * 
	 * Connect should not return before ConnectionEstablished callback is called
	 */
	void Connect() override
	{

		if (this->ConnectionEstablishedCallback)
		{
			ClientConnectionEstablishedEvent event;

			event.uri = "/api";
			event.headers = {{"Date", "Tue, 07 Mar 2023 07:35:38 GMT"}, {"Server", "ktor-server-core/1.6.7"}};
			event.protocol = "HTTP";
			event.SessionKey = SESSION_KEY;
			event.BotProfile.id = 12345_qq;
			event.BotProfile.nickname = "Elanor";
			event.BotProfile.remark = "Elanor";

			this->ConnectionEstablishedCallback(std::move(event));
		}
	}

	/**
	 * @brief Called for closing a session
	 * 
	 * Disconnect should not return before ConnectionClosed callback is called
	 */
	void Disconnect(string SessionKey) override
	{
		if (SessionKey != SESSION_KEY) return;

		if (!this->ConnectionClosedCallback) return;

		ClientConnectionClosedEvent event;
		event.code = 1000;
		event.reason = "Normal Closure";
		event.remote = false;

		this->ConnectionClosedCallback(std::move(event));
	}


	// *********************************************
	// *************** Request API *****************
	// *********************************************

	using json = nlohmann::json;

	string Version() override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":{
				"version":"v1.0.0"
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").at("version").get<string>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	QQ_t GetBotQQ() override { return 0_qq; }

	std::vector<QQ_t> BotList() override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":[
				123456789,
				987654321,
				1145141919
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<QQ_t>>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent>
	MessageFromId(string SessionKey, MessageId_t id, UID_t target) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":{
				"type":"FriendMessage",
				"messageChain":[
					{
						"type":"Source",
						"id":123456,
						"time":123456789
					},
					{
						"type":"Plain",
						"text":"Mirai"
					}
				],
				"sender":{
					"id":1234567890,
					"nickname":"",
					"remark":""
				}
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		EventTypes type = resp.at("data").at("type").get<EventTypes>();
		std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent> m;
		std::visit(
			[&type, &resp](auto&& p)
			{
				using Type = std::decay_t<decltype(p)>;
				if (type == Type::GetType())
				{
					resp.at("data").get_to(p);
				}
			},
			m);
		return m;
		MIRAI_PARSE_GUARD_END(resp);
	}


	std::vector<User> FriendList(string SessionKey) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":[
				{
					"id":123456789,
					"nickname":"",
					"remark":""
				},
				{
					"id":987654321,
					"nickname":"",
					"remark":""
				}
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<User>>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	std::vector<Group> GroupList(string SessionKey) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":[
				{
					"id":123456789,
					"name":"group1",
					"permission":"MEMBER"
				},
				{
					"id":987654321,
					"name":"group2",
					"permission":"MEMBER"
				}
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<Group>>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	std::vector<GroupMember> MemberList(string SessionKey, GID_t target) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":[
				{
					"id":1234567890,
					"memberName":"",
					"permission":"MEMBER",
					"specialTitle":"title",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":{
						"id":12345,
						"name":"group1",
						"permission":"MEMBER"
					}
				},
				{
					"id":9876543210,
					"memberName":"",
					"specialTitle":"title",
					"permission":"OWNER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":{
						"id":54321,
						"name":"group2",
						"permission":"MEMBER"
					}
				}
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<GroupMember>>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	std::vector<GroupMember> LatestMemberList(string SessionKey, GID_t target) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":[
				{
					"id":9876543210,
					"memberName":"",
					"specialTitle":"群头衔",
					"permission":"OWNER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":{
						"id":123456789,
						"name":"群名2",
						"permission":"MEMBER"
					}
				}
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<GroupMember>>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	UserProfile GetBotProfile(string SessionKey) override
	{
		json resp = json::parse(R"({
			"nickname":"nickname",
			"email":"email",
			"age":18,
			"level":1,
			"sign":"mirai",
			"sex":"UNKNOWN"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<UserProfile>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	UserProfile GetFriendProfile(string SessionKey, QQ_t target) override
	{
		json resp = json::parse(R"({
			"nickname":"nickname",
			"email":"email",
			"age":18,
			"level":1,
			"sign":"mirai",
			"sex":"UNKNOWN"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<UserProfile>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	UserProfile GetMemberProfile(string SessionKey, GID_t target, QQ_t MemberId) override
	{
		json resp = json::parse(R"({
			"nickname":"nickname",
			"email":"email",
			"age":18,
			"level":1,
			"sign":"mirai",
			"sex":"UNKNOWN"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<UserProfile>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	UserProfile GetUserProfile(string SessionKey, QQ_t target) override
	{
		json resp = json::parse(R"({
			"nickname":"nickname",
			"email":"email",
			"age":18,
			"level":1,
			"sign":"mirai",
			"sex":"UNKNOWN"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<UserProfile>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	MessageId_t SendFriendMessage(string SessionKey, QQ_t qq, const MessageChain& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MessageId_t SendFriendMessage(string SessionKey, QQ_t qq, MessageChain&& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MessageId_t SendGroupMessage(string SessionKey, GID_t group, const MessageChain& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MessageId_t SendGroupMessage(string SessionKey, GID_t group, MessageChain&& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MessageId_t SendTempMessage(string SessionKey, QQ_t qq, GID_t group, const MessageChain& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MessageId_t SendTempMessage(string SessionKey, QQ_t qq, GID_t group, MessageChain&& message,
						std::optional<MessageId_t> QuoteId) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"messageId":1234567890
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("messageId").get<MessageId_t>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	void SendNudge(string SessionKey, QQ_t target, UID_t subject, NudgeType kind) override
	{}

	void Recall(string SessionKey, MessageId_t id, UID_t target) override
	{}

	std::vector<MessageChain> RoamingMessages(string SessionKey, std::time_t TimeStart, std::time_t TimeEnd,
								UID_t target) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"success",
			"data":[]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<MessageChain>>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	std::vector<GroupFileInfo> FileList(string SessionKey, string id, string path, UID_t target,
							int64_t offset, int64_t size, bool withDownloadInfo) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data": [{
				"name":"image.png",
				"id":"/12314d-1wf13-a98ffa",
				"path":"/image.png",
				"parent":null,
				"contact":
				{
					"id":123123,
					"name":"group",
					"permission":"OWNER"
				},
				"isFile":true,
				"isDictionary":false,
				"isDirectory":false,
				"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
				"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
				"downloadTimes":10,
				"uploaderId":123456789,
				"uploadTime":1631153749,
				"lastModifyTime":1631153749,
				"downloadInfo":
				{
					"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
					"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
					"downloadTimes":10,
					"uploaderId":123456789,
					"uploadTime":1631153749,
					"lastModifyTime":1631153749,
					"url":"example.com"
				}
			}]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<GroupFileInfo>>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	GroupFileInfo GetFileInfo(string SessionKey, string id, string path, UID_t target, bool withDownloadInfo) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data": {
				"name":"image.png",
				"id":"/12314d-1wf13-a98ffa",
				"path":"/image.png",
				"parent":null,
				"contact":
				{
					"id":123123,
					"name":"group",
					"permission":"OWNER"
				},
				"isFile":true,
				"isDictionary":false,
				"isDirectory":false,
				"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
				"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
				"downloadTimes":10,
				"uploaderId":123456789,
				"uploadTime":1631153749,
				"lastModifyTime":1631153749,
				"downloadInfo":
				{
					"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
					"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
					"downloadTimes":10,
					"uploaderId":123456789,
					"uploadTime":1631153749,
					"lastModifyTime":1631153749,
					"url":"example.com"
				}
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<GroupFileInfo>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	GroupFileInfo FileMkdir(string SessionKey, string id, string path, UID_t target, string directory) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data": {
				"name":"image.png",
				"id":"/12314d-1wf13-a98ffa",
				"path":"/image.png",
				"parent":null,
				"contact":{
					"id":123123,
					"name":"group",
					"permission":"OWNER"
				},
				"isFile":false,
				"isDictionary":true,
				"isDirectory":true
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<GroupFileInfo>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	void FileDelete(string SessionKey, string id, string path, UID_t target) override
	{}

	void FileMove(string SessionKey, string id, string path, UID_t target, string TargetDirId,
				string TargetDirPath) override
	{}

	void FileRename(string SessionKey, string id, string path, UID_t target, string name) override
	{}

	GroupFileInfo FileUpload(string SessionKey, string path, UID_t target, string type, string name,
						string content) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":
			{
				"name":"image.png",
				"id":"/12314d-1wf13-a98ffa",
				"path":"/image.png",
				"parent": {
					"name": "/",
					"id": "/",
					"path": "/",
					"parent": null,
					"contact": {
						"id": 123123,
						"name": "group",
						"permission": "OWNER"
					},
					"isFile": false,
					"isDictionary": true,
					"isDirectory": true,
					"size": 0,
					"sha1": null,
					"md5": null,
					"uploaderId": null,
					"uploadTime": null,
					"lastModifyTime": null,
					"downloadInfo": null
				},
				"contact":
				{
					"id":123123,
					"name":"group",
					"permission":"OWNER"
				},
				"isFile":true,
				"isDictionary":false,
				"isDirectory":false,
				"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
				"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
				"downloadTimes":10,
				"uploaderId":123456789,
				"uploadTime":1631153749,
				"lastModifyTime":1631153749,
				"downloadInfo":
				{
					"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
					"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
					"downloadTimes":10,
					"uploaderId":123456789,
					"uploadTime":1631153749,
					"lastModifyTime":1631153749,
					"url":"example.com"
				}
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<GroupFileInfo>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	GroupFileInfo
	FileUploadChunked(string SessionKey, string path, UID_t target, string type, string name,
					std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override
	{
		json resp = json::parse(R"({
			"code":0,
			"msg":"",
			"data":
			{
				"name":"image.png",
				"id":"/12314d-1wf13-a98ffa",
				"path":"/image.png",
				"parent": {
					"name": "/",
					"id": "/",
					"path": "/",
					"parent": null,
					"contact": {
						"id": 123123,
						"name": "group",
						"permission": "OWNER"
					},
					"isFile": false,
					"isDictionary": true,
					"isDirectory": true,
					"size": 0,
					"sha1": null,
					"md5": null,
					"uploaderId": null,
					"uploadTime": null,
					"lastModifyTime": null,
					"downloadInfo": null
				},
				"contact":
				{
					"id":123123,
					"name":"group",
					"permission":"OWNER"
				},
				"isFile":true,
				"isDictionary":false,
				"isDirectory":false,
				"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
				"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
				"downloadTimes":10,
				"uploaderId":123456789,
				"uploadTime":1631153749,
				"lastModifyTime":1631153749,
				"downloadInfo":
				{
					"sha1":"93c456b52c670cb1bfcebc8bd8b5d2ad467baf72",
					"md5":"9c21942cfd37f8a18fdbc6dbefe29e3f",
					"downloadTimes":10,
					"uploaderId":123456789,
					"uploadTime":1631153749,
					"lastModifyTime":1631153749,
					"url":"example.com"
				}
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<GroupFileInfo>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	MiraiImage UploadImage(string SessionKey, string type, string image) override
	{
		json resp = json::parse(R"({
			"imageId": "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}.mirai",
			"url": "xxxxxxxxxxxxxxxxxxxx"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<MiraiImage>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MiraiImage
	UploadImageChunked(string SessionKey, string type,
					std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override
	{
		json resp = json::parse(R"({
			"imageId": "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}.mirai",
			"url": "xxxxxxxxxxxxxxxxxxxx"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<MiraiImage>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MiraiAudio UploadAudio(string SessionKey, string type, string Audio) override
	{
		json resp = json::parse(R"({
			"voiceId":"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.amr",
			"url":"xxxxxxxxxxxxxxxxxxxx"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<MiraiAudio>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	MiraiAudio
	UploadAudioChunked(string SessionKey, string type,
					std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override
	{
		json resp = json::parse(R"({
			"voiceId":"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.amr",
			"url":"xxxxxxxxxxxxxxxxxxxx"
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<MiraiAudio>();
		MIRAI_PARSE_GUARD_END(resp);
	}


	void DeleteFriend(string SessionKey, QQ_t target) override
	{}


	void Mute(string SessionKey, GID_t target, QQ_t member, std::chrono::seconds time) override
	{}

	void Unmute(string SessionKey, GID_t target, QQ_t member) override
	{}

	void Kick(string SessionKey, GID_t target, QQ_t member, string message, bool block) override
	{}

	void Quit(string SessionKey, GID_t target) override
	{}

	void MuteAll(string SessionKey, GID_t target) override
	{}

	void UnmuteAll(string SessionKey, GID_t target) override
	{}

	void SetEssence(string SessionKey, MessageId_t id, GID_t target) override
	{}

	GroupConfig GetGroupConfig(string SessionKey, GID_t target) override
	{
		json resp = json::parse(R"({
			"name":"group",
			"announcement":"anno",
			"confessTalk":true,
			"allowMemberInvite":true,
			"autoApprove":true,
			"anonymousChat":true,
			"muteAll":true
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<GroupConfig>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	void SetGroupConfig(string SessionKey, GID_t target, string name, std::optional<bool> AllowConfessTalk,
					std::optional<bool> AllowMemberInvite, std::optional<bool> AutoApprove,
					std::optional<bool> AllowAnonymousChat) override
	{}

	GroupMember GetMemberInfo(string SessionKey, GID_t target, QQ_t member) override
	{
		json resp = json::parse(R"({
			"id":987654321,
			"memberName":"群名片",
			"specialTitle":"群头衔",
			"permission":"OWNER",
			"joinTimestamp":12345678,
			"lastSpeakTimestamp":8765432,
			"muteTimeRemaining":0,
			"group":{
				"id":12345,
				"name":"群名1",
				"permission":"MEMBER"
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.get<GroupMember>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	void SetMemberInfo(string SessionKey, GID_t target, QQ_t member, string name, string title) override
	{}

	void MemberAdmin(string SessionKey, GID_t target, QQ_t member, bool assign) override
	{}


	std::vector<GroupAnnouncement> AnnoList(string SessionKey, GID_t target, int64_t offset, int64_t size) override
	{
		json resp = json::parse(R"({
			"code": 0,
			"msg": "",
			"data":[
				{
					"group":{
						"id": 123456789, 
						"name": "group name", 
						"permission": "ADMINISTRATOR"
					},
					"content": "群公告内容",
					"senderId": 987654321,
					"fid": "公告唯一id",
					"allConfirmed": false,
					"confirmedMembersCount": 0,
					"publicationTime": 1645085843
				}
			]
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<std::vector<GroupAnnouncement>>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	GroupAnnouncement AnnoPublish(string SessionKey, GID_t target, string content, string url, string path,
						string base64, bool ToNewMember, bool pinned, bool ShowEditCard,
						bool ShowPopup, bool RequireConfirm) override
	{
		json resp = json::parse(R"({
			"code": 0,
			"msg": "",
			"data":
			{
				"group":{
					"id": 123456789, 
					"name": "group name", 
					"permission": "ADMINISTRATOR"
				},
				"content": "群公告内容",
				"senderId": 987654321,
				"fid": "公告唯一id",
				"allConfirmed": false,
				"confirmedMembersCount": 0,
				"publicationTime": 1645085843
			}
		})");

		MIRAI_PARSE_GUARD_BEGIN(resp);
		return resp.at("data").get<GroupAnnouncement>();
		MIRAI_PARSE_GUARD_END(resp);
	}

	void AnnoDelete(string SessionKey, GID_t target, string fid) override
	{}


	void RespNewFriendRequestEvent(string SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
						int operate, string message) override
	{}

	void RespMemberJoinRequestEvent(string SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
						int operate, string message) override
	{}

	void RespBotInvitedJoinGroupRequestEvent(string SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
								int operate, string message) override
	{}


	void CmdExecute(string SessionKey, const MessageChain& command) override
	{}

	void CmdExecute(string SessionKey, MessageChain&& command) override
	{}
	void CmdRegister(string SessionKey, string name, std::vector<string> alias, string usage,
					string description) override
	{}

	string CallAPI(const string& path, const string& method, const string& data) override
	{
		throw NotImplementedError(("Method not implemented in CallAPI: " + method).c_str());
	}

};

} // namespace Mirai

#endif