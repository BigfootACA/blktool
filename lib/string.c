/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"blktool.h"

const char*blktool_layout_to_string(blktool_layout layout,const char*def){
	switch(layout){
		case LAYOUT_MBR:return "mbr";
		case LAYOUT_GPT:return "gpt";
		default:return def;
	}
}

bool blktool_string_to_layout(blktool_layout*layout,const char*val){
	if(!layout||!val)return false;
	else if(strcasecmp(val,"mbr")==0)*layout=LAYOUT_MBR;
	else if(strcasecmp(val,"dos")==0)*layout=LAYOUT_MBR;
	else if(strcasecmp(val,"msdos")==0)*layout=LAYOUT_MBR;
	else if(strcasecmp(val,"gpt")==0)*layout=LAYOUT_GPT;
	else if(strcasecmp(val,"efi")==0)*layout=LAYOUT_GPT;
	else if(strcasecmp(val,"uefi")==0)*layout=LAYOUT_GPT;
	else if(strcasecmp(val,"guid")==0)*layout=LAYOUT_GPT;
	else return false;
	return true;
}

const char*blktool_format_to_string(blktool_format format,const char*def){
	switch(format){
		case FORMAT_FAT12:return "fat12";
		case FORMAT_FAT16:return "fat16";
		case FORMAT_FAT32:return "fat32";
		case FORMAT_APFS:return "apfs";
		case FORMAT_BCACHE:return "bcache";
		case FORMAT_BEFS:return "befs";
		case FORMAT_BFS:return "bfs";
		case FORMAT_BITLOCKER:return "bitlocker";
		case FORMAT_CEPH:return "ceph";
		case FORMAT_BTRFS:return "btrfs";
		case FORMAT_CRAMFS:return "cramfs";
		case FORMAT_DRBD:return "drbd";
		case FORMAT_EROFS:return "erofs";
		case FORMAT_EXFAT:return "exfat";
		case FORMAT_EXFS:return "exfs";
		case FORMAT_JBD:return "jbd";
		case FORMAT_EXT2:return "ext2";
		case FORMAT_EXT3:return "ext3";
		case FORMAT_EXT4:return "ext4";
		case FORMAT_F2FS:return "f2fs";
		case FORMAT_GFS:return "gfs";
		case FORMAT_GFS2:return "gfs2";
		case FORMAT_HFS:return "hfs";
		case FORMAT_HFSPLUS:return "hfsplus";
		case FORMAT_HPFS:return "hpfs";
		case FORMAT_ISO9660:return "iso9660";
		case FORMAT_JFS:return "jfs";
		case FORMAT_LUKS:return "luks";
		case FORMAT_LVM:return "lvm";
		case FORMAT_DM:return "dm";
		case FORMAT_MINIX:return "minix";
		case FORMAT_MPOOL:return "mpool";
		case FORMAT_NSS:return "nss";
		case FORMAT_NILFS2:return "nilfs2";
		case FORMAT_NTFS:return "ntfs";
		case FORMAT_OCFS:return "ocfs";
		case FORMAT_OCFS2:return "ocfs2";
		case FORMAT_ORACLEASM:return "oracleasm";
		case FORMAT_REFS:return "refs";
		case FORMAT_REISERFS:return "reiserfs";
		case FORMAT_REISER4:return "reiser4";
		case FORMAT_ROMFS:return "romfs";
		case FORMAT_SQUASHFS:return "squashfs";
		case FORMAT_STRATIS:return "stratis";
		case FORMAT_SWAP:return "swap";
		case FORMAT_SWSUSPEND:return "swsuspend";
		case FORMAT_XENIX:return "xenix";
		case FORMAT_SYSV:return "sysv";
		case FORMAT_UBI:return "ubi";
		case FORMAT_UBIFS:return "ubifs";
		case FORMAT_UDF:return "udf";
		case FORMAT_UFS:return "ufs";
		case FORMAT_VDO:return "vdo";
		case FORMAT_VMFS:return "vmfs";
		case FORMAT_VXFS:return "vxfs";
		case FORMAT_XFS:return "xfs";
		case FORMAT_ZFS:return "zfs";
		case FORMAT_ZONEFS:return "zonefs";
		case FORMAT_RAID:return "raid";
		default:return def;
	}
}

bool blktool_string_to_format(blktool_format*format,const char*val){
	if(!format||!val)return false;
	else if(strcasecmp(val,"fat12")==0)*format=FORMAT_FAT12;
	else if(strcasecmp(val,"fat16")==0)*format=FORMAT_FAT16;
	else if(strcasecmp(val,"fat32")==0)*format=FORMAT_FAT32;
	else if(strcasecmp(val,"vfat")==0)*format=FORMAT_FAT32;
	else if(strcasecmp(val,"apfs")==0)*format=FORMAT_APFS;
	else if(strcasecmp(val,"bcache")==0)*format=FORMAT_BCACHE;
	else if(strcasecmp(val,"befs")==0)*format=FORMAT_BEFS;
	else if(strcasecmp(val,"bfs")==0)*format=FORMAT_BFS;
	else if(strcasecmp(val,"bitlocker")==0)*format=FORMAT_BITLOCKER;
	else if(strcasecmp(val,"ceph")==0)*format=FORMAT_CEPH;
	else if(strcasecmp(val,"ceph_bluestore")==0)*format=FORMAT_CEPH;
	else if(strcasecmp(val,"btrfs")==0)*format=FORMAT_BTRFS;
	else if(strcasecmp(val,"cramfs")==0)*format=FORMAT_CRAMFS;
	else if(strcasecmp(val,"drbd")==0)*format=FORMAT_DRBD;
	else if(strcasecmp(val,"erofs")==0)*format=FORMAT_EROFS;
	else if(strcasecmp(val,"fat64")==0)*format=FORMAT_EXFAT;
	else if(strcasecmp(val,"exfat")==0)*format=FORMAT_EXFAT;
	else if(strcasecmp(val,"exfs")==0)*format=FORMAT_EXFS;
	else if(strcasecmp(val,"jbd")==0)*format=FORMAT_JBD;
	else if(strcasecmp(val,"extfs")==0)*format=FORMAT_EXT2;
	else if(strcasecmp(val,"ext2")==0)*format=FORMAT_EXT2;
	else if(strcasecmp(val,"ext3")==0)*format=FORMAT_EXT3;
	else if(strcasecmp(val,"ext4")==0)*format=FORMAT_EXT4;
	else if(strcasecmp(val,"f2fs")==0)*format=FORMAT_F2FS;
	else if(strcasecmp(val,"gfs")==0)*format=FORMAT_GFS;
	else if(strcasecmp(val,"gfs2")==0)*format=FORMAT_GFS2;
	else if(strcasecmp(val,"hfs")==0)*format=FORMAT_HFS;
	else if(strcasecmp(val,"hfsplus")==0)*format=FORMAT_HFSPLUS;
	else if(strcasecmp(val,"hfs+")==0)*format=FORMAT_HFSPLUS;
	else if(strcasecmp(val,"hpfs")==0)*format=FORMAT_HPFS;
	else if(strcasecmp(val,"iso9660")==0)*format=FORMAT_ISO9660;
	else if(strcasecmp(val,"isofs")==0)*format=FORMAT_ISO9660;
	else if(strcasecmp(val,"jfs")==0)*format=FORMAT_JFS;
	else if(strcasecmp(val,"luks")==0)*format=FORMAT_LUKS;
	else if(strcasecmp(val,"lvm")==0)*format=FORMAT_LVM;
	else if(strcasecmp(val,"lvm2")==0)*format=FORMAT_LVM;
	else if(strcasecmp(val,"dm")==0)*format=FORMAT_DM;
	else if(strcasecmp(val,"minix")==0)*format=FORMAT_MINIX;
	else if(strcasecmp(val,"mpool")==0)*format=FORMAT_MPOOL;
	else if(strcasecmp(val,"nss")==0)*format=FORMAT_NSS;
	else if(strcasecmp(val,"nilfs2")==0)*format=FORMAT_NILFS2;
	else if(strcasecmp(val,"ntfs")==0)*format=FORMAT_NTFS;
	else if(strcasecmp(val,"ocfs")==0)*format=FORMAT_OCFS;
	else if(strcasecmp(val,"ocfs2")==0)*format=FORMAT_OCFS2;
	else if(strcasecmp(val,"oracleasm")==0)*format=FORMAT_ORACLEASM;
	else if(strcasecmp(val,"refs")==0)*format=FORMAT_REFS;
	else if(strcasecmp(val,"reiserfs")==0)*format=FORMAT_REISERFS;
	else if(strcasecmp(val,"reiser4")==0)*format=FORMAT_REISER4;
	else if(strcasecmp(val,"romfs")==0)*format=FORMAT_ROMFS;
	else if(strcasecmp(val,"squashfs")==0)*format=FORMAT_SQUASHFS;
	else if(strcasecmp(val,"sfs")==0)*format=FORMAT_SQUASHFS;
	else if(strcasecmp(val,"stratis")==0)*format=FORMAT_STRATIS;
	else if(strcasecmp(val,"swap")==0)*format=FORMAT_SWAP;
	else if(strcasecmp(val,"swsuspend")==0)*format=FORMAT_SWSUSPEND;
	else if(strcasecmp(val,"xenix")==0)*format=FORMAT_XENIX;
	else if(strcasecmp(val,"sysv")==0)*format=FORMAT_SYSV;
	else if(strcasecmp(val,"ubi")==0)*format=FORMAT_UBI;
	else if(strcasecmp(val,"ubifs")==0)*format=FORMAT_UBIFS;
	else if(strcasecmp(val,"udf")==0)*format=FORMAT_UDF;
	else if(strcasecmp(val,"ufs")==0)*format=FORMAT_UFS;
	else if(strcasecmp(val,"vdo")==0)*format=FORMAT_VDO;
	else if(strcasecmp(val,"vmfs")==0)*format=FORMAT_VMFS;
	else if(strcasecmp(val,"vxfs")==0)*format=FORMAT_VXFS;
	else if(strcasecmp(val,"xfs")==0)*format=FORMAT_XFS;
	else if(strcasecmp(val,"zfs")==0)*format=FORMAT_ZFS;
	else if(strcasecmp(val,"zonefs")==0)*format=FORMAT_ZONEFS;
	else if(strcasecmp(val,"raid")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"ddf_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"drbdmanage_control_volume")==0)*format=FORMAT_DRBD;
	else if(strcasecmp(val,"drbdproxy_datalog")==0)*format=FORMAT_DRBD;
	else if(strcasecmp(val,"ext4dev")==0)*format=FORMAT_EXT4;
	else if(strcasecmp(val,"hpt45x_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"hpt37x_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"isw_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"jmicron_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"linux_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"lsi_mega_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"crypto_LUKS")==0)*format=FORMAT_LUKS;
	else if(strcasecmp(val,"LVM2_member")==0)*format=FORMAT_LVM;
	else if(strcasecmp(val,"LVM1_member")==0)*format=FORMAT_LVM;
	else if(strcasecmp(val,"DM_snapshot_cow")==0)*format=FORMAT_DM;
	else if(strcasecmp(val,"DM_verity_hash")==0)*format=FORMAT_DM;
	else if(strcasecmp(val,"DM_integrity")==0)*format=FORMAT_DM;
	else if(strcasecmp(val,"nvidia_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"oracleasm")==0)*format=FORMAT_ORACLEASM;
	else if(strcasecmp(val,"promise_fasttrack_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"silicon_medley_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"squashfs3")==0)*format=FORMAT_SQUASHFS;
	else if(strcasecmp(val,"via_raid_member")==0)*format=FORMAT_RAID;
	else if(strcasecmp(val,"VMFS_volume_member")==0)*format=FORMAT_VMFS;
	else if(strcasecmp(val,"xfs_external_log")==0)*format=FORMAT_XFS;
	else if(strcasecmp(val,"zfs_member")==0)*format=FORMAT_ZFS;
	else return false;
	return true;
}

const char*blktool_media_to_string(blktool_media media,const char*def){
	switch(media){
		case MEDIA_SCSI:return "scsi";
		case MEDIA_IDE:return "ide";
		case MEDIA_SATA:return "sata";
		case MEDIA_NVME:return "nvme";
		case MEDIA_USB:return "usb";
		case MEDIA_SAS:return "sas";
		case MEDIA_RAID:return "raid";
		case MEDIA_SDMMC:return "sdmmc";
		case MEDIA_NETWORK:return "network";
		case MEDIA_VIRTUAL:return "virtual";
		default:return def;
	}
}

bool blktool_string_to_media(blktool_media*media,const char*val){
	if(!media||!val)return false;
	if(strcasecmp(val,"scsi")==0)*media=MEDIA_SCSI;
	else if(strcasecmp(val,"ide")==0)*media=MEDIA_IDE;
	else if(strcasecmp(val,"atapi")==0)*media=MEDIA_IDE;
	else if(strcasecmp(val,"sata")==0)*media=MEDIA_SATA;
	else if(strcasecmp(val,"ahci")==0)*media=MEDIA_SATA;
	else if(strcasecmp(val,"nvme")==0)*media=MEDIA_NVME;
	else if(strcasecmp(val,"usb")==0)*media=MEDIA_USB;
	else if(strcasecmp(val,"uas")==0)*media=MEDIA_USB;
	else if(strcasecmp(val,"sas")==0)*media=MEDIA_SAS;
	else if(strcasecmp(val,"raid")==0)*media=MEDIA_RAID;
	else if(strcasecmp(val,"sdcard")==0)*media=MEDIA_SDMMC;
	else if(strcasecmp(val,"mmc")==0)*media=MEDIA_SDMMC;
	else if(strcasecmp(val,"emmc")==0)*media=MEDIA_SDMMC;
	else if(strcasecmp(val,"sdmmc")==0)*media=MEDIA_SDMMC;
	else if(strcasecmp(val,"net")==0)*media=MEDIA_NETWORK;
	else if(strcasecmp(val,"network")==0)*media=MEDIA_NETWORK;
	else if(strcasecmp(val,"virtual")==0)*media=MEDIA_VIRTUAL;
	else if(strcasecmp(val,"virt")==0)*media=MEDIA_VIRTUAL;
	else return false;
	return true;
}
