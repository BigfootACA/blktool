/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include<stdio.h>
#include"internal.h"

int main(int argc,char**argv){
	int ret=1;
	bool list,r=true;
	uint64_t opers=0,opts=0;
	blktool_handle**hands=NULL;
	struct libscols_table*table=NULL;
	blktool_filters*f=blktool_filters_new();
	if(blktool_do_getopt(argc,argv,f,&opers,&opts)!=0)goto done;
	if(!(table=scols_new_table()))EDONE(fprintf(stderr,"create table failed\n"));
	if(opers==0)EDONE(fprintf(stderr,"no any operation specified\n"));
	if(!f)EDONE(fprintf(stderr,"create filter failed\n"));
	if(
		(is_oper(opers,OPER_LIST)&&is_oper(opers,OPER_LIST_PART))||
		(is_oper(opers,OPER_LIST)&&is_oper(opers,OPER_LIST_DISK))||
		(is_oper(opers,OPER_LIST_PART)&&is_oper(opers,OPER_LIST_DISK))
	)EDONE(fprintf(stderr,"conflict operation\n"));
	if(is_oper(opers,OPER_LIST_PART))
		blktool_filters_add(f,FILTER_IS_PART,VALUE_BOOLEAN,true);
	else if(is_oper(opers,OPER_LIST_DISK))
		blktool_filters_add(f,FILTER_IS_PART,VALUE_BOOLEAN,false);
	if(is_opt(opts,OPT_RAW))scols_table_enable_raw(table,true);
	if(is_opt(opts,OPT_JSON))scols_table_enable_json(table,true);
	if(is_opt(opts,OPT_EXPORT))scols_table_enable_export(table,true);
	if(is_opt(opts,OPT_NO_HEAD))scols_table_enable_noheadings(table,true);
	list=is_oper(opers,OPER_LIST)||is_oper(opers,OPER_LIST_PART)||is_oper(opers,OPER_LIST_DISK);
	if(list)opers|=OPER_PRINT_DEVICE;
	scols_table_set_name(table,list?"list":"device");
	if(!blktool_add_header(table,opts,opers))
		EDONE(fprintf(stderr,"add header failed\n"));
	if((hands=blktool_list(f,NULL))){
		if(!list)r=blktool_add_handle(table,opts,opers,hands[0]);
		else for(size_t i=0;hands[i];i++)
			r=blktool_add_handle(table,opts,opers,hands[i]);
		if(!r)EDONE(fprintf(stderr,"add handle failed\n"));
	}
	if(blktool_is_error())
		EDONE(fprintf(stderr,"list failed: %s\n",blktool_get_error()));
	scols_print_table(table);
	ret=0;
	done:
	if(f)blktool_filters_free(f);
	if(table)scols_unref_table(table);
	if(hands){
		for(size_t i=0;hands[i];i++)
			free(hands[i]);
		free(hands);
	}
	return ret;
}
