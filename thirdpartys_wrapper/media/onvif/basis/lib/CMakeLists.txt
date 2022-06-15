
set(TARGET
	libThirdWrap_Onvif
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_H_C_LIST "${TARGET_INCLUDE_DIR}/*.c")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")
FILE(GLOB_RECURSE TARGET_SRC_C_LIST "src/*.c")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_SRC_C_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_H_C_LIST} ${TARGET_DEF_LIST})

target_compile_definitions(${TARGET} PUBLIC -DBOOST_ALL_NO_LIB -D__STDC_CONSTANT_MACROS -DWITH_OPENSSL)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/OpenSSL/inc32")

if(OS STREQUAL linux)
	target_link_libraries(${TARGET} pthread dl)
endif()

target_link_libraries(${TARGET} dllFoundation)

if(OS STREQUAL win32)
	foreach(lang C CXX)
		set(CMAKE_${lang}_FLAGS_DEBUG "${CMAKE_${lang}_FLAGS_DEBUG} /Gy-")
	endforeach(lang)

	if(ARCH STREQUAL x64)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/lib/win32/x64/libeay32.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/lib/win32/x64/ssleay32.lib)
	else()
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/lib/win32/x86/libeay32.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/openssl/lib/win32/x86/ssleay32.lib)
	endif()
endif()
