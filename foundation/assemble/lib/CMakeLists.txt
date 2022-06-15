
set(TARGET
	libFoundation
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h" "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST})

add_dependencies(${TARGET} dllCommon dllLock dllDesignMode dllContainer dllFunction_v20 dllFile dllLog dllMempool dllSmart_ptr
 dllSmart_ptr_v20 dllThread dllTimestamp dllUUID dllMessage dllSignalsSlots libBoost_Wheel libString_v30)

target_link_libraries(${TARGET} dllCommon dllLock dllDesignMode dllContainer dllFunction_v20 dllFile dllLog dllMempool dllSmart_ptr
 dllSmart_ptr_v20 dllThread dllTimestamp dllUUID dllMessage dllSignalsSlots libBoost_Wheel libString_v30)

 if(_COMPILER_OPTION_)
		if(_COMPILER_OPTION_ STREQUAL "arm-openwrt-linux-uclibcgnueabi")
			target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/iconv/arm/simcom/include")
			target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/iconv/arm/simcom/lib/libiconv.a)
		endif()
endif()


if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()
	
target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})





