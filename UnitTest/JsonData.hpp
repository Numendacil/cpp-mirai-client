#include <nlohmann/json.hpp>

namespace Data
{

using json = nlohmann::json;

static json MessageData = 
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
					"messageChain": [
					{
						"type": "Plain",
						"text": "Hello"
					}
					],
					"messageId": 321
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
							"messageId": null
						}
						]
					}
					],
					"messageId": 654
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

}