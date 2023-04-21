/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

blktool_filters*blktool_filters_new(){
	blktool_filters*filters;
	if(!(filters=malloc(sizeof(blktool_filters))))return NULL;
	filters->filters=NULL;
	return filters;
}

void blktool_filters_free(blktool_filters*filters){
	if(!filters)return;
	list_free_all_def(filters->filters);
	free(filters);
}

static bool string_to_number(size_t*ret,const char*val){
	int64_t i;
	char*end=NULL;
	errno=0,i=strtoll(val,&end,0);
	if(i<0||errno!=0||end==val||(end&&*end))return false;
	*ret=i;
	return true;
}

static bool string_to_boolean(size_t*ret,const char*val){
	if(strcasecmp(val,"1")==0)*ret=1;
	else if(strcasecmp(val,"on")==0)*ret=1;
	else if(strcasecmp(val,"yes")==0)*ret=1;
	else if(strcasecmp(val,"y")==0)*ret=1;
	else if(strcasecmp(val,"ok")==0)*ret=1;
	else if(strcasecmp(val,"true")==0)*ret=1;
	else if(strcasecmp(val,"enable")==0)*ret=1;
	else if(strcasecmp(val,"enabled")==0)*ret=1;
	else if(strcasecmp(val,"0")==0)*ret=0;
	else if(strcasecmp(val,"off")==0)*ret=0;
	else if(strcasecmp(val,"no")==0)*ret=0;
	else if(strcasecmp(val,"n")==0)*ret=0;
	else if(strcasecmp(val,"false")==0)*ret=0;
	else if(strcasecmp(val,"disable")==0)*ret=0;
	else if(strcasecmp(val,"disabled")==0)*ret=0;
	else return false;
	return true;
}

struct filter_value_type{
	blktool_filter filter;
	blktool_filter_type type;
	blktool_conv_from_string str_conv;
}value_type[]={
	{FILTER_MOUNT,        VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_DEVICE,       VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_DISK,         VALUE_NUMBER,  STR_CONV_SIZE(string_to_number)},
	{FILTER_PART,         VALUE_NUMBER,  STR_CONV_SIZE(string_to_number)},
	{FILTER_UUID,         VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_LABEL,        VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_FORMAT,       VALUE_FORMAT,  STR_CONV_UINT32(blktool_string_to_format)},
	{FILTER_LAYOUT,       VALUE_LAYOUT,  STR_CONV_UINT32(blktool_string_to_layout)},
	{FILTER_MEDIA,        VALUE_MEDIA,   STR_CONV_UINT32(blktool_string_to_media)},
	{FILTER_GUID,         VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_NAME,         VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_TYPE,         VALUE_STRING,  STR_CONV_UNKNOWN},
	{FILTER_SIZE,         VALUE_NUMBER,  STR_CONV_SIZE(string_to_number)},
	{FILTER_IS_PART,      VALUE_BOOLEAN, STR_CONV_SIZE(string_to_boolean)},
	{FILTER_IS_BOOT,      VALUE_BOOLEAN, STR_CONV_SIZE(string_to_boolean)},
	{FILTER_SECTOR_SIZE,  VALUE_NUMBER,  STR_CONV_SIZE(string_to_number)},
	{FILTER_SECTOR_COUNT, VALUE_NUMBER,  STR_CONV_SIZE(string_to_number)},
	{FILTER_UNKNOWN,      VALUE_UNKNOWN, STR_CONV_UNKNOWN}
};

bool blktool_filters_add(
	blktool_filters*filters,
	blktool_filter filter,
	blktool_filter_type type,
	...
){
	va_list va;
	blktool_filter_data data;
	if(!filters)return false;
	va_start(va,type);
	data.filter=filter;
	data.value.value=va_arg(va,void*);
	va_end(va);
	for(size_t i=0;value_type[i].filter;i++){
		if(value_type[i].filter!=filter)continue;
		if(value_type[i].type!=type){
			if(type!=VALUE_STRING)continue;
			if(!blktool_call_conv_from_string(
				&value_type[i].str_conv,
				&data.value.size,
				data.value.string
			))continue;
		}
		list_obj_add_new_dup(
			&filters->filters,
			&data,sizeof(data)
		);
		return true;
	}
	return false;
}

bool blktool_filters_check_filter(
	blktool_filter_data*filter,
	blktool_handle*handle
){
	ssize_t ret;
	const char*str;
	if(!filter||!handle)return false;
	blktool_filter_value*value=&filter->value;
	switch(filter->filter){
		case FILTER_IS_BOOT:
			return value->boolean==blktool_is_boot(handle);
		case FILTER_IS_PART:
			return value->boolean==blktool_is_partition(handle);
		case FILTER_DEVICE:
			if(!(str=blktool_get_device_path(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_DISK:
			if((ret=blktool_get_disk_no(handle))<0)break;
			return value->size==(size_t)ret;
		case FILTER_PART:
			if((ret=blktool_get_part_no(handle))<0)break;
			return value->size==(size_t)ret;
		case FILTER_TYPE:
			if(!(str=blktool_get_type(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_LAYOUT:
			return value->layout!=LAYOUT_UNKNOWN&&blktool_get_layout(handle)==value->layout;
		case FILTER_FORMAT:
			return value->format!=FORMAT_UNKNOWN&&blktool_get_format(handle)==value->format;
		case FILTER_MEDIA:
			return value->media!=MEDIA_UNKNOWN&&blktool_get_media(handle)==value->media;
		case FILTER_SIZE:
			if((ret=blktool_get_size(handle))<0)break;
			return value->size==(size_t)ret;
		case FILTER_SECTOR_SIZE:
			if((ret=blktool_get_sector_count(handle))<0)break;
			return value->size==(size_t)ret;
		case FILTER_SECTOR_COUNT:
			if((ret=blktool_get_sector_size(handle))<0)break;
			return value->size==(size_t)ret;
		case FILTER_MOUNT:
			if(!(str=blktool_get_mount_point(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_UUID:
			if(!(str=blktool_get_uuid(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_LABEL:
			if(!(str=blktool_get_label(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_GUID:
			if(!(str=blktool_get_guid(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		case FILTER_NAME:
			if(!(str=blktool_get_name(handle)))break;
			return value->string&&strcasecmp(str,value->value)==0;
		default:;
	}
	return false;
}

bool blktool_filters_check(
	blktool_filters*filters,
	blktool_handle*handle
){
	list*l;
	if(!handle)return false;
	if(filters&&(l=list_first(filters->filters)))do{
		LIST_DATA_DECLARE(filter,l,blktool_filter_data*);
		if(!blktool_filters_check_filter(filter,handle))return false;
	}while((l=l->next));
	return true;
}