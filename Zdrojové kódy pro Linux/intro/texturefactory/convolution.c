#include"convolution.h"

void texturefactory_Convolution(float*Out,float*Data,float*M,
		SNSize*DataSize,SNSize*MSize,enum EIndex Type){
	SIndex I;//index do vystupnich dat
	SIndex J;//index do okoli
	SIndex K;//index do dat
	index_Init(&K,DataSize);//inicializace
	index_Init(&I,DataSize);//inicializujeme
	index_Init(&J,MSize);
	do{//projdeme vsechny data
		float Result=0;//vysledek pronasobeni
		do{//projdeme masku
			for(unsigned i=0;i<MSize->Dimension;++i)//spocitame index
				index_Mov(&K,-MSize->Size[i]/2+J.Index[i]+I.Index[i],i,Type);
			Result+=M[index_GetIndex(&J)]*Data[index_GetIndex(&K)];//nasobime
			for(unsigned i=0;i<MSize->Dimension;++i)//vratime se
				K.Index[i]=0;
		}while(!index_Inc(&J));
		Out[index_GetIndex(&I)]=Result;//ulozime konvoluci
	}while(!index_Inc(&I));
	index_Free(&I);
	index_Free(&J);
	index_Free(&K);
	mymath_Normalize(Out,nsize_Size(DataSize),0,1);//normalizujeme vysledek
}

