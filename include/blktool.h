//
// Created by bigfoot on 2023/4/9.
//

#ifndef BLKTOOL_BLKTOOL_H
#define BLKTOOL_BLKTOOL_H
#ifdef __cplusplus
extern "C"{
#endif
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"compatible.h"
#include"blktypes.h"

/**
 * release block handle
 *
 * @param handle [in] [notnull] handle
 */
extern void blktool_free_handle(blktool_handle*handle);

/**
 * create new block filters
 *
 * @return [nullable] block filters
 */
extern blktool_filters*blktool_filters_new();

/**
 * release block filters
 *
 * @param filters [in] [notnull] block filters
 */
extern void blktool_filters_free(blktool_filters*filters);

/**
 * add a filter into filters
 *
 * @param filters [in] [notnull] block filters
 * @param type    [in] filter type
 * @param ...     [in] filter value
 * @return validate and added
 */
extern bool blktool_filters_add(
	blktool_filters*filters,
	blktool_filter filter,
	blktool_filter_type type,
	...
);

/**
 * call string convertor from
 *
 * @param conv [in]  [notnull] convertor
 * @param ret  [out] [notnull] output pointer
 * @param val  [in]  [notnull] input value
 * @return parse successful
 */
extern bool blktool_call_conv_from_string(
	blktool_conv_from_string*conv,
	size_t*ret,
	const char*val
);

/**
 * call string convertor to
 *
 * @param conv [in] [notnull] convertor
 * @param val  [in] [notnull] input
 * @param def  [in] [notnull] input value
 * @return parse successful
 */
extern const char*blktool_call_conv_to_string(
	blktool_conv_to_string*conv,
	size_t val,
	const char*def
);

/**
 * convert string to blktool_layout
 *
 * @param layout [out] [notnull] layout pointer
 * @param val    [in]  [notnull] input value
 * @return parse successful
 */
extern bool blktool_string_to_layout(
	blktool_layout*layout,
	const char*val
);

/**
 * convert blktool_layout to string
 *
 * @param layout [in] layout
 * @param def    [in] [nullable] default value
 * @return [nullable] string name
 */
extern const char*blktool_layout_to_string(
	blktool_layout layout,
	const char*def
);

/**
 * convert string to blktool_format
 *
 * @param format [out] [notnull] format pointer
 * @param val    [in]  [notnull] input value
 * @return parse successful
 */
extern bool blktool_string_to_format(
	blktool_format*format,
	const char*val
);

/**
 * convert blktool_format to string
 *
 * @param format [in] format
 * @param def    [in] [nullable] default value
 * @return [nullable] string name
 */
extern const char*blktool_format_to_string(
	blktool_format format,
	const char*def
);

/**
 * convert string to blktool_media
 *
 * @param media [out] [notnull] media pointer
 * @param val   [in]  [notnull] input value
 * @return parse successful
 */
extern bool blktool_string_to_media(
	blktool_media*media,
	const char*val
);

/**
 * convert blktool_media to string
 *
 * @param media [in] media
 * @param def   [in] [nullable] default value
 * @return [nullable] string name
 */
extern const char*blktool_media_to_string(
	blktool_media media,
	const char*def
);

/**
 * is error happen
 *
 * @return true if error happen
 */
extern bool blktool_is_error();

/**
 * get error description string
 *
 * @return error string
 */
extern const char*blktool_get_error();

/**
 * clear error
 */
extern void blktool_clear_error();

/**
 * list blocks with filters
 *
 * @param filters [in]  [nullable] block filters
 * @param result  [out] [nullable] return handles count
 * @return [nullable] handles list
 */
extern blktool_handle**blktool_list(
	blktool_filters*filters,
	size_t*result
);

/**
 * get filesystem format
 *
 * @param block [in] [notnull] block handle
 * @return format
 * @example FORMAT_EXT4
 */
extern blktool_format blktool_get_format(blktool_handle*block);

/**
 * get disk layout
 *
 * @param block [in] [notnull] block handle
 * @return layout
 * @example LAYOUT_GPT
 */
extern blktool_layout blktool_get_layout(blktool_handle*block);

/**
 * get partition type (GPT / MBR type)
 *
 * @param block [in] [notnull] block handle
 * @return GUID type (GPT) or HEX type (MBR)
 * @example C12A7328-F81F-11D2-BA4B-00A0C93EC93B (GPT ESP)
 */
extern const char*blktool_get_type(blktool_handle*block);

/**
 * get disk media
 *
 * @param block [in] [notnull] block handle
 * @return media
 * @example MEDIA_NVME
 */
extern blktool_media blktool_get_media(blktool_handle*block);

/**
 * get filesystem uuid
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] UUID string
 * @example "48204ca8-675d-4a9f-b42f-34253ab6cacc"
 * @example "666b-4033"
 */
extern const char*blktool_get_uuid(blktool_handle*block);

/**
 * get partition uuid (GPT guid)
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] UUID string
 * @example "4ce01667-478b-474b-aa83-bde11901674b"
 */
extern const char*blktool_get_guid(blktool_handle*block);

/**
 * get filesystem volume label
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] UUID string
 * @example "Linux Root"
 */
extern const char*blktool_get_label(blktool_handle*block);

/**
 * get filesystem volume label
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] UUID string
 * @example "Microsoft basic data"
 * @example "userdata"
 */
extern const char*blktool_get_name(blktool_handle*block);

/**
 * get mount point
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] first mount point
 * @example "/mnt"
 * @example "D:"
 */
extern const char*blktool_get_mount_point(blktool_handle*block);

/**
 * get device path
 *
 * @param block [in] [notnull] block handle
 * @return [nullable] first mount point
 * @example "/dev/sda1"
 * @example "\\.\PhysicalDrive0\Partition1"
 */
extern const char*blktool_get_device_path(blktool_handle*block);

/**
 * get block total size
 *
 * @param block [in] [notnull] block handle
 * @return [error-negative] block size bytes
 * @example 1000204886016 (931.51 GiB / 1000 GB)
 */
extern ssize_t blktool_get_size(blktool_handle*block);

/**
 * get block logical sector size
 *
 * @param block [in] [notnull] block handle
 * @return [error-negative] sector size bytes
 * @example 512
 * @example 4096
 */
extern ssize_t blktool_get_sector_size(blktool_handle*block);

/**
 * get block logical sector count
 *
 * @param block [in] [notnull] block handle
 * @return [error-negative] sector count
 * @example 1953525168 ( * 512 = 931.51 GiB / 1000 GB)
 */
extern ssize_t blktool_get_sector_count(blktool_handle*block);

/**
 * is block a partition
 *
 * @param block [in] [notnull] block handle
 * @return is partition
 */
extern bool blktool_is_partition(blktool_handle*block);

/**
 * is block bootable
 *
 * @param block [in] [notnull] block handle
 * @return is bootable
 */
extern bool blktool_is_boot(blktool_handle*block);

/**
 * get disk number
 *
 * @param block [in] [notnull] block handle
 * @return disk number (start from 0)
 */
extern ssize_t blktool_get_disk_no(blktool_handle*block);

/**
 * get partition number
 *
 * @param block [in] [notnull] block handle
 * @return partition number (start from 1)
 */
extern ssize_t blktool_get_part_no(blktool_handle*block);

#ifdef __cplusplus
};
#endif
#endif //BLKTOOL_BLKTOOL_H
