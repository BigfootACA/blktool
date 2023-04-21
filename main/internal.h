/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef BLKTOOL_INTERNAL_H
#define BLKTOOL_INTERNAL_H
#include"getopt.h"
#include"blktool.h"
#include"defines.h"
#include"smartcols.h"

#define is_oper(opers,oper)((opers&oper)==oper)
#define is_opt(opts,opt)((opts&opt)==opt)

enum blktool_oper{
	OPER_LIST               = 0x0000000000000001ULL,
	OPER_LIST_DISK          = 0x0000000000000002ULL,
	OPER_LIST_PART          = 0x0000000000000004ULL,
	OPER_PRINT_ALL          = 0xFFFFFFFFFFFFFF00ULL,
	OPER_PRINT_IS_BOOT      = 0x0000000000000100ULL,
	OPER_PRINT_IS_PART      = 0x0000000000000200ULL,
	OPER_PRINT_MEDIA        = 0x0000000000000400ULL,
	OPER_PRINT_MOUNT        = 0x0000000000000800ULL,
	OPER_PRINT_DEVICE       = 0x0000000000001000ULL,
	OPER_PRINT_DISK         = 0x0000000000002000ULL,
	OPER_PRINT_PART         = 0x0000000000004000ULL,
	OPER_PRINT_UUID         = 0x0000000000008000ULL,
	OPER_PRINT_LABEL        = 0x0000000000010000ULL,
	OPER_PRINT_FORMAT       = 0x0000000000020000ULL,
	OPER_PRINT_LAYOUT       = 0x0000000000040000ULL,
	OPER_PRINT_GUID         = 0x0000000000080000ULL,
	OPER_PRINT_NAME         = 0x0000000000100000ULL,
	OPER_PRINT_TYPE         = 0x0000000000200000ULL,
	OPER_PRINT_READABLE     = 0x0000000000400000ULL,
	OPER_PRINT_SIZE         = 0x0000000000800000ULL,
	OPER_PRINT_SECTOR_SIZE  = 0x0000000001000000ULL,
	OPER_PRINT_SECTOR_COUNT = 0x0000000002000000ULL,
};
enum blktool_opts{
	OPT_NO_HEAD             = 0x0000000000000001ULL,
	OPT_JSON                = 0x0000000000000002ULL,
	OPT_RAW                 = 0x0000000000000004ULL,
	OPT_EXPORT              = 0x0000000000000008ULL,
};
extern int blktool_usage(int r);
extern bool blktool_add_header(
	struct libscols_table*table,
	uint64_t opts,
	uint64_t opers
);
extern bool blktool_add_handle(
	struct libscols_table*table,
	uint64_t opts,
	uint64_t opers,
	blktool_handle*hand
);
extern int blktool_do_getopt(
	int argc,
	char**argv,
	blktool_filters*f,
	uint64_t*opers,
	uint64_t*opts
);
#endif
