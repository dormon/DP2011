#include"index.h"

#define mymem_Alloc malloc
#define mymem_Free free

void index_Init(SIndex*I,SNSize*Size){
	nsize_Mov(&I->Size,Size);
	I->Index=(unsigned*)mymem_Alloc(sizeof(unsigned)*Size->Dimension);
	for(unsigned i=0;i<Size->Dimension;++i){
		I->Index[i]=0;
		I->Size.Size[i]=Size->Size[i];
	}
}

void index_Free(SIndex*I){
	I->Size.Dimension=0;
	mymem_Free(I->Index);
	nsize_Free(&I->Size);
}

int index_Inc(SIndex*Index){
	unsigned i=0;//index do indexu
	do{//ted zvysime index
		Index->Index[i]++;//zvysime hodnotu index v danem rozmeru
		if(Index->Index[i]>=Index->Size.Size[i]){//presahlo to rozsah
			Index->Index[i]=0;//vynulujeme dany index
			++i;//zvysime index do indexu
		}else return 0;//jinak je to v pohode
	}while(i<Index->Size.Dimension);//dokud existuje vyssi dimenze
	if(i==Index->Size.Dimension)return 1;//vsechny indexy pretekly
	return 0;
}

void index_Mov(SIndex*Index,int Mov,unsigned i,enum EIndex Type){
	switch(Type){
		case INDEX_REPEAT://opakujeme
			Mov%=Index->Size.Size[i];//modulo indexu
			if(((int)Index->Index[i])+Mov<0)Index->Index[i]+=Mov+Index->Size.Size[i];//pod
			else Index->Index[i]=(Index->Index[i]+Mov)%Index->Size.Size[i];//pretek*/
			//Index->Index[i]=(Index->Index[i]+(unsigned)Mov)%Index->Size.Size[i];
			break;
		case INDEX_CLAMP://zachovavame posledni
			if(((int)Index->Index[i])+Mov<0)//podteklo
				Index->Index[i]=0;//nulujeme
			else if(Index->Index[i]+Mov>=Index->Size.Size[i])//preteklo
				Index->Index[i]=Index->Size.Size[i]-1;//nastavime max
			else
				Index->Index[i]+=Mov;//jinak pricteme
			break;
	}
}

void index_ZigZag(SIndex*ZigZag,SIndex*In){
	unsigned Last=In->Size.Dimension-1;
	for(unsigned i=0;i<Last;++i)
		if(In->Index[i+1]%2)
			ZigZag->Index[i]=In->Size.Size[i]-1-In->Index[i];
		else
			ZigZag->Index[i]=In->Index[i];
	ZigZag->Index[Last]=In->Index[Last];
}


unsigned index_GetIndex(SIndex*I){
	unsigned Last=I->Size.Dimension-1;
	unsigned Result=I->Index[Last];//index do pomocneho pole
	for(unsigned k=1;k<I->Size.Dimension;++k){
		Result*=I->Size.Size[Last-k];
		Result+=I->Index[Last-k];
	}
	return Result;
}

void*index_GetData(void*Data,SIndex*I,unsigned ElemSize){
	return ((unsigned char*)Data)+index_GetIndex(I)*ElemSize;
}

void index_Set(SIndex*O,SIndex*I){
	mymem_Cpy(O->Index,I->Index,sizeof(unsigned)*I->Size.Dimension);
}

