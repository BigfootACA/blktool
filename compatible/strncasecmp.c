#define _GNU_SOURCE
#include<string.h>
#include"compatible.h"

#ifndef HAVE_STRNCASECMP
int strncasecmp(const char*s1,const char*s2,size_t n){
	#if defined(HAVE_STRNICMP)
	return strnicmp(s1,s2,n);
	#else
	#error no strncasecmp found
	return NULL;
	#endif
}
#endif
