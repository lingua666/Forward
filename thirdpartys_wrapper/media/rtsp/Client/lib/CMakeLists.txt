
set(TARGET
	libThirdWrap_RtspClient
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
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/compile/live555/src/BasicUsageEnvironment/include")
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/compile/live555/src/groupsock/include")
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/compile/live555/src/liveMedia/include")
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/compile/live555/src/UsageEnvironment/include")

if(OS STREQUAL linux)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_link_libraries(${TARGET} dllFoundation)

target_link_libraries(${TARGET} live555_BasicUsageEnvironment live555_groupsock live555_liveMedia live555_mediaServer live555_UsageEnvironment)
