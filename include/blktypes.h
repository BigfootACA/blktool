//
// Created by bigfoot on 2023/4/17.
//

#ifndef BLKTOOL_BLKTYPES_H
#define BLKTOOL_BLKTYPES_H
#include<assert.h>

typedef struct blktool_handle blktool_handle;
typedef struct blktool_filters blktool_filters;
typedef bool(*blktool_int32_conv_from_string)(int32_t*ret,const char*val);
typedef const char*(*blktool_int32_conv_to_string)(int32_t val,const char*def);
typedef bool(*blktool_uint32_conv_from_string)(uint32_t*ret,const char*val);
typedef const char*(*blktool_uint32_conv_to_string)(uint32_t val,const char*def);
typedef bool(*blktool_int64_conv_from_string)(int64_t*ret,const char*val);
typedef const char*(*blktool_int64_conv_to_string)(int64_t val,const char*def);
typedef bool(*blktool_uint64_conv_from_string)(uint64_t*ret,const char*val);
typedef const char*(*blktool_uint64_conv_to_string)(uint64_t val,const char*def);
typedef bool(*blktool_size_conv_from_string)(size_t*ret,const char*val);
typedef const char*(*blktool_size_conv_to_string)(size_t val,const char*def);
typedef bool(*blktool_ssize_conv_from_string)(ssize_t*ret,const char*val);
typedef const char*(*blktool_ssize_conv_to_string)(ssize_t val,const char*def);
typedef enum blktool_conv_type{
	CONV_UNKNOWN,
	CONV_SIZE,
	CONV_SSIZE,
	CONV_INT32,
	CONV_UINT32,
	CONV_INT64,
	CONV_UINT64,
}blktool_conv_type;
typedef struct blktool_conv_from_string{
	blktool_conv_type type;
	union{
		blktool_size_conv_from_string size;
		blktool_ssize_conv_from_string ssize;
		blktool_int32_conv_from_string int32;
		blktool_int64_conv_from_string int64;
		blktool_uint32_conv_from_string uint32;
		blktool_uint64_conv_from_string uint64;
	};
}blktool_conv_from_string;
typedef struct blktool_conv_to_string{
	blktool_conv_type type;
	union{
		blktool_size_conv_to_string size;
		blktool_ssize_conv_to_string ssize;
		blktool_int32_conv_to_string int32;
		blktool_int64_conv_to_string int64;
		blktool_uint32_conv_to_string uint32;
		blktool_uint64_conv_to_string uint64;
	};
}blktool_conv_to_string;
#define STR_CONV_UNKNOWN {.type=CONV_UNKNOWN,.size=NULL}
#define STR_CONV_SIZE(_func) {.type=CONV_SIZE,.size=(_func)}
#define STR_CONV_SSIZE(_func) {.type=CONV_SSIZE,.ssize=(_func)}
#define STR_CONV_INT32(_func) {.type=CONV_INT32,.int32=(_func)}
#define STR_CONV_INT64(_func) {.type=CONV_INT64,.int64=(_func)}
#define STR_CONV_UINT32(_func) {.type=CONV_UINT32,.uint32=(_func)}
#define STR_CONV_UINT64(_func) {.type=CONV_UINT64,.uint64=(_func)}

typedef enum blktool_filter{
	FILTER_UNKNOWN,
	FILTER_MOUNT,
	FILTER_DEVICE,
	FILTER_DISK,
	FILTER_PART,
	FILTER_UUID,
	FILTER_LABEL,
	FILTER_FORMAT,
	FILTER_LAYOUT,
	FILTER_MEDIA,
	FILTER_GUID,
	FILTER_NAME,
	FILTER_TYPE,
	FILTER_SIZE,
	FILTER_IS_PART,
	FILTER_IS_BOOT,
	FILTER_SECTOR_SIZE,
	FILTER_SECTOR_COUNT,
}blktool_filter;

typedef enum blktool_filter_type{
	VALUE_UNKNOWN,
	VALUE_BOOLEAN,
	VALUE_STRING,
	VALUE_NUMBER,
	VALUE_LAYOUT,
	VALUE_FORMAT,
	VALUE_MEDIA,
}blktool_filter_type;

typedef enum blktool_format{
	FORMAT_UNKNOWN,
	FORMAT_FAT12,
	FORMAT_FAT16,
	FORMAT_FAT32,
	FORMAT_APFS,
	FORMAT_BCACHE,
	FORMAT_BEFS,
	FORMAT_BFS,
	FORMAT_BITLOCKER,
	FORMAT_CEPH,
	FORMAT_BTRFS,
	FORMAT_CRAMFS,
	FORMAT_DRBD,
	FORMAT_EROFS,
	FORMAT_EXFAT,
	FORMAT_EXFS,
	FORMAT_JBD,
	FORMAT_EXT2,
	FORMAT_EXT3,
	FORMAT_EXT4,
	FORMAT_F2FS,
	FORMAT_GFS,
	FORMAT_GFS2,
	FORMAT_HFS,
	FORMAT_HFSPLUS,
	FORMAT_HPFS,
	FORMAT_ISO9660,
	FORMAT_JFS,
	FORMAT_LUKS,
	FORMAT_LVM,
	FORMAT_DM,
	FORMAT_MINIX,
	FORMAT_MPOOL,
	FORMAT_NSS,
	FORMAT_NILFS2,
	FORMAT_NTFS,
	FORMAT_OCFS,
	FORMAT_OCFS2,
	FORMAT_ORACLEASM,
	FORMAT_REFS,
	FORMAT_REISERFS,
	FORMAT_REISER4,
	FORMAT_ROMFS,
	FORMAT_SQUASHFS,
	FORMAT_STRATIS,
	FORMAT_SWAP,
	FORMAT_SWSUSPEND,
	FORMAT_XENIX,
	FORMAT_SYSV,
	FORMAT_UBI,
	FORMAT_UBIFS,
	FORMAT_UDF,
	FORMAT_UFS,
	FORMAT_VDO,
	FORMAT_VMFS,
	FORMAT_VXFS,
	FORMAT_XFS,
	FORMAT_ZFS,
	FORMAT_ZONEFS,
	FORMAT_RAID,
}blktool_format;

typedef enum blktool_type{
	TYPE_ESP,
	TYPE_MBR,
	TYPE_
}blktool_type;

typedef enum blktool_layout{
	LAYOUT_UNKNOWN,
	LAYOUT_MBR,
	LAYOUT_GPT,
}blktool_layout;

typedef enum blktool_media{
	MEDIA_UNKNOWN,
	MEDIA_SCSI,
	MEDIA_IDE,
	MEDIA_SATA,
	MEDIA_NVME,
	MEDIA_USB,
	MEDIA_SAS,
	MEDIA_UFS,
	MEDIA_RAID,
	MEDIA_SDMMC,
	MEDIA_NETWORK,
	MEDIA_VIRTUAL,
}blktool_media;

#endif //BLKTOOL_BLKTYPES_H
