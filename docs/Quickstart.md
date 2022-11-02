# 快速入门

## 准备工作
### 配置mirai与mirai-api-http
你需要根据mirai的相关说明安装mirai（Mirai Console Loader， 或者其他你喜欢的方式，见[用户手册](https://github.com/mamoe/mirai/blob/dev/docs/UserManual.md)），并安装[mirai-api-http](https://github.com/project-mirai/mirai-api-http)。确保以上软件可以正确运行并成功登陆Bot的账号。

cpp-mirai-client 依赖 mirai-api-http 的 [http adapter](https://github.com/project-mirai/mirai-api-http/blob/master/docs/adapter/HttpAdapter.md) 与 [websocket adapter](https://github.com/project-mirai/mirai-api-http/blob/master/docs/adapter/WebsocketAdapter.md) 来正确工作，因此你的[配置文件](https://github.com/project-mirai/mirai-api-http#settingyml模板)中应该至少有这样一条
```yaml
## 启用 http 和 ws adapter
adapters:
    - http
    - ws
```
此外，你还可能需要根据需求来配置其他选项，详情可以见 mirai-api-http 对应的文档。其中与 cpp-mirai-client 相关的部分为
verifyKey, http 监听地址以及 websocket 监听地址。

### 链接cpp-mirai-client
你需要用自己喜欢的方式编译cpp-mirai-client并确保你的项目链接到对应的库文件。如果不知道具体怎么做，可以使用 cppmirai-template 
生成一个项目模板（TBD）

## 一个简易的复读机
我们将以一个简单的复读机Bot为例，介绍cpp-mirai-client的基本用法。完整的代码可见 [examples/echo](../examples/echo.cpp)

### 头文件
为了使用 cpp-mirai-client 中的接口，你需要包含对应的头文件
```cpp
#include <libmirai/mirai.hpp>
```
这将包含所有的 cpp-mirai-client 相关类与函数的声明。你也可以根据自己的需求仅包含部分头文件
```cpp
#include <libmirai/Events/Events.hpp>	// include all event classes
#include <libmirai/Messages/PlainMessage.hpp>	// include declaration for PlainMessage class only
#include <libmirai/models.hpp>		// include everything but the client
```
如果你需要在程序中使用json对象，你还应该包括
```cpp
#include <nlohmann/json.hpp>	// include nlohmann's json library
```

### 连接mirai-api-http
为了与mirai-api-http通信，你需要一个 `Mirai::MiraiClient` 对象
```cpp
Mirai::MiraiClient client;
```
在连接之前，你需要设置一些基本的选项，这些配置被保存在 `Mirai::SessionConfigs` 对象中
```cpp
Mirai::SessionConfigs config;

// You can set them manually
config.HttpUrl = "http://localhost:8080";
config.WebsocketUrl = "ws://localhost:8080";
config.VerifyKey = "xxx";

using namespace Mirai;
config.BotQQ = 12345_qq;

using namespace std::chrono_literals;
config.ReadTimeout = 10s;
config.HeartbeatInterval = 1min;

// Or you can read them from file
config.FromFile("/path/to/config.json");

// Now set the client with the configs
client.SetSessionConfig(config);
```
万事俱备，现在可以开始连接mirai了
```cpp
try
{
	client.Connect();
}
catch(const std::exception& e)
{
	// do something...
}
```
如果你想关闭连接，只需要调用
```cpp
client.Disconnect();
```

### 处理事件
显然，仅仅只连接mirai是不够的。我们希望能接收到QQ传来的消息并做出相应的处理。一般来说，
除了好友和群聊发来的消息外，Bot还会收到消息撤回、群体禁言之类的信息，这些被统称为事件 `Events` 。
完整的事件列表可以参考 `libmirai/Events/Events.hpp` 。

在复读机Bot中，我们需要处理的是 `Mirai::FriendMessageEvent`，这代表来自好友发送的消息
```cpp
using namespace Mirai;
client.On<FriendMessageEvent>(
	[](FriendMessageEvent event)
	{
		try
		{
			event.GetMiraiClient()
			.SendFriendMessage(
				event.GetSender().id, 
				event.GetMessage(),
				std::nullopt,
				true
			);
		}
		catch(std::exception& e)
		{
			// do something...
		}
	}
);
```
我们来一点点分析这段代码。首先为了注册响应事件的回调函数，我们需要调用 `Mirai::MiraiClient::On<EventType>(callback)` ，
其中回调函数的格式为 `std::function<void(EventType)>`。为了简便，我们直接使用了lambda作为事件处理函数。

回调函数的核心语句只有一条：
```cpp
event.GetMiraiClient()
.SendFriendMessage(
	event.GetSender().id, 
	event.GetMessage(),
	std::nullopt,
	true
);
```
它可以拆分为两个部分。首先，`Mirai::EventBase::GetMiraiClient()` 负责返回一个 `Mirai::MiraiClient` 对象的引用，大部分事件（除了与网络连接相关的事件外）都拥有这个方法。我们需要这个对象来进行消息的回复以及其他可能的操作。

> `Mirai::EventBase::GetMiraiClient()` 返回的类型是 `Mirai::MiraiClient&`，而出于显而易见的原因 `Mirai::MiraiClient` 是non-copyable的。
> 因此如果你想为了方便暂时储存该对象的话，你应该使用 `auto& client = event.GetMiraiClient()` 而非 `auto client = event.GetMiraiClient()`

> 当然，你也可以直接在lambda捕获里直接捕获 `client` 对象，但是要注意循环引用的问题。

为了复读，我们需要获得发送者的资料以及发送的具体消息，这些对象是通过 `Mirai::FriendMessageEvent::GetSender()` 与
`Mirai::FriendMessageEvent::GetMessage()` 获取的。

> 大部分事件对象都定义了很多Get方法，你可以通过这些方法获取事件相关的信息。利用IDE的语法提示以及API文档可以快速获知
> 每种事件所拥有的属性。

最后，我们通过 `Mirai::MiraiClient::SendFriendMessage()` 将复读的信息发送给对方。你也可以加入其他事件的处理函数，
实现对群聊消息的复读或者对戳一戳消息的复读。
