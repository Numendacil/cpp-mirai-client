find_package(Threads REQUIRED)

find_package(httplib QUIET)
if(NOT HTTPLIB_FOUND)
	message("cpp-httplib not found. Building from local files")
	set(MIRAI_NO_LOCAL_LIBS false)

	FetchContent_Declare(
		cpp-httplib
		GIT_REPOSITORY	${URL_PREFIX}yhirose/cpp-httplib
		GIT_TAG		v0.14.0
	)
	FetchContent_MakeAvailable(cpp-httplib)

endif()

find_package(nlohmann_json QUIET)
if(NOT TARGET nlohmann_json::nlohmann_json)
	message("nlohmann_json not found. Building from local files")
	set(MIRAI_NO_LOCAL_LIBS false)

	FetchContent_Declare(
		json
		GIT_REPOSITORY	${URL_PREFIX}nlohmann/json
		GIT_TAG		fac07e22c5d7dd0423ccf31c02db5603d27e6556
	)

	set(JSON_ImplicitConversions OFF CACHE BOOL "Enable implicit conversions." FORCE)
	set(JSON_DisableEnumSerialization OFF CACHE BOOL "Disable default integer enum serialization." FORCE)

	FetchContent_MakeAvailable(json)
endif()

find_package(tomlplusplus QUIET)
if(NOT TARGET tomlplusplus::tomlplusplus)
	message("tomlplusplus not found. Building from local files")
	set(MIRAI_NO_LOCAL_LIBS false)

	FetchContent_Declare(
		tomlplusplus
		GIT_REPOSITORY	${URL_PREFIX}marzer/tomlplusplus
		GIT_TAG		941341fce6b8dfb443b94a73e381e220bbe6e377
	)
	FetchContent_MakeAvailable(tomlplusplus)
endif()

find_package(ixwebsocket QUIET)
if(NOT TARGET ixwebsocket::ixwebsocket)
	message("ixwebsocket not found. Building from local files")
	set(MIRAI_NO_LOCAL_LIBS false)

	FetchContent_Declare(
		IXWebSocket
		GIT_REPOSITORY	${URL_PREFIX}machinezone/IXWebSocket
		GIT_TAG		v11.4.4
	)

	find_package(ZLIB QUIET)
	if(TARGET ZLIB::ZLIB)
		set(USE_ZLIB ON CACHE BOOL "Enable zlib support" FORCE)
	else()
		set(USE_ZLIB OFF CACHE BOOL "Enable zlib support" FORCE)
	endif()

	find_package(OpenSSL QUIET)
	find_package(MbedTLS QUIET)
	if(APPLE)
		set(WS_USE_TLS ON)	# Defaults to SecureTranport on Apple
	else()
		if(OPENSSL_FOUND OR MBEDTLS_FOUND)
			set(WS_USE_TLS ON)
		endif()
	endif()

	if(WS_USE_TLS)
		set(USE_TLS ON CACHE BOOL "Enable TLS support" FORCE)
	endif()
	if(OPENSSL_FOUND)
		set(USE_OPEN_SSL ON CACHE BOOL "Enable OpenSSL support" FORCE)
	elseif(MBEDTLS_FOUND)
		set(USE_MBED_TLS ON CACHE BOOL "Enable MbedTLS support" FORCE)
	endif()

	FetchContent_MakeAvailable(IXWebSocket)
endif()