
set(TARGET
	libString_v20
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})

add_dependencies(${TARGET} libBase)
target_link_libraries(${TARGET} libBase)

if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
	
	if(BUILD_ANDROID)
		target_include_directories(${TARGET} PUBLIC $ENV{ANDROID_NDK_HOME}/sources/android/support/include)
	endif()
endif()
	
target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})





