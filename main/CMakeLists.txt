set(SRCS main.c options.c print.c usage.c)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(SRCS ${SRCS} blktool.rc)
endif()
add_executable(blktool-main ${SRCS})
if(ENABLE_STATIC)
	if(MSVC)
		set_target_properties(blktool-main PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		target_link_libraries(blktool-main msvcrt)
	else()
		target_link_libraries(blktool-main -static)
	endif()
endif()
target_link_libraries(
	blktool-main
	blktool-lib
	blktool-utils
	blktool-compatible
	smartcols
)
set_target_properties(blktool-main PROPERTIES OUTPUT_NAME blktool)