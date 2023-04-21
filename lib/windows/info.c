/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

HANDLE blktool_windows_get_handle(blktool_handle*block){
	NTSTATUS status;
	HANDLE handle=NULL;
	UNICODE_STRING disk;
	OBJECT_ATTRIBUTES attr;
	if(!block)return NULL;
	if(block->handle)return block->handle;
	if(!block->device[0])return NULL;
	RtlInitUnicodeString(&disk,block->device);
	InitializeObjectAttributes(&attr,&disk,0,NULL,NULL);
	status=NtOpenFile(
		&handle,GENERIC_READ|SYNCHRONIZE,&attr,&block->sb,
		FILE_SHARE_READ|FILE_SHARE_WRITE,FILE_SYNCHRONOUS_IO_NONALERT
	);
	if(!NT_SUCCESS(status))return blktool_set_perror(
		NULL,"NtOpenFile %hs failed: 0x%08lX",
		block->path,status
	);
	block->handle=handle;
	return handle;
}

ssize_t blktool_get_size(blktool_handle*block){
	HANDLE handle;
	NTSTATUS status;
	GET_LENGTH_INFORMATION info;
	if(!block)return -1;
	if(block->size>0)return (ssize_t)block->size;
	if(!(handle=blktool_windows_get_handle(block)))return -1;
	status=NtDeviceIoControlFile(
		handle,NULL,NULL,NULL,&block->sb,
		IOCTL_DISK_GET_LENGTH_INFO,
		NULL,0,&info,sizeof(info)
	);
	if(!NT_SUCCESS(status))return -1;
	block->size=info.Length.QuadPart;
	return (ssize_t)block->size;
}

ssize_t blktool_get_sector_size(blktool_handle*block){
	HANDLE handle;
	NTSTATUS status;
	DISK_GEOMETRY geo;
	if(!block)return -1;
	if(block->sector_size>0)return (ssize_t)block->sector_size;
	if(block->parent&&block->parent!=block){
		blktool_get_sector_size(block->parent);
		block->sector_size=block->parent->sector_size;
		return block->sector_size;
	}
	if(!(handle=blktool_windows_get_handle(block)))return -1;
	status=NtDeviceIoControlFile(
		handle,NULL,NULL,NULL,&block->sb,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,0,&geo,sizeof(geo)
	);
	if(!NT_SUCCESS(status))return blktool_set_error(
		-1,"ioctl %s get drive geometry head failed: 0x%08lX",
		block->path,status
	);
	block->sector_size=geo.BytesPerSector;
	return block->sector_size;
}

blktool_layout blktool_get_layout(blktool_handle*block){
	if(!block)return LAYOUT_UNKNOWN;
	if(block->layout!=LAYOUT_UNKNOWN)return block->layout;
	if(block->parent&&block->parent!=block){
		blktool_get_layout(block->parent);
		block->layout=block->parent->layout;
		return block->layout;
	}
	return LAYOUT_UNKNOWN;
}

blktool_media blktool_get_media(blktool_handle*block){
	if(!block)return MEDIA_UNKNOWN;
	if(block->media!=MEDIA_UNKNOWN)return block->media;
	if(block->parent&&block->parent!=block){
		blktool_get_media(block->parent);
		block->media=block->parent->media;
		return block->media;
	}
	return MEDIA_UNKNOWN;
}

const char*blktool_get_uuid(blktool_handle*block){
	DWORD skip;
	HANDLE*handle;
	size_t sector;
	LARGE_INTEGER off;
	char buffer[8192];
	if(!block)return NULL;
	if(block->uuid[0])return block->uuid;
	if(!block->parent||block->offset<=0)return NULL;
	if(!(handle=blktool_windows_get_handle(block->parent)))return NULL;
	sector=(size_t)blktool_get_sector_size(block);
	if(sector>sizeof(buffer))return NULL;
	off.QuadPart=block->offset;
	if(!SetFilePointerEx(handle,off,NULL,FILE_BEGIN))
		return blktool_set_perror(NULL,"seek serial failed: 0x%08lX",GetLastError());
	if(!ReadFile(handle,buffer,sector,&skip,NULL))
		return blktool_set_perror(NULL,"read serial failed: 0x%08lX",GetLastError());
	switch(blktool_get_format(block)){
		case FORMAT_NTFS:{
			uint64_t val=0;
			memcpy(&val,buffer+0x48,sizeof(val));
			snprintf(block->uuid,sizeof(block->uuid),"%016llX",val);
			return block->uuid;
		}
		case FORMAT_FAT12:
		case FORMAT_FAT16:
		case FORMAT_FAT32:{
			uint16_t val[2]={0,0};
			memcpy(val,buffer+0x43,sizeof(val));
			snprintf(block->uuid,sizeof(block->uuid),"%04X-%04X",val[1],val[0]);
			return block->uuid;
		}
		default:;
	}
	return NULL;
}
