
set(TARGET
	libThirdWrap_Sqlite
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

if(_COMPILER_OPTION_)
	if(_COMPILER_OPTION_ STREQUAL "arm-linux-gnueabihf")
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/linux/arm/htnice/6658/include")
		target_link_libraries(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/linux/arm/htnice/6658/lib/libsqlite3.a")
	elseif(_COMPILER_OPTION_ STREQUAL "arm-none-linux-gnueabi")
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/linux/arm/htnice/6511/include")
		target_link_libraries(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/linux/arm/htnice/6511/lib/libsqlite3.a")
	endif()
elseif(ARCH STREQUAL x64)
	target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/win32/include")
	if(OS STREQUAL win32)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/win32/lib/x64/sqlite3.lib)
	else()
		
	endif()
else()
	target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/win32/include")
	if(OS STREQUAL win32)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/sqlite/win32/lib/x86/sqlite3.lib)
	else()
			
	endif()
endif()
