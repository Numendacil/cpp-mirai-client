#include <random>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Messages/Messages.hpp>
#include <libmirai/Serialization/Messages/Messages.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

#include "JsonData.hpp"

using json = nlohmann::json;
using namespace Mirai;

TEST(MessagesTest, AppMessage)
{
	AppMessage app = Data::MessageData["AppMessage"].get<AppMessage>();

	EXPECT_TRUE(app.valid());
	EXPECT_TRUE(app.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = app);
	EXPECT_EQ(j["type"].get<MessageTypes>(), app.GetType());

	EXPECT_EQ(app.GetContent(), "<>");
	app.SetContent("abc");
	j = app;
	EXPECT_EQ(j.value("content", ""), "abc");

	auto app1 = app;
	auto app2 = std::move(app);
	json j1 = app1;
	json j2 = app2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, AtAllMessage)
{
	AtAllMessage atall = Data::MessageData["AtAllMessage"].get<AtAllMessage>();

	EXPECT_TRUE(atall.valid());
	EXPECT_TRUE(atall.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = atall);
	EXPECT_EQ(j["type"].get<MessageTypes>(), atall.GetType());

	EXPECT_FALSE(j.empty());
	json j_ = AtAllMessage{};
	EXPECT_EQ(j, j_);

	auto atall1 = atall;
	auto atall2 = std::move(atall);
	json j1 = atall1;
	json j2 = atall2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, AtMessage)
{
	AtMessage at = Data::MessageData["AtMessage"].get<AtMessage>();

	EXPECT_TRUE(at.valid());
	EXPECT_TRUE(at.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = at);
	EXPECT_EQ(j["type"].get<MessageTypes>(), at.GetType());

	EXPECT_EQ(at.GetTarget(), 123456_qq);
	EXPECT_EQ(at.GetDisplayName(), "@Mirai");
	AtMessage msg;
	msg.SetTarget(987654_qq);
	EXPECT_EQ(msg.GetTarget(), 987654_qq);
	EXPECT_TRUE(msg.valid());
	j = msg;
	EXPECT_FALSE(j.contains("display"));

	auto at1 = at;
	auto at2 = std::move(at);
	json j1 = at1;
	json j2 = at2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, AudioMessage)
{
	AudioMessage audio = Data::MessageData["AudioMessage"].get<AudioMessage>();

	EXPECT_TRUE(audio.valid());
	EXPECT_TRUE(audio.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = audio);
	EXPECT_EQ(j["type"].get<MessageTypes>(), audio.GetType());

	EXPECT_EQ(audio.GetAudio().id, "23C477720A37FEB6A9EE4BCCF654014F.amr");
	EXPECT_EQ(audio.GetAudio().url, "https://example.com");
	EXPECT_TRUE(audio.GetAudio().path.empty());
	EXPECT_TRUE(audio.GetAudio().base64.empty());
	EXPECT_EQ(audio.GetAudio().length, 1024);
	audio.SetPath("/tmp/a.slk");
	audio.SetAudioId("abcd.amr");
	j = audio;
	EXPECT_EQ(j.value("voiceId", ""), "abcd.amr");

	auto audio1 = audio;
	auto audio2 = std::move(audio);
	json j1 = audio1;
	json j2 = audio2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, DiceMessage)
{
	DiceMessage dice = Data::MessageData["DiceMessage"].get<DiceMessage>();

	EXPECT_TRUE(dice.valid());
	EXPECT_TRUE(dice.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = dice);
	EXPECT_EQ(j["type"].get<MessageTypes>(), dice.GetType());

	EXPECT_EQ(dice.GetValue(), 1);
	dice.SetValue(7);
	j = dice;
	EXPECT_EQ(j.value("value", 0), 1);
	dice = DiceMessage(8);
	j = dice;
	EXPECT_FALSE(dice.valid());

	auto dice1 = dice;
	auto dice2 = std::move(dice);
	json j1 = dice1;
	json j2 = dice2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, FaceMessage)
{
	FaceMessage face = Data::MessageData["FaceMessage"].get<FaceMessage>();

	EXPECT_TRUE(face.valid());
	EXPECT_TRUE(face.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = face);
	EXPECT_EQ(j["type"].get<MessageTypes>(), face.GetType());

	EXPECT_EQ(face.GetId(), 123);
	EXPECT_EQ(face.GetName(), "bu");
	face.SetName("waht");
	face.SetId(456);
	j = face;
	EXPECT_EQ(j.value("faceId", 123), 456);

	auto face1 = face;
	auto face2 = std::move(face);
	json j1 = face1;
	json j2 = face2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, FileMessage)
{
	FileMessage file = Data::MessageData["FileMessage"].get<FileMessage>();

	EXPECT_TRUE(file.valid());
	EXPECT_FALSE(file.isSendSupported());

	EXPECT_EQ(file.GetId(), "id");
	EXPECT_EQ(file.GetName(), "xxx");
	EXPECT_EQ(file.GetSize(), 128);

	auto file1 = file;
	auto file2 = std::move(file);
	EXPECT_EQ(file1.GetId(), file2.GetId());
	EXPECT_EQ(file1.GetName(), file2.GetName());
	EXPECT_EQ(file1.GetSize(), file2.GetSize());
}

TEST(MessagesTest, FlashImageMessage)
{
	FlashImageMessage flashimage = Data::MessageData["FlashImageMessage"].get<FlashImageMessage>();

	EXPECT_TRUE(flashimage.valid());
	EXPECT_TRUE(flashimage.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = flashimage);
	EXPECT_EQ(j["type"].get<MessageTypes>(), flashimage.GetType());

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

	auto flashimage1 = flashimage;
	auto flashimage2 = std::move(flashimage);
	json j1 = flashimage1;
	json j2 = flashimage2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, ForwardMessage)
{
	ForwardMessage forward = Data::MessageData["ForwardMessage"].get<ForwardMessage>();

	EXPECT_TRUE(forward.valid());
	EXPECT_TRUE(forward.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = forward);
	EXPECT_EQ(j["type"].get<MessageTypes>(), forward.GetType());

	EXPECT_EQ(forward.size(), 2);
	auto node = forward[0];
	EXPECT_EQ(node.GetSenderName(), "aaa");
	EXPECT_EQ(node.GetMessageChain().size(), 1);
	auto message = forward[1].GetMessageChain();
	EXPECT_EQ(message.size(), 1);
	EXPECT_EQ(message.GetType(0), MessageTypes::FORWARD);
	auto msg = message.GetAt<ForwardMessage>(0);
	EXPECT_EQ(msg.size(), 1);
	EXPECT_FALSE(msg[0].hasMessageId());
	EXPECT_TRUE(msg[0].valid());

	auto forward1 = forward;
	auto forward2 = std::move(forward);
	json j1 = forward1;
	json j2 = forward2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, ImageMessage)
{
	ImageMessage image = Data::MessageData["ImageMessage"].get<ImageMessage>();

	EXPECT_TRUE(image.valid());
	EXPECT_TRUE(image.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = image);
	EXPECT_EQ(j["type"].get<MessageTypes>(), image.GetType());

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

	auto image1 = image;
	auto image2 = std::move(image);
	json j1 = image1;
	json j2 = image2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, JsonMessage)
{
	JsonMessage js = Data::MessageData["JsonMessage"].get<JsonMessage>();
	EXPECT_TRUE(js.valid());
	EXPECT_TRUE(js.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = js);
	EXPECT_EQ(j["type"].get<MessageTypes>(), js.GetType());

	EXPECT_EQ(js.GetContent(), "{\"text\": \"hello\"}");
	js.SetContent("abc");
	j = js;
	EXPECT_EQ(j.value("json", ""), "abc");

	auto js1 = js;
	auto js2 = std::move(js);
	json j1 = js1;
	json j2 = js2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, MarketFaceMessage)
{
	MarketFaceMessage marketface = Data::MessageData["MarketFaceMessage"].get<MarketFaceMessage>();

	EXPECT_TRUE(marketface.valid());
	EXPECT_FALSE(marketface.isSendSupported());

	EXPECT_EQ(marketface.GetId(), 123);
	EXPECT_EQ(marketface.GetName(), "商城表情");

	auto marketface1 = marketface;
	auto marketface2 = std::move(marketface);
	EXPECT_EQ(marketface1.GetId(), marketface2.GetId());
	EXPECT_EQ(marketface1.GetName(), marketface2.GetName());
}

TEST(MessagesTest, MiraiCodeMessage)
{
	MiraiCodeMessage miraicode = Data::MessageData["MiraiCodeMessage"].get<MiraiCodeMessage>();

	EXPECT_TRUE(miraicode.valid());
	EXPECT_TRUE(miraicode.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = miraicode);
	EXPECT_EQ(j["type"].get<MessageTypes>(), miraicode.GetType());

	EXPECT_EQ(miraicode.GetCode(), "hello[mirai:at:1234567]");
	miraicode.SetCode("[mirai:image:{ABCDEFG}.png]");
	j = miraicode;
	EXPECT_EQ(j.value("code", ""), "[mirai:image:{ABCDEFG}.png]");

	auto miraicode1 = miraicode;
	auto miraicode2 = std::move(miraicode);
	json j1 = miraicode1;
	json j2 = miraicode2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, MusicShareMessage)
{
	MusicShareMessage musicshare = Data::MessageData["MusicShareMessage"].get<MusicShareMessage>();

	EXPECT_TRUE(musicshare.valid());
	EXPECT_TRUE(musicshare.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = musicshare);
	EXPECT_EQ(j["type"].get<MessageTypes>(), musicshare.GetType());

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

	auto musicshare1 = musicshare;
	auto musicshare2 = std::move(musicshare);
	json j1 = musicshare1;
	json j2 = musicshare2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, PlainMessage)
{
	PlainMessage plain = Data::MessageData["PlainMessage"].get<PlainMessage>();

	EXPECT_TRUE(plain.valid());
	EXPECT_TRUE(plain.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = plain);
	EXPECT_EQ(j["type"].get<MessageTypes>(), plain.GetType());

	EXPECT_EQ(plain.GetText(), "hello");
	plain = PlainMessage("abc");
	j = plain;
	EXPECT_EQ(j.value("text", ""), "abc");

	auto plain1 = plain;
	auto plain2 = std::move(plain);
	json j1 = plain1;
	json j2 = plain2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, PokeMessage)
{
	PokeMessage poke = Data::MessageData["PokeMessage"].get<PokeMessage>();

	EXPECT_TRUE(poke.valid());
	EXPECT_TRUE(poke.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = poke);
	EXPECT_EQ(j["type"].get<MessageTypes>(), poke.GetType());

	EXPECT_EQ(poke.GetPokeKind(), PokeType::DIANZAN);
	poke.SetPokeKind(PokeType::ROSE);
	j = poke;
	EXPECT_EQ(j.value("name", ""), "Rose");

	auto poke1 = poke;
	auto poke2 = std::move(poke);
	json j1 = poke1;
	json j2 = poke2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, QuoteMessage)
{
	QuoteMessage quote = Data::MessageData["QuoteMessage"].get<QuoteMessage>();

	EXPECT_TRUE(quote.valid());
	EXPECT_FALSE(quote.isSendSupported());

	EXPECT_NE(quote.GetTargetId(), 123456789_qq);
	EXPECT_EQ(quote.GetSenderId(), 987654321_qq);
	json j = quote.GetOriginMessage()[0];
	EXPECT_EQ(j, json::parse(R"({ "type": "Plain", "text": "text" })"));

	auto quote1 = quote;
	auto quote2 = std::move(quote);
	EXPECT_EQ(quote1.GetTargetId(), quote2.GetTargetId());
	EXPECT_EQ(quote1.GetSenderId(), quote2.GetSenderId());
}

TEST(MessagesTest, SourceMessage)
{
	SourceMessage source = Data::MessageData["SourceMessage"].get<SourceMessage>();

	EXPECT_TRUE(source.valid());
	EXPECT_FALSE(source.isSendSupported());

	EXPECT_EQ(source.GetTimestamp(), 123456);
	EXPECT_EQ(source.GetMessageId(), 654321);

	auto source1 = source;
	auto source2 = std::move(source);
	EXPECT_EQ(source1.GetTimestamp(), source2.GetTimestamp());
	EXPECT_EQ(source1.GetMessageId(), source2.GetMessageId());
}

TEST(MessagesTest, XmlMessage)
{
	XmlMessage xml = Data::MessageData["XmlMessage"].get<XmlMessage>();

	EXPECT_TRUE(xml.valid());
	EXPECT_TRUE(xml.isSendSupported());

	json j;
	EXPECT_NO_THROW(j = xml);
	EXPECT_EQ(j["type"].get<MessageTypes>(), xml.GetType());

	EXPECT_EQ(xml.GetContent(), "<XML>");
	xml.SetContent("abc");
	j = xml;
	EXPECT_EQ(j.value("xml", ""), "abc");

	auto xml1 = xml;
	auto xml2 = std::move(xml);
	json j1 = xml1;
	json j2 = xml2;
	EXPECT_EQ(j1, j2);
}

TEST(MessagesTest, MessageElementTest)
{
	MessageElement m;
	EXPECT_EQ(m.type(), MessageTypes::ENUM_END);
	EXPECT_FALSE(m);
	EXPECT_THROW(m.allowSend(), std::runtime_error);

	m = std::make_unique<PlainMessage>("text");
	EXPECT_EQ(m.type(), MessageTypes::PLAIN);
	EXPECT_TRUE(m);
	EXPECT_NO_THROW(m.as<PlainMessage>());
	EXPECT_THROW(m.as<AppMessage>(), TypeDismatch);

	MessageElement m1(m);
	const MessageElement m2(std::make_unique<AppMessage>());
	EXPECT_TRUE(m1.valid());
	EXPECT_FALSE(m2.valid());

	EXPECT_TRUE(m1.visit(
		[](auto&& m)
		{
			using Type = std::decay_t<decltype(m)>;
			return std::is_same_v<Type, PlainMessage>;
		}
	));

	EXPECT_FALSE(m2.visit(
		[](const auto& m)
		{
			using Type = std::decay_t<decltype(m)>;
			return std::is_same_v<Type, PlainMessage>;
		}
	));
}

TEST(MessagesTest, TemplateTest)
{
	MessageChain m;
	m.push_back(PlainMessage{"aaa"});
	m.emplace_back<ImageMessage>("1", "2", "3", "4");
	m.push_back(SourceMessage{});
	m.push_back(PlainMessage{"bbb"});
	m.emplace_back<PokeMessage>(PokeMessage{PokeType::ENUM_END});
	m.emplace_back<MessageTypes::MUSIC_SHARE>(MusicShareType::NETEASECLOUDMUSIC, "a", "a", "a", "a", "a", "a");

	EXPECT_EQ(m.GetAt<PokeMessage>(4).valid(), false);
	const MessageChain m_const = m;
	json j = m_const.GetAt<MusicShareMessage>(5);
	EXPECT_EQ(j["kind"].get<std::string>(), "NeteaseCloudMusic");
	EXPECT_THROW(auto msg = m.GetAt<PlainMessage>(1), TypeDismatch);

	m.emplace<PlainMessage>(m.begin() + 2, "ccc");
	auto plain = m.GetAll<PlainMessage>();
	EXPECT_EQ(plain.size(), 3);
	EXPECT_EQ(plain[1].GetText(), "ccc");
}

TEST(MessagesTest, ConstructorTest)
{
	MessageChain m;
	m.push_back(AppMessage{});
	m.push_back(AtAllMessage{});
	m.push_back(AudioMessage{});
	m.push_back(DiceMessage{});
	m.push_back(MarketFaceMessage{});

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
	for (std::size_t i = 0; i < 100000; i++)
	{
		auto idx = randidx(rng);
		msg += data[idx];
	}
	m = msg.get<MessageChain>();
	EXPECT_EQ(m.size(), msg.size());
	for (std::size_t i = 0; i < msg.size(); i++)
	{
		EXPECT_EQ(m.GetType(i), msg[i]["type"].get<MessageTypes>());
	}
}