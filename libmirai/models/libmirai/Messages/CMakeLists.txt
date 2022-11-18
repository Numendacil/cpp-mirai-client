target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE
	MessageBase.hpp

	MessageChain.hpp
	MessageChain.cpp

	AppMessage.hpp
	AtAllMessage.hpp
	AtMessage.hpp
	AudioMessage.hpp
	DiceMessage.hpp
	FaceMessage.hpp
	FileMessage.hpp
	FlashImageMessage.hpp
	ForwardMessage.hpp
	ForwardMessage.cpp
	ForwardMessageNode.hpp
	ImageMessage.hpp
	JsonMessage.hpp
	MarketFaceMessage.hpp
	MiraiCodeMessage.hpp
	MusicShareMessage.hpp
	PlainMessage.hpp
	PokeMessage.hpp
	QuoteMessage.hpp
	SourceMessage.hpp
	XmlMessage.hpp

	Messages.hpp
)