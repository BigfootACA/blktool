/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"
#include<linux/fs.h>
#include<sys/ioctl.h>

ssize_t blktool_get_size(blktool_handle*disk){
	int fd;
	ssize_t ret=-1;
	if(!disk||!disk->path[0])return -1;
	if(disk->size>0)return (ssize_t)disk->size;
	if((fd=blktool_linux_get_fd(disk))<0)
		EDONE(blktool_set_error(0,"open disk failed: %m"));
	if(ioctl(fd,BLKGETSIZE64,&disk->size)!=0||disk->size==0)
		EDONE(blktool_set_error(0,"ioctl disk BLKGETSIZE64 failed: %m"));
	ret=(ssize_t)disk->size;
	done:
	return ret;
}

ssize_t blktool_get_sector_size(blktool_handle*disk){
	int fd;
	ssize_t ret=-1;
	if(!disk||!disk->path[0])return -1;
	if(disk->sector_size>0)
		return (ssize_t)disk->sector_size;
	if((fd=blktool_linux_get_fd(disk))<0)
		EDONE(blktool_set_error(0,"open disk failed: %m"));
	if(ioctl(fd,BLKSSZGET,&disk->sector_size)!=0||disk->sector_size==0)
		EDONE(blktool_set_error(0,"ioctl disk BLKSSZGET failed: %m"));
	ret=(ssize_t)disk->sector_size;
	done:
	return ret;
}

ssize_t blktool_get_sector_count(blktool_handle*disk){
	ssize_t ret=-1;
	if(!disk||!disk->path[0])return -1;
	if(disk->sector_count>0)
		return (ssize_t)disk->sector_count;
	blktool_get_size(disk);
	blktool_get_sector_size(disk);
	if(disk->size<=0||disk->sector_size<=0)goto done;
	disk->sector_count=disk->size/disk->sector_size;
	ret=(ssize_t)disk->sector_count;
	done:
	return ret;
}
