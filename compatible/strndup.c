#include<stdlib.h>
#include"compatible.h"

#ifndef HAVE_STRNDUP
char*strndup(const char*s,size_t n){
	size_t len=strnlen(s,n);
	char*new=malloc((len+1)*sizeof(char));
	if(!new)return NULL;
	new[len]=0;
	return memcpy(new,s,len);
}
#endif