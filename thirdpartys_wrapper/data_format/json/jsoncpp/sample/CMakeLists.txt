
set( TARGET
	SampleJson
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
		
add_dependencies(${TARGET} dllThirdWrap_Json)
target_link_libraries(${TARGET} dllThirdWrap_Json)

if(UNIX)
	target_link_libraries(${TARGET} pthread dl)
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
