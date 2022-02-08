#include"cave.h"

void cave_Init(
		SCave*Cave,
		SColorBuffer Volume,
		float Threshold,
		unsigned AmbientMask,
		unsigned TextureSize,
		SGPUShaderProgram*Program){
	Cave->Program=Program;
	Cave->Volume=Volume;
	cave_BuildNorAmb(&Cave->NormalAmbient,&Cave->Volume,AmbientMask,Threshold);//NOR AO

	cave_VolumeToTriangles(&Cave->GPUPoints,&Cave->GPUTriangles,//na troj.
			&Cave->Volume,Threshold);

	cave_BuildTextures(Cave,TextureSize);
		gpu_InitAttribute(&Cave->Position,Program->Attributes[ATT0_POSITION],3,
			GL_FLOAT,GL_FALSE,0,(GLvoid*)0);
}

void cave_Draw(SCave*Cave){
	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_DOWN);
	gpu_SetTexture(&Cave->Down);//dolni textura

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_DOWNBUMP);
	gpu_SetTexture(&Cave->DownBump);//dolni bump mapa

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_SIDE);
	gpu_SetTexture(&Cave->Side);//bocni textura

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_SIDEBUMP);
	gpu_SetTexture(&Cave->SideBump);//bocni bump mapa


	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_UP);
	gpu_SetTexture(&Cave->Up);//horni textura

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_UPBUMP);
	gpu_SetTexture(&Cave->UpBump);//horni bump mapa

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_SEDIMENT);
	gpu_SetTexture(&Cave->Sediment);//sedimenty


	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_NORAMB);
	gpu_SetTexture(&Cave->NorAmb);//normaly a AO

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_WET);
	gpu_SetTexture(&Cave->Wet);//mokra textura

	gpu_SetTextureUnit(Cave->Program->TextureUnits+UNIT0_CAUSTIC);
	gpu_SetTexture(&Cave->Caustic);//kaustiky

	gpu_BindBuffer(&Cave->GPUPoints);//nastavime buffer bodu
	gpu_SetAttribute(&Cave->Position);//pripojima atribut
	gpu_BindBuffer(&Cave->GPUTriangles);//nastavime buffer elementu

	glDrawElements(//vykreslime
			GL_TRIANGLES,
			Cave->GPUTriangles.Size/sizeof(unsigned),
			GL_UNSIGNED_INT,
			NULL);
}

void cave_BuildVolume(SColorBuffer*Volume,unsigned Size){
	SNSize BufferSize;//velikost  buffer
	unsigned MaskSize=2;//velikost vyhlazovaci masky
	float MidArg=2;//parametr pro midpoint algoritmus
	SColorBuffer InitBuffer;//buffer pro vyhlazeni

	nsize_Inita(&BufferSize,3,Size,Size,Size);//inici. velikost

	myseed=3125466577;
	colorbuffer_Init(&InitBuffer,BB_MIDPOINT,&BufferSize,&MidArg);//
	//void*VOP=colorbuffer_GenVoronoiParam(&BufferSize,0.005);
	//colorbuffer_Init(&InitBuffer,BB_VORONOID,&BufferSize,VOP);

	colorbuffer_Circle(&InitBuffer,10);//omezime na kruh
	colorbuffer_Transform(Volume,&InitBuffer,TT_SMOOTH,0,&MaskSize);//vyhladime
	colorbuffer_Free(&InitBuffer);//volnime prubezne buffery
	nsize_Free(&BufferSize);//uvolnime velikost
}

void cave_BuildNorAmb(
		SColorBuffer*NorAmb,
		SColorBuffer*Volume,
		unsigned Size,
		float Threshold){
	SColorBuffer Thres;//naprahovany objem
	SColorBuffer Amb;//ambient occlusion
	SColorBuffer Normal;//normaly
	colorbuffer_Transform(&Thres,Volume,TT_THRESHOLD,0,&Threshold);//naprahujeme
	colorbuffer_Transform(&Amb,&Thres,TT_SMOOTH,0,&Size);//vypocet AO
	colorbuffer_Normal(&Normal,Volume,0);//vypocet normal
	SColorBuffer*Buffers[2]={&Normal,&Amb};//Buffery pro kompozici
	unsigned Source[4]={0,0,0,1};//ze kterych bufferu se zerou data
	unsigned Channel[4]={0,1,2,0};//ze kterych kanalu bufferu se zerou data
	colorbuffer_Compose(NorAmb,Buffers,Source,Channel,4);//provedeme kompozici
	colorbuffer_Free(&Thres);//uvolnime naprahovani
	colorbuffer_Free(&Amb);//uvolnime ambient
	colorbuffer_Free(&Normal);//uvolnime normaly
	DrawBar();
}

void cave_VolumeToTriangles(
		SGPUBuffer*Points,
		SGPUBuffer*Triangles,
		SColorBuffer*Volume,
		float Threshold){
	SPack Pack;//pro orezani poctu vrcholu
	pack_Init(&Pack);//inicializujeme pack
	SIndex Ind;//pro pruchod bufferem
	float CubeSize=1./Volume->Size.Size[0];//velikost jedne kosticky
	float P[100];
	unsigned NumPoint;
	unsigned T[100];
	unsigned NumTriangle;

	index_Init(&Ind,&Volume->Size);//inic. index

	do{//provedeme marching tetrahedra
		unsigned char Corner=0;//nastavene vrcholy
		float Aprox[8];//hodnota ve vrcholech
		for(int d=0;d<8;++d){//projdeme vrcholy
			for(int de=0;de<3;++de)//projdeme bity vrcholu
				index_Mov(&Ind,(d>>de)&1,de,INDEX_REPEAT);//posuneme index
			Aprox[d]=Volume->Data[0][index_GetIndex(&Ind)];//hodnota
			if(Aprox[d]>Threshold)//pokud je vetsi nez prah
				Corner|=1<<d;//nastavime vrchold
			for(int de=0;de<3;++de)//projdeme bity vrcholu
				index_Mov(&Ind,-((d>>de)&1),de,INDEX_REPEAT);//posuneme index
		}

		float CubePos[3]={//souradnice kostky
			Ind.Index[0]*CubeSize,
			Ind.Index[1]*CubeSize,
			Ind.Index[2]*CubeSize};
		NumPoint=0;
		NumTriangle=0;
		mt_Get(P,&NumPoint,T,&NumTriangle,Corner,
				CubePos,CubeSize,Aprox,Threshold);
		FLOAT_DIFF=0.000001;
		pack_Insert(&Pack,P,NumPoint,T,NumTriangle);
	}while(!index_Inc(&Ind));
	gpu_InitBuffer(Points,sizeof(float)*3*Pack.Point.Used,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);//buffer bodu
	mymem_Cpy(Points->Data,Pack.Point.Data,Points->Size);//prekopirujeme data
	gpu_ChangeBuffer(Points);//data se v bufferu zmenila
	gpu_InitBuffer(Triangles,sizeof(unsigned)*3*Pack.Triangle.Used,
			GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);//buffer trojuhelniku
	mymem_Cpy(Triangles->Data,Pack.Triangle.Data,Triangles->Size);//prekopir.
	gpu_ChangeBuffer(Triangles);//data se v bufferu zmenila
	pack_Free(&Pack);//uvolnime pack
	index_Free(&Ind);//uvolnime index
	DrawBar();
}

void cave_BuildTextures(
		SCave*Cave,
		unsigned Size){
	SNSize Size1D;//velikost 1D textur
	SNSize Size2D;//velikost 2D textur
	nsize_Inita(&Size1D,1,Size);//inicializace 1D textur
	nsize_Inita(&Size2D,2,Size,Size);//inicializace 2D textur
	float MidpointParam=2;//puleni u midpoint
	SColorBuffer Side0;//pocatecni buffer
	SColorBuffer Side1;//aplikova operace0
	SColorBuffer Side2;//normaly
	SColorBuffer Side;//bocni textura
	SColorBuffer SideBump;//bocni bumpmapa
	SColorBuffer Sediment0;//pocatecni sedimenty
	SColorBuffer Sediment;//sedimenty
	SColorBuffer Up0;//pocatecni vrchni textura
	SColorBuffer Up1;//normaly
	SColorBuffer Up;//horni textura
	SColorBuffer UpBump;//horni bumpmapa
	SColorBuffer Down0;//pocatecni dolni textura
	SColorBuffer Down1;//normaly
	SColorBuffer Down;//dolni textura
	SColorBuffer DownBump;//dolni bump mapa
	SColorBuffer Wet0;//mokra textura
	SColorMap SideColorMap;//barevny prechod pro bocni texturu
	SColorMap UpColorMap;//barvny prechod pro horni texturu
	SColorMap DownColorMap;//barevny prechodp pro bocni texturu

	SColorMapTemplate SideTemplate={234,10,31,2};
	colormap_Generate(&SideColorMap,&SideTemplate);//vygenerujeme barevny prechod

	SColorMapTemplate DownTemplate={234,10,250,2};
	colormap_Generate(&DownColorMap,&DownTemplate);//vygenerujeme barevny prechod

	SColorMapTemplate UpTemlate={234,10,100,2};
	colormap_Generate(&UpColorMap,&UpTemlate);//vygenerujeme barevny prechod

	colorbuffer_Init(&Side0,BB_MIDPOINT,&Size2D,&MidpointParam);

	//Side, SideBump
	colorbuffer_Transform(&Side1,&Side0,TT_WHEEZING,0,NULL);
	colorbuffer_ToColor(&Side,&Side1,&SideColorMap);
	gpu_InitTexture(&Cave->Side,1,0,Side);

	colorbuffer_Normal(&Side2,&Side1,0);
	colorbuffer_Bump(&SideBump,&Side2);
	gpu_InitTexture(&Cave->SideBump,1,0,SideBump);

	//Down, DownBump
	colorbuffer_Transform(&Down0,&Side0,TT_SCRAP,0,NULL);
	colorbuffer_ToColor(&Down,&Down0,&DownColorMap);
	gpu_InitTexture(&Cave->Down,1,0,Down);

	colorbuffer_Normal(&Down1,&Down0,0);
	colorbuffer_Bump(&DownBump,&Down1);
	gpu_InitTexture(&Cave->DownBump,1,0,DownBump);

	//Up, UpBump
	colorbuffer_Transform(&Up0,&Side0,TT_SCRAP,0,NULL);
	colorbuffer_ToColor(&Up,&Up0,&UpColorMap);
	gpu_InitTexture(&Cave->Up,1,0,Up);

	colorbuffer_Normal(&Up1,&Up0,0);
	colorbuffer_Bump(&UpBump,&Up1);
	gpu_InitTexture(&Cave->UpBump,1,0,UpBump);

	//Sediment
	colorbuffer_Init(&Sediment0,BB_MIDPOINT,&Size1D,&MidpointParam);
	colorbuffer_ToColor(&Sediment,&Sediment0,&SideColorMap);
	gpu_InitTexture(&Cave->Sediment,1,0,Sediment);

	//NormalAmbient
	gpu_InitTexture(&Cave->NorAmb,0,0,Cave->NormalAmbient);

	//mokra textura
	SNSize WetSize;
	nsize_Mov(&WetSize,&Cave->NormalAmbient.Size);
	WetSize.Size[0]*=2;
	WetSize.Size[1]*=2;
	WetSize.Size[2]*=2;


/*	colorbuffer_Alloc(&Wet0,1,&WetSize);*/
	colorbuffer_Init(&Wet0,BB_EMPTY,&WetSize,(void*)1);
	unsigned NumD=nsize_Size(&Wet0.Size);
	for(unsigned i=0;i<NumD;++i)Wet0.Data[0][i]=1;
	gpu_InitTexture(&Cave->Wet,0,0,Wet0);

	//kaustiky
	void*VOROP=colorbuffer_GenVoronoiParam(&Cave->NormalAmbient.Size,0.003);
	SColorBuffer Cau;
	colorbuffer_Init(&Cau,BB_VORONOID,&Cave->NormalAmbient.Size,VOROP);
	mymem_Free(&VOROP);
	gpu_InitTexture(&Cave->Caustic,0,0,Cau);


	colorbuffer_Free(&Side0);
	colorbuffer_Free(&Side1);
	colorbuffer_Free(&Side2);
	colorbuffer_Free(&Sediment0);
	colorbuffer_Free(&Up0);
	colorbuffer_Free(&Up1);
	colorbuffer_Free(&Down0);
	colorbuffer_Free(&Down1);

	colormap_Free(&SideColorMap);//uvolnime bocni barevny prechod
	colormap_Free(&UpColorMap);//uvolnime horni barevny prechod
	colormap_Free(&DownColorMap);//uvolnime dolni barevny prechod
	nsize_Free(&Size1D);//uvolnime velikost 1D textur
	nsize_Free(&Size2D);//uvolnime velikost 2D textur
	nsize_Free(&WetSize);
}

void cave_Free(SCave*Cave){
	gpu_FreeTexture(&	Cave->Caustic);
	gpu_FreeTexture(&	Cave->Down);
	gpu_FreeTexture(&	Cave->DownBump);
	gpu_FreeTexture(&	Cave->NorAmb);
	gpu_FreeTexture(&	Cave->Sediment);
	gpu_FreeTexture(&	Cave->Side);
	gpu_FreeTexture(&	Cave->SideBump);
	
	gpu_FreeTexture(&	Cave->Up);
	gpu_FreeTexture(&	Cave->UpBump);
	gpu_FreeTexture(&	Cave->Wet);

	colorbuffer_Free(&Cave->Volume);
	gpu_FreeBuffer(&Cave->GPUPoints);
	gpu_FreeBuffer(&Cave->GPUTriangles);

}

