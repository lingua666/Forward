
set(TARGET
	dllThirdWrap_Sqlite
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_DEF_LIST "${TARGET_INCLUDE_DIR}/*.def")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} ${GEN_DLL_TYPE} ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_DEF_LIST})
if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/thirdpartys/uncompile/sqlite/include")

add_dependencies(${TARGET} libThirdWrap_Sqlite)
target_link_libraries(${TARGET} libThirdWrap_Sqlite)

#if(OS STREQUAL win32)
#	if(ARCH STREQUAL x64)
#		target_link_libraries(${TARGET} ${PROJECT_SOURCE_DIR}/thirdpartys/uncompile/sqlite/lib/x64/sqlite3.lib)
#	else()
#		target_link_libraries(${TARGET} ${PROJECT_SOURCE_DIR}/thirdpartys/uncompile/sqlite/lib/x86/sqlite3.lib)
#	endif()
#endif()
