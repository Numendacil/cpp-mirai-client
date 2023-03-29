#include <gtest/gtest.h>

#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include "libmirai/Types/BasicTypes.hpp"
#include <string>
#include <thread>

using json = nlohmann::json;
using namespace Mirai;
using namespace std::literals;

// NOLINTBEGIN(*)
TEST(TypesTest, IDTypes)
{
	constexpr UID_t id = 1234_qq;
	static_assert(int64_t(id) == int64_t(1234_gid));
	json j = id;
	EXPECT_EQ(j.get<int64_t>(), int64_t(id));
}

TEST(TypesTest, EnumTypes)
{
	json j;

	j = SEX::MALE;
	EXPECT_EQ(j.get<std::string>(), "MALE");
	j = PERMISSION::MEMBER;
	EXPECT_EQ(j.get<std::string>(), "MEMBER");
	j = NudgeType::STRANGER;
	EXPECT_EQ(j.get<std::string>(), "Stranger");
	j = MusicShareType::MIGUMUSIC;
	EXPECT_EQ(j.get<std::string>(), "MiguMusic");
	j = PokeType::CHUOYICHUO;
	EXPECT_EQ(j.get<std::string>(), "ChuoYiChuo");
	j = HonorChangeType::ACHIEVE;
	EXPECT_EQ(j.get<std::string>(), "achieve");
}

TEST(TypesTest, User)
{
	json j;

	User u;
	u.id = 12345_qq;
	u.nickname = "xxx";
	u.remark = "abc";
	j = u;
	EXPECT_EQ(j.at("id").get<QQ_t>(), u.id);
	EXPECT_EQ(j.at("nickname").get<std::string>(), u.nickname);
	EXPECT_EQ(j.at("remark").get<std::string>(), u.remark);
	User u1 = j.get<User>();
	EXPECT_EQ(u1, u);
}

TEST(TypesTest, Group)
{
	json j;

	Group g;
	g.id = 12345_gid;
	g.name = "xxx";
	g.permission = PERMISSION::ADMINISTRATOR;
	j = g;
	EXPECT_EQ(j.at("id").get<GID_t>(), g.id);
	EXPECT_EQ(j.at("name").get<std::string>(), g.name);
	EXPECT_EQ(j.at("permission").get<PERMISSION>(), g.permission);
	Group g1 = j.get<Group>();
	EXPECT_EQ(g1, g);
}

TEST(TypesTest, GroupMember)
{
	json j;

	GroupMember m;
	m.id = 1234567890_qq;
	m.MemberName = "member_name";
	m.permission = PERMISSION::OWNER;
	m.SpecialTitle = "title";
	m.JoinTimestamp = 8888;
	m.LastSpeakTimestamp = 9999;
	m.MuteTimeRemaining = 1s;
	m.group.id = 12345_gid;
	m.group.name = "xxx";
	m.group.permission = PERMISSION::ADMINISTRATOR;
	j = m;
	EXPECT_EQ(j.at("id").get<QQ_t>(), m.id);
	EXPECT_EQ(j.at("memberName").get<std::string>(), m.MemberName);
	EXPECT_EQ(j.at("permission").get<PERMISSION>(), m.permission);
	EXPECT_EQ(j.at("specialTitle").get<std::string>(), m.SpecialTitle);
	EXPECT_EQ(j.at("joinTimestamp").get<std::time_t>(), m.JoinTimestamp);
	EXPECT_EQ(j.at("lastSpeakTimestamp").get<std::time_t>(), m.LastSpeakTimestamp);
	EXPECT_EQ(j.at("muteTimeRemaining").get<std::time_t>(), m.MuteTimeRemaining.count());
	EXPECT_EQ(j.at("group").get<Group>(), m.group);
	GroupMember m1 = j.get<GroupMember>();
	EXPECT_EQ(m1, m);
}

TEST(TypesTest, UserProfile)
{
	json j;

	UserProfile u;
	u.nickname = "nickname";
	u.email = "email@example.com";
	u.age = 18;
	u.level = 1;
	u.sign = "mirai";
	u.sex = SEX::UNKNOWN;
	j = u;
	EXPECT_EQ(j.at("nickname").get<std::string>(), u.nickname);
	EXPECT_EQ(j.at("email").get<std::string>(), u.email);
	EXPECT_EQ(j.at("age").get<int>(), u.age);
	EXPECT_EQ(j.at("level").get<int>(), u.level);
	EXPECT_EQ(j.at("sign").get<std::string>(), u.sign);
	EXPECT_EQ(j.at("sex").get<SEX>(), u.sex);
	UserProfile u1 = j.get<UserProfile>();
	EXPECT_EQ(u1, u);
}

TEST(TypesTest, ClientDevice)
{
	json j;

	ClientDevice d;
	d.id = 123;
	d.platform = "PHONE";
	j = d;
	EXPECT_EQ(j.at("id").get<int64_t>(), d.id);
	EXPECT_EQ(j.at("platform").get<std::string>(), d.platform);
	ClientDevice d1 = j.get<ClientDevice>();
	EXPECT_EQ(d1, d);
}

TEST(TypesTest, GroupConfig)
{
	json j = json::parse(R"(
		{
			"name":"xxx",
			"confessTalk":true,
			"allowMemberInvite":true,
			"autoApprove":false,
			"anonymousChat":true,
			"muteAll":false
		}
	)");

	GroupConfig config;
	j.get_to(config);
	EXPECT_EQ(config.name, "xxx");
	EXPECT_EQ(config.AllowConfessTalk, true);
	EXPECT_EQ(config.AllowMemberInvite, true);
	EXPECT_EQ(config.AutoApprove, false);
	EXPECT_EQ(config.AllowAnonymousChat, true);
	EXPECT_EQ(config.MuteAll, false);
}

TEST(TypesTest, GroupAnnouncement)
{
	json j = json::parse(R"(
		{
			"group": 
			{
				"id": 123456789, 
				"name": "group name", 
				"permission": "ADMINISTRATOR"
			},
			"content": "xxx",
			"senderId": 987654321,
			"fid": "id",
			"allConfirmed": false,
			"confirmedMembersCount": 1,
			"publicationTime": 1645085843
		}
	)");

	GroupAnnouncement anno;
	j.get_to(anno);
	EXPECT_EQ(anno.group.id, 123456789_gid);
	EXPECT_EQ(anno.group.name, "group name");
	EXPECT_EQ(anno.group.permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(anno.content, "xxx");
	EXPECT_EQ(anno.SenderId, 987654321_qq);
	EXPECT_EQ(anno.fid, "id");
	EXPECT_EQ(anno.AllConfirmed, false);
	EXPECT_EQ(anno.ConfirmedCount, 1);
	EXPECT_EQ(anno.PublicationTime, 1645085843);
}

TEST(TypesTest, GroupFileInfo)
{
	json j = json::parse(R"(
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
	)");

	{
		GroupFileInfo info;
		j.get_to(info);
		EXPECT_EQ(info.name, "image.png");
		EXPECT_EQ(info.id, "/12314d-1wf13-a98ffa");
		EXPECT_EQ(info.path, "/image.png");

		EXPECT_TRUE(info.parent);
		EXPECT_EQ(info.parent->name, "/");
		EXPECT_EQ(info.parent->id, "/");
		EXPECT_EQ(info.parent->path, "/");
		EXPECT_FALSE(info.parent->parent);
		EXPECT_EQ(info.parent->contact.id, 123123_gid);
		EXPECT_EQ(info.parent->contact.name, "group");
		EXPECT_EQ(info.parent->contact.permission, PERMISSION::OWNER);
		EXPECT_FALSE(info.parent->isFile);
		EXPECT_FALSE(info.parent->file);

		EXPECT_EQ(info.contact.id, 123123_gid);
		EXPECT_EQ(info.contact.name, "group");
		EXPECT_EQ(info.contact.permission, PERMISSION::OWNER);
		EXPECT_TRUE(info.isFile);
		EXPECT_TRUE(info.file);
		EXPECT_EQ(info.file->md5, "9c21942cfd37f8a18fdbc6dbefe29e3f");
		EXPECT_EQ(info.file->sha1, "93c456b52c670cb1bfcebc8bd8b5d2ad467baf72");
		EXPECT_EQ(info.file->UploaderId, 123456789_qq);
		EXPECT_EQ(info.file->UploadTime, 1631153749);
		EXPECT_EQ(info.file->LastModifyTime, 1631153749);
		EXPECT_EQ(info.DownloadUrl, "example.com");
	}

	{
		GroupFileInfo info1;
		from_json(std::move(j), info1);
		EXPECT_EQ(info1.name, "image.png");
		EXPECT_EQ(info1.id, "/12314d-1wf13-a98ffa");
		EXPECT_EQ(info1.path, "/image.png");
		
		EXPECT_TRUE(info1.parent);
		EXPECT_EQ(info1.parent->name, "/");
		EXPECT_EQ(info1.parent->id, "/");
		EXPECT_EQ(info1.parent->path, "/");
		EXPECT_FALSE(info1.parent->parent);
		EXPECT_EQ(info1.parent->contact.id, 123123_gid);
		EXPECT_EQ(info1.parent->contact.name, "group");
		EXPECT_EQ(info1.parent->contact.permission, PERMISSION::OWNER);
		EXPECT_FALSE(info1.parent->isFile);
		EXPECT_FALSE(info1.parent->file);

		EXPECT_EQ(info1.contact.id, 123123_gid);
		EXPECT_EQ(info1.contact.name, "group");
		EXPECT_EQ(info1.contact.permission, PERMISSION::OWNER);
		EXPECT_EQ(info1.isFile, true);
		EXPECT_TRUE(info1.file);
		EXPECT_EQ(info1.file->md5, "9c21942cfd37f8a18fdbc6dbefe29e3f");
		EXPECT_EQ(info1.file->sha1, "93c456b52c670cb1bfcebc8bd8b5d2ad467baf72");
		EXPECT_EQ(info1.file->UploaderId, 123456789_qq);
		EXPECT_EQ(info1.file->UploadTime, 1631153749);
		EXPECT_EQ(info1.file->LastModifyTime, 1631153749);
		EXPECT_EQ(info1.DownloadUrl, "example.com");
	}
}

TEST(TypesTest, MiraiImage)
{
	json j = json::parse(R"(
		{
			"imageId":"image.jpg",
			"url":"http://example.com",
			"path":null,
			"base64":null,
			"width":1360,
			"height":1556,
			"size":1909306,
			"imageType":"JPG",
			"isEmoji":false
		}
	)");

	{

		MiraiImage img;
		j.get_to(img);
		EXPECT_EQ(img.id, "image.jpg");
		EXPECT_EQ(img.url, "http://example.com");
		EXPECT_EQ(img.path, "");
		EXPECT_EQ(img.base64, "");
		EXPECT_EQ(img.width, 1360);
		EXPECT_EQ(img.height, 1556);
		EXPECT_EQ(img.size, 1909306);
		EXPECT_EQ(img.ImageType, "JPG");
		EXPECT_FALSE(img.isEmoji);

		json j1 = img;
		EXPECT_EQ(j1.at("imageId").get<std::string>(), "image.jpg");

		img.id.clear();
		img.url.clear();
		img.base64 = "base64";
		j1 = img;
		EXPECT_EQ(j1.at("base64").get<std::string>(), "base64");

	}

	{

		MiraiImage img1;
		from_json(std::move(j), img1);
		EXPECT_EQ(img1.id, "image.jpg");
		EXPECT_EQ(img1.url, "http://example.com");
		EXPECT_EQ(img1.path, "");
		EXPECT_EQ(img1.base64, "");
		EXPECT_EQ(img1.width, 1360);
		EXPECT_EQ(img1.height, 1556);
		EXPECT_EQ(img1.size, 1909306);
		EXPECT_EQ(img1.ImageType, "JPG");
		EXPECT_FALSE(img1.isEmoji);

		img1.id.clear();
		img1.url.clear();
		img1.base64 = "base64";
		json j1 = std::move(img1);
		EXPECT_EQ(j1.at("base64").get<std::string>(), "base64");

	}
}

TEST(TypesTest, MiraiAudio)
{
	json j = json::parse(R"(
		{
			"voiceId":"audio.amr",
			"url":"http://example.com",
			"path":null,
			"base64":null,
			"length":5
		}
	)");

	{

		MiraiAudio audio;
		j.get_to(audio);
		EXPECT_EQ(audio.id, "audio.amr");
		EXPECT_EQ(audio.url, "http://example.com");
		EXPECT_EQ(audio.path, "");
		EXPECT_EQ(audio.base64, "");
		EXPECT_EQ(audio.length, 5);

		json j1 = audio;
		EXPECT_EQ(j1.at("voiceId").get<std::string>(), "audio.amr");

		audio.id.clear();
		audio.url.clear();
		audio.base64 = "base64";
		j1 = audio;
		EXPECT_EQ(j1.at("base64").get<std::string>(), "base64");

	}

	{

		MiraiAudio audio1;
		from_json(std::move(j), audio1);
		EXPECT_EQ(audio1.id, "audio.amr");
		EXPECT_EQ(audio1.url, "http://example.com");
		EXPECT_EQ(audio1.path, "");
		EXPECT_EQ(audio1.base64, "");
		EXPECT_EQ(audio1.length, 5);

		audio1.id.clear();
		audio1.url.clear();
		audio1.base64 = "base64";
		json j1 = std::move(audio1);
		EXPECT_EQ(j1.at("base64").get<std::string>(), "base64");

	}
}

// NOLINTEND(*)