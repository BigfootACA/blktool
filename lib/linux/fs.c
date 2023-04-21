/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

#define PROC_MNT "/proc/mounts"

const char*blktool_get_mount_point(blktool_handle*block){
	dev_t dev;
	struct stat st;
	char device[4096],mount[4096];
	if(!block||!block->path[0])return NULL;
	if(block->mount[0])return block->mount;
	if(stat(block->path,&st)!=0||!S_ISBLK(st.st_mode))
		return blktool_set_perror(NULL,"target not a block");
	dev=st.st_rdev;
	FILE*f=fopen(PROC_MNT,"r");
	if(!f)return blktool_set_perror(
		NULL,"open %s failed: %m",PROC_MNT
	);
	while(!feof(f)){
		memset(device,0,sizeof(device));
		memset(mount,0,sizeof(mount));
		if(fscanf(
			f,"%s %s %*s %*s %*d %*d\n",
			device,mount
		)<0)continue;
		if(
			stat(device,&st)!=0||
			!S_ISBLK(st.st_mode)||
			st.st_rdev!=dev
		)continue;
		strncpy(
			block->mount,mount,
			sizeof(block->mount)-1
		);
		return block->mount;
	}
	return NULL;
}

blktool_media blktool_get_media(blktool_handle*block){
	size_t cnt,i;
	char buff1[4096],buff2[4096],buff3[4096];
	if(!block||!block->name[0])return MEDIA_UNKNOWN;
	if(block->media!=MEDIA_UNKNOWN)return block->media;
	if(strncmp(block->name,"hd",2)==0)block->media=MEDIA_IDE;
	else if(strncmp(block->name,"md",2)==0)block->media=MEDIA_RAID;
	else if(strncmp(block->name,"nbd",3)==0)block->media=MEDIA_NETWORK;
	else if(strncmp(block->name,"dm-",3)==0)block->media=MEDIA_VIRTUAL;
	else if(strncmp(block->name,"ram",3)==0)block->media=MEDIA_VIRTUAL;
	else if(strncmp(block->name,"loop",4)==0)block->media=MEDIA_VIRTUAL;
	else if(strncmp(block->name,"zram",4)==0)block->media=MEDIA_VIRTUAL;
	else if(strncmp(block->name,"mmcblk",6)==0)block->media=MEDIA_SDMMC;
	else if(strncmp(block->name,"nvme",4)==0)block->media=MEDIA_NVME;
	if(block->media!=MEDIA_UNKNOWN)return block->media;
	snprintf(buff2,sizeof(buff2)-1,"/sys/class/block/%s",block->name);
	if(!realpath(buff2,buff1))return MEDIA_UNKNOWN;
	if(strncmp(buff1,"/sys/devices",12)!=0)return MEDIA_UNKNOWN;
	for(cnt=0,i=0;buff1[i];i++)if(buff1[i]=='/')cnt++;
	for(i=0;i<cnt;i++){
		memset(buff2,0,sizeof(buff2));
		strlcat(buff2,"/sys/class/block/",sizeof(buff2)-1);
		strlcat(buff2,block->name,sizeof(buff2)-1);
		for(size_t k=0;k<i;k++)
			strlcat(buff2,"/..",sizeof(buff2)-1);
		strlcat(buff2,"/driver",sizeof(buff2)-1);
		if(access(buff2,F_OK)!=0||!realpath(buff2,buff3))continue;
		if(strstr(buff3,"/usb"))block->media=MEDIA_USB;
		else if(strstr(buff3,"/ufs"))block->media=MEDIA_UFS;
		else if(strstr(buff3,"/pata"))block->media=MEDIA_IDE;
		else if(strstr(buff3,"/nvme"))block->media=MEDIA_NVME;
		else if(strstr(buff3,"/mmc_host/"))block->media=MEDIA_SDMMC;
		if(block->media!=MEDIA_UNKNOWN)return block->media;
	}
	if(strstr(buff1,"/virtual/"))block->media=MEDIA_VIRTUAL;
	else if(strstr(buff1,"/mmc_host/"))block->media=MEDIA_SDMMC;
	else if(strstr(buff1,"/nvme/"))block->media=MEDIA_NVME;
	else if(strstr(buff1,"/usb"))block->media=MEDIA_USB;
	else if(strstr(buff1,"/ufs"))block->media=MEDIA_UFS;
	else if(strncmp(block->name,"sd",2)==0)block->media=MEDIA_SCSI;
	else if(strncmp(block->name,"sr",2)==0)block->media=MEDIA_SCSI;
	return block->media;
}
