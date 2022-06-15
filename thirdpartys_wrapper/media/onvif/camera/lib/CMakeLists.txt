
set( TARGET
	libThirdWrap_Onvif_Camera
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

add_dependencies(${TARGET} dllFoundation libThirdWrap_Onvif libThirdWrap_RtspClient dllThirdWrap_FFmpeg)

target_link_libraries(${TARGET} dllFoundation libThirdWrap_Onvif libThirdWrap_RtspClient dllThirdWrap_FFmpeg)

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

