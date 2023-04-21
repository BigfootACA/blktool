/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef BLKTOOL_INTERNAL_H
#define BLKTOOL_INTERNAL_H
#include"blktool.h"
#include"list.h"
typedef struct blktool_filters{
	list*filters;
}blktool_filters;

typedef union blktool_filter_value{
	void*value;
	char*string;
	size_t size;
	bool boolean;
	blktool_handle*handle;
	blktool_layout layout;
	blktool_format format;
	blktool_media media;
}blktool_filter_value;

typedef struct blktool_filter_data{
	blktool_filter filter;
	blktool_filter_type type;
	blktool_filter_value value;
}blktool_filter_data;

extern list*blktool_list_impl();
extern int blktool_set_error(int r,const char*fmt,...);
extern void*blktool_set_perror(void*r,const char*fmt,...);
extern bool blktool_filters_check(blktool_filters*filters,blktool_handle*handle);
#endif //BLKTOOL_INTERNAL_H
