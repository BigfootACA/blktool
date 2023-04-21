/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

static const char*sopts="BCHM:E:D:P:U:L:F:G:N:T:A:lpdanjreh";
static const struct option lopts[]={
	// select device
	{"boot",       no_argument,       NULL, 'B'}, // by bootable
	{"is-part",    no_argument,       NULL, 'C'}, // by is a partition
	{"is-disk",    no_argument,       NULL, 'H'}, // by is a disk
	{"mount",      required_argument, NULL, 'M'}, // by mount point
	{"device",     required_argument, NULL, 'E'}, // by device path
	{"disk",       required_argument, NULL, 'D'}, // by disk number
	{"part",       required_argument, NULL, 'P'}, // by partition number
	{"uuid",       required_argument, NULL, 'U'}, // by filesystem uuid
	{"label",      required_argument, NULL, 'L'}, // by filesystem volume label
	{"format",     required_argument, NULL, 'F'}, // by filesystem format
	{"layout",     required_argument, NULL, 'A'}, // by disk layout
	{"media",      required_argument, NULL, 'I'}, // by disk media
	{"guid",       required_argument, NULL, 'G'}, // by GPT uuid
	{"name",       required_argument, NULL, 'N'}, // by GPT name
	{"type",       required_argument, NULL, 'T'}, // by GPT type

	// operations
	{"list",                no_argument, NULL, 'l'},
	{"list-part",           no_argument, NULL, 'p'},
	{"list-disk",           no_argument, NULL, 'd'},
	{"print-all",           no_argument, NULL, 'a'},
	{"print-is-bootable",   no_argument, NULL, 0x2001},
	{"print-is-partition",  no_argument, NULL, 0x2002},
	{"print-media",         no_argument, NULL, 0x2003},
	{"print-mount",         no_argument, NULL, 0x2004},
	{"print-device",        no_argument, NULL, 0x2005},
	{"print-disk",          no_argument, NULL, 0x2006},
	{"print-part",          no_argument, NULL, 0x2007},
	{"print-uuid",          no_argument, NULL, 0x2008},
	{"print-label",         no_argument, NULL, 0x2009},
	{"print-format",        no_argument, NULL, 0x200A},
	{"print-layout",        no_argument, NULL, 0x200B},
	{"print-guid",          no_argument, NULL, 0x200C},
	{"print-name",          no_argument, NULL, 0x200D},
	{"print-type",          no_argument, NULL, 0x200E},
	{"print-size",          no_argument, NULL, 0x200F},
	{"print-readable-size", no_argument, NULL, 0x2010},
	{"print-sector-size",   no_argument, NULL, 0x2011},
	{"print-sector-count",  no_argument, NULL, 0x2012},

	// options
	{"no-head", no_argument, NULL, 'n'},
	{"json",    no_argument, NULL, 'j'},
	{"raw",     no_argument, NULL, 'r'},
	{"export",  no_argument, NULL, 'e'},
	{"help",    no_argument, NULL, 'h'},
	{NULL,0,NULL,0}
};

int blktool_do_getopt(
	int argc,char**argv,
	blktool_filters*f,
	uint64_t*opers,uint64_t*opts
){
	int o;
	while((o=b_getlopt(argc,argv,sopts,lopts,NULL))>0)switch(o){
		case 'h':return blktool_usage(1);
		case 'C':if(!blktool_filters_add(f,FILTER_IS_PART,VALUE_BOOLEAN,true))goto inv;break;
		case 'H':if(!blktool_filters_add(f,FILTER_IS_PART,VALUE_BOOLEAN,false))goto inv;break;
		case 'B':if(!blktool_filters_add(f,FILTER_IS_BOOT,VALUE_BOOLEAN,true))goto inv;break;
		case 'M':if(!blktool_filters_add(f,FILTER_MOUNT,VALUE_STRING,b_optarg))goto inv;break;
		case 'E':if(!blktool_filters_add(f,FILTER_DEVICE,VALUE_STRING,b_optarg))goto inv;break;
		case 'U':if(!blktool_filters_add(f,FILTER_UUID,VALUE_STRING,b_optarg))goto inv;break;
		case 'L':if(!blktool_filters_add(f,FILTER_LABEL,VALUE_STRING,b_optarg))goto inv;break;
		case 'F':if(!blktool_filters_add(f,FILTER_FORMAT,VALUE_STRING,b_optarg))goto inv;break;
		case 'G':if(!blktool_filters_add(f,FILTER_GUID,VALUE_STRING,b_optarg))goto inv;break;
		case 'N':if(!blktool_filters_add(f,FILTER_NAME,VALUE_STRING,b_optarg))goto inv;break;
		case 'T':if(!blktool_filters_add(f,FILTER_TYPE,VALUE_STRING,b_optarg))goto inv;break;
		case 'A':if(!blktool_filters_add(f,FILTER_LAYOUT,VALUE_STRING,b_optarg))goto inv;break;
		case 'D':if(!blktool_filters_add(f,FILTER_DISK,VALUE_STRING,b_optarg))goto inv;break;
		case 'P':if(!blktool_filters_add(f,FILTER_PART,VALUE_STRING,b_optarg))goto inv;break;
		case 'I':if(!blktool_filters_add(f,FILTER_MEDIA,VALUE_STRING,b_optarg))goto inv;break;
		case 'n':*opts|=OPT_NO_HEAD;break;
		case 'j':*opts|=OPT_JSON;break;
		case 'r':*opts|=OPT_RAW;break;
		case 'e':*opts|=OPT_EXPORT;break;
		case 'l':*opers|=OPER_LIST;break;
		case 'p':*opers|=OPER_LIST_PART;break;
		case 'd':*opers|=OPER_LIST_DISK;break;
		case 'a':*opers|=OPER_PRINT_ALL;break;
		case 0x2001:*opers|=OPER_PRINT_IS_BOOT;break;
		case 0x2002:*opers|=OPER_PRINT_IS_PART;break;
		case 0x2003:*opers|=OPER_PRINT_MEDIA;break;
		case 0x2004:*opers|=OPER_PRINT_MOUNT;break;
		case 0x2005:*opers|=OPER_PRINT_DEVICE;break;
		case 0x2006:*opers|=OPER_PRINT_DISK;break;
		case 0x2007:*opers|=OPER_PRINT_PART;break;
		case 0x2008:*opers|=OPER_PRINT_UUID;break;
		case 0x2009:*opers|=OPER_PRINT_LABEL;break;
		case 0x200A:*opers|=OPER_PRINT_FORMAT;break;
		case 0x200B:*opers|=OPER_PRINT_LAYOUT;break;
		case 0x200C:*opers|=OPER_PRINT_GUID;break;
		case 0x200D:*opers|=OPER_PRINT_NAME;break;
		case 0x200E:*opers|=OPER_PRINT_TYPE;break;
		case 0x200F:*opers|=OPER_PRINT_SIZE;break;
		case 0x2010:*opers|=OPER_PRINT_READABLE;break;
		case 0x2011:*opers|=OPER_PRINT_SECTOR_SIZE;break;
		case 0x2012:*opers|=OPER_PRINT_SECTOR_COUNT;break;
		default:return 1;
	}
	return 0;
	inv:
	fprintf(stderr,"invalid value for filter\n");
	return 1;
}