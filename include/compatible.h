//
// Created by bigfoot on 2023/4/10.
//

#ifndef BLKTOOL_COMPATIBLE_H
#define BLKTOOL_COMPATIBLE_H
#include"config.h"
#include<string.h>
#include<stdint.h>
#ifdef __linux__
#include<sys/types.h>
#endif
#ifndef HAVE_STRNCASECMP
extern int strncasecmp(const char*s1,const char*s2,size_t n);
#endif
#ifndef HAVE_STRCASECMP
extern int strcasecmp(const char*s1,const char*s2);
#endif
#ifndef HAVE_STRNDUP
extern char*strndup(const char*s,size_t n);
#endif
#ifndef HAVE_WCWIDTH
#include<wchar.h>
extern int wcwidth(wchar_t wc);
#endif
#ifndef HAVE_GETPROGNAME
extern const char*getprogname(void);
#endif
#ifndef HAVE_MEMPCPY
extern void*mempcpy(void*dest,const void*src,size_t n);
#endif
#if \
	SIZE_CHAR != 1 || SIZE_UNSIGNED_CHAR != 1 ||\
	SIZE_SHORT != 2 || SIZE_UNSIGNED_SHORT != 2 || \
	SIZE_INT != 4 || SIZE_UNSIGNED_INT != 4 || \
	SIZE_LONG_LONG != 8 || SIZE_UNSIGNED_LONG_LONG != 8 ||\
	(SIZE_LONG != 4 && SIZE_LONG != 8) ||\
	(SIZE_UNSIGNED_LONG != 4 && SIZE_UNSIGNED_LONG != 8)
#error unsupported type size
#endif
#ifndef HAVE_SIZE_INT8_T
typedef char int8_t;
#endif
#ifndef HAVE_SIZE_INT16_T
typedef short int16_t;
#endif
#ifndef HAVE_SIZE_INT32_T
typedef int int32_t;
#endif
#ifndef HAVE_SIZE_INT64_T
#if SIZE_LONG == 8
typedef long int64_t;
#elif SIZE_LONG_LONG == 8
typedef long long int64_t;
#else
#error int64_t not found
#endif
#endif
#ifndef HAVE_SIZE_UINT8_T
typedef unsigned char uint8_t;
#endif
#ifndef HAVE_SIZE_UINT16_T
typedef unsigned short uint16_t;
#endif
#ifndef HAVE_SIZE_UINT32_T
typedef unsigned int uint32_t;
#endif
#ifndef HAVE_SIZE_UINT64_T
#if SIZE_UNSIGNED_LONG == 8
typedef unsigned long uint64_t;
#elif SIZE_UNSIGNED_LONG_LONG == 8
typedef unsigned long long uint64_t;
#else
#error uint64_t not found
#endif
#endif
#ifndef HAVE_SIZE_SSIZE_T
#if SIZE_POINTER == 8
typedef int64_t ssize_t;
#elif SIZE_POINTER == 4
typedef int32_t ssize_t;
#else
#error unknown pointer size
#endif
#endif
#ifndef HAVE_SIZE_SIZE_T
#if SIZE_POINTER == 8
typedef uint64_t size_t;
#elif SIZE_POINTER == 4
typedef uint32_t size_t;
#else
#error unknown pointer size
#endif
#endif
#ifndef HAVE_SIZE_MODE_T
typedef uint32_t mode_t;
#endif
#endif //BLKTOOL_COMPATIBLE_H
