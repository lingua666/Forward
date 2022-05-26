
set(TARGET
	libThirdWrap_Openssl
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_DEF_LIST})

target_compile_definitions(${TARGET} PUBLIC -DBOOST_ALL_NO_LIB -D__STDC_CONSTANT_MACROS)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

if(OS STREQUAL win32)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "/OPT:NOREF")
endif()

add_dependencies(${TARGET} libFoundation)
target_link_libraries(${TARGET} libFoundation)

if(_COMPILER_OPTION_)
		if(_COMPILER_OPTION_ STREQUAL "arm-linux-gnueabihf")
			target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6658/include")
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6658/lib/libssl.a)
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6658/lib/libcrypto.a)
		elseif(_COMPILER_OPTION_ STREQUAL "arm-none-linux-gnueabi")
			target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6511/include")
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6511/lib/libssl.a)
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/htnice/6511/lib/libcrypto.a)
		elseif(_COMPILER_OPTION_ STREQUAL "arm-openwrt-linux-uclibcgnueabi")
			target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/simcom/include")
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/simcom/lib/libssl.a)
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/arm/simcom/lib/libcrypto.a)
		endif()
elseif(ARCH STREQUAL x64)
	if(OS STREQUAL win32)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/lib/x64/release/libssl.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/lib/x64/release/libcrypto.lib)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/lib/x64/libssl.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/lib/x64/libcrypto.a)	
	endif()
else()
	if(OS STREQUAL win32)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/lib/x86/release/libssl.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/windows/lib/x86/release/libcrypto.lib)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/lib/x86/libssl.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/linux/lib/x86/libcrypto.a)		
	endif()
endif()

#必须放在libssl.a libcrypto.a(库加载顺序必须先libssl.a后libcrypto.a)之后，否则编译是会出现undefined reference to `pthread_atfork'问题
if(UNIX)
	if(NOT BUILD_ANDROID)	#非android系统
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()