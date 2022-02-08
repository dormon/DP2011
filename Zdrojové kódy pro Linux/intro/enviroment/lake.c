#include"lake.h"

void lake_Inita(
		SLake*Lake,
		float*Pos,
		float A,
		unsigned WaveSize,
		SGPUTexture CubeMap,
		SGPUShaderProgram*Program){
	Lake->Program=Program;
	Lake->Cube=CubeMap;
	gpu_InitBuffer(&Lake->GPUPoints,sizeof(float)*3*4,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
	float*Ptr=(float*)Lake->GPUPoints.Data;
	for(unsigned i=0;i<4;++i){
		vector_Mov(Ptr+i*3,Pos);
		Ptr[i*3+0]+=A/2-A*(i/2);
		Ptr[i*3+2]+=A/2-A*(i==0||i==3);
	}

	gpu_ChangeBuffer(&Lake->GPUPoints);
	gpu_InitAttribute(&Lake->Position,Program->Attributes[ATT3_POSITION],3,
			GL_FLOAT,GL_FALSE,0,0);

	SNSize WaveS;
	nsize_Inita(&WaveS,3,WaveSize,WaveSize,WaveSize);
	SColorBuffer Wave;
	SColorBuffer Wave0;
/*	
	void*Voro=colorbuffer_GenVoronoiParam(&WaveS,0.01);
	colorbuffer_Init(&Wave0,BB_VORONOID,&WaveS,Voro);
	mymem_Free(Voro);
//	*/
//*
	float ttt=2;
	colorbuffer_Init(&Wave0,BB_MIDPOINT,&WaveS,&ttt);
//	*/
	colorbuffer_Normal2D3D(&Wave,&Wave0,0);
	gpu_InitTexture(&Lake->Wave,1,0,Wave);

	colorbuffer_Free(&Wave0);
	nsize_Free(&WaveS);

}

void lake_Init(
		SLake*Lake,
		float*Pos,
		float A,
		unsigned CubeSize,
		unsigned WaveSize,
		SGPUShaderProgram*Program){
	Lake->Program=Program;
	gpu_InitBuffer(&Lake->GPUPoints,sizeof(float)*3*4,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
	float*Ptr=(float*)Lake->GPUPoints.Data;
	for(unsigned i=0;i<4;++i){
		vector_Mov(Ptr+i*3,Pos);
		Ptr[i*3+0]+=A/2-A*(i/2);
		Ptr[i*3+2]+=A/2-A*(i==0||i==3);
	}

	gpu_ChangeBuffer(&Lake->GPUPoints);
	gpu_InitAttribute(&Lake->Position,Program->Attributes[ATT3_POSITION],3,
			GL_FLOAT,GL_FALSE,0,0);
	SColorBuffer Buf[6];
	SColorBuffer BA[6];

	SNSize CubeS;
	SColorMap Map;
	SColorMapTemplate Template={/*.Seed=*/234,/*.Num=*/10,/*.StartHue=*/31,/*.MaxStep=*/2};

		//.Seed=23423,.Num=5,.StartHue=250,.MaxStep=10};

	nsize_Inita(&CubeS,2,CubeSize,CubeSize);
	unsigned seed[]={
 533673057,
				 1740274513,
				 3197773990,
				 2982010460,
				 1385632655,
				 433947078
	};
	for(unsigned i=0;i<6;++i){
//		colorbuffer_Init(Buf+i,BB_EMPTY,&CubeS,(void*)1);
		float T=2;
		colorbuffer_Init(BA+i,BB_MIDPOINT,&CubeS,&T);
//		fprintf(stderr,"SEED: %u\n",myseed);
		myseed=seed[i];
		Template.Seed=Random(0,1)*100000;
		Template.StartHue=Random(0,1)*360;
		colormap_Generate(&Map,&Template);//vygenerujeme barevny prechod

		colorbuffer_ToColor(Buf+i,BA+i,&Map);
		colormap_Free(&Map);

		/*
		float CCC[6][3]={{1,0,0},{0,1,0,},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};
		for(unsigned c=0;c<3;++c)
			for(unsigned k=0;k<nsize_Size(&CubeS);++k)
				Buf[i].Data[c][k]=CCC[i][c];
				*/
	}
	gpu_InitTexture(&Lake->Cube,0,1,Buf[0],Buf[1],Buf[2],Buf[3],Buf[4],Buf[5]);
	SNSize WaveS;
	nsize_Inita(&WaveS,3,WaveSize,WaveSize,WaveSize);
	SColorBuffer Wave;
	SColorBuffer Wave0;
/*	
	void*Voro=colorbuffer_GenVoronoiParam(&WaveS,0.01);
	colorbuffer_Init(&Wave0,BB_VORONOID,&WaveS,Voro);
	mymem_Free(Voro);
//	*/
//*
	float ttt=2.0;
	colorbuffer_Init(&Wave0,BB_MIDPOINT,&WaveS,&ttt);
//	*/
	colorbuffer_Normal2D3D(&Wave,&Wave0,0);
	gpu_InitTexture(&Lake->Wave,1,0,Wave);

	colorbuffer_Free(&Wave0);
	nsize_Free(&CubeS);
	nsize_Free(&WaveS);
}

/**
 * @brief Uvolni jezero
 *
 * @param Lake jezero
 */
void lake_Free(SLake*Lake){
	//gpu_FreeTexture(&Lake->Cube);
	gpu_FreeTexture(&Lake->Wave);
	gpu_FreeBuffer(&Lake->GPUPoints);
}

/**
 * @brief Vykresli jezero
 *
 * @param Lake jezero
 */
void lake_Draw(SLake*Lake){
	gpu_SetTextureUnit(Lake->Program->TextureUnits+UNIT3_WAVE);
	gpu_SetTexture(&Lake->Wave);
	gpu_SetTextureUnit(Lake->Program->TextureUnits+UNIT3_CUBE);
	gpu_SetTexture(&Lake->Cube);

	gpu_BindBuffer(&Lake->GPUPoints);
	gpu_SetAttribute(&Lake->Position);
	glDrawArrays(GL_QUADS,0,4);
}

