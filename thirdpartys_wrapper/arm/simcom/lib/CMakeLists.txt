
set(TARGET
	libThirdWrap_Simcom
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h" "inc/*.h" "sdk-includes")
FILE(GLOB_RECURSE TARGET_SRC_C_LIST "src/*.c")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_SRC_C_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_H_C_LIST} ${TARGET_DEF_LIST})

add_dependencies(${TARGET} libFoundation)
target_link_libraries(${TARGET} libFoundation)

target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/libs/libsdk.a 
${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/usr/lib/libuci.so ${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/usr/lib/libubox.a)
target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/inc/ ${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/usr/include ${CMAKE_SOURCE_DIR}/thirdpartys_wrapper/arm/simcom/lib/sdk-includes/common)

target_link_libraries(${TARGET} pthread dl)
