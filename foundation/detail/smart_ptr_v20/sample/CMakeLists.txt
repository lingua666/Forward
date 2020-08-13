
set( TARGET
	SampleSmart_ptr_v20
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
		
add_dependencies(${TARGET} libSmart_ptr_v20 dllTimestamp)

find_boost(BOOST_LIBS_STRING)
target_link_libraries(${TARGET} ${BOOST_LIBS_STRING} libSmart_ptr_v20 dllTimestamp)
target_include_directories(${TARGET} PUBLIC ${BOOST_INCLUDE_DIRS} ${TARGET_INCLUDE_DIR})

if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
