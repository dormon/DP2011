#include"waterfall.h"

void waterfall_InitParticle(SWaterFall*W,unsigned p,float TTL){
	float*Ptr=(float*)W->GPUData.Data;
	vector_Mov(Ptr+p*5,W->Emitor);
	vector_Rnd(W->Velocity+p*3);
	vector_KMul(W->Velocity+p*3,.8*Random(.01,.1));//TODO jako parametr
	Ptr[p*5+3]=RandRange(&W->Size);
	Ptr[p*5+4]=W->MaxTTL*TTL;
}

void waterfall_Init(
		SWaterFall*W,
		float*E,
		SGPUTexture Particle,
		SGPUTextureUnit ParticleUnit,
		unsigned N,
		SCollide Collide,
		GLuint PositionA,
		GLuint SizeA,
		GLuint TTLA,
		GLuint MaxTTLUniform,
		SRandRange Size,
		float MinHeight,
		unsigned MaxTTL){
	vector_Mov(W->Emitor,E);//pozice emitoru
	W->Particle=Particle;
	W->ParticleUnit=ParticleUnit;
	W->MaxTTLUniform=MaxTTLUniform;
	W->Size=Size;
	W->Num=N;
	W->Collide=Collide;
	W->MinHeight=MinHeight;
	W->MaxTTL=MaxTTL*1e-3;
	W->Velocity=(float*)mymem_Alloc(sizeof(float)*3*W->Num);

	gpu_InitBuffer(&W->GPUData,sizeof(float)*5*W->Num,
			GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);
	gpu_InitAttribute(&W->PositionAttribute,PositionA,3,GL_FLOAT,GL_FALSE,
			5*sizeof(float),0);
	gpu_InitAttribute(&W->SizeAttribute,SizeA,1,GL_FLOAT,GL_FALSE,
			5*sizeof(float),(GLvoid*)(3*sizeof(float)));
	gpu_InitAttribute(&W->TTLAttribute,TTLA,1,GL_FLOAT,GL_FALSE,
			5*sizeof(float),(GLvoid*)(4*sizeof(float)));

	for(unsigned p=0;p<W->Num;++p)
		waterfall_InitParticle(W,p,(float)p/W->Num);

	gpu_ChangeBuffer(&W->GPUData);
}

void waterfall_Update(SWaterFall*Fall,unsigned Dt){
	float dt=Dt*1e-3;//cas v sekundach
	float*Ptr=(float*)Fall->GPUData.Data;//cpu data
	for(unsigned p=0;p<Fall->Num;++p){//projdeme castice
		if(Ptr[p*5+4]<=dt){//zivot castice se naplnil smrtka prichazi
			waterfall_InitParticle(Fall,p,1);//inicializujme castici
			continue;
		}else Ptr[p*5+4]-=dt;
		/*float NewPos[3];//nova pozice
		float NewVec[3];//nova rychlost
		vector_Mov(NewVec,(float*)Gravity);//g
		vector_KMul(NewVec,dt);//g*t

		vector_Add(Fall->Velocity+p*3,NewVec);//pricteme rychlost
		if(collide_GetNewPositionVelocity(Ptr+p*5,
					Fall->Velocity+p*3,Dt,&Fall->Collide)){
			vector_KMul(Fall->Velocity+p*3,.8);
		}*/
		
		float OldPos[3];//stara pozice
		float OldVel[3];//stara rychlost
		float V[3];//pomocny vektor pro vypocet
		vector_Mov(OldVel,Fall->Velocity+p*3);//stara rychlost
		vector_Mov(OldPos,Ptr+p*5);//stara pozice
		vector_Mov(V,(float*)Gravity);//g
		vector_KMul(V,dt);//g*t delta rychlosti
		vector_Add(Fall->Velocity+p*3,V);//soucet rychlost
//		vector_Rnd(V);//nahodny smer brauunovy rychlosti
//		vector_KMul(V,RandRange(&Fall->BraunSpeed));//vynasobime braunovou rychlosti
//		vector_Add(P->Velocity+p*3,V);//soucet ryhclosti
		vector_Mov(V,Fall->Velocity+p*3);//v
		vector_KMul(V,dt);//v*t delta pozice
		vector_Add(Ptr+p*5,V);//soucet pozic

		if(collide_GetNewPosVel(Ptr+p*5,Fall->Velocity+p*3,OldPos,OldVel,&Fall->Collide))
			vector_KMul(Fall->Velocity+p*3,.8);//tlumeni


		if(Ptr[p*5+1]<Fall->MinHeight)//mimo rozsah zivota
			waterfall_InitParticle(Fall,p,1);//inicializujme castici
	}
	gpu_ChangeBuffer(&Fall->GPUData);
}

void waterfall_Draw(SWaterFall*Fall){
	gpu_SetTextureUnit(&Fall->ParticleUnit);//nastavme texturovaci jednotku
	gpu_SetTexture(&Fall->Particle);//nastavime texturu
	gpu_BindBuffer(&Fall->GPUData);//pribindujeme buffery
	gpu_SetAttribute(&Fall->PositionAttribute);//atribut pozice
	gpu_SetAttribute(&Fall->SizeAttribute);//atribut velikosti
	gpu_SetAttribute(&Fall->TTLAttribute);//casovy atribut
	glUniform1f(Fall->MaxTTLUniform,Fall->MaxTTL);//maximalni cas zivota
	glDrawArrays(GL_POINTS,0,Fall->Num);//vykreslime castice
}

