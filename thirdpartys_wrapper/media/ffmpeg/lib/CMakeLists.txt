
set(TARGET
	libThirdWrap_FFmpeg
	)
	
#Traverse the file
set ( TARGET_INCLUDE_DIR
	include
	)

FILE(GLOB_RECURSE TARGET_H_LIST "${TARGET_INCLUDE_DIR}/*.h")
FILE(GLOB_RECURSE TARGET_HPP_LIST "${TARGET_INCLUDE_DIR}/*.hpp")
FILE(GLOB_RECURSE TARGET_SRC_LIST "src/*.cpp")

add_target(${TARGET} StaticLib ${TARGET_SRC_LIST} ${TARGET_H_LIST} ${TARGET_HPP_LIST} ${TARGET_DEF_LIST})
if(UNIX)
	if(NOT BUILD_ANDROID)	#∑«androidœµÕ≥
		target_link_libraries(${TARGET} pthread dl)
	endif()
endif()

target_compile_definitions(${TARGET} PUBLIC -DBOOST_ALL_NO_LIB -D__STDC_CONSTANT_MACROS)
target_include_directories(${TARGET} PUBLIC ${TARGET_INCLUDE_DIR})

if(OS STREQUAL win32)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "/OPT:NOREF")
endif()

target_link_libraries(${TARGET} dllFoundation)

#set ( TARGET_TYPE static)
set ( TARGET_TYPE shared)

if(ARCH STREQUAL x64)
	if(OS STREQUAL win32)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/avcodec.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/avdevice.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/avfilter.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/avformat.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/avutil.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/postproc.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/swresample.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x64/${TARGET_TYPE}/lib/swscale.lib)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libavcodec.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libavdevice.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libavfilter.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libavformat.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libavutil.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libpostproc.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libswresample.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x64/${TARGET_TYPE}/lib/libswscale.dll.a)
	endif()
else()
	if(OS STREQUAL win32)
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/avcodec.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/avdevice.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/avfilter.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/avformat.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/avutil.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/postproc.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/swresample.lib)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/windows/x86/${TARGET_TYPE}/lib/swscale.lib)
	else()
		target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/include")
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libavcodec.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libavdevice.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libavfilter.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libavformat.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libavutil.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libpostproc.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libswresample.dll.a)
		target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/thirdpartys/uncompile/ffmpeg/linux/x86/${TARGET_TYPE}/lib/libswscale.dll.a)
	endif()
endif()
