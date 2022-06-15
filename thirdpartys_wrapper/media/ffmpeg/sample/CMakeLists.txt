
set( TARGET
	SampleThirdWrap_FFmpeg
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_SRC_LIST "*.cpp")

add_target(${TARGET} ConsoleApp ${TARGET_SRC_LIST})

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)

if(OS STREQUAL win32)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "/OPT:NOREF")
endif()
		
add_dependencies(${TARGET} dllThirdWrap_FFmpeg dllThirdWrap_D3D)
target_link_libraries(${TARGET} dllThirdWrap_FFmpeg dllThirdWrap_D3D)

if(OS STREQUAL linux)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})
