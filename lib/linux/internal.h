/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef BLKTOOL_LINUX_INTERNAL_H
#define BLKTOOL_LINUX_INTERNAL_H
#include"../internal.h"
#if !defined(__linux__) || defined(WIN32)
#error not Linux
#endif
#include<ctype.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include<sys/stat.h>
#include<blkid/blkid.h>
#include<libfdisk/libfdisk.h>

typedef struct blktool_handle{
	int refcnt;
	char path[4096];
	char mount[4096];
	char name[256];
	char uuid[128];
	char label[256];
	char part_type[128];
	char part_uuid[128];
	char part_label[256];
	int fd;
	bool is_part;
	blktool_handle*parent;
	blktool_format format;
	blktool_layout layout;
	blktool_media media;
	struct fdisk_context*ctx;
	struct fdisk_partition*pa;
	struct fdisk_parttype*pt;
	time_t last;
	size_t size;
	size_t sector_size;
	size_t sector_count;
	size_t disk;
	size_t part;
}blktool_handle;

extern int blktool_linux_get_fd(blktool_handle*disk);
extern ssize_t fd_read_file(int at,char*buff,size_t len,bool lf,char*path,...);
extern ssize_t read_file(char*buff,size_t len,bool lf,char*path,...);
extern bool fd_read_file_num(int fd,int64_t*out,int base,char*path,...);
extern bool read_file_num(int64_t*out,int base,char*path,...);
extern bool fd_is_type(int fd,int err,mode_t type,const char*path,...);
#endif
