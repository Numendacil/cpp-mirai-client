#include <gtest/gtest.h>
#include <random>
#include <stdexcept>
#include <string>

#include <libmirai/Messages/Messages.hpp>
#include <nlohmann/json.hpp>

#include "JsonData.hpp"

using json = nlohmann::json;
using namespace Mirai;

TEST(MessagesTest, AppMessage)
{
	AppMessage app = Data::MessageData["AppMessage"];

	EXPECT_TRUE(app.isValid());
	EXPECT_NO_THROW(app.ToJson());
	EXPECT_EQ(app.ToJson().value("type", ""), "App");
	auto app1 = app.CloneUnique();
	std::shared_ptr<MessageBase> app2 = app.CloneUnique();
	EXPECT_EQ(app1->ToJson(), app2->ToJson());

	EXPECT_EQ(app.GetContent(), "<>");
	app.SetContent("abc");
	EXPECT_EQ(app.ToJson().value("content", ""), "abc");
}

TEST(MessagesTest, AtAllMessage)
{
	AtAllMessage atall = Data::MessageData["AtAllMessage"];

	EXPECT_TRUE(atall.isValid());
	EXPECT_NO_THROW(atall.ToJson());
	EXPECT_EQ(atall.ToJson().value("type", ""), "AtAll");
	auto atall1 = atall.CloneUnique();
	std::shared_ptr<MessageBase> atall2 = atall.CloneUnique();
	EXPECT_EQ(atall1->ToJson(), atall2->ToJson());

	EXPECT_FALSE(atall.ToJson().empty());
	EXPECT_EQ(atall.ToJson(), AtAllMessage{}.ToJson());
}

TEST(MessagesTest, AtMessage)
{
	AtMessage at = Data::MessageData["AtMessage"];

	EXPECT_TRUE(at.isValid());
	EXPECT_NO_THROW(at.ToJson());
	EXPECT_EQ(at.ToJson().value("type", ""), "At");
	auto at1 = at.CloneUnique();
	std::shared_ptr<MessageBase> at2 = at.CloneUnique();
	EXPECT_EQ(at1->ToJson(), at2->ToJson());

	EXPECT_EQ(at.GetTarget(), 123456_qq);
	EXPECT_EQ(at.GetDisplayName(), "@Mirai");
	AtMessage msg;
	msg.SetTarget(987654_qq);
	EXPECT_EQ(msg.GetTarget(), 987654_qq);
	EXPECT_TRUE(msg.isValid());
	EXPECT_FALSE(msg.ToJson().contains("display"));
}

TEST(MessagesTest, AudioMessage)
{
	AudioMessage audio = Data::MessageData["AudioMessage"];

	EXPECT_TRUE(audio.isValid());
	EXPECT_NO_THROW(audio.ToJson());
	EXPECT_EQ(audio.ToJson().value("type", ""), "Voice");
	auto audio1 = audio.CloneUnique();
	std::shared_ptr<MessageBase> audio2 = audio.CloneUnique();
	EXPECT_EQ(audio1->ToJson(), audio2->ToJson());

	EXPECT_EQ(audio.GetAudio().id, "23C477720A37FEB6A9EE4BCCF654014F.amr");
	EXPECT_EQ(audio.GetAudio().url, "https://example.com");
	EXPECT_TRUE(audio.GetAudio().path.empty());
	EXPECT_TRUE(audio.GetAudio().base64.empty());
	EXPECT_EQ(audio.GetAudio().length, 1024);
	audio.SetPath("/tmp/a.slk");
	audio.SetAudioId("abcd.amr");
	EXPECT_EQ(audio.ToJson().value("voiceId", ""), "abcd.amr");
}

TEST(MessagesTest, DiceMessage)
{
	DiceMessage dice = Data::MessageData["DiceMessage"];

	EXPECT_TRUE(dice.isValid());
	EXPECT_NO_THROW(dice.ToJson());
	EXPECT_EQ(dice.ToJson().value("type", ""), "Dice");
	auto dice1 = dice.CloneUnique();
	std::shared_ptr<MessageBase> dice2 = dice.CloneUnique();
	EXPECT_EQ(dice1->ToJson(), dice2->ToJson());

	EXPECT_EQ(dice.GetValue(), 1);
	dice.SetValue(7);
	EXPECT_EQ(dice.ToJson().value("value", 0), 1);
	dice = DiceMessage(8);
	EXPECT_FALSE(dice.isValid());
}

TEST(MessagesTest, FaceMessage)
{
	FaceMessage face = Data::MessageData["FaceMessage"];

	EXPECT_TRUE(face.isValid());
	EXPECT_NO_THROW(face.ToJson());
	EXPECT_EQ(face.ToJson().value("type", ""), "Face");
	auto face1 = face.CloneUnique();
	std::shared_ptr<MessageBase> face2 = face.CloneUnique();
	EXPECT_EQ(face1->ToJson(), face2->ToJson());

	EXPECT_EQ(face.GetId(), 123);
	EXPECT_EQ(face.GetName(), "bu");
	face.SetName("waht");
	face.SetId(456);
	EXPECT_EQ(face.ToJson().value("faceId", 123), 456);
}

TEST(MessagesTest, FileMessage)
{
	FileMessage file = Data::MessageData["FileMessage"];

	EXPECT_TRUE(file.isValid());
	EXPECT_NO_THROW(file.ToJson());
	auto file1 = file.CloneUnique();
	std::shared_ptr<MessageBase> file2 = file.CloneUnique();
	EXPECT_EQ(file1->ToJson(), file2->ToJson());

	EXPECT_EQ(file.GetId(), "id");
	EXPECT_EQ(file.GetName(), "xxx");
	EXPECT_EQ(file.GetSize(), 128);
	EXPECT_TRUE(file.ToJson().empty());
}

TEST(MessagesTest, FlashImageMessage)
{
	FlashImageMessage flashimage = Data::MessageData["FlashImageMessage"];

	EXPECT_TRUE(flashimage.isValid());
	EXPECT_NO_THROW(flashimage.ToJson());
	EXPECT_EQ(flashimage.ToJson().value("type", ""), "FlashImage");
	auto flashimage1 = flashimage.CloneUnique();
	std::shared_ptr<MessageBase> flashimage2 = flashimage.CloneUnique();
	EXPECT_EQ(flashimage1->ToJson(), flashimage2->ToJson());

	EXPECT_EQ(flashimage.GetImage().id, "23C477720A37FEB6A9EE4BCCF654014F.png");
	EXPECT_EQ(flashimage.GetImage().url, "https://example.com");
	EXPECT_TRUE(flashimage.GetImage().path.empty());
	EXPECT_TRUE(flashimage.GetImage().base64.empty());
	EXPECT_EQ(flashimage.GetImage().size, 1024);
	EXPECT_EQ(flashimage.GetImage().width, 1920);
	EXPECT_EQ(flashimage.GetImage().height, 1080);
	EXPECT_EQ(flashimage.GetImage().ImageType, "PNG");
	flashimage.SetPath("/tmp/a.jpg");
	flashimage.SetImageId("abcd.png");
	EXPECT_EQ(flashimage.ToJson().value("imageId", ""), "abcd.png");
}

TEST(MessagesTest, ForwardMessage)
{
	ForwardMessage forward;
	forward.FromJson(Data::MessageData["ForwardMessage"]);

	EXPECT_TRUE(forward.isValid());
	EXPECT_NO_THROW(forward.ToJson());
	EXPECT_EQ(forward.ToJson().value("type", ""), "Forward");
	auto forward1 = forward.CloneUnique();
	std::shared_ptr<MessageBase> forward2 = forward.CloneUnique();
	EXPECT_EQ(forward1->ToJson(), forward2->ToJson());

	EXPECT_EQ(forward.size(), 2);
	auto node = forward[0];
	EXPECT_EQ(node.GetSenderName(), "aaa");
	EXPECT_EQ(node.GetMessageChain().size(), 1);
	auto message = forward[1].GetMessageChain();
	EXPECT_EQ(message.size(), 1);
	EXPECT_EQ(message[0]->GetType(), "Forward");
	auto msg = message.GetAt<ForwardMessage>(0);
	EXPECT_EQ(msg.size(), 1);
	EXPECT_FALSE(msg[0].hasMessageId());
	EXPECT_TRUE(msg[0].isValid());
}

TEST(MessagesTest, ImageMessage)
{
	ImageMessage image = Data::MessageData["ImageMessage"];

	EXPECT_TRUE(image.isValid());
	EXPECT_NO_THROW(image.ToJson());
	EXPECT_EQ(image.ToJson().value("type", ""), "Image");
	auto image1 = image.CloneUnique();
	std::shared_ptr<MessageBase> image2 = image.CloneUnique();
	EXPECT_EQ(image1->ToJson(), image2->ToJson());

	EXPECT_EQ(image.GetImage().id, "23C477720A37FEB6A9EE4BCCF654014F.png");
	EXPECT_EQ(image.GetImage().url, "https://example.com");
	EXPECT_TRUE(image.GetImage().path.empty());
	EXPECT_TRUE(image.GetImage().base64.empty());
	EXPECT_EQ(image.GetImage().size, 1024);
	EXPECT_EQ(image.GetImage().width, 1920);
	EXPECT_EQ(image.GetImage().height, 1080);
	EXPECT_EQ(image.GetImage().ImageType, "PNG");
	image.SetPath("/tmp/a.jpg");
	image.SetImageId("abcd.png");
	EXPECT_EQ(image.ToJson().value("imageId", ""), "abcd.png");
}

TEST(MessagesTest, JsonMessage)
{
	JsonMessage j = Data::MessageData["JsonMessage"];
	EXPECT_TRUE(j.isValid());
	EXPECT_NO_THROW(j.ToJson());
	EXPECT_EQ(j.ToJson().value("type", ""), "Json");
	auto j1 = j.CloneUnique();
	std::shared_ptr<MessageBase> j2 = j.CloneUnique();
	EXPECT_EQ(j1->ToJson(), j2->ToJson());

	EXPECT_EQ(j.GetContent(), "{\"text\": \"hello\"}");
	EXPECT_EQ(j.GetJson(), json({{"text", "hello"}}));
	j.SetContent("abc");
	EXPECT_EQ(j.ToJson().value("json", ""), "abc");
}

TEST(MessagesTest, MarketFaceMessage)
{
	MarketFaceMessage marketface = json::parse(R"(
		{
			"type": "MarketFace",
			"id": 123,
			"name": "商城表情"
		}
	)");

	EXPECT_TRUE(marketface.isValid());
	EXPECT_NO_THROW(marketface.ToJson());
	auto marketface1 = marketface.CloneUnique();
	std::shared_ptr<MessageBase> marketface2 = marketface.CloneUnique();
	EXPECT_EQ(marketface1->ToJson(), marketface2->ToJson());

	EXPECT_EQ(marketface.GetId(), 123);
	EXPECT_EQ(marketface.GetName(), "商城表情");
	marketface.SetName("waht");
	marketface.SetId(456);
	EXPECT_TRUE(marketface.ToJson().empty());
}

TEST(MessagesTest, MiraiCodeMessage)
{
	MiraiCodeMessage miraicode = Data::MessageData["MiraiCodeMessage"];

	EXPECT_TRUE(miraicode.isValid());
	EXPECT_NO_THROW(miraicode.ToJson());
	EXPECT_EQ(miraicode.ToJson().value("type", ""), "MiraiCode");
	auto miraicode1 = miraicode.CloneUnique();
	std::shared_ptr<MessageBase> miraicode2 = miraicode.CloneUnique();
	EXPECT_EQ(miraicode1->ToJson(), miraicode2->ToJson());

	EXPECT_EQ(miraicode.GetCode(), "hello[mirai:at:1234567]");
	miraicode.SetCode("[mirai:image:{ABCDEFG}.png]");
	EXPECT_EQ(miraicode.ToJson().value("code", ""), "[mirai:image:{ABCDEFG}.png]");
}

TEST(MessagesTest, MusicShareMessage)
{
	MusicShareMessage musicshare = Data::MessageData["MusicShareMessage"];

	EXPECT_TRUE(musicshare.isValid());
	EXPECT_NO_THROW(musicshare.ToJson());
	EXPECT_EQ(musicshare.ToJson().value("type", ""), "MusicShare");
	auto musicshare1 = musicshare.CloneUnique();
	std::shared_ptr<MessageBase> musicshare2 = musicshare.CloneUnique();
	EXPECT_EQ(musicshare1->ToJson(), musicshare2->ToJson());

	EXPECT_EQ(musicshare.GetKind(), MusicShareMessage::NETEASECLOUDMUSIC);
	EXPECT_EQ(musicshare.ToJson(), json::parse(R"(
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
	)"));
	musicshare.SetKind(MusicShareMessage::KUGOUMUSIC);
	EXPECT_EQ(musicshare.ToJson().value("kind", ""), "KugouMusic");
}

TEST(MessagesTest, PlainMessage)
{
	PlainMessage plain = Data::MessageData["PlainMessage"];

	EXPECT_TRUE(plain.isValid());
	EXPECT_NO_THROW(plain.ToJson());
	EXPECT_EQ(plain.ToJson().value("type", ""), "Plain");
	auto plain1 = plain.CloneUnique();
	std::shared_ptr<MessageBase> plain2 = plain.CloneUnique();
	EXPECT_EQ(plain1->ToJson(), plain2->ToJson());

	EXPECT_EQ(plain.GetText(), "hello");
	plain = PlainMessage("abc");
	EXPECT_EQ(plain.ToJson().value("text", ""), "abc");
}

TEST(MessagesTest, PokeMessage)
{
	PokeMessage poke = Data::MessageData["PokeMessage"];

	EXPECT_TRUE(poke.isValid());
	EXPECT_NO_THROW(poke.ToJson());
	EXPECT_EQ(poke.ToJson().value("type", ""), "Poke");
	auto poke1 = poke.CloneUnique();
	std::shared_ptr<MessageBase> poke2 = poke.CloneUnique();
	EXPECT_EQ(poke1->ToJson(), poke2->ToJson());

	EXPECT_EQ(poke.GetPokeKind(), PokeMessage::DIANZAN);
	poke.SetPokeKind(PokeMessage::ROSE);
	EXPECT_EQ(poke.ToJson().value("name", ""), "Rose");
}

TEST(MessagesTest, QuoteMessage)
{
	QuoteMessage quote = Data::MessageData["QuoteMessage"];

	EXPECT_TRUE(quote.isValid());
	EXPECT_NO_THROW(quote.ToJson());
	auto quote1 = quote.CloneUnique();
	std::shared_ptr<MessageBase> quote2 = quote.CloneUnique();
	EXPECT_EQ(quote1->ToJson(), quote2->ToJson());

	EXPECT_NE(quote.GetTargetId(), 123456789_qq);
	EXPECT_EQ(quote.GetSenderId(), 987654321_qq);
	EXPECT_EQ(quote.GetOriginMessage()[0]->ToJson(), json::parse(R"({ "type": "Plain", "text": "text" })"));
	EXPECT_TRUE(quote.ToJson().empty());
}

TEST(MessagesTest, SourceMessage)
{
	SourceMessage source = Data::MessageData["SourceMessage"];

	EXPECT_TRUE(source.isValid());
	EXPECT_NO_THROW(source.ToJson());
	auto source1 = source.CloneUnique();
	std::shared_ptr<MessageBase> source2 = source.CloneUnique();
	EXPECT_EQ(source1->ToJson(), source2->ToJson());

	EXPECT_EQ(source.GetTimestamp(), 123456);
	EXPECT_EQ(source.GetMessageId(), 654321);
	EXPECT_TRUE(source.ToJson().empty());
}

TEST(MessagesTest, XmlMessage)
{
	XmlMessage xml = Data::MessageData["XmlMessage"];

	EXPECT_TRUE(xml.isValid());
	EXPECT_NO_THROW(xml.ToJson());
	EXPECT_EQ(xml.ToJson().value("type", ""), "Xml");
	auto xml1 = xml.CloneUnique();
	std::shared_ptr<MessageBase> xml2 = xml.CloneUnique();
	EXPECT_EQ(xml1->ToJson(), xml2->ToJson());

	EXPECT_EQ(xml.GetContent(), "<XML>");
	xml.SetContent("abc");
	EXPECT_EQ(xml.ToJson().value("xml", ""), "abc");
}

TEST(MessagesTest, TemplateTest)
{
	MessageChain m;
	m.Append(PlainMessage{"aaa"});
	m.Append<ImageMessage>("1", "2", "3", "4");
	m.Append(SourceMessage{});
	m.Append(PlainMessage{"bbb"});
	m.Append<PokeMessage>(PokeMessage{PokeMessage::PokeKind::UNKNOWN});
	m.Append<MusicShareMessage>(MusicShareMessage::MusicKind::NETEASECLOUDMUSIC, "a", "a", "a", "a", "a", "a");

	EXPECT_EQ(m.GetAt<PokeMessage>(4).isValid(), false);
	const MessageChain m_const = m;
	EXPECT_EQ(m_const.GetAt<MusicShareMessage>(5).ToJson()["kind"].get<std::string>(), "NeteaseCloudMusic");
	EXPECT_THROW(auto msg = m.GetAt<PlainMessage>(1), TypeDismatch);

	m.Insert<PlainMessage>(m.begin() + 2, "ccc");
	auto plain = m.GetAll<PlainMessage>();
	EXPECT_EQ(plain.size(), 3);
	EXPECT_EQ(plain[1].GetText(), "ccc");
}

TEST(MessagesTest, ConstructorTest)
{
	MessageChain m;
	m.Append(AppMessage{});
	m.Append(AtAllMessage{});
	m.Append(AudioMessage{});
	m.Append(DiceMessage{});
	m.Append(MarketFaceMessage{});

	MessageChain m2 = std::move(m);
	MessageChain m3;
	m3 = m2;
	EXPECT_EQ(m2.size(), 5);
	EXPECT_EQ(m3.size(), 5);
	EXPECT_THROW(m.GetAt<AudioMessage>(2), std::out_of_range);
	EXPECT_EQ(m3.GetAt<MarketFaceMessage>(4).GetType(), "MarketFace");
	EXPECT_NE(&m2[2], &m3[2]);
}

TEST(MessagesTest, OperatorTest)
{
	MessageChain m;
	m += SourceMessage();
	m += PlainMessage("wow");
	EXPECT_EQ(m.size(), 2);
	m += m;
	EXPECT_EQ(m.size(), 4);
	m.erase(m.cbegin() + 1);
	m.erase(m.cbegin() + 2);
	EXPECT_TRUE(m.ToJson().empty());

	auto m2 = PlainMessage("waht") + m;
	auto m3 = m + PlainMessage("waht");
	auto m4 = m3 + m;
	EXPECT_EQ(m4.size(), 5);
	EXPECT_EQ(m2.ToJson(), m3.ToJson());
	EXPECT_EQ(m2.ToJson(), m4.ToJson());
}

TEST(MessagesTest, SerializationTest)
{
	MessageChain m;
	m.FromJson(json::parse("[]"));
	EXPECT_EQ(m.size(), 0);

	std::random_device dev;
	std::mt19937 rng(dev());
	json data;
	for (const auto& p : Data::MessageData.items())
	{
		data += p.value();
	}
	std::uniform_int_distribution<unsigned long> randidx(0, data.size() - 1);
	json msg;
	for (std::size_t i = 0; i < 10000; i++)
	{
		auto idx = randidx(rng);
		msg += data[idx];
	}
	m = msg.get<MessageChain>();
	EXPECT_EQ(m.size(), msg.size());
	for (std::size_t i = 0; i < msg.size(); i++)
	{
		EXPECT_EQ(m[i]->GetType(), msg[i]["type"].get<std::string>());
	}
}