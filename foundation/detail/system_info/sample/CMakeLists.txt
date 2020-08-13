
set( TARGET
	SampleSystem_Info
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

add_dependencies(${TARGET} dllSystem_Info)
target_link_libraries(${TARGET} dllSystem_Info)

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
