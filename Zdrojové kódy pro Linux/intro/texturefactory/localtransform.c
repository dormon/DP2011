#include"localtransform.h"

float Inverse(float*Data,SIndex*Index,void*){
//	Param=Param;
	return 1-Data[index_GetIndex(Index)];
}

float Wheezing(float*Data,SIndex*Index,void*){
//	Param=Param;
	SIndex R;//druhy index
	index_Init(&R,&Index->Size);//inicializujeme
	index_Set(&R,Index);//nastavime
	unsigned RealIndex=index_GetIndex(&R);
	float cl=Data[RealIndex];
	for(int i=1;i<8;++i){
		index_Mov(&R,i*fabs(sin(i/12.0)*cl+1)*cos(cl*i),0,INDEX_REPEAT);
		index_Mov(&R,i*fabs(sin(i/12.0)*cl+1)*sin(cl*i),1,INDEX_REPEAT);
		RealIndex=index_GetIndex(&R);
		cl=Data[RealIndex];
	}
	index_Free(&R);//uvolnime index
	return cl;
}

float Flowers(float*Data,SIndex*Index,void*){
//	Param=Param;
	SIndex R;//druhy index
	index_Init(&R,&Index->Size);//inicializujeme
	index_Set(&R,Index);//nastavime
	unsigned RealIndex=index_GetIndex(&R);
	float cl=Data[RealIndex];
	for(int i=1;i<10;++i){
		float numlist=9;
		float dx,dy;

		index_Mov(&R,1,0,INDEX_REPEAT);
		dx=Data[index_GetIndex(&R)];
		index_Mov(&R,-2,0,INDEX_REPEAT);
		dx-=Data[index_GetIndex(&R)];
		index_Mov(&R,1,0,INDEX_REPEAT);

		index_Mov(&R,1,1,INDEX_REPEAT);
		dy=Data[index_GetIndex(&R)];
		index_Mov(&R,-2,1,INDEX_REPEAT);
		dy-=Data[index_GetIndex(&R)];
		index_Mov(&R,1,1,INDEX_REPEAT);

		index_Mov(&R,sin(dy*100*numlist)+sin(10*cl*i),0,INDEX_REPEAT);
		index_Mov(&R,sin(dy*100*numlist)+cos(10*cl*i),1,INDEX_REPEAT);
		cl=Data[index_GetIndex(&R)];
	}
	index_Free(&R);//uvolnime index
	return cl;
}

float Scrap(float*Data,SIndex*Index,void*){
	//Param=Param;
	SIndex R;//druhy index
	index_Init(&R,&Index->Size);//inicializujeme
	index_Set(&R,Index);//nastavime
	unsigned RealIndex=index_GetIndex(&R);
	float cl=Data[RealIndex];
	for(int i=1;i<10;++i){

		index_Mov(&R,cl*12*sin(i-5)*(i-5)*sin((1.0/i)*cl*MY_PI*2),0,INDEX_REPEAT);
		index_Mov(&R,cl*12*sin(i-5)*(i-5)*cos((1.0/i)*cl*MY_PI*2),1,INDEX_REPEAT);
		float z;
		index_Mov(&R,-10,0,INDEX_REPEAT);
		index_Mov(&R,-10,1,INDEX_REPEAT);
		z=Data[index_GetIndex(&R)];
		index_Mov(&R,20,0,INDEX_REPEAT);
		index_Mov(&R,20,1,INDEX_REPEAT);
		z-=Data[index_GetIndex(&R)];
		index_Mov(&R,-10,0,INDEX_REPEAT);
		index_Mov(&R,-10,1,INDEX_REPEAT);
		z=fabs(z);
		cl=z;
	}
	index_Free(&R);//uvolnime index
	return cl;
}

float Threshold(float*Data,SIndex*Index,void*Param){
	if(Data[index_GetIndex(Index)]>*((float*)Param))return 1;
	return 0;
}

const LOCALTRANSFORMFCE LocalTransformFce[]={
	Inverse,
	// pod toto pridavame
	
	Flowers,
	Scrap,
	Threshold,

	// nad toto pridavame
	Wheezing
};


