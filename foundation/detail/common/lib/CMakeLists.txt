
set(TARGET
	libCommon
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})

add_dependencies(${TARGET} libString libString_v20)
target_link_libraries(${TARGET} libString libString_v20)

if(UNIX)
	if(NOT BUILD_ANDROID)	#��androidϵͳ
		target_link_libraries(${TARGET} pthread dl)
	endif()
elseif(OS STREQUAL win32 AND VS_VER GREATER 100 )
		target_link_libraries(${TARGET} legacy_stdio_definitions)
endif()
	
target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

