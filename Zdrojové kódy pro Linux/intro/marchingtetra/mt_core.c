#include"mt_core.h"

#define NUM_EDGE 19

/**
 * @brief Tabulka vertexu na hranach tetrahedronu
 */
const unsigned char mt_ubCornerPoints[3*NUM_EDGE]={
	1,0,0,
	0,1,0,
	0,0,1,
	2,1,0,
	2,0,1,
	1,2,0,
	0,2,1,
	2,2,1,
	1,0,2,
	0,1,2,
	2,1,2,
	1,2,2,
	0,1,1,//yz
	2,1,1,
	1,0,1,//xz
	1,2,1,
	1,1,0,//xy
	1,1,2,
	1,1,1
};//3*19=57

/**
 * @brief Tabulka hran tetrahedronu
 */
const unsigned char mt_TetraEdge[6*6]={
	1,12, 6,18,15,11,//OK
	4,14, 0,10,13,18,//OK
	8, 2,14,17,10,18,//OK
	5,16, 1, 7,15,18,//OK
	3, 0,16,13, 7,18,//OK
	9,12, 2,11,17,18 //OK
};//3*19+6*6=57+36=93

/**
 * @brief Tabulka rohu tetrahedronu
 */
const unsigned char mt_TetraCorner[6*4]={
	0,2,6,7,//OK
	5,1,0,7,//OK
	4,5,0,7,//OK
	3,2,0,7,//OK
	1,3,0,7,//OK
	6,4,0,7 //OK
};//3*19+6*6+6*4=57+36+24=117

/**
 * @brief Tabulka trojuhelniku pro konfigurace tetrahedronu
 */
const unsigned mt_Triangles[16]={
	0x00000000,//0000
	0x00031003,//0001
	0x00024003,//0010
	0x31441206,//0011
	0x00052103,//0100
	0x35203206,//0101
	0x15054006,//0110
	0x00054303,//0111
	0x00045303,//1000
	0x51045006,//1001
	0x25053006,//1010
	0x00025103,//1011
	0x34142106,//1100
	0x00042003,//1101
	0x00013003,//1110
	0x00000000 //1111
};//3*19+6*6+6*4+16*4=57+36+24+64=181

const unsigned char mt_EdgeToCorner[NUM_EDGE]={
	0x10,
	0x20,
	0x40,
	0x31,
	0x51,
	0x32,
	0x62,
	0x73,
	0x54,
	0x64,
	0x75,
	0x76,
	0x60,
	0x71,
	0x50,
	0x72,
	0x30,
	0x74,
	0x70
};//3*19+6*6+6*4+16*4+19=57+36+24+64+19=200 byte tabulek

unsigned**mt_PreTriangles=0;//predpocitane trojuhelniky
float*AproxPoint;//aproximovane body

void mt_PreGet(unsigned*Triangles,unsigned*NumTriangle,unsigned char Corner){
	*NumTriangle=0;//budeme zvysovat
	for(unsigned th=0;th<6;++th){//projdeme tetrahedrony
		unsigned char TetraHedronCorner=0;//rohy tetrahedronu
		for(unsigned thc=0;thc<4;++thc)//spocitame rohy tetrahedronu konfiguraci
			TetraHedronCorner|=((Corner>>mt_TetraCorner[th*4+thc])&1)<<thc;
		unsigned TetraTriangles=mt_Triangles[TetraHedronCorner];//ziskame trojuh
		unsigned TetraTrianglesNum=TetraTriangles&0xf;
		for(unsigned t=0;t<TetraTrianglesNum;++t)//projdeme rohy
			Triangles[(*NumTriangle)*3+t]=
				mt_TetraEdge[th*6+((TetraTriangles>>(8+4*t))&0xf)];//cislo rohu
		(*NumTriangle)+=TetraTrianglesNum/3;//zvysime pocet trojuhelniku	
	}
}

/**
 * @brief Provede aproximaci bodu
 *
 * @param NewPos nova pozice bodu
 * @param Point cislo bodu
 * @param AVal hodnota na zacatku hrany
 * @param BVal hodnota na konci hrany
 * @param T hodnota prahu
 */
void mt_AproxPoint(float*NewPos,unsigned char Point,float AVal,float BVal,
		float T){
	float t;//aproximovana hodnota
	//if(AVal==BVal)t=1;//pro jistotu TODO mozna pryc
	//else t=2*(.5-AVal)/(BVal-AVal);
	t=2*(T-AVal)/(BVal-AVal);

	for(unsigned k=0;k<3;++k)
		if(mt_ubCornerPoints[Point*3+k]==1){//jen pokud nejsme na hrane krychle
			NewPos[k]=t;//zapis aproximovane hodnoty
		}else//jsme na hrane krychle (0,2) pouze presun
			NewPos[k]=mt_ubCornerPoints[Point*3+k];//presun
}

/*
 * @brief Spocita skutecne pozice do AproxPoint
 *
 * @param Start zacatek kostky
 * @param Size velikost kostky
 * @param CornerValue hodnoty v rozich (kdyz null ignoruje)
 * @param T prah (ingnoruje pri CornerValue==NULL)
 */
void mt_ComputePoint(float*Start,float Size,float*CornerValue,float T){
	if(CornerValue){//neni null budeme aproximovat
		for(unsigned i=0;i<NUM_EDGE;++i)//projdeme hrany
			mt_AproxPoint(AproxPoint+i*3,i,//aproximujeme bod na nich
					CornerValue[mt_EdgeToCorner[i]&0xf],
					CornerValue[(mt_EdgeToCorner[i]>>4)&0xf],T);
	}else{//jen prekopirujeme body
		for(unsigned i=0;i<NUM_EDGE*3;++i)
			AproxPoint[i]=mt_ubCornerPoints[i];//proste prekopirovani bodu
	}
	//nyni provedeme presun a zmenu meritka
	for(unsigned i=0;i<NUM_EDGE;++i){
		for(unsigned k=0;k<3;++k){
			AproxPoint[i*3+k]*=
				//Size/2;
				Size/2;
			AproxPoint[i*3+k]+=Start[k];//posuneme
		}
	}
}

void mt_Init(){
	AproxPoint=(float*)mymem_Alloc(sizeof(float)*NUM_EDGE*3);//alokujeme aproximacni body
	mt_PreTriangles=(unsigned**)mymem_Alloc(sizeof(unsigned*)*256);//alokujeme konfiguraci
	for(unsigned i=0;i<256;++i){
		mt_PreTriangles[i]=(unsigned*)mymem_Alloc(sizeof(unsigned)*(6*6+1));//vic jich neni
		mt_PreGet(mt_PreTriangles[i]+1,mt_PreTriangles[i],i);//konfigurace
	}
}

void mt_DeInit(){
	mymem_Free((void**)&AproxPoint);//dealokujeme aproximacni body
	for(unsigned i=0;i<256;++i)
		mymem_Free((void**)&mt_PreTriangles[i]);//uvolnime konfigurace
	mymem_Free((void**)&mt_PreTriangles);//dealokujeme pole konfiguraci
}

void mt_Get(
		float*Points,
		unsigned*NumPoint,
		unsigned*Triangles,
		unsigned*NumTriangle,
		unsigned char Corners,
		float*Start,
		float Size,
		float*CornerValue,
		float Threshold){
	mt_ComputePoint(Start,Size,CornerValue,Threshold);//vypocitame pozice

	*NumTriangle=mt_PreTriangles[Corners][0];//pocet trojuhelniku
	*NumPoint=0;//budeme zvetsovat

	int LookUp[NUM_EDGE];//lookup tabulka
	for(unsigned i=0;i<NUM_EDGE;++i)LookUp[i]=-1;//vyplnime -1

	unsigned*TPtr=mt_PreTriangles[Corners];//ukazatel na aktualni konfiguraci
	for(unsigned t=0;t<TPtr[0]*3;++t){//projdeme body trojuhelnikuu
		unsigned PointID=TPtr[1+t];//cislo rohu trojuhlenika
		if(LookUp[PointID]<0)//tento index jeste nebyl
			LookUp[PointID]=(*NumPoint)++;//zapiseme pocitadlo bodu a zvysime
		for(unsigned k=0;k<3;++k)//slozky souradnic bodu
			Points[LookUp[PointID]*3+k]=AproxPoint[PointID*3+k];//zapiseme bod
		Triangles[t]=LookUp[PointID];//zapiseme index bodu
	}
}

