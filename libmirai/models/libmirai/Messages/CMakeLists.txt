target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE
	MessageBase.hpp

	MessageChain.hpp
	MessageChain.cpp

	AppMessage.hpp
	AppMessage.cpp
	AtAllMessage.hpp
	AtAllMessage.cpp
	AtMessage.hpp
	AtMessage.cpp
	AudioMessage.hpp
	AudioMessage.cpp
	DiceMessage.hpp
	DiceMessage.cpp
	FaceMessage.hpp
	FaceMessage.cpp
	FileMessage.hpp
	FileMessage.cpp
	FlashImageMessage.hpp
	FlashImageMessage.cpp
	ForwardMessage.hpp
	ForwardMessage.cpp
	ImageMessage.hpp
	ImageMessage.cpp
	JsonMessage.hpp
	JsonMessage.cpp
	MarketFaceMessage.hpp
	MarketFaceMessage.cpp
	MiraiCodeMessage.hpp
	MiraiCodeMessage.cpp
	MusicShareMessage.hpp
	MusicShareMessage.cpp
	PlainMessage.hpp
	PlainMessage.cpp
	PokeMessage.hpp
	PokeMessage.cpp
	QuoteMessage.hpp
	QuoteMessage.cpp
	SourceMessage.hpp
	SourceMessage.cpp
	XmlMessage.hpp
	XmlMessage.cpp

	Messages.hpp
)