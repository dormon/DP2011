#include"carpet.h"

void carpet_Init(SObject*Carpet,SCarpetTemplate T){
	float*PP=(float*)mymem_Alloc(sizeof(float)*3*T.NumX*T.NumY);
	float*CC=(float*)mymem_Alloc(sizeof(float)*2*T.NumX*T.NumY);
	unsigned*II=(unsigned*)mymem_Alloc(sizeof(unsigned)*3*2*(T.NumX-1)*(T.NumY-1));
	unsigned*JJ;unsigned NumJJ;
	float M[16];
	matrix_TRSMatrix(M,T.Translate,T.Rot,T.A,T.Scale);
	unsigned ActT=0;

	for(unsigned y=0;y<T.NumY;++y)
		for(unsigned x=0;x<T.NumX;++x){
			unsigned Base=y*T.NumX+x;
			float Pos[3];
			CC[Base*2+0]=Pos[0]=1.*x/(T.NumX-1);
			CC[Base*2+1]=Pos[1]=1.*y/(T.NumY-1);
			Pos[2]=0;
			matrix_VectorMul(PP+Base*3,M,Pos,1);
			if(y<T.NumY-1&&x<T.NumX-1){
				II[ActT*3+0]=Base;
				II[ActT*3+1]=Base+1;
				II[ActT*3+2]=Base+T.NumX;
				ActT++;
				II[ActT*3+0]=Base+T.NumX;
				II[ActT*3+1]=Base+1;
				II[ActT*3+2]=Base+T.NumX+1;
				ActT++;
			}
		}

	geometry_ComputeJoints(&JJ,&NumJJ,T.NumX*T.NumY,II,ActT,0);

	object_Init(Carpet,T.Collide,1,PP,CC,T.NumX*T.NumY,II,ActT*3,JJ,NumJJ,
			T.Texture,T.BumpTexture,T.Program);
	mymem_Free((void**)&PP);
	mymem_Free((void**)&CC);
	mymem_Free((void**)&II);
	mymem_Free((void**)&JJ);
}

void carpet_InitTexture(SGPUTexture*Texture,SGPUTexture*BumpTexture,unsigned S,unsigned C){
	SNSize Size;
	nsize_Inita(&Size,2,S,S);
	SColorBuffer Tex0,Tex00,Tex1,Tex2,Tex3;
	void*VoroP=colorbuffer_GenVoronoiParam(&Size,0.001);
	colorbuffer_Init(&Tex0,BB_VORONOID,&Size,VoroP);
	colorbuffer_Transform(&Tex00,&Tex0,TT_WHEEZING,0,NULL);

	SColorMap Grad;
	SColorMapTemplate GradTemp={2343,10,C,10};
	colormap_Generate(&Grad,&GradTemp);
	colorbuffer_ToColor(&Tex1,&Tex00,&Grad);
	float Orange[3]={.8,.8,0};
	unsigned O=20;
	for(unsigned i=O;i<S-O;++i){
		for(int c=0;c<3;++c){
			Tex1.Data[c][i*S+O]=Orange[c];
			Tex1.Data[c][i*S+S-O-1]=Orange[c];
			Tex1.Data[c][O*S+i]=Orange[c];
			Tex1.Data[c][(S-O)*S+i]=Orange[c];
		}
	}
	colorbuffer_Normal(&Tex2,&Tex00,0);
	colorbuffer_Bump(&Tex3,&Tex2);

	gpu_InitTexture(Texture,1,0,Tex1);
	gpu_InitTexture(BumpTexture,1,0,Tex3);

	colorbuffer_Free(&Tex0);
	colorbuffer_Free(&Tex00);

	colorbuffer_Free(&Tex2);
	colormap_Free(&Grad);
	mymem_Free((void**)&VoroP);
	nsize_Free(&Size);

}

