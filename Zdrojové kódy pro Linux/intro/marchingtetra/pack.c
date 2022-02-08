#include"pack.h"

const SADTFCE POINTARRAYFCE={
	/*.Free=*/NULL,
	/*.Copy=*/NULL,
	/*.Cmp=*/NULL,
	/*.Size=*/sizeof(float)*3
};

const SADTFCE TRIANGLEINDEXARRAYFCE={
	/*.Free=*/NULL,
	/*.Copy=*/NULL,
	/*.Cmp=*/NULL,
	/*.Size=*/sizeof(unsigned)*3
};

void pack_Init(SPack*Pack){
	relist_Init(&Pack->Point,(SADTFCE*)&POINTARRAYFCE);//pole bodu
	relist_Init(&Pack->Triangle,(SADTFCE*)&TRIANGLEINDEXARRAYFCE);
}

void pack_Free(void*Pack){
	SPack*P=(SPack*)Pack;//pro snazsi pristu
	relist_Free(&P->Point);
	relist_Free(&P->Triangle);
}


/**
 * @brief Vlozi bod do stromu
 *
 * @param P souradnice bodu
 *
 * @return vraci index bodu
 */
unsigned pack_AddPoint(SPack*Pack,float*P){
	unsigned Index=Pack->Point.Used;
	float*ptr=(float*)Pack->Point.Data;
	for(unsigned i=0;i<Pack->Point.Used;++i){
		if(vector_Eq(ptr+i*3,P)){
			Index=i;
			break;
		}
	}
	if(Index==Pack->Point.Used)//bod je unikatni vlozime jej do pole
		relist_Push(&Pack->Point,P);//
	return Index;//vracime index bodu
}

void pack_Insert(
		SPack*Pack,
		float*Points,
		unsigned NumPoint,
		unsigned*Triangles,
		unsigned NumTriangle){
	if(NumPoint&&NumTriangle){
		unsigned*LookUpTable=(unsigned*)mymem_Alloc(sizeof(unsigned)*NumPoint);
		for(unsigned i=0;i<NumPoint;++i)
			LookUpTable[i]=pack_AddPoint(Pack,Points+i*3);//vlozime bod
		unsigned T[3];
		for(unsigned i=0;i<NumTriangle;++i){
			for(int k=0;k<3;++k)T[k]=LookUpTable[Triangles[i*3+k]];
			if((T[0]!=T[1])&&(T[0]!=T[2])&&(T[1]!=T[2]))//body nejsou stejne
				relist_Push(&Pack->Triangle,T);
		}
		mymem_Free((void**)&LookUpTable);
	}
}

