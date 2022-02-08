#include"particle.h"

void particlesystem_Init(
		SParticleSystem*P,
		unsigned Num,
		float*Emitor,
		float Damping,
		unsigned MaxTTL,
		SRandRange Size,
		SRandRange Speed,
		SRandRange BraunSpeed,
		SGPUTexture Particle,
		SCollide Collide,
		SGPUShaderProgram*Program){
	P->IsSprite=1;
	P->Program=Program;
	P->Num=Num;//pocet castic
	vector_Mov(P->Emitor,Emitor);//presuneme amitor
	P->Damping=Damping;
	P->MaxTTL=MaxTTL*1e-3;//cas v sekundach
	P->Size=Size;//velikost castic
	P->Speed=Speed;//rychlost castic
	P->BraunSpeed=BraunSpeed;//braunova rychlost
	P->Velocity=(float*)mymem_Alloc(sizeof(float)*3*P->Num);//alokace rychlosti
	P->Particle=Particle;//textura
	P->Collide=Collide;//kolizni system

	gpu_InitBuffer(&P->GPUDataPosition,sizeof(float)*3*P->Num,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer dat
	gpu_InitBuffer(&P->GPUDataNormal,sizeof(float)*3*P->Num,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer dat
	gpu_InitBuffer(&P->GPUDataSize,sizeof(float)*1*P->Num,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer dat
	gpu_InitBuffer(&P->GPUDataTTL,sizeof(float)*1*P->Num,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);//buffer dat

	gpu_InitAttribute(&P->PositionAttribute,Program->Attributes[ATT1_POSITION],
			3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);//pozicni atribut
	gpu_InitAttribute(&P->NormalAttribute,Program->Attributes[ATT1_NORMAL],
			3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);//pozicni atribut
	gpu_InitAttribute(&P->SizeAttribute,Program->Attributes[ATT1_SIZE],
			1,GL_FLOAT,GL_FALSE,1*sizeof(float),0);
	gpu_InitAttribute(&P->TTLAttribute,Program->Attributes[ATT1_TTL],
			1,GL_FLOAT,GL_FALSE,1*sizeof(float),0);

	for(unsigned p=0;p<P->Num;++p){//projdeme castice
		particlesystem_InitParticle(P,p,(float)p/P->Num);//inicializujeme castice
		vector_Mov(((float*)P->GPUDataNormal.Data)+p*3,(float*)OneVector);
	}

	gpu_ChangeBuffer(&P->GPUDataPosition);
	gpu_ChangeBuffer(&P->GPUDataNormal);
	gpu_ChangeBuffer(&P->GPUDataSize);
	gpu_ChangeBuffer(&P->GPUDataTTL);
}

void particlesystem_InitParticle(
		SParticleSystem*P,
		unsigned Particle,
		float TTL){
	float*Ptr=(float*)P->GPUDataPosition.Data;//ukazatel na CPU data
	vector_Mov(Ptr+Particle*3,P->Emitor);//presumene na pozici emitoru
	vector_Rnd(P->Velocity+Particle*3);//nahodny jednotkovy vektor
	vector_KMul(P->Velocity+Particle*3,RandRange(&P->Speed));//rychlost
	Ptr=(float*)P->GPUDataSize.Data;
	Ptr[Particle]=RandRange(&P->Size);//velikost
	Ptr=(float*)P->GPUDataTTL.Data;
	Ptr[Particle]=P->MaxTTL*TTL;//doba zivota
}


void particlesystem_Draw(SParticleSystem*P){
	gpu_SetTextureUnit(P->Program->TextureUnits+UNIT1_TEXTURE);//t. jednotka
	gpu_SetTexture(&P->Particle);//nastavime texturu
	gpu_BindBuffer(&P->GPUDataPosition);//pribindujeme buffery
	gpu_SetAttribute(&P->PositionAttribute);//atribut pozice
	gpu_BindBuffer(&P->GPUDataNormal);//pribindujeme buffery
	gpu_SetAttribute(&P->NormalAttribute);//atribut pozice
	gpu_BindBuffer(&P->GPUDataSize);//pribindujeme buffery
	gpu_SetAttribute(&P->SizeAttribute);//atribut velikosti
	gpu_BindBuffer(&P->GPUDataTTL);//pribindujeme buffery
	gpu_SetAttribute(&P->TTLAttribute);//casovy atribut
	glUniform1f(P->Program->Uniforms[UNI1_MAXTTL],P->MaxTTL);//maximalni cas zivota
	glUniform1f(P->Program->Uniforms[UNI1_SPRITE],P->IsSprite);//je to sprite
	glDrawArrays(GL_POINTS,0,P->Num);//vykreslime castice
}

void particlesystem_Update(SParticleSystem*P,unsigned Dt){
	float dt=Dt*1e-3;//cas v sekundach
	float*Ptr=(float*)P->GPUDataPosition.Data;//cpu data
	float*TTLPtr=(float*)P->GPUDataTTL.Data;//
	for(unsigned p=0;p<P->Num;++p){//projdeme castice
		if(TTLPtr[p]<=dt){//zivot castice se naplnil smrtka prichazi
			//TODO BOX OF LIFE
			particlesystem_InitParticle(P,p,1);//inicializujme castici
			continue;
		}else TTLPtr[p]-=dt;//odecteme delta casu od delky zivota castice
		float OldPos[3];//stara pozice
		float OldVel[3];//stara rychlost
		float V[3];//pomocny vektor pro vypocet
		vector_Mov(OldVel,P->Velocity+p*3);//stara rychlost
		vector_Mov(OldPos,Ptr+p*3);//stara pozice
		vector_Mov(V,(float*)Gravity);//g
		vector_KMul(V,dt);//g*t delta rychlosti
		vector_Add(P->Velocity+p*3,V);//soucet rychlost
		vector_Rnd(V);//nahodny smer brauunovy rychlosti
		vector_KMul(V,RandRange(&P->BraunSpeed));//vynasobime braunovou rychlosti
		vector_Add(P->Velocity+p*3,V);//soucet ryhclosti
		vector_Mov(V,P->Velocity+p*3);//v
		vector_KMul(V,dt);//v*t delta pozice
		vector_Add(Ptr+p*3,V);//soucet pozic

		if(collide_GetNewPosVel(Ptr+p*3,P->Velocity+p*3,OldPos,OldVel,&P->Collide))
			vector_KMul(P->Velocity+p*3,P->Damping);//tlumeni
	}
	gpu_ChangeBuffer(&P->GPUDataPosition);
	gpu_ChangeBuffer(&P->GPUDataNormal);
	gpu_ChangeBuffer(&P->GPUDataSize);
	gpu_ChangeBuffer(&P->GPUDataTTL);
}

void particlesystem_Free(SParticleSystem*P){
	gpu_FreeBuffer(&P->GPUDataPosition);//uvolnime buffer
	gpu_FreeBuffer(&P->GPUDataNormal);//uvolnime buffer
	gpu_FreeBuffer(&P->GPUDataSize);//uvolnime buffer
	gpu_FreeBuffer(&P->GPUDataTTL);//uvolnime buffer
	gpu_FreeTexture(&P->Particle);//uvolnime texturu
	mymem_Free((void**)&P->Velocity);//uvolnime rychlosti
}

void particlesystem_DrawWet(
		SColorBuffer Wet,
		SParticleSystem*PS,
		unsigned Stride,
		float Val){
	for(unsigned i=0;i<PS->Num;i+=Stride)
		colorbuffer_FastAddValue3D(Wet.Data[0],Wet.Size.Size,
				(float*)PS->GPUDataPosition.Data+i*3,Val);
}

void particlesystem_InitTexture(SGPUTexture*Texture,unsigned S){
	SNSize ParticleSize;
	nsize_Inita(&ParticleSize,2,S,S);
	SIndex III;
	index_Init(&III,&ParticleSize);

	SColorBuffer Particle,CompParticle;
	void*VOPP=colorbuffer_GenVoronoiParam(&ParticleSize,0.03);
	colorbuffer_Init(&Particle,BB_VORONOID,&ParticleSize,VOPP);
	colorbuffer_Circle(&Particle,10);

	SColorBuffer*BUF[4]={&Particle};
	unsigned BUFS[4]={0,0,0,0};
	unsigned BUFC[4]={0,0,0,0};
	colorbuffer_Compose(&CompParticle,BUF,BUFS,BUFC,4);

	do{
		unsigned iii=index_GetIndex(&III);
		CompParticle.Data[0][iii]=1;
		CompParticle.Data[1][iii]=1;
		CompParticle.Data[2][iii]=1;
		CompParticle.Data[3][iii]=pow(CompParticle.Data[3][iii],3);
	}while(!index_Inc(&III));


	gpu_InitTexture(Texture,1,0,CompParticle);
	nsize_Free(&ParticleSize);
	mymem_Free(&VOPP);
	index_Free(&III);
	colorbuffer_Free(&Particle);
}

