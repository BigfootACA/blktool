/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef BLKTOOL_WINDOWS_INTERNAL_H
#define BLKTOOL_WINDOWS_INTERNAL_H
#include"../internal.h"
#if defined(__linux__) || !defined(WIN32)
#error not Windows
#endif
#include<stdio.h>
#include<windows.h>
#include<wchar.h>
#include<ntstatus.h>
#include<diskguid.h>
#include<winternl.h>
#include<ntddscsi.h>
#include<ntdddisk.h>
#include<initguid.h>
#include"winapi.h"

typedef struct blktool_handle{
	int refcnt;
	char path[512];
	char name[256];
	char uuid[128];
	char label[256];
	char part_type[128];
	char part_uuid[128];
	char part_label[256];
	char mount[512];
	WCHAR device[512];
	blktool_handle*parent;
	HANDLE handle;
	IO_STATUS_BLOCK sb;
	PARTITION_INFORMATION_EX info;
	blktool_format format;
	blktool_layout layout;
	blktool_media media;
	bool is_boot;
	bool is_part;
	size_t offset;
	size_t size;
	size_t sector_size;
	size_t disk;
	size_t part;
}blktool_handle;

typedef struct win_volume{
	WCHAR device[512];
	char name[256];
	char mount[1024];
	char type[256];
	DISK_EXTENT ext;
}win_volume;

extern HANDLE blktool_windows_get_handle(blktool_handle*block);
#endif
