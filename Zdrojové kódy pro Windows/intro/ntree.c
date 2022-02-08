#include"ntree.h"

void ntree_Init(SNTree*T,SADTFCE*F){
	T->F=F;
	T->Root=0;
}

void ntree_Free(void*A){
	SNTree*T=(SNTree*)A;//pro prehlednost
	ntree_ItemFree(T->Root,T->F);//uvolnime strom
	T->Root=NULL;
}

void ntree_ItemFree(struct SNTreeItem*A,SADTFCE*F){
	if(A==NULL)return;//nula nic
	for(unsigned i=0;i<A->NumNext;++i)
		ntree_ItemFree(A->Next[i],F);//uvolnime nasledniky
	mymem_Free((void**)&A->Next);//uvolnime pole nasledniku
	adt_FreeData(A->Data,F);//uvolnime data
	mymem_Free((void**)&A);
}

void ntree_ItemCreate(
		struct SNTreeItem**A,
		struct SNTreeItem*Parent,
		unsigned Id,
		unsigned NumNext){
	*A=(SNTreeItem*)mymem_Alloc(sizeof(struct SNTreeItem));//provedeme alokaci prvku
	struct SNTreeItem*T=*A;
	T->Data=0;
	T->NumNext=NumNext;
	T->Next=(SNTreeItem**)mymem_Alloc(sizeof(struct SNTreeItem*)*NumNext);//alok. nasledniku
	for(unsigned i=0;i<NumNext;++i)
		T->Next[i]=NULL;//nulovani nasledniku
	T->Parent=Parent;
	T->Id=Id;
}

