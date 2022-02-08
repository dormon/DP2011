#include"skybox.h"

void RenderSkyBox(SGPUTexture*B);

void skybox_Init(
		SSkyBox*SkyBox,
		SGPUTexture SkyBoxTexture,
		SGPUShaderProgram*Program){
	SkyBox->Program=Program;
	gpu_InitBuffer(&SkyBox->GPUData,sizeof(float)*6*8,GL_ARRAY_BUFFER,GL_STATIC_DRAW);

	gpu_InitBuffer(&SkyBox->GPUQuads,sizeof(unsigned)*4*6,
			GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);

	gpu_InitAttribute(&SkyBox->Position,Program->Attributes[ATT5_POSITION],3,
			GL_FLOAT,GL_FALSE,sizeof(float)*6,(GLvoid*)0);

	gpu_InitAttribute(&SkyBox->Normal,Program->Attributes[ATT5_NORMAL],3,
			GL_FLOAT,GL_FALSE,sizeof(float)*6,(GLvoid*)(3*sizeof(float)));
	
	float*D=(float*)SkyBox->GPUData.Data;
	for(unsigned i=0;i<8;++i){
		for(unsigned b=0;b<3;++b)
			D[i*6+3+b]=D[i*6+b]=-1.+((i>>b)&1)*2.;
		vector_Nor(D+i*6+3);
	}

	unsigned*In=(unsigned*)SkyBox->GPUQuads.Data;
	In[0]=0;
	In[1]=1;
	In[2]=3;
	In[3]=2;

	In[4]=4;
	In[5]=5;
	In[6]=7;
	In[7]=6;


	In[8]=0;
	In[9]=2;
	In[10]=6;
	In[11]=4;

	In[12]=1;
	In[13]=3;
	In[14]=7;
	In[15]=5;


	In[16]=0;
	In[17]=1;
	In[18]=5;
	In[19]=4;

	In[20]=2;
	In[21]=3;
	In[22]=7;
	In[23]=6;
	gpu_ChangeBuffer(&SkyBox->GPUData);
	gpu_ChangeBuffer(&SkyBox->GPUQuads);
	SkyBox->SkyBox=SkyBoxTexture;
}

void skybox_Draw(SSkyBox*SkyBox){
	gpu_SetTextureUnit(SkyBox->Program->TextureUnits+UNIT5_SKYBOX);
	gpu_SetTexture(&SkyBox->SkyBox);
	gpu_BindBuffer(&SkyBox->GPUData);
	gpu_SetAttribute(&SkyBox->Position);
	gpu_SetAttribute(&SkyBox->Normal);
	gpu_BindBuffer(&SkyBox->GPUQuads);
	glDrawElements(GL_QUADS,4*6,GL_UNSIGNED_INT,NULL);
}

void skybox_Free(SSkyBox*SkyBox){
	gpu_FreeBuffer(&SkyBox->GPUData);
	gpu_FreeBuffer(&SkyBox->GPUQuads);
	gpu_FreeTexture(&SkyBox->SkyBox);
}

