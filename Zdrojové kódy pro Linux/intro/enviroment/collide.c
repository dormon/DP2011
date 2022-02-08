#include"collide.h"

void collide_Init(
		SCollide*Collide,
		SColorBuffer*Volume,
		SColorBuffer*Normal,
		float Threshold){
	Collide->Normal=Normal;
	Collide->Volume=Volume;
	Collide->Threshold=Threshold;
}

void collide_GetNormal(float*Normal,SCollide*Collide,float*Position){
	for(unsigned i=0;i<3;++i)
		Normal[i]=colorbuffer_FastGetValue3D(Collide->Normal->Data[i],
				Collide->Normal->Size.Size,Position)*2-1;
	vector_Nor(Normal);
}


int collide_GetNewPositionVelocity(
		float*Pos,
		float*Vel,
		unsigned Dt,
		SCollide*Collide){
	float DVel[3];
	float BPos[3];
	vector_Mov(DVel,Vel);//v
	vector_KMul(DVel,Dt*1e-3);//v*t
	vector_Mov(BPos,Pos);//p
	vector_Add(BPos,DVel);//p+v*t
	float ValB=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],//hustota
			Collide->Volume->Size.Size,BPos);
	if(ValB<=Collide->Threshold){
		float ValA=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],//hustota
			Collide->Volume->Size.Size,Pos);
		float t=(Collide->Threshold-ValA)/(ValB-ValA);//parametr mezi body AB
		if(ValB==ValA)t=.5;
		t=mymath_Clamp(t,0,1);
		vector_KMul(Pos,1-t);
		vector_KMul(BPos,t);
		vector_Add(Pos,BPos);//bod kolize
		float N[3];//normala v koliznim bode
		collide_GetNormal(N,Collide,Pos);//ziskame normalu
		float NewV[3];//odrazena rychlost
		vector_Reflect(NewV,DVel,N);
		vector_KMul(NewV,1-t);//dojedeme jeste zbytek drahy po odrazu
		vector_Add(Pos,NewV);//vysledna pozice
		vector_Reflect(NewV,Vel,N);
		vector_Mov(Vel,NewV);//vysledna rychlost*/
		return 1;
	}
	vector_Mov(Pos,BPos);//presuneme bod
	return 0;
}

int collide_GetNewPosVel(
		float*NewPos,
		float*NewVel,
		float*OldPos,
		float*OldVel,
		SCollide*Collide){
	float ValA,ValB,t;//hustota v bode A,B a parametr vahy
	if((ValB=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
			Collide->Volume->Size.Size,NewPos))<=Collide->Threshold){
		ValA=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
			Collide->Volume->Size.Size,OldPos);

		if(ValB==ValA)t=.5;//hustoty jsou stejne
		else t=(Collide->Threshold-ValA)/(ValB-ValA);//parametr
		t=mymath_Clamp(t,0,1);//projistotu orezeme

		vector_KMul(OldPos,1-t);//vahujeme predchazejici pozici
		vector_KMul(NewPos,t);//vahujeme stavajici pozici
		vector_Add(NewPos,OldPos);//kolizni bod

		vector_KMul(OldVel,1-t);//vahujeme predchazejici rychlost
		vector_KMul(NewVel,t);//vahujeme stavajici rychlost
		vector_Add(NewVel,OldVel);//kolizni rychlost
	
		float N[3];//normala v koliznim bode
		collide_GetNormal(N,Collide,NewPos);//ziskame normalu

		float V[3];//nova rychlost
		vector_Reflect(V,NewVel,N);//odrazime rychlost
		vector_Mov(NewVel,V);//presuneme odrazenou rychlost
		return 1;
	}
	return 0;
}


void collide_GetEdgePoint(float*Pos,float*Normal,SCollide*Collide){
	float V,NextV;//hodnota hustota v predchazejicim bode a nasledujicim
	float NextPos[3];//nasledujici bod
	float Max=fmax(Collide->Volume->Size.Size[0],Collide->Volume->Size.Size[1]);
	Max=fmax(Max,Collide->Volume->Size.Size[2]);//TODO stejne nebo udelat obecne do funkce
semm:
	do{//dokud nenajdeme bod v jeskyni
		for(unsigned i=0;i<3;++i)NextPos[i]=Random(0,1);//generujeme bod
	}while((NextV=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
				Collide->Volume->Size.Size,NextPos))>=Collide->Threshold);
	float Vec[3];
	vector_Rnd(Vec);
	vector_KMul(Vec,1/Max);
	do{//dokud neprekrocime stenu jeskyne
		V=NextV;//nova hodnota se stane starou
		vector_Mov(Pos,NextPos);//stejne tam pozice
//		collide_GetNormal(Normal,Collide,Pos);//ziskame normalu
//		vector_KMul(Normal,1/Max);
		vector_Sub(NextPos,Vec);//Normal);//ziskame novy bod
		if(NextPos[0]<0||NextPos[1]<0||NextPos[2]<0||NextPos[0]>=1||NextPos[1]>=1||NextPos[2]>=1)goto semm;
	}while((NextV=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
				Collide->Volume->Size.Size,NextPos))<Collide->Threshold);
	float t=(Collide->Threshold-V)/(NextV-V);//parametr mezi body AB
	vector_KMul(Pos,1-t);//A*(1-t)
	vector_KMul(NextPos,t);//B*(t)
	vector_Add(Pos,NextPos);//A*(1-t)+B*(t)
	collide_GetNormal(Normal,Collide,Pos);//ziskame normalu
}

float collide_ComputeDepth(float*Pos,float*Vec,SCollide*Collide){
	float ActP[3];//aktualni pozice
	float V[3];//vektor posunu
	float ValA,ValB,Max;//hodnoty hustoty a maximalni rozmer
	Max=fmax(Collide->Volume->Size.Size[0],Collide->Volume->Size.Size[1]);
	Max=fmax(Max,Collide->Volume->Size.Size[2]);//TODO stejne nebo udelat obecne do funkce
	vector_Mov(ActP,Pos);//presuneme bod
	vector_Mov(V,Vec);//presuneme vektor
	vector_Nor(V);//normalizujeme vektor
	vector_KMul(V,1/Max);
	ValA=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
			Collide->Volume->Size.Size,ActP);
	//return .5;

	if(ValA<=Collide->Threshold)return 0;
	vector_Add(ActP,V);//pricteme vektor
	while((ValB=colorbuffer_FastGetValue3D(Collide->Volume->Data[0],
					Collide->Volume->Size.Size,ActP))>Collide->Threshold){
		ValA=ValB;
		vector_Add(ActP,V);
	}
	float t=(Collide->Threshold-ValA)/(ValB-ValA);//parametr mezi body AB
	vector_KMul(V,-(1-t));//(ActP-V)*(1-t)+ActP*(t) => ActP - V*(1-t)
	vector_Add(ActP,V);//dokrocime na misto kolize
	vector_Sub(ActP,Pos);
	//return .5;
	return vector_Len(ActP);
}

