/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

static inline int add_null(
	struct libscols_line*line,
	uint64_t opts,
	uint64_t opers,
	size_t*p
){
	(void)opers;
	if(
		is_opt(opts,OPT_RAW)||
		is_opt(opts,OPT_JSON)
	)return 0;
	return scols_line_set_data(line,*p,"-");
}
static inline int add_string(
	struct libscols_line*line,
	uint64_t opts,
	uint64_t opers,
	size_t*p,
	const char*str
){
	int r;
	if(!str||!*str)r=add_null(line,opts,opers,p);
	else r=scols_line_set_data(line,*p,str);
	(*p)++;
	return r;
}

static inline int add_number(
	struct libscols_line*line,
	uint64_t opts,
	uint64_t opers,
	size_t*p,
	ssize_t size
){
	int r;
	char buff[64];
	if(size>=0){
		snprintf(
			buff,
			sizeof(buff)-1,
			"%zd",size
		);
		r=scols_line_set_data(
			line,*p,buff
		);
	}else r=add_null(
		line,opts,
		opers,p
	);
	(*p)++;
	return r;
}

static inline int add_boolean(
	struct libscols_line*line,
	size_t*p,
	bool val
){
	int r=scols_line_set_data(
		line,*p,
		val?"1":"0"
	);
	(*p)++;
	return r;
}

enum map_type{
	TYPE_NUMBER,
	TYPE_STRING,
	TYPE_ENUM,
	TYPE_BOOLEAN,
	TYPE_SIZE,
};

static struct map_table{
	enum blktool_oper oper;
	int json_type;
	enum map_type map_type;
	char*name;
	union{
		const char*(*str)(blktool_handle*block);
		int32_t(*int32_num)(blktool_handle*block);
		uint32_t(*uint32_num)(blktool_handle*block);
		int64_t(*int64_num)(blktool_handle*block);
		uint64_t(*uint64_num)(blktool_handle*block);
		bool(*boolean)(blktool_handle*block);
	}getter;
	blktool_conv_to_string conv;
}map_table[]={
	{
		.oper=OPER_PRINT_DISK,
		.json_type=SCOLS_JSON_NUMBER,
		.map_type=TYPE_NUMBER,
		.name="DISK",
		.getter.int64_num=blktool_get_disk_no,
	},{
		.oper=OPER_PRINT_PART,
		.json_type=SCOLS_JSON_NUMBER,
		.map_type=TYPE_NUMBER,
		.name="PART",
		.getter.int64_num=blktool_get_part_no,
	},{
		.oper=OPER_PRINT_DEVICE,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="DEVICE",
		.getter.str=blktool_get_device_path,
	},{
		.oper=OPER_PRINT_MOUNT,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="MOUNT",
		.getter.str=blktool_get_mount_point,
	},{
		.oper=OPER_PRINT_UUID,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="UUID",
		.getter.str=blktool_get_uuid,
	},{
		.oper=OPER_PRINT_LABEL,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="LABEL",
		.getter.str=blktool_get_label,
	},{
		.oper=OPER_PRINT_GUID,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="GUID",
		.getter.str=blktool_get_guid,
	},{
		.oper=OPER_PRINT_NAME,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="NAME",
		.getter.str=blktool_get_name,
	},{
		.oper=OPER_PRINT_SIZE,
		.json_type=SCOLS_JSON_NUMBER,
		.map_type=TYPE_NUMBER,
		.name="SIZE",
		.getter.int64_num=blktool_get_size,
	},{
		.oper=OPER_PRINT_READABLE,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_SIZE,
		.name="READABLE",
		.getter.int64_num=blktool_get_size,
	},{
		.oper=OPER_PRINT_SECTOR_SIZE,
		.json_type=SCOLS_JSON_NUMBER,
		.map_type=TYPE_NUMBER,
		.name="SECTOR-SIZE",
		.getter.int64_num=blktool_get_sector_size,
	},{
		.oper=OPER_PRINT_SECTOR_COUNT,
		.json_type=SCOLS_JSON_NUMBER,
		.map_type=TYPE_NUMBER,
		.name="SECTOR-COUNT",
		.getter.int64_num=blktool_get_sector_count,
	},{
		.oper=OPER_PRINT_TYPE,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_STRING,
		.name="TYPE",
		.getter.str=blktool_get_type,
	},{
		.oper=OPER_PRINT_FORMAT,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_ENUM,
		.name="FORMAT",
		.getter.uint32_num=blktool_get_format,
		.conv=STR_CONV_UINT32(blktool_format_to_string),
	},{
		.oper=OPER_PRINT_LAYOUT,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_ENUM,
		.name="LAYOUT",
		.getter.uint32_num=blktool_get_layout,
		.conv=STR_CONV_UINT32(blktool_layout_to_string),
	},{
		.oper=OPER_PRINT_MEDIA,
		.json_type=SCOLS_JSON_STRING,
		.map_type=TYPE_ENUM,
		.name="MEDIA",
		.getter.uint32_num=blktool_get_media,
		.conv=STR_CONV_UINT32(blktool_media_to_string),
	},{
		.oper=OPER_PRINT_IS_BOOT,
		.json_type=SCOLS_JSON_BOOLEAN,
		.map_type=TYPE_BOOLEAN,
		.name="IS-BOOTABLE",
		.getter.boolean=blktool_is_boot,
	},{
		.oper=OPER_PRINT_IS_PART,
		.json_type=SCOLS_JSON_BOOLEAN,
		.map_type=TYPE_BOOLEAN,
		.name="IS-PART",
		.getter.boolean=blktool_is_partition,
	},
	{0,0,0,NULL,{NULL},STR_CONV_UNKNOWN}
};

bool blktool_add_handle(
	struct libscols_table*table,
	uint64_t opts,uint64_t opers,
	blktool_handle*hand
){
	int r;
	ssize_t val_s;
	size_t p=0,val_u;
	const char*val;
	char buf[4096];
	struct map_table*v;
	struct libscols_line*line;
	if(!(line=scols_table_new_line(table,NULL)))return false;
	for(size_t i=0;(v=&map_table[i])->name;i++){
		if(!is_oper(opers,v->oper))continue;
		switch(v->map_type){
			case TYPE_STRING:r=add_string(line,opts,opers,&p,v->getter.str(hand));break;
			case TYPE_NUMBER:r=add_number(line,opts,opers,&p,v->getter.int64_num(hand));break;
			case TYPE_BOOLEAN:r=add_boolean(line,&p,v->getter.boolean(hand));break;
			case TYPE_ENUM:
				val_u=v->getter.uint32_num(hand);
				val=blktool_call_conv_to_string(&v->conv,val_u,NULL);
				r=add_string(line,opts,opers,&p,val);
			break;
			case TYPE_SIZE:
				memset(buf,0,sizeof(buf));
				if((val_s=v->getter.int64_num(hand))>=0)
					make_readable_str_buf(buf,sizeof(buf),val_s,1,0);
				r=add_string(line,opts,opers,&p,buf);
			break;
			default:r=-1;
		}
		if(r!=0)return false;
	}
	return true;
}

bool blktool_add_header(
	struct libscols_table*table,
	uint64_t opts,
	uint64_t opers
){
	(void)opts;
	struct libscols_column*col;
	for(size_t i=0;map_table[i].name;i++){
		if(!is_oper(opers,map_table[i].oper))continue;
		if(!(col=scols_new_column()))return false;
		if(scols_column_set_name(col,map_table[i].name)!=0)goto done;
		if(scols_column_set_json_type(col,map_table[i].json_type)!=0)goto done;
		if(scols_table_add_column(table,col)!=0)goto done;
		scols_unref_column(col);
	}
	return true;
	done:
	scols_unref_column(col);
	return false;
}
