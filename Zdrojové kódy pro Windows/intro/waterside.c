#include"waterside.h"

void waterside_InitHeightMap(
		SColorBuffer*HeightMap,
		unsigned S,
		float Min,
		float Max,
		unsigned Seed){
	myseed=Seed;

	SNSize Size;
	nsize_Inita(&Size,2,S,S);

	float Div=2;
	colorbuffer_Init(HeightMap,BB_MIDPOINT,&Size,&Div);
	mymath_Normalize(HeightMap->Data[0],S*S,Min,Max);

	nsize_Free(&Size);
}

