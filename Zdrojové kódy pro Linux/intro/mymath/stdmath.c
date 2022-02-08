#include"stdmath.h"

float mymath_Clamp(float X,float Min,float Max){
	if(X<Min)return Min;
	if(X>Max)return Max;
	return X;
}

void mymath_MinMax(float*Min,float*Max,float*F,unsigned Len){
	*Min=FLOAT_MAX;
	*Max=FLOAT_MIN;
	for(unsigned i=0;i<Len;++i){
		*Min=fminf(*Min,F[i]);
		*Max=fmaxf(*Max,F[i]);
	}
}

void mymath_Normalize(float*F,unsigned Len,float Min,float Max){
	float min,max;
	mymath_MinMax(&min,&max,F,Len);
	for(unsigned i=0;i<Len;++i)F[i]=(F[i]-min)/(max-min)*(Max-Min)+Min;
}


