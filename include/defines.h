//
// Created by bigfoot on 2023/4/9.
//

#ifndef BLKTOOL_DEFINES_H
#define BLKTOOL_DEFINES_H
#include<errno.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#if defined(WIN32)
#include<windows.h>
#endif
#define EGO(expr,tag)do{expr;goto tag;}while(0)
#define EDONE(expr) EGO(expr,done)
#define ENUM(err) -(errno=(err))
#define ERET(err) do{return ENUM(err);}while(0)
#define EPRET(err) do{errno=(err);return NULL;}while(0)
#define RET(e) do{return (errno=(e))}while(0)
#define DONE(e) EDONE(errno=(e))
#define XDONE(e) EDONE(if(errno==0)errno=(e))
#define XRET(e,d) do{return (errno=((e)?:(d)));}while(0)
#define EXRET(e) do{return errno?:e}while(0)
#define EXPRET(e) do{if(errno==0)errno=e;return NULL;}while(0)
#define BOOL2STR(b) ((b)?"true":"false")
#define ARRLEN(x)(sizeof(x)/sizeof((x)[0]))
#ifndef MIN
#ifdef min
#define MIN min
#else
#define MIN(a,b)({\
	typeof(a)_a=(a);\
	typeof(b)_b=(b);\
	(_b)>(_a)?(_a):(_b);\
})
#endif
#endif
#ifndef MAX
#ifdef max
#define MAX max
#else
#define MAX(a,b)({\
	typeof(a)_a=(a);\
	typeof(b)_b=(b);\
	(_b)<(_a)?(_a):(_b);\
})
#endif
#endif
#define round_up(val,alg) (((val)+(alg)-1)&~((alg)-1))
#define round_down(val,alg) ((val)&~((alg)-1))
typedef int runnable_t(void*);
static inline size_t s_strlcpy(char *buf,const char*src,size_t len){
        char*d0=buf;
        if(!len--)goto finish;
        for(;len&&(*buf=*src);len--,src++,buf++);
        *buf=0;
	finish:
        return buf-d0+strlen(src);
}
static inline size_t s_strlcat(char*buf,const char*src,size_t len){
        size_t slen=strnlen(buf,len);
        return slen==len?
		(slen+strlen(src)):
		(slen+s_strlcpy(buf+slen,src,len-slen));
}
static inline void*s_memdup(void*mem,size_t len){
	void*dup=malloc(len);
	if(!dup)EPRET(ENOMEM);
	memcpy(dup,mem,len);
	return dup;
}
#undef strlcat
#undef strlcpy
#define strlcat s_strlcat
#define strlcpy s_strlcpy
extern const char*make_readable_str_buf(
	char*buf,size_t len,
	unsigned long long val,
	unsigned long block_size,
	unsigned long display
);
extern uint32_t crc32(void*buffer,size_t length);
#endif //BLKTOOL_DEFINES_H
