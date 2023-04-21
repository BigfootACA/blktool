//
// Created by bigfoot on 2023/4/10.
//
#include"internal.h"

static int handle_free(void*d){
	blktool_free_handle(d);
	return 0;
}

blktool_handle**blktool_list(
	blktool_filters*filters,
	size_t*cnt
){
	int count;
	list*l,*n,*lst;
	size_t size,i=0;
	blktool_handle**hands=NULL;
	blktool_clear_error();
	if(!(lst=blktool_list_impl()))return NULL;
	list_sort(lst,list_string_sorter);
	if((l=list_first(lst)))do{
		n=l->next;
		LIST_DATA_DECLARE(hand,l,blktool_handle*);
		if(!blktool_filters_check(filters,hand))
			list_obj_del(&lst,l,handle_free);
	}while((l=n));
	if(!lst){
		if(!blktool_is_error())
			blktool_set_error(0,"no any match block found");
		return NULL;
	}
	if((l=list_first(lst)))lst=l;
	else EDONE(blktool_set_error(0,"invalid list"));
	if((count=list_count(lst))<=0)
		EDONE(blktool_set_error(0,"invalid list count"));
	if(cnt)*cnt=count;
	size=sizeof(blktool_handle*)*(count+1);
	if(!(hands=malloc(size)))
		EDONE(blktool_set_error(0,"alloc array failed"));
	memset(hands,0,size);
	if((l=list_first(lst)))do{
		hands[i++]=LIST_DATA(l,blktool_handle*);
	}while((l=l->next)&&i<(size_t)count);
	list_free_all(lst,NULL);
	if(hands)blktool_clear_error();
	return hands;
	done:
	if(hands)free(hands);
	list_free_all(lst,handle_free);
	return NULL;
}
