target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE
	IMessage.hpp

	MessageChain.hpp
	MessageChain.cpp

	AppMessage.hpp
	AtAllMessage.hpp
	AtMessage.hpp
	AudioMessage.hpp
	DiceMessage.hpp
	FaceMessage.hpp
	FileMessage.hpp
	ForwardMessage.hpp
	ForwardMessage.cpp
	ForwardMessageNode.hpp
	ImageMessage.hpp
	JsonMessage.hpp
	MiraiCodeMessage.hpp
	MusicShareMessage.hpp
	PlainMessage.hpp
	PokeMessage.hpp
	QuoteMessage.hpp
	QuoteMessage.cpp
	SourceMessage.hpp
	XmlMessage.hpp
)