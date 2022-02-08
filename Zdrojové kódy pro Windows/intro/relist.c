#include"relist.h"

void relist_Init(SReList*List,SADTFCE*F){
	List->F=F;//funkce pro praci
	List->Data=NULL;//data
	List->Size=0;//celkova velikost
	List->Used=0;//pocet pouzitych prvku
}

void relist_Free(void*List){
	SReList*L=(SReList*)List;//pro snadnejsi zapis
	for(unsigned i=0;i<L->Used;++i){//projdeme pouzite prvky a uvolnime je
		unsigned char*Ptr=(unsigned char*)mymem_Alloc(adt_ElemSize(L->F));
		mymem_Cpy(Ptr,(unsigned char*)L->Data+i*adt_ElemSize(L->F),L->F->Size);
		adt_FreeData(Ptr,L->F);
	}
	mymem_Free(&L->Data);//uvolnime cele pole
}

int relist_Realloc(SReList*List){
	if(List->Size){//nejakou velikost uz to melo takze budeme roztahovat
		void*Carry=mymem_Realloc(List->Data,List->Size*2*adt_ElemSize(List->F));
		if(!Carry)return 1;//nepodarilo se realokovat misto
		List->Data=Carry;
		List->Size*=2;//dvojnasobne misto
	}else{//prvni alokace
		List->Data=mymem_Alloc(adt_ElemSize(List->F));//pro jeden prvek
		if(!List->Data)return 1;//nepodarilo se alokovat misto
		List->Size=1;//zatim velikost jedna
		List->Used=0;//zatim nic nepouzito
	}
	return 0;//v poradku
}

int relist_Push(SReList*List,void*Data){
	if(List->Used==List->Size){
		if(relist_Realloc(List))return 1;//nepodarilo se realokovat
	}
	mymem_Cpy(//nakopirujeme data
			(char*)List->Data+adt_ElemSize(List->F)*List->Used,//na prvni volne misto
			Data,//nakopirujeme data
			adt_ElemSize(List->F));//o dane velikost
	List->Used++;//zvysime pocet pouzitych
	return 0;//v poradku
}

void*relist_Pop(SReList*List){
	List->Used--;//zmensime pocet prvku
	return(unsigned char*)List->Data+(List->Used+1)*adt_ElemSize(List->F);
}

void*relist_GetData(SReList*List,unsigned Index){
	return((char*)List->Data)+Index*adt_ElemSize(List->F);
}

void relist_Delete(SReList*List,unsigned Ind){
	if(List->Used<=Ind)return;//kdyby nahodou
	//kvuli memmove a overlapingu
	for(unsigned k=Ind;k<List->Used-1;++k)
		mymem_Cpy((char*)List->Data+adt_ElemSize(List->F)*k,
				(char*)List->Data+adt_ElemSize(List->F)*(k+1),adt_ElemSize(List->F));
	List->Used--;
}

void relist_Insert(SReList*List,unsigned Ind,void*Data){
	if(List->Used<=Ind)return;//kdyby nahodou
	relist_Push(List,Data);//rozsirime seznam
	for(unsigned k=Ind;k<List->Used-1;++k)
		mymem_Cpy(
				(char*)List->Data+adt_ElemSize(List->F)*(List->Used-1-(k-Ind)),
				(char*)List->Data+adt_ElemSize(List->F)*(List->Used-2-(k-Ind)),
				adt_ElemSize(List->F));
	mymem_Cpy((char*)List->Data+adt_ElemSize(List->F)*Ind,Data,adt_ElemSize(List->F));
	//presuneme data
}

