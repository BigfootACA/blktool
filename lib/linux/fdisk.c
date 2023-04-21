/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

static struct fdisk_context*blktool_get_fdisk_ctx(
	blktool_handle*block
){
	struct fdisk_context*ctx;
	if(!block)return NULL;
	if(block->ctx)return block->ctx;
	if(block->is_part)return blktool_get_fdisk_ctx(block->parent);
	if(!(ctx=fdisk_new_context()))goto done;
	if(fdisk_assign_device(ctx,block->path,true)!=0)goto done;
	block->ctx=ctx;
	return ctx;
	done:
	fdisk_unref_context(ctx);
	return NULL;
}

static struct fdisk_partition*blktool_get_fdisk_part(
	blktool_handle*block
){
	struct fdisk_context*ctx=NULL;
	struct fdisk_partition*ret=NULL;
	if(!block||!block->is_part)return NULL;
	if(block->pa)return block->pa;
	if(!(ctx=blktool_get_fdisk_ctx(block)))return NULL;
	if(fdisk_get_partition(ctx,block->part-1,&ret)!=0)return NULL;
	block->pa=ret;
	return ret;
}

bool blktool_is_boot(blktool_handle*block){
	struct fdisk_partition*part=NULL;
	if(!(part=blktool_get_fdisk_part(block)))return false;
	return fdisk_partition_is_bootable(part)!=0;
}

const char*blktool_get_type(blktool_handle*block){
	if(!block)return NULL;
	if(block->part_type[0])return block->part_type;
	if(!block->pt){
		struct fdisk_partition*part=NULL;
		if(!(part=blktool_get_fdisk_part(block)))return false;
		if(!(block->pt=fdisk_partition_get_type(part)))return false;
	}
	switch(block->layout){
		case LAYOUT_MBR:{
			unsigned int type;
			if((type=fdisk_parttype_get_code(block->pt))<=0)return false;
			snprintf(block->part_type,sizeof(block->part_type)-1,"%02X",type);
			return block->part_type;
		}
		case LAYOUT_GPT:{
			const char*type;
			if(!(type=fdisk_parttype_get_string(block->pt)))break;
			strncpy(block->part_type,type,sizeof(block->part_type)-1);
			return block->part_type;
		}
		default:;
	}
	return NULL;
}
