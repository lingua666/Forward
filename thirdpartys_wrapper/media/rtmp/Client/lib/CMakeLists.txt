
set(TARGET
	libThirdWrap_RtmpClient
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

if(OS STREQUAL win32)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "/OPT:NOREF")
endif()

if(OS STREQUAL linux)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

add_dependencies(${TARGET} librtmp libFoundation)
target_link_libraries(${TARGET} librtmp libFoundation)
