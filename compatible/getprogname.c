#define _GNU_SOURCE
#ifdef WIN32
#include<windows.h>
#else
#include<errno.h>
#endif
#include"compatible.h"

#ifndef HAVE_GETPROGNAME
const char*getprogname(void){
	#if defined(HAVE_PROGRAM_INVOCATION_SHORT_NAME)
	return program_invocation_short_name;
	#elif defined(WIN32)
	static char buff[4096];
	size_t len;
	char*p;
	GetModuleFileNameA(NULL,buff,sizeof(buff));
	if((p=strrchr(buff,'\\')))
		strncpy(buff,p+1,sizeof(buff)-1);
	if((len=strlen(buff))>0&&strcasecmp(buff+len-4,".exe")==0)
		buff[len-4]=0;
	return buff;
	#else
	#error no progname backend found
	return NULL;
	#endif
}
#endif
