if(_COMPILER_OPTION_)
	if(_COMPILER_OPTION_ STREQUAL "arm-openwrt-linux-uclibcgnueabi")
		add_subdirectory(lib)
		add_subdirectory(sample)
	endif()
endif()






