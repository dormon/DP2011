#include"box_swarm.h"

void boxswarm_Init(SObject*BoxSwarm,SBoxSwarmTemplate T){

	float*BoxP=(float*)mymem_Alloc(sizeof(float)*3*8*T.Num);//pozice
	float*BoxC=(float*)mymem_Alloc(sizeof(float)*2*8*T.Num);//coordinaty
	unsigned*BoxI=(unsigned*)mymem_Alloc(sizeof(unsigned)*6*2*3*T.Num);//index
	unsigned*Joints=(unsigned*)mymem_Alloc(sizeof(unsigned)*8*7/2*2*T.Num);

	float PP[8*3],CC[8*2]={0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1};
	for(int i=0;i<8;++i){
		for(int b=0;b<3;++b)
			PP[i*3+b]=(i>>b)&1;
	}
	unsigned II[6*2*3]={
		0,1,3,
		0,3,2,
		4,7,5,
		4,6,7,

		0,5,1,
		0,4,5,
		2,3,7,
		2,7,6,

		0,6,4,
		0,2,6,
		1,5,7,
		1,7,3
	};

	unsigned*JJ;unsigned NumJJ;
	geometry_ComputeJoints(&JJ,&NumJJ,8,BoxI,12,1);//spoje jedne bedny
	myseed=T.Seed;
	for(unsigned i=0;i<T.Num;++i){//projdeme bedny
		float M[16];
		float Translate[3],Rotate[3],Scale[3];
		vector_Rnd(Translate);
		vector_KMul(Translate,T.r);
		vector_Add(Translate,T.S);
		vector_Rnd(Rotate);
		for(int j=0;j<3;++j)
			Scale[j]=RandRange(&T.Size[j]);
		matrix_TRSMatrix(M,Translate,Rotate,Random(0,MY_PI*2),Scale);

		//pozice
		for(unsigned p=0;p<8;++p)
			matrix_VectorMul(BoxP+(i*8+p)*3,M,PP+p*3,1);
		//koordinaty
		for(unsigned c=0;c<8;++c){
			BoxC[(i*8+c)*2+0]=CC[c*2+0];
			BoxC[(i*8+c)*2+1]=CC[c*2+1];
		}
		//indexy
		for(unsigned j=0;j<6*2*3;++j){
			BoxI[i*6*2*3+j]=II[j]+8*i;
		}
		//spoje
		for(unsigned j=0;j<8*7/2*2;++j){
			Joints[i*8*7/2*2+j]=JJ[j]+8*i;
			//fprintf(stderr,"%u ",Joints[i*8*7/2*2+j]);
		}
		//fprintf(stderr,"\n");
	}
	//fprintf(stderr,"SWARM\n");
	object_Init(BoxSwarm,T.Collide,0,BoxP,BoxC,T.Num*8,BoxI,12*T.Num*3,
			Joints,8*7/2*T.Num,T.Texture,T.BumpTexture,T.Program);
	//fprintf(stderr,"SWARM\n");

	mymem_Free((void**)&BoxP);
	mymem_Free((void**)&BoxC);
	mymem_Free((void**)&BoxI);
	mymem_Free((void**)&Joints);
	mymem_Free((void**)&JJ);

}

