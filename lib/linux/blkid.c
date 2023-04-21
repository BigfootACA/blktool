/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"
#include<threads.h>

static thread_local blkid_cache cache=NULL;

static blktool_format blktool_check_fat(blktool_handle*disk){
	char buff[8];
	int fd;
	blktool_format ret=FORMAT_UNKNOWN;
	if(!disk->path[0])return -1;
	if((fd=open(disk->path,O_RDONLY))<0)
		EDONE(blktool_set_error(0,"open disk failed: %m"));
	if(pread(fd,buff,sizeof(buff),0x52)!=8)
		EDONE(blktool_set_error(0,"read fat type failed: %m"));
	if(memcmp(buff,"FAT     ",sizeof(buff))==0)ret=FORMAT_FAT12;
	else if(memcmp(buff,"FAT12   ",sizeof(buff))==0)ret=FORMAT_FAT12;
	else if(memcmp(buff,"FAT16   ",sizeof(buff))==0)ret=FORMAT_FAT16;
	else if(memcmp(buff,"FAT32   ",sizeof(buff))==0)ret=FORMAT_FAT32;
	done:
	if(fd>0)close(fd);
	return ret;
}

blktool_format blktool_get_format(blktool_handle*disk){
	char*val;
	if(!disk||!disk->path[0])return FORMAT_UNKNOWN;
	if(!cache)blkid_get_cache(&cache,NULL);
	if(disk->format!=FORMAT_UNKNOWN)return disk->format;
	if((val=blkid_get_tag_value(cache,"TYPE",disk->path))){
		if(strcasecmp(val,"vfat")==0)
			disk->format=blktool_check_fat(disk);
		else if(!blktool_string_to_format(&disk->format,val))
			disk->format=FORMAT_UNKNOWN;
		free(val);
	}
	return disk->format;
}

blktool_layout blktool_get_layout(blktool_handle*disk){
	char*val;
	if(!disk||!disk->path[0])return LAYOUT_UNKNOWN;
	if(!cache)blkid_get_cache(&cache,NULL);
	if(disk->layout!=LAYOUT_UNKNOWN)return disk->layout;
	if((val=blkid_get_tag_value(cache,"PTTYPE",disk->path))){
		if(strcasecmp(val,"gpt")==0)disk->layout=LAYOUT_GPT;
		if(strcasecmp(val,"dos")==0)disk->layout=LAYOUT_MBR;
		free(val);
	}
	return disk->layout;
}

static const char*blktool_str_get(
	blktool_handle*disk,
	const char**tag,
	char*buff,
	size_t len,
	bool upper
){
	char*val;
	if(!disk->path[0]||!buff)return NULL;
	if(!cache)blkid_get_cache(&cache,NULL);
	for(size_t i=0;tag[i]&&!(val=blkid_get_tag_value(cache,tag[i],disk->path));i++);
	if(!val)return NULL;
	memset(buff,0,len);
	strncpy(buff,val,len-1);
	if(upper)for(size_t i=0;buff[i]&&i<len;i++)
		if(islower(buff[i]))buff[i]=toupper(buff[i]);
	free(val);
	return buff;
}

const char*blktool_get_uuid(blktool_handle*disk){
	if(!disk)return NULL;
	if(disk->uuid[0])return disk->uuid;
	return blktool_str_get(
		disk,(const char*[]){"UUID","PTUUID",NULL},
		disk->uuid,sizeof(disk->uuid),true
	);
}

const char*blktool_get_guid(blktool_handle*disk){
	if(!disk)return NULL;
	if(disk->part_uuid[0])return disk->part_uuid;
	return blktool_str_get(
		disk,(const char*[]){"PARTUUID",NULL},
		disk->part_uuid,sizeof(disk->part_uuid),true
	);
}

const char*blktool_get_label(blktool_handle*disk){
	if(!disk)return NULL;
	if(disk->label[0])return disk->label;
	return blktool_str_get(
		disk,(const char*[]){"LABEL",NULL},
		disk->label,sizeof(disk->label),false
	);
}

const char*blktool_get_name(blktool_handle*disk){
	if(!disk)return NULL;
	if(disk->part_label[0])return disk->part_label;
	return blktool_str_get(
		disk,(const char*[]){"PARTLABEL",NULL},
		disk->part_label,sizeof(disk->part_label),false
	);
}
