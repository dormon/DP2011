#include"mymem.h"

void*mymem_Alloc(unsigned NumByte){
	return malloc(NumByte);
}

void*mymem_Realloc(void*Mem,unsigned Num){
	return realloc(Mem,Num);
}

void mymem_Free(void**Mem){
	if(*Mem!=NULL){
		free(*Mem);
		*Mem=NULL;
	}
}

void mymem_Cpy(void*D,void*S,unsigned Num){
	memcpy(D,S,Num);
}

int mymem_Cmp(void*D,void*S,unsigned Num){
	return memcmp(D,S,Num);
}

