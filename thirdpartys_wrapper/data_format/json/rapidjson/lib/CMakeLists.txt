
set(TARGET
	libThirdWrap_RapidJson
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})

add_dependencies(${TARGET} libFoundation)
target_link_libraries(${TARGET} libFoundation)

if(UNIX)
	target_link_libraries(${TARGET} pthread dl)
endif()
	
target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/thirdpartys/compile/json/rapidjson-1.0.2/include)




