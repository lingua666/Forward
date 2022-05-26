
set(TARGET 
	SampleThirdWrap_Onvif_Camera
	)
	
set(SOURCES
	Sample.cpp
	)

list(LENGTH SERVER listlen)
if(listlen GREATER 0)
	math(EXPR listlen "${listlen} - 1")
endif()

foreach(loop_var RANGE 0 ${listlen})
	list(GET TARGET ${loop_var} SvrElement)
	list(GET SOURCES ${loop_var} SoucElement)
	add_target(${SvrElement} ConsoleApp ${SoucElement})
	
	add_dependencies(${SvrElement} dllFoundation libThirdWrap_Onvif_Camera)
	target_link_libraries(${SvrElement} dllFoundation libThirdWrap_Onvif_Camera)
	
endforeach(loop_var)

target_compile_definitions(${TARGET} PRIVATE -DBOOST_ALL_NO_LIB)






