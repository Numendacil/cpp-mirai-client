target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE
	IEvent.hpp

	BotEvents.hpp
	CommandExecutedEvent.hpp
	FriendEvents.hpp
	GroupEvents.hpp
	MemberEvents.hpp
	MiraiClientEvents.hpp
	NewFriendRequestEvent.hpp
	NudgeEvent.hpp
	OtherClientEvents.hpp
	StrangerMessageEvents.hpp
	TempMessageEvents.hpp
)