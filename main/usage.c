/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

int blktool_usage(int r){
	fprintf(stderr,
		"Usage: blktool [FILTERS...] [FIELDS...]\n"
		"Block Tools\n"
		"Filters:\n"
		"\t-B, --boot:            By bootable\n"
		"\t-M, --mount MOUNT:     By mount point\n"
		"\t-E, --device DEVICE:   By device path\n"
		"\t-D, --disk DISK:       By disk number\n"
		"\t-P, --part PART:       By partition number\n"
		"\t-U, --uuid UUID:       By filesystem uuid\n"
		"\t-L, --label LABEL:     By filesystem volume label\n"
		"\t-F, --format FORMAT:   By filesystem format\n"
		"\t-A, --layout LAYOUT:   By disk layout\n"
		"\t-G, --guid GUID:       By GPT uuid\n"
		"\t-N, --name NAME:       By GPT name\n"
		"\t-T, --type TYPE:       By GPT type\n"
		"Operations:\n"
		"\t-l, --list:            List all match blocks\n"
		"\t-p, --list-part:       List all match partitions\n"
		"\t-d, --list-disk:       List all match disks\n"
		"\t-a, --print-all:       Print all items\n"
		"\t--print-is-bootable:   Print is bootable\n"
		"\t--print-is-partition:  Print is partition\n"
		"\t--print-media:         Print disk media type\n"
		"\t--print-mount:         Print mount point\n"
		"\t--print-device:        Print device path\n"
		"\t--print-disk:          Print disk number\n"
		"\t--print-part:          Print partition number\n"
		"\t--print-uuid:          Print filesystem uuid\n"
		"\t--print-label:         Print filesystem volume label\n"
		"\t--print-format:        Print filesystem format\n"
		"\t--print-layout:        Print disk layout\n"
		"\t--print-guid:          Print GPT/MBR id\n"
		"\t--print-name:          Print GPT name\n"
		"\t--print-type:          Print GPT/MBR type\n"
		"\t--print-size:          Print total size\n"
		"\t--print-readable-size: Print readable size\n"
		"\t--print-sector-size:   Print sector size\n"
		"\t--print-sector-count:  Print sector count\n"
		"Options:\n"
		"\t-n, --no-head:         Do not print header\n"
		"\t-j, --json:            Output as JSON format\n"
		"\t-r, --raw:             Output as RAW format\n"
		"\t-e, --export:          Output as export format\n"
		"\t-h, --help:            Show this help\n"
	);
	return r;
}
