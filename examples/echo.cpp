#include <iostream>
#include <libmirai/mirai.hpp>
#include <string>

int main()
{
	using namespace Mirai;
	MiraiClient client;
	HttpWsAdaptorConfig config;

	/* Set your other configs here
	config.FromJsonFile("config.json");
	*/
	config.AutoReconnect = false;

	client.SetAdaptor(MakeHttpWsAdaptor(std::move(config)));

	client.On<FriendMessageEvent>(
		[](FriendMessageEvent event)
		{
			try
			{
				event.GetMiraiClient().SendFriendMessage(
					event.GetSender().id, 
					event.GetMessage(),
					std::nullopt
				);
			}
			catch(std::exception& e)
			{
				std::cout << "Error occured while reply: " << e.what() << std::endl;
			}
		}
	);

	try
	{
		client.Connect();
	}
	catch(std::exception& e)
	{
		std::cout << "Error occured while connecting to mirai: " << e.what() << std::endl;
	}

	std::cout << "Successfully connected!"<< std::endl;

	std::string cmd;
	while (std::cin >> cmd)
	{
		if (cmd == "exit")
		{
			client.Disconnect();
			break;
		}
	}

	return 0;
}