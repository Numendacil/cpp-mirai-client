target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE

	MessageChain.hpp
	MessageChain.cpp

	AppMessage.cpp
	AtAllMessage.cpp
	AtMessage.cpp
	AudioMessage.cpp
	DiceMessage.cpp
	FaceMessage.cpp
	FileMessage.cpp
	ForwardMessage.cpp
	ImageMessage.cpp
	JsonMessage.cpp
	MarketFaceMessage.cpp
	MiraiCodeMessage.cpp
	MusicShareMessage.cpp
	PlainMessage.cpp
	PokeMessage.cpp
	QuoteMessage.cpp
	SourceMessage.cpp
	XmlMessage.cpp

	ForwardMessageNode.hpp
	ForwardMessageNode.cpp
)