#include"tunnel.h"

void tunnel_Init(SColorBuffer*Tunnel,STunnelTemplate T){
	SColorBuffer Volume0;//pomocny buffer
	SNSize VolS;//velikost bufferu
	nsize_Inita(&VolS,3,T.Size,T.Size,T.Size);//inicializace velikosti
	myseed=T.Seed;//seminko
	float VVV=2.4;colorbuffer_Init(&Volume0,BB_MIDPOINT,&VolS,&VVV);//sum
	colorbuffer_KMul(&Volume0,.5);//vynasobime .5

	SNSize ST;//delka cesty
	unsigned DNum=1000;//pocet kroku cesty
	nsize_Inita(&ST,1,DNum);//inicializujeme velikost cesty
	SColorBuffer RT;//polomer cesty
	SColorBuffer AT;//uhel natoceni cesty
	float DT=2;//delitel sumu cesty
	colorbuffer_Init(&RT,BB_MIDPOINT,&ST,&DT);//sum
	colorbuffer_Init(&AT,BB_MIDPOINT,&ST,&DT);//sum
	float Z[3]={1,1,1};//z vektor cesty
	vector_KMul(Z,1./DNum);//deleno delka cesty
	float X[3],Y[3];//x y vektory souradneho systemu cesty
	vector_Rnd(X);//nahodne x
	vector_Ortho(Y,X,Z);//kolme y
	vector_Ortho(X,Y,Z);//kolme x

	float DStart[3]={0,0,0};//zacatek cesty
	float PosPP[3]={0,0,0};//pozice
	float ActPP[3];//aktualni pozice
	vector_Mov(ActPP,DStart);//aktualni pozice je pocatek cesty
	for(unsigned i=0;i<DNum;++i){//projdeme cestu
		vector_Mov(PosPP,ActPP);//presuneme aktualni pozici
		for(int j=0;j<3;++j){//nemuzeme se dostat mimo kostku
			if(PosPP[j]>=1)PosPP[j]=1;
			if(PosPP[j]<=0)PosPP[j]=0;
		}
		colorbuffer_FastAddValue3D(Volume0.Data[0],VolS.Size,PosPP,.7);//zapiseme
		float V[3],W[3];//pomocne vektory
		vector_Mov(V,X);//v=x*r*cos(a)
		vector_KMul(V,cos(AT.Data[0][i]*2*MY_PI)*RT.Data[0][i]);
		vector_KMul(V,10./T.Size);

		vector_Mov(W,Y);//w=y*r*sin(a)
		vector_KMul(V,sin(AT.Data[0][i]*2*MY_PI)*RT.Data[0][i]);
		vector_KMul(W,10./T.Size);

		vector_Mov(ActPP,Z);
		vector_KMul(ActPP,i);
		vector_Add(ActPP,DStart);//ziskame pozici

		vector_Add(ActPP,V);//pricteme cos
		vector_Add(ActPP,W);//pricteme sin
	}
	unsigned Mask=2;//vyhlazovani
	colorbuffer_Transform(Tunnel,&Volume0,TT_SMOOTH,0,&Mask);//vyhladime cestu

	nsize_Free(&VolS);
	colorbuffer_Free(&Volume0);
	nsize_Free(&ST);
	colorbuffer_Free(&RT);
	colorbuffer_Free(&AT);
}

