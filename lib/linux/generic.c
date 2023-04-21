/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

int blktool_linux_get_fd(blktool_handle*block){
	if(!block)return -1;
	if(block->fd>=0)return block->fd;
	if(!block->path[0])return -1;
	return block->fd=open(block->path,O_RDONLY);
}

const char*blktool_get_device_path(blktool_handle*block){
	if(!block||!block->path[0])return NULL;
	return block->path;
}

bool blktool_is_partition(blktool_handle*block){
	return block?block->is_part:false;
}

ssize_t blktool_get_disk_no(blktool_handle*block){
	return block?(ssize_t)block->disk:-1;
}

ssize_t blktool_get_part_no(blktool_handle*block){
	return block&&block->is_part?(ssize_t)block->part:-1;
}
