#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <libmirai/Client/Client.hpp>
#include <libmirai/Adaptors/GMockAdaptor.hpp>

#include "JsonData.hpp"

using namespace Mirai;

// NOLINTBEGIN(*)

TEST(ClientTest, ClientTest)
{
	auto adaptor = MakeGMockAdaptor([](GMockAdaptor& adaptor)
	{
		EXPECT_CALL(adaptor, MessageFromId)
		.Times(4)
		.WillOnce(testing::Return(FriendMessageEvent()))
		.WillOnce(testing::Return(GroupMessageEvent()))
		.WillOnce(testing::Return(TempMessageEvent()))
		.WillOnce(testing::Return(StrangerMessageEvent()));
	});

	MiraiClient client(std::move(adaptor));
	EXPECT_NO_THROW(client.Connect());

	EXPECT_NO_THROW(client.GetFriendMessage(12345, 111_qq));
	EXPECT_NO_THROW(client.GetGroupMessage(12345, 111_gid));
	EXPECT_NO_THROW(client.GetTempMessage(12345, 111_gid));
	EXPECT_THROW(client.GetTempMessage(12345, 111_gid), TypeDismatchError);


	EXPECT_NO_THROW(client.Disconnect());

}

// NOLINTEND(*)