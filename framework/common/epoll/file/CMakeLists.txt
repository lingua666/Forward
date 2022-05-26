
set(TARGET
	libEPoll_File
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})
if(UNIX)
	if(NOT BUILD_ANDROID)	#非android系统
		target_link_libraries(${TARGET} pthread dl)
	endif()
	#操作超过2G大文件
	target_compile_definitions(${TARGET} PRIVATE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE)
endif()

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})	

add_dependencies(${TARGET} dllFoundation)
target_link_libraries(${TARGET} dllFoundation)

add_subdirectory(sample)





