
add_library(
	blktool-utils
	STATIC
	getopt.c
	list.c
	getlopt.c
	readable.c
)
target_link_libraries(blktool-utils blktool-compatible)
