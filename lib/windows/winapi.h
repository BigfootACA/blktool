/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef BLKTOOL_WINAPI_H
#define BLKTOOL_WINAPI_H
#include<windows.h>
#include<winternl.h>
typedef struct _OBJECT_DIRECTORY_INFORMATION{
	UNICODE_STRING Name;
	UNICODE_STRING TypeName;
}OBJECT_DIRECTORY_INFORMATION,*POBJECT_DIRECTORY_INFORMATION;
extern NTSTATUS WINAPI NtOpenDirectoryObject(
	_Out_ PHANDLE            DirectoryHandle,
	_In_  ACCESS_MASK        DesiredAccess,
	_In_  POBJECT_ATTRIBUTES ObjectAttributes
);
extern NTSTATUS WINAPI NtQueryDirectoryObject(
	_In_      HANDLE  DirectoryHandle,
	_Out_opt_ PVOID   Buffer,
	_In_      ULONG   Length,
	_In_      BOOLEAN ReturnSingleEntry,
	_In_      BOOLEAN RestartScan,
	_Inout_   PULONG  Context,
	_Out_opt_ PULONG  ReturnLength
);
#ifndef DIRECTORY_QUERY
#define DIRECTORY_QUERY 0x0001
#endif
#ifndef DIRECTORY_TRAVERSE
#define DIRECTORY_TRAVERSE 0x0002
#endif
#ifndef DIRECTORY_CREATE_OBJECT
#define DIRECTORY_CREATE_OBJECT 0x0004
#endif
#ifndef DIRECTORY_CREATE_SUBDIRECTORY
#define DIRECTORY_CREATE_SUBDIRECTORY 0x0008
#endif
#ifndef DIRECTORY_ALL_ACCESS
#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)
#endif
#endif
