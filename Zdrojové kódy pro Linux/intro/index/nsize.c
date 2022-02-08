#include"nsize.h"

/*
void nsize_Init(SNSize*S,unsigned Dimension,unsigned*Size){
	S->Dimension=Dimension;
	unsigned NumByte=sizeof(unsigned)*S->Dimension;
	S->Size=mymem_Alloc(NumByte);
	mymem_Cpy(S->Size,Size,NumByte);
}*/

void nsize_Inita(SNSize*S,unsigned Dimension,...){
	S->Dimension=Dimension;
	S->Size=(unsigned*)mymem_Alloc(sizeof(unsigned)*Dimension);
	va_list args;
	va_start(args,Dimension);
	for(unsigned i=0;i<Dimension;++i)
		S->Size[i]=va_arg(args,unsigned);
	va_end(args);
}


void nsize_Mov(SNSize*S,SNSize*N){
	S->Dimension=N->Dimension;
	unsigned NumByte=sizeof(unsigned)*S->Dimension;
	S->Size=(unsigned*)mymem_Alloc(NumByte);
	mymem_Cpy(S->Size,N->Size,NumByte);
	//nsize_Init(S,N->Dimension,N->Size);
}

void nsize_Free(void*S){
	SNSize*Size=(SNSize*)S;
	mymem_Free((void**)&Size->Size);
	Size->Dimension=0;
}

unsigned nsize_Size(SNSize*S){
	unsigned Result=1;//1* neco je neco ne 0* neco
	for(unsigned i=0;i<S->Dimension;++i)//projdeme rozmery
		Result*=S->Size[i];//nasobime
	return Result;//vracime roznasobene
}

