# 进阶教程

## 异常处理

cpp-mirai-client 中的大部分函数均有可能会抛出异常。其种类可分为以下几个方面：

* MAH错误 `Mirai::MiraiApiHttpException`

	在向mirai发送请求时，由于参数错误、bot权限不足或其它原因导致mirai-api-http返回报错信息将会抛出此类异常。通常发生于各种请求类API处。

	> 在 `Mirai::MiraiApiHttpException` 下还存在有更加细化的异常分类，如 `Mirai::IllegalSession`, `Mirai::PermissionDenied` 等，可以根据需求捕获。

* 网络错误 `Mirai::NetworkException`

	由于网络原因无法与mirai通信产生的异常。

* 消息解析错误 `Mirai::ParseError`

	在解析mirai传来的消息时产生错误导致的异常，可能原因包括mirai-api-http插件更新导致的不兼容变更。

* 类型匹配错误 `Mirai::TypeDismatchError`

	预期类型与实际类型不一致导致的异常，通常发生于获取消息链 `Mirai::MessageChain` 中的某一特定元素时。

* 未实现方法错误 `Mirai::NotImplementedError`

	目前未使用。在调用某些预留但未实现的接口时抛出的异常，例如在Websocket模式下mirai-api-http无法上传文件。


## 特殊事件

为了处理连接mirai时可能发生的状况，cpp-mirai-client 定义了四种特殊的事件类型，位于 [MiraiClientEvents.hpp](../../libmirai/models/libmirai/Events/MiraiClientEvents.hpp) 中。它们可以像普通事件一样注册回调函数

```cpp
client.On<Mirai::ClientConnectionEstablishedEvent>(
	[](Mirai::ClientConnectionEstablishedEvent event)
	{
		// Do something
	}
);
```

* `Mirai::ClientConnectionEstablishedEvent` 和 `Mirai::ClientConnectionClosedEvent `分别对应连接建立和断开时产生的事件。其中，<code>[ClientConnectionEstablishedEvent](@ref Mirai::ClientConnectionEstablishedEvent)</code> 会在 <code>[Connect()](@ref Mirai::MiraiClient::Connect())</code> 成功返回前触发，而 <code>[ClientConnectionClosedEvent](@ref Mirai::ClientConnectionClosedEvent)</code> 会在 <code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code> 成功返回前触发

	> 即使没有调用<code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>，由网络等原因产生的掉线仍然会触发 <code>[ClientConnectionClosedEvent](@ref Mirai::ClientConnectionClosedEvent)</code>。如果开启了自动重连功能，则连接恢复时会再次触发 <code>[ClientConnectionEstablishedEvent](@ref Mirai::ClientConnectionEstablishedEvent)</code>。

	> 不要在回调函数中直接调用 <code>[Connect()](@ref Mirai::MiraiClient::Connect())</code> 与 <code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>，这会导致死锁。考虑使用额外的线程来完成这一功能。


* `Mirai::ClientConnectionErrorEvent` 会在连接产生错误时触发，如网络原因导致的连接失败，或MAH握手信息不一致等。若错误导致连接断开则会再产生 <code>[ClientConnectionClosedEvent](@ref Mirai::ClientConnectionClosedEvent)</code> 事件，因此无需在 <code>[ClientConnectionErrorEvent](@ref Mirai::ClientConnectionErrorEvent)</code> 中添加处理断连相关的逻辑。


* `Mirai::ClientParseErrorEvent `会在解析消息产生错误时触发（主动调用API产生的异常不包括在内，而是会直接抛出），一般来说只需要记录错误即可。

## 多线程相关

由于cpp-mirai-client在处理消息循环时使用了多线程，因此为保证线程安全，`Mirai::MiraiClient` 的调用者应满足以下约定：

* 在调用<code>[Connect()](@ref Mirai::MiraiClient::Connect())</code>之后，<code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>返回之前，不应修改其设置以及注册回调函数，如调用 `Mirai::MiraiClient::SetAdaptor()`，`Mirai::MiraiClient::SetLogger()` 等。

* <code>[Connect()](@ref Mirai::MiraiClient::Connect())</code>不应该被同时并发调用（但是可以并发调用<code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>）。<code>[Connect()](@ref Mirai::MiraiClient::Connect())</code>返回后可以继续多次调用<code>[Connect()](@ref Mirai::MiraiClient::Connect())</code>，将不会产生任何效果，<code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>同理

* 在消息回调函数内直接调用<code>[Connect()](@ref Mirai::MiraiClient::Connect())</code>与<code>[Disconnect()](@ref Mirai::MiraiClient::Disconnect())</code>可能会产生死锁，因此应该避免出现这种情况。