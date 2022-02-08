/**
 * 64kB opengl intro
 * Zdrojovy soubor pro vlastni matematiku
 * @author Tomas Milet, xmilet01
 * @file mymath.c
 * @date 23.12.2009
 * @version 0.0
 */

#include "mymath.h"

float Hold(float){
	return 0;
}

float Linear(float t){
	return t;
}

float Hermite(float t){
	return t*t*(3-2*t);
}

float FifthDegreePoly(float t){
	return t*t*t*(10+t*(-15+6*t));
}

FCEF1F1 AproxFce[]={
	Hold,
	Linear,
	Hermite,
	FifthDegreePoly,
	Linear
};

float Interpolation(float a,float b,float t){
	return a*(1-t)+b*t;//navrat interpolace
}

float RandRange(SRandRange*Range){
	return Range->Mean+Range->Deviation*(2*Range->Fce(Random(0,1))-1);
}

void CatmullRomKoef(float*C,float t1){
	float t2=t1*t1;//t^2
	float t3=t2*t1;//t^3
	C[0]=- .5*t3 +     t2 - .5*t1 + 0;
	C[1]= 1.5*t3 - 2.5*t2 +  0    + 1.;
	C[2]=-1.5*t3 +   2*t2 + .5*t1 + 0.;
	C[3]=  .5*t3 -  .5*t2 +  0    + 0.;
}

float CatmullRom(float*P,float t){
	float C[4];//koef
	CatmullRomKoef(C,t);//spocitame koef
	return vector_AMul(P,C)+P[3]*C[3];//roznasobime skalarne
}

void vector_CatmullRom(float*Res,float*A,float*B,float*C,float*D,float t){
	float K[4];//koef
	CatmullRomKoef(K,t);//spocitame koeficienty
	for(int i=0;i<3;++i){//projdeme slozky vektoru
		Res[i]=A[i]*K[0]+B[i]*K[1]+C[i]*K[2]+D[i]*K[3];//skalarni roznasobeni
	}
}

void vector_Interpolation(float*c,float*a,float*b,float t){
	c[0]=Interpolation(a[0],b[0],t);
	c[1]=Interpolation(a[1],b[1],t);
	c[2]=Interpolation(a[2],b[2],t);
}



float triangle_area_base(float a,float b,float c)
{
	//heronuv vzorec
	float s=(a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

float triangle_area(float*A,float*B,float*C)
{
	//vector stran
	float a[3],b[3],c[3];
	vector_Mov(a,B);
	vector_Sub(a,A);
	vector_Mov(b,C);
	vector_Sub(b,B);
	vector_Mov(c,A);
	vector_Sub(c,C);
	//obsah
	return triangle_area_base(vector_Len(a),vector_Len(b),vector_Len(c));
}
