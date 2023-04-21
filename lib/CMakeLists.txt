set(SRCS
	conv.c
	list.c
	error.c
	filter.c
	string.c
)
set(DEPENDS blktool-utils blktool-compatible)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(SRCS ${SRCS}
		linux/fs.c
		linux/file.c
		linux/list.c
		linux/ioctl.c
		linux/fdisk.c
		linux/blkid.c
		linux/generic.c
	)
	set(DEPENDS ${DEPENDS} blkid fdisk uuid)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(SRCS ${SRCS}
		windows/list.c
		windows/info.c
		windows/generic.c
	)
	set(DEPENDS ${DEPENDS} ntdll shlwapi)
else()
	message(FATAL_ERROR "Unknown system")
endif()
if(ENABLE_STATIC)
	add_library(blktool-lib STATIC ${SRCS})
else()
	add_library(blktool-lib SHARED ${SRCS})
endif()
target_link_libraries(blktool-lib ${DEPENDS})
set_target_properties(blktool-lib PROPERTIES OUTPUT_NAME blktool)
