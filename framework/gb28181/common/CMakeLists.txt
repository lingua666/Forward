
set(TARGET
	libFrameWork_Gb28181
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST})
if(UNIX)
	if(NOT BUILD_ANDROID)	#��androidϵͳ
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})	

add_dependencies(${TARGET} dllFoundation libNetFrame_CmdServer_V10 libNetFrame_AsyncCmdClients libNetFrame_StreamServer libNetFrame_AsyncStreamClients libNetFrame_UDP_Stream libFrameWork_Rtsp libProto_Sip_V20 libThirdWrap_Xml)
target_link_libraries(${TARGET} dllFoundation libNetFrame_CmdServer_V10 libNetFrame_AsyncCmdClients libNetFrame_StreamServer libNetFrame_AsyncStreamClients libNetFrame_UDP_Stream libFrameWork_Rtsp libProto_Sip_V20 libThirdWrap_Xml)

add_subdirectory(sample)





