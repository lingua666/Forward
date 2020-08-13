if(NETFRAME_CMDSESSION_ENABLE EQUAL 1)
	add_subdirectory(command)
endif()

if(NETFRAME_STREAMSESSION_ENABLE EQUAL 1)
	add_subdirectory(stream)
endif()

if(NETFRAME_RAWSESSION_ENABLE EQUAL 1)
	add_subdirectory(raw)
endif()