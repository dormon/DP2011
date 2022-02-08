#include"skyboxgenerate.h"

void sky_GetColor(float*Color,float*Vector,void*Template){
	SSkyTemplate*T=(SSkyTemplate*)Template;
	vector_Nor(T->SunVector);
	float A[3];
	vector_Make(A,T->SunVector,Vector);
	float sx=T->SunVector[0],sz=T->SunVector[2];//xz slozky
	float vx=Vector[0],vz=Vector[2];//xz slozky
	float ls=hypot(sx,sz);//velikost
	float lv=hypot(vx,vz);//velikost
	sx/=ls;sz/=ls;//normalizace
	vx/=lv;vz/=lv;//normalizace
	float u=hypot(sx-vx,sz-vz)/2;
	float f=vector_Len(A);
	float r=pow(fabs(Vector[1]),T->Exp);
	for(int i=0;i<3;++i){
		if(f<1)
			Color[i]=
				T->BaseColor[i]*(pow(f,.2))+
				T->StartColor[i]*(1-pow(f,.2));
		else
			Color[i]=
				T->BaseColor[i]*(1-pow((f-1),2))+
				T->EndColor[i]*((pow(f-1,2)));
		if(fabs(T->SunVector[1])>.7)
			Color[i]=Color[i]*r+T->HorizontColor[i]*(1-r);
		else{
			if(u<.3)
				Color[i]=Color[i]*r+T->HorizontColor[i]*(1-r);
			else{
				float g=fmaxf(0,((u-.3)/.7-.5));
				Color[i]=Color[i]*r+T->HorizontColor[i]*(1-r)*(1-g);
			}
		}
	}
	Color[3]=1;//absolutni kryti
}

void sun_GetColor(float*Color,float*Vector,void*Template){
	SSunTemplate*T=(SSunTemplate*)Template;
	vector_Mov(Color,T->SunColor);//max intezita
	vector_Nor(T->SunVector);
	float Ang=acos(vector_AMul(T->SunVector,Vector));
	if(Ang<=T->SunSize){//divame se primo do slunce
		Color[3]=1;//zadna pruhlednost
	}else{
		float r;//e^((-Bloom*angle)^Exp)
		r=pow(T->SunBloom*(1-(Ang-T->SunSize)/(MY_PI-T->SunSize)),T->SunExp);
		r=exp(-r);
		float min=exp(-pow(T->SunBloom,T->SunExp));
		float max=1;
		r=1-(r-min)/(max-min);
		Color[3]=r;
	}
}

unsigned noise_Gen(unsigned x){
	return ((x*(x*(x*(x*(x+1)+1)+1)+1)));
}

float noise_Noise(int x,int y){
	int n=x+y*57;
	n=(n<<13)^n;
	unsigned v=noise_Gen(n);
	return ((float)v/(float)(((unsigned)(-1)>>1)))-1;
}

float noise_SmoothNoise(float x,float y){
	int ix=(int)x,iy=(int)y;//cele hodnoty
	float A=noise_Noise(ix,iy);//4 body
	float B=noise_Noise(ix+1,iy);//kolem generovaneho bodu
	float C=noise_Noise(ix,iy+1);
	float D=noise_Noise(ix+1,iy+1);
	float dx=x-ix;//<0,1>
	float dy=y-iy;//<0,1>
	float SA=B*dx+A*(1-dx);//interpolace mezi B,A
	float SB=D*dx+C*(1-dx);//interpolace mezi D,C
	return SB*dy+SA*(1-dy);//interpolace mezi stredy
}

float noise_GetNoise(float x,float y,float f,float a,float p,int Num){
	float Result=0;
	for(int k=0;k<Num;++k){
		Result=Result+noise_SmoothNoise(x*f,y*f)*a;
		f*=2;//zdvojnasobime frekvenci
		a=a*p;
	}
	return Result;
}

void cloud_GetColor(float*Color,float*Vector,void*Template){
	SCloudTemplate*T=(SCloudTemplate*)Template;
	float h=100;//aktualni vyska vrstvy
	float x;//hodnota sumu
	float t;//nasobitel x,z souradnic
	float intensity;//intenzita mraku
	for(int i=0;i<4;++i)Color[i]=0;//max pruhlednost
	if(Vector[1]<=0)return;//divame se dolu
	t=h/Vector[1];//ziskame nasobitel
	x=noise_GetNoise(T->Start[0]+Vector[0]*t,T->Start[1]+Vector[2]*t,
			T->Frequency,T->Amplitude,T->Persistence,T->Num);//ziskame sum
	intensity=(x+T->Coverage)*T->Density;
	if(intensity<0)intensity=0;//podteceni
	if(intensity>1)intensity=1;//preteceni
	for(int j=0;j<3;++j)Color[j]=T->Color[j]*intensity;
	Color[3]=intensity;
	//tlumeni alphy na horizontu (aliasu)
	float ht=1;//nasobitel alfy
	float starht=.2;
	if(Vector[1]<starht)ht=pow(Vector[1]/starht,1);
	Color[3]*=ht;
	if(Color[3]<0)Color[3]=0;
	if(Color[3]>1)Color[3]=1;
}

void skybox_Render(SGPUTexture*B,SSkyBoxTemplate Template){

	unsigned S=B->Data[0].Size.Size[0];

	for(unsigned f=0;f<6;++f){
		for(unsigned y=0;y<S;++y)
			for(unsigned x=0;x<S;++x){
				float NN[3]={
					-(2.*x/S-1.)-.5/S,
					-(2.*y/S-1.)-.5/S,
					-1.
				};

				float N[3];
				vector_Nor(NN);
				switch(f){
					case 0:N[0]=+NN[2];N[1]=-NN[1];N[2]=-NN[0];break;
					case 1:N[0]=-NN[2];N[1]=-NN[1];N[2]=+NN[0];break;
					case 2:N[0]=+NN[0];N[1]=+NN[2];N[2]=+NN[1];break;
					case 3:N[0]=+NN[0];N[1]=-NN[2];N[2]=-NN[1];break;
					case 4:N[0]=+NN[0];N[1]=-NN[1];N[2]=+NN[2];break;
					case 5:N[0]=-NN[0];N[1]=-NN[1];N[2]=-NN[2];break;
				}
				float Color[3]={0,0,0};
				float ActColor[4];
				for(unsigned i=0;i<Template.NumElement;++i){//projdeme elementy
					Template.SkyBoxElement[i].GetColor(ActColor,N,//renderujeme element
							Template.SkyBoxElement[i].Template);
					for(unsigned c=0;c<3;++c)//smichani pomoci alfa
						Color[c]=Color[c]*(1-ActColor[3])+ActColor[c]*ActColor[3];
				}
				for(unsigned c=0;c<3;++c)B->Data[f].Data[c][y*S+x]=Color[c];
			}
		DrawBar();
	}
	B->DataChanged=1;
}

void skybox_Generate(SGPUTexture*SkyBox,SSkyBoxTemplate Template){
	skybox_Empty(SkyBox,Template.Size);
	skybox_Render(SkyBox,Template);
}

void skybox_Empty(SGPUTexture*SkyBox,unsigned Size){
	SColorBuffer B[6];//seste bufferu
	SNSize BSize;//velikost bufferu
	nsize_Inita(&BSize,2,Size,Size);//velikost
	for(unsigned i=0;i<6;++i){//alokace bufferu
		colorbuffer_Init(B+i,BB_EMPTY,&BSize,(void*)3);
	}
	gpu_InitTexture(SkyBox,0,1,B[0],B[1],B[2],B[3],B[4],B[5]);
	nsize_Free(&BSize);
}

