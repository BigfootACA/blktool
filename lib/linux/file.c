/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"

bool fd_is_type(
	int fd,
	int err,
	mode_t type,
	const char*path,
	...
){
	bool ret;
	va_list va;
	struct stat st;
	char rpath[PATH_MAX]={0};
	va_start(va,path);
	vsnprintf(rpath,PATH_MAX-1,path,va);
	va_end(va);
	errno=EIO;
	if(fstatat(
		fd,rpath,&st,
		AT_SYMLINK_NOFOLLOW
	)<0)return false;
	errno=0,ret=(((st.st_mode)&S_IFMT)==type);
	if(errno==0&&err!=0&&!ret)errno=err;
	return ret;
}

static ssize_t _fd_read_file(
	int at,
	char*buff,
	size_t len,
	bool lf,
	char*path,
	va_list va
){
	int fd;
	char rpath[PATH_MAX];
	if(!path||!buff||len<=0)ERET(EINVAL);
	memset(rpath,0,sizeof(rpath));
	vsnprintf(rpath,sizeof(rpath)-1,path,va);
	memset(buff,0,len);
	if((fd=openat(at,rpath,O_RDONLY))<0)return -1;
	ssize_t s=read(fd,buff,len-1);
	if(s>0&&!lf){
		if(buff[s-1]=='\n')buff[--s]=0;
		if(buff[s-1]=='\r')buff[--s]=0;
	}
	close(fd);
	return s;
}

static bool _fd_read_file_num(
	int at,
	int64_t*out,
	int base,
	char*path,
	va_list va
){
	char buff[4096],*end=NULL;
	if(!out||!path)ERET(EINVAL);
	memset(buff,0,sizeof(buff));
	ssize_t r=_fd_read_file(
		at,buff,
		sizeof(buff),
		false,path,va
	);
	if(r<=0)return false;
	errno=0,*out=strtoll(buff,&end,base);
	if(errno!=0||end==buff||(end&&*end))return false;
	return true;
}

ssize_t fd_read_file(
	int at,
	char*buff,
	size_t len,
	bool lf,
	char*path,
	...
){
	va_list va;
	va_start(va,path);
	ssize_t r=_fd_read_file(
		at,buff,len,
		lf,path,va
	);
	va_end(va);
	return r;
}

ssize_t read_file(
	char*buff,
	size_t len,
	bool lf,
	char*path,
	...
){
	va_list va;
	va_start(va,path);
	ssize_t r=_fd_read_file(
		AT_FDCWD,buff,
		len,lf,path,va
	);
	va_end(va);
	return r;
}

bool fd_read_file_num(
	int fd,
	int64_t*out,
	int base,
	char*path,
	...
){
	va_list va;
	va_start(va,path);
	ssize_t r=_fd_read_file_num(
		fd,out,base,
		path,va
	);
	va_end(va);
	return r;
}

bool read_file_num(
	int64_t*out,
	int base,
	char*path,
	...
){
	va_list va;
	va_start(va,path);
	ssize_t r=_fd_read_file_num(
		AT_FDCWD,out,
		base,path,va
	);
	va_end(va);
	return r;
}
