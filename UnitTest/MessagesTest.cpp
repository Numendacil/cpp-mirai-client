#include <gtest/gtest.h>
#include <random>
#include <stdexcept>
#include <string>

#include <libmirai/Messages/Messages.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Serialization/Messages/MessageBase.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Messages/ForwardMessageNode.hpp>
#include <nlohmann/json.hpp>

#include "JsonData.hpp"
#include "libmirai/Messages/AtAllMessage.hpp"

using json = nlohmann::json;
using namespace Mirai;

TEST(MessagesTest, AppMessage)
{
	AppMessage app = Data::MessageData["AppMessage"].get<AppMessage>();

	EXPECT_TRUE(app.isValid());

	json j = app;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "App");

	EXPECT_EQ(app.GetContent(), "<>");
	app.SetContent("abc");
	j = app;
	EXPECT_EQ(j.value("content", ""), "abc");

	auto app1 = app.CloneUnique();
	std::shared_ptr<MessageBase> app2 = app.CloneUnique();
	json j1 = *app1;
	json j2 = *app2;
	EXPECT_EQ(j1, j2);

}

TEST(MessagesTest, AtAllMessage)
{
	AtAllMessage atall = Data::MessageData["AtAllMessage"].get<AtAllMessage>();

	EXPECT_TRUE(atall.isValid());
	
	json j = atall;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "AtAll");
	EXPECT_FALSE(j.empty());
	json j_ = AtAllMessage{};
	EXPECT_EQ(j, j_);

	auto atall1 = atall.CloneUnique();
	std::shared_ptr<MessageBase> atall2 = atall.CloneUnique();
	json j1 = *atall1;
	json j2 = *atall2;
	EXPECT_EQ(j1, j2);

}

TEST(MessagesTest, AtMessage)
{
	AtMessage at = Data::MessageData["AtMessage"].get<AtMessage>();

	EXPECT_TRUE(at.isValid());
	
	json j = at;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "At");
	
	auto at1 = at.CloneUnique();
	std::shared_ptr<MessageBase> at2= at.CloneUnique();
	json j1 = *at1;
	json j2 = *at2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(at.GetTarget(), 123456_qq);
	EXPECT_EQ(at.GetDisplayName(), "@Mirai");
	AtMessage msg;
	msg.SetTarget(987654_qq);
	EXPECT_EQ(msg.GetTarget(), 987654_qq);
	EXPECT_TRUE(msg.isValid());
	j = msg;
	EXPECT_FALSE(j.contains("display"));
}

TEST(MessagesTest, AudioMessage)
{
	AudioMessage audio = Data::MessageData["AudioMessage"].get<AudioMessage>();

	EXPECT_TRUE(audio.isValid());
	
	json j = audio;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Voice");
	
	auto audio1 = audio.CloneUnique();
	std::shared_ptr<MessageBase> audio2= audio.CloneUnique();
	json j1 = *audio1;
	json j2 = *audio2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(audio.GetAudio().id, "23C477720A37FEB6A9EE4BCCF654014F.amr");
	EXPECT_EQ(audio.GetAudio().url, "https://example.com");
	EXPECT_TRUE(audio.GetAudio().path.empty());
	EXPECT_TRUE(audio.GetAudio().base64.empty());
	EXPECT_EQ(audio.GetAudio().length, 1024);
	audio.SetPath("/tmp/a.slk");
	audio.SetAudioId("abcd.amr");
	j = audio;
	EXPECT_EQ(j.value("voiceId", ""), "abcd.amr");
}

TEST(MessagesTest, DiceMessage)
{
	DiceMessage dice = Data::MessageData["DiceMessage"].get<DiceMessage>();

	EXPECT_TRUE(dice.isValid());
	
	json j = dice;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Dice");
	
	auto dice1 = dice.CloneUnique();
	std::shared_ptr<MessageBase> dice2= dice.CloneUnique();
	json j1 = *dice1;
	json j2 = *dice2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(dice.GetValue(), 1);
	dice.SetValue(7);
	j = dice;
	EXPECT_EQ(j.value("value", 0), 1);
	dice = DiceMessage(8);
	j = dice;
	EXPECT_FALSE(dice.isValid());
}

TEST(MessagesTest, FaceMessage)
{
	FaceMessage face = Data::MessageData["FaceMessage"].get<FaceMessage>();

	EXPECT_TRUE(face.isValid());
	
	json j = face;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Face");
	
	auto face1 = face.CloneUnique();
	std::shared_ptr<MessageBase> face2= face.CloneUnique();
	json j1 = *face1;
	json j2 = *face2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(face.GetId(), 123);
	EXPECT_EQ(face.GetName(), "bu");
	face.SetName("waht");
	face.SetId(456);
	j = face;
	EXPECT_EQ(j.value("faceId", 123), 456);
}

TEST(MessagesTest, FileMessage)
{
	FileMessage file = Data::MessageData["FileMessage"].get<FileMessage>();

	EXPECT_TRUE(file.isValid());
	
	json j = file;
	EXPECT_NO_THROW(j);
	
	auto file1 = file.CloneUnique();
	std::shared_ptr<MessageBase> file2= file.CloneUnique();
	json j1 = *file1;
	json j2 = *file2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(file.GetId(), "id");
	EXPECT_EQ(file.GetName(), "xxx");
	EXPECT_EQ(file.GetSize(), 128);
	EXPECT_TRUE(j.empty());
}

TEST(MessagesTest, FlashImageMessage)
{
	FlashImageMessage flashimage = Data::MessageData["FlashImageMessage"].get<FlashImageMessage>();

	EXPECT_TRUE(flashimage.isValid());
	
	json j = flashimage;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "FlashImage");
	
	auto flashimage1 = flashimage.CloneUnique();
	std::shared_ptr<MessageBase> flashimage2= flashimage.CloneUnique();
	json j1 = *flashimage1;
	json j2 = *flashimage2;
	EXPECT_EQ(j1, j2);

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
	j = flashimage;
	EXPECT_EQ(j.value("imageId", ""), "abcd.png");
}

TEST(MessagesTest, ForwardMessage)
{
	ForwardMessage forward = Data::MessageData["ForwardMessage"].get<ForwardMessage>();

	EXPECT_TRUE(forward.isValid());
	
	json j = forward;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Forward");
	
	auto forward1 = forward.CloneUnique();
	std::shared_ptr<MessageBase> forward2= forward.CloneUnique();
	json j1 = *forward1;
	json j2 = *forward2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(forward.size(), 2);
	auto node = forward[0];
	EXPECT_EQ(node.GetSenderName(), "aaa");
	EXPECT_EQ(node.GetMessageChain().size(), 1);
	auto message = forward[1].GetMessageChain();
	EXPECT_EQ(message.size(), 1);
	EXPECT_EQ(message[0].GetType(), MessageTypes::FORWARD);
	auto msg = message.GetAt<ForwardMessage>(0);
	EXPECT_EQ(msg.size(), 1);
	EXPECT_FALSE(msg[0].hasMessageId());
	EXPECT_TRUE(msg[0].isValid());
}

TEST(MessagesTest, ImageMessage)
{
	ImageMessage image = Data::MessageData["ImageMessage"].get<ImageMessage>();

	EXPECT_TRUE(image.isValid());
	
	json j = image;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Image");
	
	auto image1 = image.CloneUnique();
	std::shared_ptr<MessageBase> image2= image.CloneUnique();
	json j1 = *image1;
	json j2 = *image2;
	EXPECT_EQ(j1, j2);

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
	j = image;
	EXPECT_EQ(j.value("imageId", ""), "abcd.png");
}

TEST(MessagesTest, JsonMessage)
{
	JsonMessage js = Data::MessageData["JsonMessage"].get<JsonMessage>();
	EXPECT_TRUE(js.isValid());
	
	json j = js;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Json");
	
	auto js1 = js.CloneUnique();
	std::shared_ptr<MessageBase> js2= js.CloneUnique();
	json j1 = *js1;
	json j2 = *js2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(js.GetContent(), "{\"text\": \"hello\"}");
	js.SetContent("abc");
	j = js;
	EXPECT_EQ(j.value("json", ""), "abc");
}

TEST(MessagesTest, MarketFaceMessage)
{
	MarketFaceMessage marketface = Data::MessageData["MarketFaceMessage"].get<MarketFaceMessage>();

	EXPECT_TRUE(marketface.isValid());
	
	json j = marketface;
	EXPECT_NO_THROW(j);
	
	auto marketface1 = marketface.CloneUnique();
	std::shared_ptr<MessageBase> marketface2= marketface.CloneUnique();
	json j1 = *marketface1;
	json j2 = *marketface2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(marketface.GetId(), 123);
	EXPECT_EQ(marketface.GetName(), "商城表情");
	marketface.SetName("waht");
	marketface.SetId(456);
	j = marketface;
	EXPECT_TRUE(j.empty());
}

TEST(MessagesTest, MiraiCodeMessage)
{
	MiraiCodeMessage miraicode = Data::MessageData["MiraiCodeMessage"].get<MiraiCodeMessage>();

	EXPECT_TRUE(miraicode.isValid());
	
	json j = miraicode;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "MiraiCode");
	
	auto miraicode1 = miraicode.CloneUnique();
	std::shared_ptr<MessageBase> miraicode2= miraicode.CloneUnique();
	json j1 = *miraicode1;
	json j2 = *miraicode2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(miraicode.GetCode(), "hello[mirai:at:1234567]");
	miraicode.SetCode("[mirai:image:{ABCDEFG}.png]");
	j = miraicode;
	EXPECT_EQ(j.value("code", ""), "[mirai:image:{ABCDEFG}.png]");
}

TEST(MessagesTest, MusicShareMessage)
{
	MusicShareMessage musicshare = Data::MessageData["MusicShareMessage"].get<MusicShareMessage>();

	EXPECT_TRUE(musicshare.isValid());
	
	json j = musicshare;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "MusicShare");
	
	auto musicshare1 = musicshare.CloneUnique();
	std::shared_ptr<MessageBase> musicshare2= musicshare.CloneUnique();
	json j1 = *musicshare1;
	json j2 = *musicshare2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(musicshare.GetKind(), MusicShareType::NETEASECLOUDMUSIC);
	EXPECT_EQ(j, json::parse(R"(
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
	musicshare.SetKind(MusicShareType::KUGOUMUSIC);
	j = musicshare;
	EXPECT_EQ(j.value("kind", ""), "KugouMusic");
}

TEST(MessagesTest, PlainMessage)
{
	PlainMessage plain = Data::MessageData["PlainMessage"].get<PlainMessage>();

	EXPECT_TRUE(plain.isValid());
	
	json j = plain;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Plain");
	
	auto plain1 = plain.CloneUnique();
	std::shared_ptr<MessageBase> plain2= plain.CloneUnique();
	json j1 = *plain1;
	json j2 = *plain2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(plain.GetText(), "hello");
	plain = PlainMessage("abc");
	j = plain;
	EXPECT_EQ(j.value("text", ""), "abc");
}

TEST(MessagesTest, PokeMessage)
{
	PokeMessage poke = Data::MessageData["PokeMessage"].get<PokeMessage>();

	EXPECT_TRUE(poke.isValid());
	
	json j = poke;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Poke");
	
	auto poke1 = poke.CloneUnique();
	std::shared_ptr<MessageBase> poke2= poke.CloneUnique();
	json j1 = *poke1;
	json j2 = *poke2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(poke.GetPokeKind(), PokeType::DIANZAN);
	poke.SetPokeKind(PokeType::ROSE);
	j = poke;
	EXPECT_EQ(j.value("name", ""), "Rose");
}

TEST(MessagesTest, QuoteMessage)
{
	QuoteMessage quote = Data::MessageData["QuoteMessage"].get<QuoteMessage>();

	EXPECT_TRUE(quote.isValid());
	
	json j = quote;
	EXPECT_NO_THROW(j);
	
	auto quote1 = quote.CloneUnique();
	std::shared_ptr<MessageBase> quote2= quote.CloneUnique();
	json j1 = *quote1;
	json j2 = *quote2;
	EXPECT_EQ(j1, j2);

	EXPECT_NE(quote.GetTargetId(), 123456789_qq);
	EXPECT_EQ(quote.GetSenderId(), 987654321_qq);
	json j_ = quote.GetOriginMessage()[0];
	EXPECT_EQ(j_, json::parse(R"({ "type": "Plain", "text": "text" })"));
	EXPECT_TRUE(j.empty());
}

TEST(MessagesTest, SourceMessage)
{
	SourceMessage source = Data::MessageData["SourceMessage"].get<SourceMessage>();

	EXPECT_TRUE(source.isValid());
	
	json j = source;
	EXPECT_NO_THROW(j);
	
	auto source1 = source.CloneUnique();
	std::shared_ptr<MessageBase> source2= source.CloneUnique();
	json j1 = *source1;
	json j2 = *source2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(source.GetTimestamp(), 123456);
	EXPECT_EQ(source.GetMessageId(), 654321);
	EXPECT_TRUE(j.empty());
}

TEST(MessagesTest, XmlMessage)
{
	XmlMessage xml = Data::MessageData["XmlMessage"].get<XmlMessage>();

	EXPECT_TRUE(xml.isValid());
	
	json j = xml;
	EXPECT_NO_THROW(j);
	EXPECT_EQ(j.value("type", ""), "Xml");
	
	auto xml1 = xml.CloneUnique();
	std::shared_ptr<MessageBase> xml2= xml.CloneUnique();
	json j1 = *xml1;
	json j2 = *xml2;
	EXPECT_EQ(j1, j2);

	EXPECT_EQ(xml.GetContent(), "<XML>");
	xml.SetContent("abc");
	j = xml;
	EXPECT_EQ(j.value("xml", ""), "abc");
}

TEST(MessagesTest, TemplateTest)
{
	MessageChain m;
	m.Append(PlainMessage{"aaa"});
	m.Append<ImageMessage>("1", "2", "3", "4");
	m.Append(SourceMessage{});
	m.Append(PlainMessage{"bbb"});
	m.Append<PokeMessage>(PokeMessage{PokeType::ENUM_END});
	m.Append<MessageTypes::MUSIC_SHARE>(MusicShareType::NETEASECLOUDMUSIC, "a", "a", "a", "a", "a", "a");

	EXPECT_EQ(m.GetAt<PokeMessage>(4).isValid(), false);
	const MessageChain m_const = m;
	json j = m_const.GetAt<MusicShareMessage>(5);
	EXPECT_EQ(j["kind"].get<std::string>(), "NeteaseCloudMusic");
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
	EXPECT_EQ(m3.GetAt<MessageTypes::MARKET_FACE>(4).GetType(), MessageTypes::MARKET_FACE);
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
	json j = m;
	EXPECT_TRUE(j.empty());

	auto m2 = PlainMessage("waht") + m;
	auto m3 = m + PlainMessage("waht");
	auto m4 = m3 + m;
	EXPECT_EQ(m4.size(), 5);
	EXPECT_EQ(j, j);
	EXPECT_EQ(j, j);
}

TEST(MessagesTest, SerializationTest)
{
	MessageChain m = json::parse("[]");
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
		EXPECT_EQ(m[i].GetType(), msg[i]["type"].get<MessageTypes>());
	}
}