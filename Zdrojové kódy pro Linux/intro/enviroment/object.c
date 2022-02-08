#include"object.h"

void object_Init(
		SObject*Object,
		SCollide Collide,
		float Smooth,
		float*Points,
		float*Coords,
		unsigned NumPoints,
		unsigned*Index,
		unsigned NumIndex,
		unsigned*Joints,
		unsigned NumJoints,
		SGPUTexture Texture,
		SGPUTexture BumpTexture,
		SGPUShaderProgram*Program){
	Object->Program=Program;
	Object->Texture=Texture;
	Object->BumpTexture=BumpTexture;
	Object->Smooth=Smooth;

	gpu_InitBuffer(&Object->GPUDataPosition,sizeof(float)*NumPoints*3,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer pozic
	gpu_InitBuffer(&Object->GPUDataNormal,sizeof(float)*NumPoints*3,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer pozic
	gpu_InitBuffer(&Object->GPUDataCoord,sizeof(float)*NumPoints*2,
			GL_ARRAY_BUFFER,GL_STATIC_DRAW);//buffer pozic
	gpu_InitBuffer(&Object->GPUDataIndex,sizeof(unsigned)*NumIndex,
			GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);

	gpu_InitAttribute(&Object->PositionAttribute,
			Program->Attributes[ATT2_POSITION],3,
			GL_FLOAT,GL_FALSE,0,(GLvoid*)0);//atribut pozice
	gpu_InitAttribute(&Object->NormalAttribute,
			Program->Attributes[ATT2_NORMAL],3,
			GL_FLOAT,GL_FALSE,0,(GLvoid*)0);//atribut pozice
	gpu_InitAttribute(&Object->CoordAttribute,
			Program->Attributes[ATT2_COORD],2,
			GL_FLOAT,GL_FALSE,0,(GLvoid*)0);//atribut pozice

	mymem_Cpy(Object->GPUDataPosition.Data,Points,
			sizeof(float)*3*NumPoints);
	mymem_Cpy(Object->GPUDataIndex.Data,Index,
			sizeof(unsigned)*NumIndex);
	mymem_Cpy(Object->GPUDataCoord.Data,Coords,
			sizeof(float)*2*NumPoints);

	gpu_ChangeBuffer(&Object->GPUDataPosition);
	gpu_ChangeBuffer(&Object->GPUDataIndex);
	gpu_ChangeBuffer(&Object->GPUDataCoord);
	gpu_ChangeBuffer(&Object->GPUDataNormal);
	//fprintf(stderr,"QOX\n");
	elastic_Init(&Object->Elastic,(float*)Object->GPUDataPosition.Data,
			NumPoints,Joints,NumJoints,Collide);

	elastic_SetElasticity(&Object->Elastic,400);
}


void object_Draw(SObject*Object){
	gpu_SetTextureUnit(Object->Program->TextureUnits+UNIT2_TEXTURE);
	gpu_SetTexture(&Object->Texture);
	gpu_SetTextureUnit(Object->Program->TextureUnits+UNIT2_BUMPTEXTURE);
	gpu_SetTexture(&Object->BumpTexture);
	gpu_BindBuffer(&Object->GPUDataPosition);
	gpu_SetAttribute(&Object->PositionAttribute);
	gpu_BindBuffer(&Object->GPUDataNormal);
	gpu_SetAttribute(&Object->NormalAttribute);
	gpu_BindBuffer(&Object->GPUDataCoord);
	gpu_SetAttribute(&Object->CoordAttribute);
	gpu_BindBuffer(&Object->GPUDataIndex);
	glUniform1f(Object->Program->Uniforms[UNI2_SMOOTH],Object->Smooth);
	glDrawElements(
			GL_TRIANGLES,
			Object->GPUDataIndex.Size/sizeof(unsigned),
			GL_UNSIGNED_INT,
			NULL);
}


void object_Update(SObject*Object,unsigned Dt){
	elastic_Update(&Object->Elastic,Dt);
	gpu_ChangeBuffer(&Object->GPUDataPosition);
}

void object_ComputeNormal(SObject*Object){
	geometry_ComputeNormal(
			(float*)Object->GPUDataNormal.Data,3,
			(float*)Object->GPUDataPosition.Data,3,
			Object->GPUDataPosition.Size/sizeof(float)/3,
			(unsigned*)Object->GPUDataIndex.Data,
			Object->GPUDataIndex.Size/sizeof(unsigned)/3);
	gpu_ChangeBuffer(&Object->GPUDataNormal);
}

void object_Free(SObject*Object){
	gpu_FreeBuffer(&Object->GPUDataPosition);
	gpu_FreeBuffer(&Object->GPUDataNormal);
	gpu_FreeBuffer(&Object->GPUDataCoord);
	gpu_FreeBuffer(&Object->GPUDataIndex);
	gpu_FreeTexture(&Object->BumpTexture);
	gpu_FreeTexture(&Object->Texture);
	elastic_Free(&Object->Elastic);
}

