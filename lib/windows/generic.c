/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

ssize_t blktool_get_sector_count(blktool_handle*block){
	if(blktool_get_size(block)<=0)return -1;
	if(blktool_get_sector_size(block)<=0)return -1;
	return block->size/block->sector_size;
}

const char*blktool_get_mount_point(blktool_handle*block){
	return block?block->mount:NULL;
}

const char*blktool_get_device_path(blktool_handle*block){
	return block?block->path:NULL;
}

bool blktool_is_partition(blktool_handle*block){
	return block?block->is_part:false;
}

bool blktool_is_boot(blktool_handle*block){
	return block?block->is_boot:false;
}

ssize_t blktool_get_disk_no(blktool_handle*block){
	return block?(ssize_t)block->disk:-1;
}

ssize_t blktool_get_part_no(blktool_handle*block){
	return block&&block->is_part?(ssize_t)block->part:-1;
}

blktool_format blktool_get_format(blktool_handle*block){
	return block?block->format:FORMAT_UNKNOWN;
}

const char*blktool_get_type(blktool_handle*block){
	return block&&block->part_type[0]?block->part_type:NULL;
}

const char*blktool_get_guid(blktool_handle*block){
	return block&&block->part_uuid[0]?block->part_uuid:NULL;
}

const char*blktool_get_label(blktool_handle*block){
	return block&&block->label[0]?block->label:NULL;
}

const char*blktool_get_name(blktool_handle*block){
	return block&&block->part_label[0]?block->part_label:NULL;
}
