/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"internal.h"
#include<blkid/blkid.h>
#include<dirent.h>

static const char*search[]={
	"/dev/%s",
	"/dev/block/%s",
	NULL
};

void blktool_free_handle(blktool_handle*handle){
	if(!handle)return;
	handle->refcnt--;
	if(handle->refcnt>0)return;
	if(handle->fd>=0)close(handle->fd);
	free(handle);
}

static blktool_handle*blktool_handle_from_name(const char*name){
	struct stat st;
	char path[4096];
	bool found=false;
	blktool_handle*hand;
	if(!name)return NULL;
	for(size_t i=0;search[i];i++){
		memset(path,0,sizeof(path));
		snprintf(path,sizeof(path)-1,search[i],name);
		if(lstat(path,&st)!=0)continue;
		if(!S_ISBLK(st.st_mode))continue;
		found=true;
		break;
	}
	if(!found||!(hand=malloc(sizeof(blktool_handle))))return NULL;
	memset(hand,0,sizeof(blktool_handle));
	strncpy(hand->path,path,sizeof(hand->path)-1);
	strncpy(hand->name,name,sizeof(hand->name)-1);
	hand->fd=-1,hand->refcnt=1;
	return hand;
}

static bool list_handler_time_sorter(list*l1,list*l2){
	LIST_DATA_DECLARE(s1,l1,blktool_handle*);
	LIST_DATA_DECLARE(s2,l2,blktool_handle*);
	return s1->last>s2->last;
}

static void blktool_list_children(int fd,blktool_handle*parent,list**lst){
	blktool_handle*hand=NULL;
	struct dirent*ent;
	struct stat st;
	int64_t out=0;
	DIR*d=NULL;
	int dfd;
	if((dfd=openat(fd,parent->name,O_DIRECTORY))<0)
		EDONE(blktool_set_error(0,"open %s failed: %m",parent->name));
	if(!(d=fdopendir(dfd)))
		EDONE(blktool_set_error(0,"open dir %s failed: %m",parent->name));
	while((ent=readdir(d))){
		if(ent->d_type!=DT_DIR||ent->d_name[0]=='.')continue;
		if(!fd_read_file_num(dfd,&out,10,"%s/size",ent->d_name)||out==0)continue;
		if(!fd_read_file_num(dfd,&out,10,"%s/partition",ent->d_name)||out<=0)continue;
		if(!(hand=blktool_handle_from_name(ent->d_name)))continue;
		if(fstatat(dfd,ent->d_name,&st,0)!=0||!S_ISDIR(st.st_mode))continue;
		hand->layout=blktool_get_layout(parent);
		hand->last=st.st_mtim.tv_sec;
		hand->disk=parent->disk;
		hand->parent=parent;
		hand->is_part=true;
		hand->part=out;
		parent->refcnt++;
		blktool_list_children(dfd,hand,lst);
		if(list_obj_add_new(lst,hand)==0)continue;
		blktool_free_handle(hand);
	}
	done:
	if(d)closedir(d);
	else if(dfd>=0)close(dfd);
}

list*blktool_list_impl(){
	static const char*path="/sys/block";
	blktool_handle*hand=NULL;
	struct dirent*ent;
	list*lst=NULL,*l;
	struct stat st;
	int64_t out=0;
	size_t id=0;
	DIR*d=NULL;
	int dfd;
	if((dfd=open(path,O_DIRECTORY))<0)
		EDONE(blktool_set_error(0,"open %s failed: %m",path));
	if(!(d=fdopendir(dfd)))
		EDONE(blktool_set_error(0,"open dir %s failed: %m",path));
	while((ent=readdir(d))){
		if(ent->d_type!=DT_LNK||ent->d_name[0]=='.')continue;
		if(!fd_read_file_num(dfd,&out,10,"%s/size",ent->d_name)||out==0)continue;
		if(!(hand=blktool_handle_from_name(ent->d_name)))continue;
		if(fstatat(dfd,ent->d_name,&st,0)!=0||!S_ISDIR(st.st_mode))continue;
		hand->last=st.st_mtim.tv_sec;
		if(list_obj_add_new(&lst,hand)!=0)blktool_free_handle(hand);
	}
	list_sort(lst,list_handler_time_sorter);
	if((l=list_first(lst)))do{
		hand=LIST_DATA(l,blktool_handle*);
		if(!hand||hand->is_part)continue;
		hand->disk=id++;
		blktool_list_children(dfd,hand,&lst);
	}while((l=l->next));
	done:
	if(d)closedir(d);
	else if(dfd>=0)close(dfd);
	return list_first(lst);
}
