#include"globaltransform.h"

void Smooth(float*O,float*I,SNSize*S,void*P){
	unsigned EdgeSize=(*((unsigned*)P))*2+1;//velikost hrany
	SNSize MS;//velikost jadra
	nsize_Mov(&MS,S);//alokace
	for(unsigned i=0;i<MS.Dimension;++i)
		MS.Size[i]=EdgeSize;//nastaveni velikosti hrany
	unsigned MSize=nsize_Size(&MS);
	float*M=(float*)mymem_Alloc(sizeof(float)*MSize);//alokace jadra

	//for(unsigned i=0;i<MSize;++i)M[i]=1./MSize;
	float Max=0;//maximalni prvek jadra
	SIndex II;//index do jadra
	index_Init(&II,&MS);//inicializujeme index do jadra
	do{//projdeme prvky jadra
		float Dist=0;//vzdalenost
		for(unsigned i=0;i<II.Size.Dimension;++i){//projdeme slozky indexu
			float DistX=(II.Size.Size[i]/2)-II.Index[i];//spocitame rozdil
			Dist+=DistX*DistX;//skalarni nasobek
		}
		Dist=sqrt(Dist);//vzdalenost
		if(Max<Dist)Max=Dist;//spocitame maximalni vzdalenost
		M[index_GetIndex(&II)]=Dist;//vlozime vzdalenost
	}while(!index_Inc(&II));
	
	float Sum=0;//soucet prvku jadra
	for(unsigned i=0;i<MSize;++i){//prevod z <0,Max> na <Max+1,1> a suma
		M[i]=Max-M[i]+1;
		Sum+=M[i];//vypocet sumy
	}

	for(unsigned i=0;i<MSize;++i)M[i]/=Sum;//normalizace
	index_Free(&II);

	texturefactory_Convolution(O,I,M,S,&MS,INDEX_REPEAT);//konvoluce
	nsize_Free(&MS);//uvolnime velikost jadra
	mymem_Free((void**)&M);//uvolnime jadro
}


void Edge(float*,float*,SNSize*,void*){

}


const GLOBALTRANSFORMFCE GlobalTransformFce[]={
	Smooth,//prvni

	Edge//posledni
};

