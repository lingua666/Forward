
set( TARGET
	SampleNetframework
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)
		
add_dependencies(${TARGET} dllNetFrame_CmdServer_V10 dllNetFrame_CmdServer dllNetFrame_AsyncCmdClients dllNetFrame_RawServer dllNetFrame_AsyncRawClients dllNetFrame_StreamServer dllNetFrame_AsyncStreamClients)
target_link_libraries(${TARGET} dllNetFrame_CmdServer_V10 dllNetFrame_CmdServer dllNetFrame_AsyncCmdClients dllNetFrame_RawServer dllNetFrame_AsyncRawClients dllNetFrame_StreamServer dllNetFrame_AsyncStreamClients)

if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
