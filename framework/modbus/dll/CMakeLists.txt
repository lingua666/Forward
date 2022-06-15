
set(TARGET
	dllFrameWork_Modbus
	)
	
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

add_dependencies(${TARGET} libFrameWork_Modbus)
target_link_libraries(${TARGET} libFrameWork_Modbus)

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})





