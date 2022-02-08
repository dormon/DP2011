#include"terrain.h"

void terrain_Init(
		STerrain*Terrain,
		STerrainTemplate T){

	SNSize Size=T.Terrain.Size;
	unsigned NumPoints=Size.Size[0]*Size.Size[1];
	unsigned NumTriangles=(Size.Size[0]-1)*(Size.Size[1]-1)*2;

	Terrain->Program=T.Program;

	Terrain->BumpTexture=T.BumpTexture;
	Terrain->HeightTexture=T.HeightTexture;
	Terrain->RockTexture=T.RockTexture;

	mymath_MinMax(&Terrain->MinHeight,&Terrain->MaxHeight,
			T.Terrain.Data[0],NumPoints);
	Terrain->SeaHeight=T.SeaHeight;
	Terrain->MountainHeight=T.MountainHeight;

	gpu_InitBuffer(&Terrain->GPUData,sizeof(float)*3*2*NumPoints,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);
	gpu_InitBuffer(&Terrain->GPUTriangles,sizeof(unsigned)*3*NumTriangles,
			GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
	gpu_InitAttribute(&Terrain->Position,T.Program->Attributes[ATT4_POSITION],3,
			GL_FLOAT,GL_FALSE,sizeof(float)*6,(GLvoid*)0);
	gpu_InitAttribute(&Terrain->Normal,T.Program->Attributes[ATT4_NORMAL],3,
			GL_FLOAT,GL_FALSE,sizeof(float)*6,(GLvoid*)(3*sizeof(float)));
	
	SIndex Index;
	index_Init(&Index,&Size);

	float*DPtr=(float*)Terrain->GPUData.Data;
	unsigned*IPtr=(unsigned*)Terrain->GPUTriangles.Data;

	do{//spocitame vrcholy
		unsigned II=index_GetIndex(&Index);//index do textury
		DPtr[II*6+0]=1.*Index.Index[0]/Index.Size.Size[0];//x
		DPtr[II*6+1]=T.Terrain.Data[0][II];//y
		DPtr[II*6+2]=1.*Index.Index[1]/Index.Size.Size[1];//z
	}while(!index_Inc(&Index));

	//spocitame indexy
	for(unsigned y=0;y<Size.Size[1]-1;++y)
		for(unsigned x=0;x<Size.Size[0]-1;++x){
			IPtr[(y*(Size.Size[0]-1)+x)*6  ]=(y  )*Size.Size[0]+(x  );
			IPtr[(y*(Size.Size[0]-1)+x)*6+1]=(y  )*Size.Size[0]+(x+1);
			IPtr[(y*(Size.Size[0]-1)+x)*6+2]=(y+1)*Size.Size[0]+(x  );
			IPtr[(y*(Size.Size[0]-1)+x)*6+3]=(y+1)*Size.Size[0]+(x  );
			IPtr[(y*(Size.Size[0]-1)+x)*6+4]=(y  )*Size.Size[0]+(x+1);
			IPtr[(y*(Size.Size[0]-1)+x)*6+5]=(y+1)*Size.Size[0]+(x+1);
		}

	geometry_ComputeNormal(DPtr+3,6,DPtr,6,NumPoints,IPtr,NumTriangles);

	gpu_ChangeBuffer(&Terrain->GPUData);
	gpu_ChangeBuffer(&Terrain->GPUTriangles);

	index_Free(&Index);
}

void terrain_InitTexture(
		SGPUTexture*BumpTexture,
		unsigned Seed,
		unsigned S){
	SColorBuffer Tex0,Tex0Mid0,Tex0Mid1,Tex2,Tex3;

	SNSize Size;
	nsize_Inita(&Size,2,S,S);

	float Div=2;
	myseed=Seed;
	colorbuffer_Init(&Tex0Mid0,BB_MIDPOINT,&Size,&Div);
	colorbuffer_KMul(&Tex0Mid0,1);
	colorbuffer_Init(&Tex0Mid1,BB_MIDPOINT,&Size,&Div);
	colorbuffer_KMul(&Tex0Mid1,1);

	colorbuffer_Mix(&Tex0,&Tex0Mid0,&Tex0Mid1,MT_DIFF);


	colorbuffer_Normal(&Tex2,&Tex0,0);
	colorbuffer_Bump(&Tex3,&Tex2);

	gpu_InitTexture(BumpTexture,1,0,Tex3);


	nsize_Free(&Size);
	colorbuffer_Free(&Tex0Mid0);
	colorbuffer_Free(&Tex0Mid1);
	colorbuffer_Free(&Tex0);
	colorbuffer_Free(&Tex2);
}

void terrain_GradientTexture(SGPUTexture*Texture,unsigned S,SColorMap ColorMap){
	SColorBuffer Grad0,Grad1;
	SNSize Size;
	nsize_Inita(&Size,1,S);
	colorbuffer_Init(&Grad0,BB_EMPTY,&Size,(void*)1);
	for(unsigned i=0;i<S;++i)Grad0.Data[0][i]=1.*i/(S-1);
	colorbuffer_ToColor(&Grad1,&Grad0,&ColorMap);
	gpu_InitTexture(Texture,1,0,Grad1);
	colorbuffer_Free(&Grad0);
	nsize_Free(&Size);
}

void terrain_Draw(STerrain*Terrain){
	gpu_SetTextureUnit(Terrain->Program->TextureUnits+UNIT4_TERRAINBUMP);
	gpu_SetTexture(&Terrain->BumpTexture);
	gpu_SetTextureUnit(Terrain->Program->TextureUnits+UNIT4_HEIGHTTEXTURE);
	gpu_SetTexture(&Terrain->HeightTexture);
	gpu_SetTextureUnit(Terrain->Program->TextureUnits+UNIT4_ROCKTEXTURE);
	gpu_SetTexture(&Terrain->RockTexture);
	gpu_BindBuffer(&Terrain->GPUData);
	gpu_SetAttribute(&Terrain->Position);
	gpu_SetAttribute(&Terrain->Normal);
	gpu_BindBuffer(&Terrain->GPUTriangles);
	glUniform1f(Terrain->Program->Uniforms[UNI4_MINHEIGHT],Terrain->MinHeight);
	glUniform1f(Terrain->Program->Uniforms[UNI4_MAXHEIGHT],Terrain->MaxHeight);
	glUniform1f(Terrain->Program->Uniforms[UNI4_SEAHEIGHT],Terrain->SeaHeight);
	glUniform1f(Terrain->Program->Uniforms[UNI4_MOUNTAINHEIGHT],Terrain->MountainHeight);
	glDrawElements(
			GL_TRIANGLES,
			Terrain->GPUTriangles.Size/sizeof(unsigned),
			GL_UNSIGNED_INT,
			NULL);
}


void terrain_Free(STerrain*Terrain){
	gpu_FreeBuffer(&Terrain->GPUData);
	gpu_FreeBuffer(&Terrain->GPUTriangles);
	gpu_FreeTexture(&Terrain->BumpTexture);
	gpu_FreeTexture(&Terrain->RockTexture);
	gpu_FreeTexture(&Terrain->HeightTexture);
}


