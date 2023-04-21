#define _GNU_SOURCE
#include<string.h>
#include"compatible.h"

#ifndef HAVE_STRCASECMP
int strcasecmp(const char*s1,const char*s2){
	#if defined(HAVE_STRICMP)
	return stricmp(s1,s2);
	#else
	#error no strcasecmp found
	return NULL;
	#endif
}
#endif
