/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"shlwapi.lib")

void blktool_free_handle(blktool_handle*handle){
	if(!handle)return;
	handle->refcnt--;
	if(handle->refcnt>0)return;
	if(handle->handle)NtClose(handle->handle);
	free(handle);
}

static void guid_to_string(char*buff,size_t len,GUID*guid){
	memset(buff,0,len);
	snprintf(
		buff,len-1,"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid->Data1,guid->Data2,guid->Data3,guid->Data4[0],guid->Data4[1],guid->Data4[2],
		guid->Data4[3],guid->Data4[4],guid->Data4[5],guid->Data4[6],guid->Data4[7]
	);
}

static blktool_handle*blktool_handle_from_name(PWSTR name){
	blktool_handle*hand;
	if(!name||!(hand=malloc(sizeof(blktool_handle))))return NULL;
	memset(hand,0,sizeof(blktool_handle));
	snprintf(hand->path,sizeof(hand->path)-1,"\\\\.\\%ws",name);
	snprintf(hand->name,sizeof(hand->name)-1,"%ws",name);
	hand->refcnt=1;
	return hand;
}

static bool vol_hand_cmp(list*l,void*d){
	blktool_handle*hand=d;
	LIST_DATA_DECLARE(vol,l,win_volume*);
	return
		hand&&vol&&hand->disk==vol->ext.DiskNumber&&
		hand->info.StartingOffset.QuadPart==vol->ext.StartingOffset.QuadPart&&
		hand->info.PartitionLength.QuadPart==vol->ext.ExtentLength.QuadPart;
}

static bool hand_fill_vol(blktool_handle*hand,list*vols){
	list*l=list_search_one(vols,vol_hand_cmp,hand);
	if(!l)return true;
	LIST_DATA_DECLARE(vol,l,win_volume*);
	if(!vol)return false;
	strncpy(hand->label,vol->name,sizeof(hand->label)-1);
	strncpy(hand->mount,vol->mount,sizeof(hand->mount)-1);
	wcsncpy(hand->device,vol->device,(sizeof(hand->device)/sizeof(WCHAR))-1);
	if(!blktool_string_to_format(&hand->format,vol->type))
		hand->format=FORMAT_UNKNOWN;
	return true;
}

static bool blk_get_layout(
	blktool_handle*block,
	PDRIVE_LAYOUT_INFORMATION_EX*layout
){
	void*p;
	size_t size;
	HANDLE handle;
	bool ret=false;
	NTSTATUS status;
	if(!(handle=blktool_windows_get_handle(block)))return false;
	size=sizeof(DRIVE_LAYOUT_INFORMATION_EX)*8;
	if(!(*layout=malloc(size)))EDONE(blktool_set_error(
		0,"malloc %zu failed",size
	));
	while(true){
		status=NtDeviceIoControlFile(
			handle,NULL,NULL,NULL,&block->sb,
			IOCTL_DISK_GET_DRIVE_LAYOUT_EX,
			NULL,0,*layout,size
		);
		if(status==STATUS_BUFFER_TOO_SMALL){
			size*=2,p=realloc(*layout,size);
			if(!p)EDONE(blktool_set_error(
				0,"realloc %zu failed",size
			));
			*layout=p;
			continue;
		}
		if(!NT_SUCCESS(status))EDONE(blktool_set_error(
			0,"ioctl %s get layout failed: 0x%08lX",
			block->path,status
		));
		break;
	}
	ret=true;
	done:
	return ret;
}

static bool block_fill_layout(
	blktool_handle*block,
	PDRIVE_LAYOUT_INFORMATION_EX layout
){
	switch(layout->PartitionStyle){
		case PARTITION_STYLE_MBR:
			block->layout=LAYOUT_MBR;
			snprintf(
				block->uuid,
				sizeof(block->uuid)-1,
				"%08lX",layout->Mbr.Signature
			);
		break;
		case PARTITION_STYLE_GPT:
			block->layout=LAYOUT_GPT;
			guid_to_string(
				block->uuid,
				sizeof(block->uuid),
				&layout->Gpt.DiskId
			);
		break;
		default:
			blktool_set_error(0,"unknown layout for %s",block->path);
			return false;
	}
	return true;
}

static bool hand_fill_layout(
	blktool_handle*hand,
	PPARTITION_INFORMATION_EX info
){
	switch(blktool_get_layout(hand)){
		case LAYOUT_MBR:
			hand->is_boot=info->Mbr.BootIndicator!=0;
			snprintf(
				hand->part_type,
				sizeof(hand->part_type),
				"%02X",info->Mbr.PartitionType
			);
			if(hand->parent)snprintf(
				hand->part_uuid,
				sizeof(hand->part_uuid),
				"%s-%02zu",
				hand->parent->part_type,
				hand->part
			);
		break;
		case LAYOUT_GPT:
			guid_to_string(
				hand->part_uuid,
				sizeof(hand->part_uuid),
				&info->Gpt.PartitionId
			);
			guid_to_string(
				hand->part_type,
				sizeof(hand->part_type),
				&info->Gpt.PartitionType
			);
			snprintf(
				hand->part_label,
				sizeof(hand->part_label),
				"%ls",info->Gpt.Name
			);
		break;
		default:return false;
	}
	return true;
}

static bool hand_get_prop(blktool_handle*hand){
	bool ret=false;
	HANDLE handle;
	NTSTATUS status;
	STORAGE_PROPERTY_QUERY sq={
		StorageDeviceProperty,
		PropertyStandardQuery
	};
	STORAGE_DESCRIPTOR_HEADER sh={0};
	PSTORAGE_DEVICE_DESCRIPTOR sd=NULL;
	if(hand->is_part)return true;
	if(!(handle=blktool_windows_get_handle(hand)))return false;
	status=NtDeviceIoControlFile(
		handle,NULL,NULL,NULL,&hand->sb,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&sq,sizeof(sq),&sh,sizeof(sh)
	);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"ioctl %s query storage prop head failed: 0x%08lX",
		hand->path,status
	));
	if(sh.Size<=0)return false;
	if(!(sd=malloc(sh.Size)))
		EDONE(blktool_set_error(0,"alloc %d failed",sh.Size));
	status=NtDeviceIoControlFile(
		handle,NULL,NULL,NULL,&hand->sb,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&sq,sizeof(sq),sd,sh.Size
	);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"ioctl %s query storage prop failed: 0x%08lX",
		hand->path,status
	));
	if(sd->Version<sizeof(STORAGE_DEVICE_DESCRIPTOR))
		EDONE(blktool_set_error(0,"invalid version of desc"));
	switch(sd->BusType){
		case BusTypeScsi:hand->media=MEDIA_SCSI;break;
		case BusTypeAtapi:hand->media=MEDIA_IDE;break;
		case BusTypeAta:hand->media=MEDIA_IDE;break;
		case BusTypeFibre:hand->media=MEDIA_NETWORK;break;
		case BusTypeUsb:hand->media=MEDIA_USB;break;
		case BusTypeRAID:hand->media=MEDIA_RAID;break;
		case BusTypeiScsi:hand->media=MEDIA_NETWORK;break;
		case BusTypeSas:hand->media=MEDIA_SAS;break;
		case BusTypeSata:hand->media=MEDIA_SATA;break;
		case BusTypeSd:hand->media=MEDIA_SDMMC;break;
		case BusTypeMmc:hand->media=MEDIA_SDMMC;break;
		case BusTypeVirtual:hand->media=MEDIA_VIRTUAL;break;
		case BusTypeFileBackedVirtual:hand->media=MEDIA_VIRTUAL;break;
		case BusTypeSpaces:hand->media=MEDIA_VIRTUAL;break;
		case BusTypeNvme:hand->media=MEDIA_NVME;break;
		case BusTypeUfs:hand->media=MEDIA_UFS;break;
		default:;
	}
	ret=true;
	done:
	if(sd)free(sd);
	return ret;
}

static bool hand_get_geometry(blktool_handle*hand){
	bool ret=false;
	HANDLE handle;
	NTSTATUS status;
	DISK_GEOMETRY geo;
	if(hand->is_part)return true;
	if(!(handle=blktool_windows_get_handle(hand)))return false;
	status=NtDeviceIoControlFile(
		handle,NULL,NULL,NULL,&hand->sb,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,0,&geo,sizeof(geo)
	);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"ioctl %s get drive geometry head failed: 0x%08lX",
		hand->path,status
	));
	hand->sector_size=geo.BytesPerSector;
	ret=true;
	done:
	return ret;
}

static bool hand_parse_layout(
	blktool_handle*block,size_t n,
	PDRIVE_LAYOUT_INFORMATION_EX layout,
	list**lst,
	list*vols
){
	WCHAR buff[512];
	blktool_handle*hand;
	PPARTITION_INFORMATION_EX info=&layout->PartitionEntry[n];
	if(n+1!=info->PartitionNumber)return false;
	if(info->PartitionStyle!=layout->PartitionStyle)return false;
	swprintf(
		buff,sizeof(buff),
		L"%hs\\Partition%d",
		block->name,
		info->PartitionNumber
	);
	if(!(hand=blktool_handle_from_name(buff)))return false;
	hand->offset=info->StartingOffset.QuadPart;
	hand->size=info->PartitionLength.QuadPart;
	hand->part=info->PartitionNumber;
	hand->disk=block->disk;
	hand->parent=block;
	hand->is_part=true;
	block->refcnt++;
	memcpy(
		&hand->info,info,
		sizeof(PARTITION_INFORMATION_EX)
	);
	if(!hand_fill_layout(hand,info))return false;
	if(!hand_fill_vol(hand,vols))return false;
	if(list_obj_add_new(lst,hand)==0)return false;
	blktool_free_handle(hand);
	return true;
}

static bool blktool_list_children(
	blktool_handle*block,
	list**lst,list*vols
){
	bool ret=false;
	PDRIVE_LAYOUT_INFORMATION_EX layout=NULL;
	if(!blk_get_layout(block,&layout))goto done;
	if(!block_fill_layout(block,layout))goto done;
	for(DWORD n=0;n<layout->PartitionCount;n++)
		hand_parse_layout(block,n,layout,lst,vols);
	ret=true;
	done:
	if(layout)free(layout);
	return ret;
}

static void vol_read_type(PWSTR name,win_volume*info){
	WCHAR type[256],label[256];
	memset(type,0,sizeof(type));
	memset(label,0,sizeof(label));
	if(!GetVolumeInformationW(
		name,
		label,sizeof(label),
		NULL,NULL,NULL,
		type,sizeof(type)
	))return;
	if(type[0])snprintf(
		info->type,sizeof(info->type)-1,
		"%ls",type
	);
	if(label[0])snprintf(
		info->name,sizeof(info->name)-1,
		"%ls",label
	);
}

static void vol_read_mount(PWSTR name,win_volume*info){
	DWORD size=0;
	WCHAR mount[512];
	memset(mount,0,sizeof(mount));
	if(GetVolumePathNamesForVolumeNameW(
		name,mount,sizeof(mount),&size
	)&&mount[0]&&size>=2)snprintf(
		info->mount,
		sizeof(info->mount)-1,
		"%ls",mount
	);
}

static bool vol_read_ext(PWSTR name,win_volume*info){
	HANDLE hand;
	NTSTATUS status;
	UNICODE_STRING vn;
	IO_STATUS_BLOCK sb;
	OBJECT_ATTRIBUTES attr;
	VOLUME_DISK_EXTENTS ve;
	memset(&sb,0,sizeof(sb));
	memset(&vn,0,sizeof(vn));
	memset(&attr,0,sizeof(attr));
	swprintf(info->device,sizeof(info->device)-1,L"\\GLOBAL??%s",&name[3]);
	for(size_t i=wcslen(info->device);i>0&&info->device[i-1]==L'\\';i--)info->device[i-1]=0;
	RtlInitUnicodeString(&vn,info->device);
	InitializeObjectAttributes(&attr,&vn,0,NULL,NULL);
	status=NtOpenFile(
		&hand,SYNCHRONIZE,
		&attr,&sb,FILE_SHARE_READ,
		FILE_SYNCHRONOUS_IO_NONALERT
	);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"open volume %ls failed: 0x%08lX",
		info->device,status
	));
	status=NtDeviceIoControlFile(
		hand,NULL,NULL,NULL,&sb,
		IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
		NULL,0,&ve,sizeof(ve)
	);
	NtClose(hand);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"ioctl volume %ls get disk extents failed: 0x%08lX",
		info->device,status
	));
	if(ve.NumberOfDiskExtents!=1)goto done;
	memcpy(&info->ext,&ve.Extents[0],sizeof(info->ext));
	return true;
	done:return false;
}

static int get_volumes(list**vols){
	int ret=-1;
	HANDLE hand;
	WCHAR name[256];
	win_volume info;
	memset(&name,0,sizeof(name));
	hand=FindFirstVolumeW(name,sizeof(name));
	if(!hand||hand==INVALID_HANDLE_VALUE)
		EDONE(blktool_set_error(0,"FindFirstVolumeW failed"));
	do{
		memset(&info,0,sizeof(info));
		if(!vol_read_ext(name,&info))continue;
		vol_read_type(name,&info);
		vol_read_mount(name,&info);
		list_obj_add_new_dup(vols,&info,sizeof(info));
	}while(FindNextVolumeW(hand,name,sizeof(name)));
	ret=0;
	done:
	return ret;
}

static void add_hand(list**lst,PWSTR name){
	blktool_handle*hand;
	if(wcsncmp(name,L"PhysicalDrive",13)!=0)return;
	if(!(hand=blktool_handle_from_name(name)))return;
	swscanf_s(name,L"PhysicalDrive%zu",&hand->disk);
	swprintf(hand->device,sizeof(hand->device),L"\\??\\%ls",name);
	if(!hand_get_prop(hand))goto done;
	if(!hand_get_geometry(hand))goto done;
	if(list_obj_add_new(lst,hand)!=0)goto done;
	return;
	done:
	blktool_free_handle(hand);
}

list*blktool_list_impl(){
	void*n;
	HANDLE dir;
	size_t size;
	NTSTATUS status;
	bool restart=true;
	UNICODE_STRING root;
	blktool_handle*hand;
	OBJECT_ATTRIBUTES attr;
	ULONG index=0,bytes=0,start=0;
	list*lst=NULL,*vols=NULL,*l,*r=NULL;
	POBJECT_DIRECTORY_INFORMATION info=NULL;
	get_volumes(&vols);
	memset(&root,0,sizeof(root));
	memset(&attr,0,sizeof(attr));
	RtlInitUnicodeString(&root,L"\\GLOBAL??");
	InitializeObjectAttributes(&attr,&root,0,NULL,NULL);
	status=NtOpenDirectoryObject(
		&dir,DIRECTORY_QUERY|DIRECTORY_TRAVERSE,&attr
	);
	if(!NT_SUCCESS(status))EDONE(blktool_set_error(
		0,"NtOpenDirectoryObject failed: 0x%08lX",status
	));
	size=sizeof(OBJECT_DIRECTORY_INFORMATION)+(512*sizeof(WCHAR)*2);
	if(!(info=malloc(size)))EDONE(blktool_set_error(
		0,"malloc %zu failed",size
	));
	while(true){
		status=NtQueryDirectoryObject(
			dir,info,size,false,
			restart,&index,&bytes
		);
		if(status==STATUS_NO_MORE_ENTRIES)break;
		if(status==STATUS_MORE_ENTRIES){
			size*=2,n=realloc(info,size);
			if(!n)EDONE(blktool_set_error(
				0,"realloc %zu failed",size
			));
			info=n;
			continue;
		}
		if(!NT_SUCCESS(status))EDONE(blktool_set_error(
			0,"NtQueryDirectoryObject failed: 0x%08lX",status
		));
		for(ULONG i=0;i<index-start;i++)
			add_hand(&lst,info[i].Name.Buffer);
		start=index,restart=false;
	}
	if((l=list_first(lst)))do{
		hand=LIST_DATA(l,blktool_handle*);
		if(!hand||hand->is_part)continue;
		blktool_list_children(hand,&lst,vols);
	}while((l=l->next));
	r=lst;
	done:
	if(info)free(info);
	list_free_all_def(vols);
	return r;
}
