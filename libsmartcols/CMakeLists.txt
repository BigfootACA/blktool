if(NOT SYSTEM_SMARTCOLS)
	add_library(smartcols STATIC
		calculate.c
		cell.c
		column.c
		grouping.c
		iter.c
		line.c
		print-api.c
		print.c
		symbols.c
		table.c
		version.c
		walk.c
		mbsalign.c
		jsonwrt.c
		buffer.c
	)
	target_link_libraries(smartcols blktool-compatible)
endif()