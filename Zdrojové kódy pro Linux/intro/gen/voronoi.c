#include"voronoi.h"

/**
 * @brief Reprezentuje vyhledavi strukturu BSPTree
 */
struct BSPTree{
	struct BSPTree*Next[2];//levy a pravy podstrom
	int*Data;//data (DVA BODY A POLOMERY KRUZNIC OKOLO)
};

SNSize*VoronoiSize;//pro rychlejsi rekurzi
int*VoronoiPoint;//vkladany bod do stromu nebo posledni nejblizsi bod

/*
 * @brief Spocita: d0=A[0]-B[0] d0^2+d1^2+... s ohledem na pretikani
 *
 * @param A Bod A
 * @param B Bod B
 * @param Size rozmery kostky
 *
 * @return vraci wrap(A[0]-B[0])^2+wrap(A[1]-B[1])^2+...
 */
unsigned WrapAMul(int*A,int*B,SNSize*Size){
	unsigned Result=0;//vysledek
	for(unsigned i=0;i<Size->Dimension;++i){//pres dimenze
		int d=abs(A[i]-B[i]);//vzdalenost bodu v ose dimenze
		if((unsigned)d>(Size->Size[i]>>1))d=Size->Size[i]-d;//je li vetsi nez 
		Result+=d*d;//x0*x0+x1*x1+...+d*d+...
	}
	return Result;
}

/**
 * @brief Spocita polomery kruznic
 *
 * @param Root koren stromu
 * @param P bod od ktereho se meri polomer kruznice
 * @param Act Aktualni nejvetsi vzdalenost
 *
 * @return vraci vzdalenost k nejvzdalenejsimu potomkovi
 */
unsigned GetRadiusRec(
		struct BSPTree**Root,
		int*P,
		unsigned Act){
	if(*Root){
		unsigned D=VoronoiSize->Dimension;//dimenze diagramu
		unsigned DistA=WrapAMul((*Root)->Data,P,VoronoiSize);//vzalenost A
		unsigned DistB=WrapAMul((*Root)->Data+D+1,P,VoronoiSize);//vzdalenost B
		if(DistA<DistB)DistA=DistB;//B je vetsi
		if(Act<DistA)Act=DistA;//aktualni je mensi
		for(int i=0;i<2;++i){//levy a pravy podstrom - tam taky chceme polomer
			Act=GetRadiusRec((*Root)->Next+i,P,Act);//ziskame vzdalenost
			int Offset=i*(1+D);
			(*Root)->Data[D+Offset]=
				GetRadiusRec((*Root)->Next+i,(*Root)->Data+Offset,0);
		}
	}
	return Act;//vracime nejvetsi vzdalenost
}

/*
 * @brief Vlozi do stromu dalsi bod nebo jej inicializuje ze dvou bodu
 *
 * @param Root koren stromu
 * @param A Bod A (pouziva se jen pri inicializaci stromu)
 */
void BuildTreeRec(
		struct BSPTree**Root,
		int*A){
	unsigned D=VoronoiSize->Dimension;
	if(*Root==NULL){//prvotni vytvoreni
		*Root=(struct BSPTree*)mymem_Alloc(sizeof(struct BSPTree));
		(*Root)->Next[0]=(*Root)->Next[1]=NULL;//levy a pravy 
		(*Root)->Data=(int*)mymem_Alloc(sizeof(int)*(D*2+2));//2 body+polomer
		mymem_Cpy((*Root)->Data,A,sizeof(int)*D);
		mymem_Cpy((*Root)->Data+1+D,VoronoiPoint,sizeof(int)*D);
		(*Root)->Data[D]=0;//polomer
		(*Root)->Data[D+1+D]=0;//polomer
	}else{//jsou tam dva body
		unsigned DistA=WrapAMul((*Root)->Data,VoronoiPoint,VoronoiSize);//vzdal A
		unsigned DistB=WrapAMul((*Root)->Data+D+1,VoronoiPoint,VoronoiSize);// B
		if(DistA<=DistB)BuildTreeRec(&(*Root)->Next[0],(*Root)->Data);
		else BuildTreeRec(&(*Root)->Next[1],(*Root)->Data+D+1);
	}
}


/*
 * @brief Vytvori strom pro rychly vypocet distancniho bufferu
 *
 * @param Root koren stromu
 * @param P body
 * @param NumP pocet bodu
 */
void BuildTree(struct BSPTree**Root,int*P,unsigned NumP){
	unsigned D=VoronoiSize->Dimension;
	VoronoiPoint=P+D;
	BuildTreeRec(Root,P);//inicializace
	for(unsigned i=2;i<NumP;++i){
		VoronoiPoint=P+D*i;
		BuildTreeRec(Root,NULL);//ted ostatni body
	}
	for(int i=0;i<2;++i){//levy a pravy
		int Offset=i*(1+D);//offset na data
		(*Root)->Data[D+Offset]=
			GetRadiusRec((*Root)->Next+i,(*Root)->Data+Offset,0);//pro A
	}
}

/*
 * @brief Uvolni strom
 *
 * @param Root koren
 */
void DeleteTree(struct BSPTree**Root){
	if(*Root){//je co uklizet
		mymem_Free((void**)&(*Root)->Data);//uvolnime data
		DeleteTree(&(*Root)->Next[0]);//uvolnime levy podstrom
		DeleteTree(&(*Root)->Next[1]);//uvolnime pravy podstrom
		mymem_Free((void**)&(*Root));//uvolnime koren
		*Root=NULL;//naplnime nulou
	}
}


/**
 * @brief Spocita vzdalenost k nelbjizsimu
 *
 * @param Root koren stromu
 * @param P bod
 * @param Rad zatim nalezena vzdalenost
 *
 * @return vraci nejmensi vzdalenost
 */
unsigned DistToClosest(
		struct BSPTree*Root,
		unsigned*P,
		unsigned Rad){
	if(Root){//jde jit hloubeji
		unsigned D=VoronoiSize->Dimension;
		unsigned Dist;//vzdalenost
		for(int i=0;i<2;++i){//projdeme potomky
			int Offset=i*(1+D);//offset na data
			Dist=WrapAMul(Root->Data+Offset,(int*)P,VoronoiSize);//vzdalenost k bodu
			if(sqrt(Dist)<=sqrt(Rad)+sqrt(Root->Data[D+Offset])){//je tam prekryv
				if(Dist<Rad){//vzdalenost je mensi nez doposud
					Rad=Dist;//prepiseme vzdalenost
					VoronoiPoint=Root->Data+Offset;//zmenime nejblizsiho
				}
				Rad=DistToClosest(Root->Next[i],P,Rad);//hloub
			}
		}
	}
	return Rad;//vracime vzdalenost
}

/*
 * @brief Vypocita distancni buffer
 *
 * @param Root strom s body
 *
 * @return vraci distancni buffer
 */
float*ComputeDistBuffer(
		struct BSPTree*Root){
	//unsigned D=VoronoiSize->Dimension;
	float*Result=(float*)mymem_Alloc(sizeof(float)*nsize_Size(VoronoiSize));
	SIndex Ind;//index do pole
	index_Init(&Ind,VoronoiSize);//inicializace indexu
	VoronoiPoint=Root->Data;//nejblizsi bod (zatim nahodny)
	do{
		unsigned RAD=WrapAMul((int*)Ind.Index,VoronoiPoint,VoronoiSize);//zatim
		unsigned DD=DistToClosest(Root,Ind.Index,RAD);//nejmensi vzdalenost
		Result[index_GetIndex(&Ind)]=sqrt(DD);//ulozime do bufferu
	}while(!index_Inc(&Ind));//zvysime index
	index_Free(&Ind);//uvolneni indexu
	return Result;
}

float*VoronoiDiagram(
		SNSize*Size,
		unsigned*Points,
		unsigned NumPoint){
	float*Result;//vysledek
	struct BSPTree*Root=NULL;//strom
	VoronoiSize=Size;//pro rychlejsi rekurzi globalni promenne
	BuildTree(&Root,(int*)Points,NumPoint);//spocteme si strom
	Result=ComputeDistBuffer(Root);//spocitame si distancni buffer
	mymath_Normalize(Result,nsize_Size(Size),0,1);//normalizujeme jej
	DeleteTree(&Root);//uvolnime strom
	return Result;//navrat vysledku
}


