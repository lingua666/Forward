
set( TARGET
	libFrameWork_AsyncUarts
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
		

if(OS STREQUAL win32)	
	add_dependencies(${TARGET} libIOCP_File)
	target_link_libraries(${TARGET} libIOCP_File)
elseif(OS STREQUAL linux)
	add_dependencies(${TARGET} libEPoll_File)
	target_link_libraries(${TARGET} libEPoll_File)
	if(NOT BUILD_ANDROID)	#��androidϵͳ
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
