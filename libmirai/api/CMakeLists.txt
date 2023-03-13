target_sources(
	${MIRAI_LIB_TARGET_NAME} PRIVATE 
	
	libmirai/Client/Client.hpp
	libmirai/Client/Client.cpp

	libmirai/Adaptors/IAdaptor.hpp
	libmirai/Adaptors/HttpWsAdaptor.hpp
	libmirai/Adaptors/GMockAdaptor.hpp
)