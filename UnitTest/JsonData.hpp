#include <nlohmann/json.hpp>

namespace Data
{

using json = nlohmann::json;

static const json MessageData = 
{
	{
		"AppMessage", json::parse(R"(
			{
				"type": "App",
				"content": "<>"
			}
		)")
	},
	{
		"AtAllMessage", json::parse(R"(
			{
				"type": "AtAll"
			}
		)")
	},
	{
		"AtMessage", json::parse(R"(
			{
				"type": "At",
				"target": 123456,
				"display": "@Mirai"
			}
		)")
	},
	{
		"AudioMessage", json::parse(R"(
			{
				"type": "Voice",
				"voiceId": "23C477720A37FEB6A9EE4BCCF654014F.amr",
				"url": "https://example.com",
				"path": null,
				"base64": null,
				"length": 1024
			}
		)")
	},
	{
		"DiceMessage", json::parse(R"(
			{
				"type": "Dice",
				"value": 1
			}
		)")
	},
	{
		"FaceMessage", json::parse(R"(
			{
				"type": "Face",
				"faceId": 123,
				"name": "bu"
			}
		)")
	},
	{
		"FileMessage", json::parse(R"(
			{
				"type": "File",
				"id": "id",
				"name": "xxx",
				"size": 128
			}
		)")
	},
	{
		"FlashImageMessage", json::parse(R"(
			{
				"type": "FlashImage",
				"imageId": "23C477720A37FEB6A9EE4BCCF654014F.png",
				"url": "https://example.com",
				"path": null,
				"base64": null,
				"size": 1024,
				"width": 1920,
				"height": 1080,
				"imageType": "PNG"
			}
		)")
	},
	{
		"ForwardMessage", json::parse(R"(
			{
				"type": "Forward",
				"nodeList": [
				{
					"senderId": 123,
					"time": 0,
					"senderName": "aaa",
					"messageChain": 
					[
						{
							"type": "Plain",
							"text": "txt"
						}
					],
					"messageId": 123,
					"messageRef": {
						"messageId": 123,
						"target": 321
					}
				},
				{
					"senderId": 456,
					"time": 2,
					"senderName": "bbb",
					"messageChain": [
					{
						"type": "Forward",
						"nodeList": [
						{
							"senderId": 999,
							"time": 1,
							"senderName": "ccc",
							"messageChain": [
							{
								"type": "Image",
								"imageId": "xxx.png",
								"url": null,
								"path": null,
								"base64": null
							}
							],
							"messageId": null,
							"messageRef": null
						}
						]
					}
					],
					"messageId": null,
					"messageRef": null
				}
				] 
			}
		)")
	},
	{
		"ImageMessage", json::parse(R"(
			{
				"type": "Image",
				"imageId": "23C477720A37FEB6A9EE4BCCF654014F.png",
				"url": "https://example.com",
				"path": null,
				"base64": null,
				"size": 1024,
				"width": 1920,
				"height": 1080,
				"imageType": "PNG"
			}
		)")
	},
	{
		"JsonMessage", json::parse(R"(
			{
				"type": "Json",
				"json": "{\"text\": \"hello\"}"
			}
		)")
	},
	{
		"MarketFaceMessage", json::parse(R"(
			{
				"type": "MarketFace",
				"id": 123,
				"name": "商城表情"
			}
		)")
	},
	{
		"MiraiCodeMessage", json::parse(R"(
			{
				"type": "MiraiCode",
				"code": "hello[mirai:at:1234567]"
			}
		)")
	},
	{
		"MusicShareMessage", json::parse(R"(
			{
				"type": "MusicShare",
				"kind": "NeteaseCloudMusic",
				"title": "ジェリーフィッシュ",
				"summary": "Yunomi/ローラーガール",
				"jumpUrl": "http://example.com",
				"pictureUrl": "http://example.com/picture.png",
				"musicUrl": "http://example.com/music.mp3",
				"brief":"[分享]ジェリーフィッシュ"
			}
		)")
	},
	{
		"PlainMessage", json::parse(R"(
			{
				"type": "Plain",
				"text": "hello"
			}
		)")
	},
	{
		"PokeMessage", json::parse(R"(
			{
				"type": "Poke",
				"name": "DianZan"
			}
		)")
	},
	{
		"QuoteMessage", json::parse(R"(
			{
				"type": "Quote",
				"id": 123456,
				"groupId": 123456789,
				"senderId": 987654321,
				"targetId": 123456789,
				"origin": [
					{"type": "Plain", "text": "text" }
				] 
			}
		)")
	},
	{
		"SourceMessage", json::parse(R"(
			{
				"type": "Source",
				"id": 654321,
				"time": 123456
			}
		)")
	},
	{
		"XmlMessage", json::parse(R"(
			{
				"type": "Xml",
				"xml": "<XML>"
			}
		)")
	}
};

static const json EventData = 
{
	{
		"BotOnlineEvent", json::parse(R"(
			{
				"type":"BotOnlineEvent",
				"qq":123456
			}
		)")
	},
	{
		"BotOfflineEventActive", json::parse(R"(
			{
				"type":"BotOfflineEventActive",
				"qq":123456
			}
		)")
	},
	{
		"BotOfflineEventDropped", json::parse(R"(
			{
				"type":"BotOfflineEventDropped",
				"qq":123456
			}
		)")
	},
	{
		"FriendInputStatusChangedEvent", json::parse(R"(
			{
				"type":"FriendInputStatusChangedEvent",
				"friend": 
				{
					"id": 123123,
					"nickname": "nick",
					"remark": "remark"
				}, 
				"inputting": true
			}
		)")
	},
	{
		"FriendNickChangedEvent", json::parse(R"(
			{
				"type": "FriendNickChangedEvent",
				"friend": 
				{
					"id": 123123,
					"nickname": "new nickname",
					"remark": "remark"
				}, 
				"from": "origin nickname",
				"to": "new nickname"
			}
		)")
	},
	{
		"BotGroupPermissionChangeEvent", json::parse(R"(
			{
				"type": "BotGroupPermissionChangeEvent",
				"origin": "MEMBER",
				"current": "ADMINISTRATOR",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "ADMINISTRATOR"
				}
			}
		)")
	},
	{
		"BotMuteEvent", json::parse(R"(
			{
				"type": "BotMuteEvent",
				"durationSeconds": 600,
				"operator": 
				{
					"id": 123456789,
					"memberName": "aaa",
					"permission": "ADMINISTRATOR",
					"specialTitle":"xxx",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"BotUnmuteEvent", json::parse(R"(
			{
				"type": "BotUnmuteEvent",
				"operator": 
				{
					"id": 123456789,
					"memberName": "aaa",
					"permission": "ADMINISTRATOR",
					"specialTitle":"xxx",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"BotJoinGroupEvent", json::parse(R"(
			{
				"type": "BotJoinGroupEvent",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"invitor": null
			}
		)")
	},
	{
		"BotLeaveEventActive", json::parse(R"(
			{
				"type": "BotLeaveEventActive",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				}
			}
		)")
	},
	{
		"BotLeaveEventKick", json::parse(R"(
			{
				"type": "BotLeaveEventKick",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator":
				{
					"id": 1111,
					"memberName":"Marisa",
					"permission":"OWNER",
					"specialTitle":"xxx",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"BotLeaveEventDisband", json::parse(R"(
			{
				"type": "BotLeaveEventDisband",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator":
				{
					"id": 1111,
					"memberName":"Marisa",
					"permission":"OWNER",
					"specialTitle":"xxx",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"GroupRecallEvent", json::parse(R"(
			{
				"type": "GroupRecallEvent",
				"authorId": 123456,
				"messageId": 123456789,
				"time": 1234679,
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "ADMINISTRATOR"
				},
				"operator": null
			}
		)")
	},
	{
		"FriendRecallEvent", json::parse(R"(
			{
				"type": "FriendRecallEvent",
				"authorId": 123456,
				"messageId": 123456789,
				"time": 1234679,
				"operator": 123456
			}
		)")
	},
	{
		"NudgeEvent", json::parse(R"(
			{
				"type": "NudgeEvent",
				"fromId": 123456,
				"subject": 
				{
					"id": 8888,
					"kind": "Group"
				},
				"action": "戳了戳",
				"suffix": "的脸",
				"target": 654321
			}
		)")
	},
	{
		"GroupNameChangeEvent", json::parse(R"(
			{
				"type": "GroupNameChangeEvent",
				"origin": "mirai technology",
				"current": "MIRAI TECHNOLOGY",
				"group": 
				{
					"id": 123456789,
					"name": "MIRAI TECHNOLOGY",
					"permission": "MEMBER"
				},
				"operator": 
				{
					"id": 123456,
					"memberName": "xxx",
					"permission": "ADMINISTRATOR",
					"specialTitle":"xxxx",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "MIRAI TECHNOLOGY",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"GroupEntranceAnnouncementChangeEvent", json::parse(R"(
			{
				"type": "GroupEntranceAnnouncementChangeEvent",
				"origin": "abc",
				"current": "cba",
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator": null
			}
		)")
	},
	{
		"GroupMuteAllEvent", json::parse(R"(
			{
				"type": "GroupMuteAllEvent",
				"origin": false,
				"current": true,
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator": null
			}
		)")
	},
	{
		"GroupAllowAnonymousChatEvent", json::parse(R"(
			{
				"type": "GroupAllowAnonymousChatEvent",
				"origin": false,
				"current": true,
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator": 
				{
					"id":1234567890,
					"memberName":"x",
					"specialTitle":"xx",
					"permission":"ADMINISTRATOR",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"GroupAllowConfessTalkEvent", json::parse(R"(
			{
				"type": "GroupAllowConfessTalkEvent",
				"origin": false,
				"current": true,
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"isByBot": false
			}
		)")
	},
	{
		"GroupAllowMemberInviteEvent", json::parse(R"(
			{
				"type": "GroupAllowMemberInviteEvent",
				"origin": false,
				"current": true,
				"group": 
				{
					"id": 123456789,
					"name": "Mirai Technology",
					"permission": "MEMBER"
				},
				"operator": null
			}
		)")
	},
	{
		"MemberJoinEvent", json::parse(R"(
			{
				"type": "MemberJoinEvent",
				"member": 
				{
					"id":1234567890,
					"memberName":"aaa",
					"specialTitle":"xxx",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":
					{
						"id":12345,
						"name":"xxx",
						"permission":"ADMINISTRATOR"
					}
				},
				"invitor": null
			}
		)")
	},
	{
		"MemberLeaveEventKick", json::parse(R"(
			{
				"type": "MemberLeaveEventKick",
				"member": 
				{
					"id":1234567890,
					"memberName":"aaa",
					"specialTitle":"",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":
					{
						"id":12345,
						"name":"xxx",
						"permission":"MEMBER"
					}
				},
				"operator": 
				{
					"id":88888888,
					"memberName":"bbb",
					"specialTitle":"ccc",
					"permission":"OWNER",
					"joinTimestamp":0,
					"lastSpeakTimestamp":6666666,
					"muteTimeRemaining":0,
					"group":
					{
						"id":12345,
						"name":"xxx",
						"permission":"MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberLeaveEventQuit", json::parse(R"(
			{
				"type": "MemberLeaveEventQuit",
				"member": 
				{
					"id": 1234,
					"memberName": "run",
					"permission": "MEMBER",
					"group":
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberCardChangeEvent", json::parse(R"(
			{
				"type": "MemberCardChangeEvent",
				"origin": "old",
				"current": "new",
				"member": 
				{
					"id":1234567890,
					"memberName":"aaa",
					"specialTitle":"xxx",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining":0,
					"group":
					{
						"id":12345,
						"name":"xxx",
						"permission":"MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberSpecialTitleChangeEvent", json::parse(R"(
			{
				"type": "MemberSpecialTitleChangeEvent",
				"origin": "origin",
				"current": "new",
				"member": 
				{
					"id": 1234,
					"memberName": "bbb",
					"permission": "MEMBER",
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberPermissionChangeEvent", json::parse(R"(
			{
				"type": "MemberPermissionChangeEvent",
				"origin": "MEMBER",
				"current": "ADMINISTRATOR",
				"member": 
				{
					"id": 1234,
					"memberName": "bbb",
					"permission": "MEMBER",
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberMuteEvent", json::parse(R"(
			{
				"type": "MemberMuteEvent",
				"durationSeconds": 600,
				"member": 
				{
					"id":1234,
					"memberName":"aaa",
					"specialTitle":"xxx",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining": 600,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				},
				"operator":
				{
					"id":4321,
					"memberName":"bbb",
					"specialTitle":"xxx",
					"permission":"OWNER",
					"joinTimestamp":1619334595,
					"lastSpeakTimestamp":1660370799,
					"muteTimeRemaining":0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				}
			}
		)")
	},
	{
		"MemberUnmuteEvent", json::parse(R"(
			{
				"type": "MemberUnmuteEvent",
				"member": 
				{
					"id":1234,
					"memberName":"aaa",
					"specialTitle":"xxx",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining": 0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "OWNER"
					}
				},
				"operator": null
			}
		)")
	},
	{
		"MemberHonorChangeEvent", json::parse(R"(
			{
				"type": "MemberHonorChangeEvent",
				"member": 
				{
					"id":1234,
					"memberName":"aaa",
					"specialTitle":"xxx",
					"permission":"MEMBER",
					"joinTimestamp":12345678,
					"lastSpeakTimestamp":8765432,
					"muteTimeRemaining": 0,
					"group": 
					{
						"id": 123456789,
						"name": "Mirai Technology",
						"permission": "MEMBER"
					}
				},
				"action": "achieve",
				"honor": "龙王"
			}
		)")
	},
	{
		"NewFriendRequestEvent", json::parse(R"(
			{
				"type": "NewFriendRequestEvent",
				"eventId": 12345678,
				"fromId": 123456,
				"groupId": 0,
				"nick": "xxx",
				"message": "hehe"
			}
		)")
	},
	{
		"MemberJoinRequestEvent", json::parse(R"(
			{
				"type": "MemberJoinRequestEvent",
				"eventId": 12345678,
				"fromId": 123456,
				"groupId": 654321,
				"groupName": "Group",
				"nick": "xxx",
				"message": "hehe",
				"invitorId": 88888888
			}
		)")
	},
	{
		"BotInvitedJoinGroupRequestEvent", json::parse(R"(
			{
				"type": "BotInvitedJoinGroupRequestEvent",
				"eventId": 12345678,
				"fromId": 123456,
				"groupId": 654321,
				"groupName": "Group",
				"nick": "xxx",
				"message": "hehe"
			}
		)")
	},
	{
		"OtherClientOnlineEvent", json::parse(R"(
			{
				"type": "OtherClientOnlineEvent",
				"client": 
				{
					"id": 1,
					"platform": "WINDOWS"
				},
				"kind": 69899
			}
		)")
	},
	{
		"OtherClientOfflineEvent", json::parse(R"(
			{
				"type": "OtherClientOfflineEvent",
				"client": 
				{
					"id": 1,
					"platform": "WINDOWS"
				}
			}
		)")
	},
	{
		"CommandExecutedEvent", json::parse(R"(
			{
				"type": "CommandExecutedEvent",
				"name": "shutdown",
				"friend": 
				{
					"id": 123123,
					"nickname": "nick",
					"remark": "remark"
				},
				"member": null,
				"args": 
				[
				{
					"type": "Plain",
					"text": "myself"
				}
				]
			}
		)")
	},
	{
		"FriendMessage", json::parse(R"(
			{
				"type": "FriendMessage",
				"sender": 
				{
					"id": 123,
					"nickname": "xxx",
					"remark": "who"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"GroupMessage", json::parse(R"(
			{
				"type": "GroupMessage",
				"sender": 
				{
					"id": 123,
					"memberName": "aaa",
					"specialTitle": "xxx",
					"permission": "MEMBER",
					"joinTimestamp": 123,
					"lastSpeakTimestamp": 456,
					"muteTimeRemaining": 0,
					"group": 
					{
						"id": 1234567890,
						"name": "bbb",
						"permission": "ADMINISTRATOR"
					}
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"TempMessage", json::parse(R"(
			{
				"type": "TempMessage",
				"sender": {
					"id": 123,
					"memberName": "aaa",
					"specialTitle": "xxx",
					"permission": "MEMBER",
					"joinTimestamp": 123,
					"lastSpeakTimestamp": 456,
					"muteTimeRemaining": 0,
					"group": 
					{
						"id": 1234567890,
						"name": "bbb",
						"permission": "ADMINISTRATOR"
					}
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"StrangerMessage", json::parse(R"(
			{
				"type": "StrangerMessage",
				"sender": 
				{
					"id": 123,
					"nickname": "xxx",
					"remark": "who"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"OtherClientMessage", json::parse(R"(
			{
				"type": "OtherClientMessage",
				"sender": 
				{
					"id": 123,
					"platform": "MOBILE"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"FriendSyncMessage", json::parse(R"(
			{
				"type": "FriendSyncMessage",
				"subject": 
				{
					"id": 123,
					"nickname": "xxx",
					"remark": "who"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"GroupSyncMessage", json::parse(R"(
			{
				"type": "GroupSyncMessage",
				"subject": 
				{
					"id": 1234567890,
					"name": "bbb",
					"permission": "MEMBER"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"TempSyncMessage", json::parse(R"(
			{
				"type": "TempSyncMessage",
				"subject": {
					"id": 123,
					"memberName": "aaa",
					"specialTitle": "xxx",
					"permission": "MEMBER",
					"joinTimestamp": 123,
					"lastSpeakTimestamp": 456,
					"muteTimeRemaining": 0,
					"group": 
					{
						"id": 1234567890,
						"name": "bbb",
						"permission": "ADMINISTRATOR"
					}
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	},
	{
		"StrangerSyncMessage", json::parse(R"(
			{
				"type": "StrangerSyncMessage",
				"subject": 
				{
					"id": 123,
					"nickname": "xxx",
					"remark": "who"
				},
				"messageChain": [{
					"type": "Plain",
					"text": "hello"
				}]
			}
		)")
	}
};

}