/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"
#include<stdio.h>
#include<stdarg.h>

static char error_msg[4096]={0};

void blktool_clear_error(){
	memset(error_msg,0,sizeof(error_msg));
}

bool blktool_is_error(){
	return error_msg[0]!=0;
}

int blktool_set_error(int r,const char*fmt,...){
	va_list va;
	blktool_clear_error();
	if(fmt){
		va_start(va,fmt);
		vsnprintf(error_msg,sizeof(error_msg)-1,fmt,va);
		va_end(va);
	}
	return r;
}

void*blktool_set_perror(void*r,const char*fmt,...){
	va_list va;
	blktool_clear_error();
	if(fmt){
		va_start(va,fmt);
		vsnprintf(error_msg,sizeof(error_msg)-1,fmt,va);
		va_end(va);
	}
	return r;
}

const char*blktool_get_error(){
	return error_msg;
}
