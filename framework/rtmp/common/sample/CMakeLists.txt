
set( TARGET
	SampleFrameWork_RtmpCommon
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
		
add_dependencies(${TARGET} libFrameWork_RtmpCommon)
target_link_libraries(${TARGET} libFrameWork_RtmpCommon)

if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
