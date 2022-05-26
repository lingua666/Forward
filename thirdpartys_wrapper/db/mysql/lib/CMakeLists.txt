
set(TARGET
	libThirdWrap_Mysql
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_DEF_LIST})
if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

target_link_libraries(${TARGET} dllFoundation)
if(OS STREQUAL win32)
	if(ARCH STREQUAL x64)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/include/win32/x64")
		target_link_libraries(${TARGET} optimized ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/win32/x64/mysqlclient.lib)
		target_link_libraries(${TARGET} debug ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/win32/x64/mysqlclient_gd.lib)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/include/win32/x86")
		target_link_libraries(${TARGET} optimized ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/win32/x86/mysqlclient.lib)
		target_link_libraries(${TARGET} debug ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/win32/x86/mysqlclient_gd.lib)
	endif()
else()
	if(ARCH STREQUAL x64)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/include/linux/x64")	
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/linux/x64/libmysqlclient.a)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/include/linux/x86")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/mysql/lib/linux/x86/libmysqlclient.a)
	endif()
endif()
