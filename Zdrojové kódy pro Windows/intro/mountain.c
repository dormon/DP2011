#include"mountain.h"

void mountain_InitHeightMap(
		SColorBuffer*HeightMap,
		unsigned S,
		float Min,
		float Max,
		unsigned Seed){
	myseed=Seed;
	SColorBuffer TerrainMid,TerrainVoro;

	SNSize Size;
	nsize_Inita(&Size,2,S,S);

	float Div=2;
	colorbuffer_Init(&TerrainMid,BB_MIDPOINT,&Size,&Div);
	void*VoroP=colorbuffer_GenVoronoiParam(&Size,0.003);
	colorbuffer_Init(&TerrainVoro,BB_VORONOID,&Size,VoroP);

	colorbuffer_KMul(&TerrainMid,.1);
	colorbuffer_KMul(&TerrainVoro,1.9);
	colorbuffer_Mix(HeightMap,&TerrainMid,&TerrainVoro,MT_ADD);

	mymath_Normalize(HeightMap->Data[0],S*S,Min,Max);

	colorbuffer_Free(&TerrainMid);
	colorbuffer_Free(&TerrainVoro);
	mymem_Free(&VoroP);
	nsize_Free(&Size);
}

