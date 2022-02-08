#include"plant.h"

void plant_Init(
		SPlant*Plant,
		SCollide Collide,
		float*Position,
		float Length,
		unsigned Num,
		SGPUTexture Texture,
		SGPUShaderProgram*Program){
	Plant->Program=Program;
	Plant->Num=Num;//pocet casti rostliny
	Plant->Texture=Texture;//textura lisu
	gpu_InitBuffer(&Plant->GPUPositionData,sizeof(float)*(Plant->Num*(1+3)+1)*3,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer pozic
	gpu_InitBuffer(&Plant->GPUSizeTTLData,sizeof(float)*(Plant->Num*(1+3)+1)*2,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);//buffer velikosti
	gpu_InitAttribute(&Plant->Position,Program->Attributes[ATT1_POSITION],3,
			GL_FLOAT,GL_FALSE,0,(GLvoid*)0);//atribut pozice
	gpu_InitAttribute(&Plant->Size,Program->Attributes[ATT1_SIZE],1,
			GL_FLOAT,GL_FALSE,2*sizeof(float),(GLvoid*)0);//atribut celikosti
	gpu_InitAttribute(&Plant->TTL,Program->Attributes[ATT1_TTL],1,
			GL_FLOAT,GL_FALSE,2*sizeof(float),(GLvoid*)sizeof(float));

	//vypocitame souradnice bodu
	float*Ptr=(float*)Plant->GPUPositionData.Data;//ukazatel na pozice
	for(unsigned k=0;k<=Plant->Num;++k){//projdeme kousky
		vector_Mov(Ptr+k*3,Position);
		Ptr[k*3+1]+=-Length*(float)k/Plant->Num;
	}
	float*SizeTTL=(float*)Plant->GPUSizeTTLData.Data;
	for(unsigned k=0;k<Plant->Num;++k){
		SizeTTL[k*2+0]=.007-0.006*k/Plant->Num;//TODO
		SizeTTL[k*2+1]=1;
		float Angle=Random(0,MY_PI*2);
		float V[3];
		float R=.005;//TODO
		for(unsigned i=0;i<3;++i){
			Angle+=MY_PI*2/3;
			V[0]=R*cos(Angle);
			V[1]=0;
			V[2]=R*sin(Angle);
			unsigned Base=Plant->Num+1+k*3+i;
			SizeTTL[Base*2+0]=.007-0.006*k/Plant->Num;//TODO
			SizeTTL[Base*2+1]=1;
			vector_Mov (Ptr+Base*3,Ptr+(k+0)*3);
			vector_Add (Ptr+Base*3,Ptr+(k+1)*3);
			vector_KMul(Ptr+Base*3,.5);
			vector_Add (Ptr+Base*3,V);
		}
	}
	SizeTTL[Plant->Num*2+0]=.001;//TODO
	SizeTTL[Plant->Num*2+1]=1;


	gpu_ChangeBuffer(&Plant->GPUPositionData);
	gpu_ChangeBuffer(&Plant->GPUSizeTTLData);
	//vypocitame spoje
	unsigned NumEdge=Plant->Num*(1+3*2+3);//kmen a sestisten
	unsigned*Edge=(unsigned*)mymem_Alloc(sizeof(unsigned)*NumEdge*2);//alokace
	unsigned ActEdge=0;
	for(unsigned k=0;k<Plant->Num;++k){
		Edge[(ActEdge)*2]=k;//kmen
		Edge[(ActEdge++)*2+1]=k+1;

		for(unsigned i=0;i<3;++i){
			Edge[(ActEdge)*2]=k;
			Edge[(ActEdge++)*2+1]=(Plant->Num+1)+3*k+i;

			Edge[(ActEdge)*2]=(Plant->Num+1)+3*k+i;
			Edge[(ActEdge++)*2+1]=k+1;

			Edge[(ActEdge)*2]=(Plant->Num+1)+3*k+i;
			Edge[(ActEdge++)*2+1]=(Plant->Num+1)+3*k+((i+1)%3);

		}
	}
	//alokace elastickeho systemu
	elastic_Init(&Plant->Elastic,(float*)Plant->GPUPositionData.Data,
			Plant->GPUPositionData.Size/(sizeof(float)*3),Edge,NumEdge,Collide);
	Plant->Elastic.Nodes[0].NotFixed=0;

	mymem_Free((void**)&Edge);//uvolnime spoje
}


void plant_Draw(SPlant*Plant){
	gpu_SetTextureUnit(Plant->Program->TextureUnits+UNIT1_TEXTURE);
	gpu_SetTexture(&Plant->Texture);//textura listu
	gpu_BindBuffer(&Plant->GPUPositionData);//pribindujeme buffer pozice
	gpu_SetAttribute(&Plant->Position);//nastavime atribut pozice
	gpu_BindBuffer(&Plant->GPUSizeTTLData);//pribindujeme buffer velikosti
	gpu_SetAttribute(&Plant->Size);//nastavime atribut velikosti
	gpu_SetAttribute(&Plant->TTL);//nastavime atribut casu zivota
	glUniform1f(Plant->Program->Uniforms[UNI1_MAXTTL],1);//maximalni cas zivota
	glUniform1f(Plant->Program->Uniforms[UNI1_SPRITE],1);//je to sprite
	glDrawArrays(GL_POINTS,/*Plant->Num+1*/0,(Plant->Num)*(1+3)+1);//vykreslime body
}

void plant_Update(SPlant*Plant,unsigned Dt){
	elastic_Update(&Plant->Elastic,Dt);//vypocitame elasticky system
	gpu_ChangeBuffer(&Plant->GPUPositionData);//data se zmenila
}

void plant_InitTexture(SGPUTexture*Texture,unsigned S){
	SNSize ParticleSize;
	nsize_Inita(&ParticleSize,2,S,S);
	SIndex III;
	index_Init(&III,&ParticleSize);

	SColorBuffer PlantA,PlantB,PlantC,PlantD;

	void*PLANTPP=colorbuffer_GenVoronoiParam(&ParticleSize,0.01);
	colorbuffer_Init(&PlantD,BB_VORONOID,&ParticleSize,PLANTPP);//alfa
	mymem_Free(&PLANTPP);

	colorbuffer_Transform(&PlantA,&PlantD,TT_SCRAP,0,NULL);

	SColorMapTemplate PlantTemlate={234,10,57,5};
	SColorMap PlantColorMap;
	colormap_Generate(&PlantColorMap,&PlantTemlate);//vygenerujeme barevny prechod

	colorbuffer_ToColor(&PlantB,&PlantA,&PlantColorMap);

	colorbuffer_Circle(&PlantD,10);//alfa dokruhu

	SColorBuffer*BUF[]={&PlantB,&PlantD};
	unsigned BUFS[]={0,0,0,1};
	unsigned BUFC[]={0,1,2,0};
	colorbuffer_Compose(&PlantC,BUF,BUFS,BUFC,4);//kombozice

	do{
		unsigned iii=index_GetIndex(&III);
		PlantC.Data[0][iii]=pow(PlantC.Data[0][iii],.9);
		PlantC.Data[1][iii]=pow(PlantC.Data[1][iii],.8);
	}while(!index_Inc(&III));


	gpu_InitTexture(Texture,1,0,PlantC);

	nsize_Free(&ParticleSize);
	index_Free(&III);
	colorbuffer_Free(&PlantA);
	colorbuffer_Free(&PlantB);
	colorbuffer_Free(&PlantD);
	colormap_Free(&PlantColorMap);
}

void plant_Free(SPlant*Plant){
	gpu_FreeBuffer(&Plant->GPUPositionData);
	gpu_FreeBuffer(&Plant->GPUSizeTTLData);
	gpu_FreeTexture(&Plant->Texture);
	elastic_Free(&Plant->Elastic);
}

