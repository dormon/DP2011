#include"moss.h"

/*
void moss_Init(
		SMoss*Moss,
		unsigned Num,
		SCollide*Collide,
		SGPUTexture Texture,
		SGPUShaderProgram*Program){
	Moss->Program=Program;
	Moss->Num=Num;
	Moss->Texture=Texture;
	gpu_InitBuffer(&Moss->GPUData,sizeof(float)*7*Moss->Num,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);
	gpu_InitAttribute(&Moss->Position,Program->Attributes[ATT1_POSITION],3,
			GL_FLOAT,GL_FALSE,7*sizeof(float),0);
	gpu_InitAttribute(&Moss->Normal,Program->Attributes[ATT1_NORMAL],3,
			GL_FLOAT,GL_FALSE,7*sizeof(float),(GLvoid*)(3*sizeof(float)));
	gpu_InitAttribute(&Moss->Size,Program->Attributes[ATT1_SIZE],1,
			GL_FLOAT,GL_FALSE,7*sizeof(float),(GLvoid*)(6*sizeof(float)));
	float*Ptr=Moss->GPUData.Data;
	for(unsigned i=0;i<Moss->Num;++i){
		collide_GetEdgePoint(Ptr+i*7,Ptr+i*7+3,Collide);
		Ptr[i*7+6]=Random(.001,.01);
	}

	gpu_ChangeBuffer(&Moss->GPUData);//prave jsme zmenili data
}
// */
//*
void moss_Init(
		SMoss*Moss,
		unsigned Num,
		SCollide*Collide,
		SGPUTexture Texture,
		SGPUShaderProgram*Program){
	Moss->Program=Program;
	Moss->Num=Num*4;
	Moss->Texture=Texture;
	gpu_InitBuffer(&Moss->GPUData,sizeof(float)*8*Moss->Num,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);
	gpu_InitAttribute(&Moss->Position,Program->Attributes[ATT1_POSITION],3,
			GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(0*sizeof(float)));
	gpu_InitAttribute(&Moss->Normal,Program->Attributes[ATT1_NORMAL],3,
			GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
	gpu_InitAttribute(&Moss->Size,Program->Attributes[ATT1_SIZE],1,
			GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
	gpu_InitAttribute(&Moss->TTL,Program->Attributes[ATT1_TTL],1,
			GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(7*sizeof(float)));

	float*Ptr=(float*)Moss->GPUData.Data;
	float P[3];
	float N[3];
	float XY[2][3];
	float V[3];
	for(unsigned i=0;i<Num;++i){
		collide_GetEdgePoint(P,N,Collide);
		float Size=Random(.001,.01);
		vector_Rnd(XY[0]);
		vector_Ortho(XY[1],N,XY[0]);
		vector_Ortho(XY[0],N,XY[1]);
		for(unsigned k=0;k<4;++k){
			unsigned Base=i*8*4+k*8;
			vector_Mov(Ptr+Base,P);
			vector_Mov(Ptr+Base+3,N);
			vector_Mov(V,XY[k/2]);
			vector_KMul(V,(1-2*(k%2))*.1);
			vector_Add(Ptr+Base+3,V);
			vector_Nor(Ptr+Base+3);
			Ptr[Base+6]=Size;
			Ptr[Base+7]=1;//TTL
		}
	}
	gpu_ChangeBuffer(&Moss->GPUData);//prave jsme zmenili data
}
// */
void moss_Draw(SMoss*Moss){
	gpu_SetTextureUnit(Moss->Program->TextureUnits+UNIT1_TEXTURE);//nastavime texturovaci jednotku
	gpu_SetTexture(&Moss->Texture);//nastavime texturu
	gpu_BindBuffer(&Moss->GPUData);//pribindujeme buffer
	gpu_SetAttribute(&Moss->Position);//atribut pozice
	gpu_SetAttribute(&Moss->Normal);//atribut normaly
	gpu_SetAttribute(&Moss->Size);//atribut velikosti
	gpu_SetAttribute(&Moss->TTL);//atribut
	glUniform1f(Moss->Program->Uniforms[UNI1_MAXTTL],1);
	glUniform1f(Moss->Program->Uniforms[UNI1_SPRITE],0);
	glDrawArrays(GL_POINTS,0,Moss->Num);//vykreslime
}

void moss_Free(SMoss*Moss){
	gpu_FreeTexture(&Moss->Texture);
	gpu_FreeBuffer(&Moss->GPUData);
}


