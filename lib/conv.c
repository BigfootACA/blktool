//
// Created by bigfoot on 2023/4/20.
//
#include "internal.h"

bool blktool_call_conv_from_string(
	blktool_conv_from_string*conv,
	size_t*ret,
	const char*val
){
	if(!conv||!ret||!val)return false;
	*ret=0;
	switch(conv->type){
		case CONV_SIZE:{
			size_t v=0;
			if(!conv->size(&v,val))return false;
			*ret=(size_t)v;
		}break;
		case CONV_SSIZE:{
			ssize_t v=0;
			if(!conv->ssize(&v,val))return false;
			*ret=(size_t)v;
		}break;
		case CONV_INT32:{
			int32_t v=0;
			if(!conv->int32(&v,val))return false;
			*ret=(size_t)v;
		}break;
		case CONV_UINT32:{
			uint32_t v=0;
			if(!conv->uint32(&v,val))return false;
			*ret=(size_t)v;
		}break;
		case CONV_INT64:{
			int64_t v=0;
			if(!conv->int64(&v,val))return false;
			*ret=(size_t)v;
		}break;
		case CONV_UINT64:{
			uint64_t v=0;
			if(!conv->uint64(&v,val))return false;
			*ret=(size_t)v;
		}break;
		default:return false;
	}
	return true;
}

const char*blktool_call_conv_to_string(
	blktool_conv_to_string*conv,
	size_t val,
	const char*def
){
	const char*ret=NULL;
	if(!conv)return false;
	switch(conv->type){
		case CONV_SIZE:ret=conv->size(val,def);break;
		case CONV_SSIZE:ret=conv->ssize(val,def);break;
		case CONV_INT32:ret=conv->int32(val,def);break;
		case CONV_UINT32:ret=conv->uint32(val,def);break;
		case CONV_INT64:ret=conv->int64(val,def);break;
		case CONV_UINT64:ret=conv->uint64(val,def);break;
		default:;
	}
	return ret;
}
