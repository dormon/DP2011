#include"vector.h"

const float NullVector[]={0,0,0};
const float OneVector[]={1,1,1};
const float XVector[]={1,0,0};
const float YVector[]={0,1,0};
const float ZVector[]={0,0,1};
const float MaxVector[]={10e10,10e10,10e10};
float Gravity[]={0,-9.81,0};

void vector_Add(float*a,float*b){
	a[0]+=b[0];
	a[1]+=b[1];
	a[2]+=b[2];
}

void vector_Sub(float*a,float*b){
	a[0]-=b[0];
	a[1]-=b[1];
	a[2]-=b[2];
}

void vector_Mul(float*a,float*b){
	a[0]*=b[0];
	a[1]*=b[1];
	a[2]*=b[2];
}

float vector_AMul(float*a,float*b){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void vector_KMul(float*a,float K){
	a[0]*=K;
	a[1]*=K;
	a[2]*=K;
}

float vector_Len(float*a){
	return sqrt(vector_AMul(a,a));
}

void vector_Nor(float*a){
	float l=vector_Len(a);
	if(l==0)return;
	vector_KMul(a,1/l);
}

void vector_Rot(float*a){
	a[0]=1/a[0];
	a[1]=1/a[1];
	a[2]=1/a[2];
}

void vector_Ortho(float*z,float*x,float*y){
/*	z[0]=x[1]*y[2]-x[2]*y[1];
	z[1]=y[0]*x[2]-x[0]*y[2];
	z[2]=x[0]*y[1]-y[0]*x[1];*/
	vector_Cross(z,x,y);
	vector_Nor(z);
}

void vector_Cross(float*z,float*x,float*y){
	z[0]=x[1]*y[2]-x[2]*y[1];
	z[1]=y[0]*x[2]-x[0]*y[2];
	z[2]=x[0]*y[1]-y[0]*x[1];
}

void vector_Mov(float*a,float*b){
	a[0]=b[0];
	a[1]=b[1];
	a[2]=b[2];

}

int vector_Cmp(float*a){
	return (a[0]!=0)||(a[1]!=0)||(a[2]!=0);
}

void vector_Rnd(float*a){
	do{
		a[0]=Random(-1,1);
		a[1]=Random(-1,1);
		a[2]=Random(-1,1);
	}while(vector_Len(a)==0);
	vector_Nor(a);
}

void vector_LN(float*b,float*a){
	do{
		vector_Rnd(b);
	}while(vector_AMul(b,a)==vector_Len(a)*vector_Len(b));
}

float vector_Cos(float*u,float*v){
	float amul=vector_AMul(u,v);
	if(amul==0)return 0;
	return amul/vector_Len(u)/vector_Len(v);
}

void vector_Make(float*O,float*A,float*B){
	vector_Mov(O,B);
	vector_Sub(O,A);
}

void vector_Reflect(float*O,float*V,float*N){
	vector_Mov(O,N);
	vector_KMul(O,-2*vector_AMul(V,N));
	vector_Add(O,V);
}

int vector_Eq(float*A,float*B){
	return float_Eq(A[0],B[0])&&float_Eq(A[1],B[1])&&float_Eq(A[2],B[2]);
}

int vector_Index(float*S,float Size,float*X){
	int Index=0;
	for(int i=0;i<3;++i)Index|=((X[i]-S[i])>Size/2)<<i;
	return Index;
}

void vector_IndexToVector(float*V,int Index){
	for(int i=0;i<3;++i)V[i]=(Index>>i)&1;
}

void vector_CubeStart(float*NewS,float*S,float Size,int Index){
	vector_IndexToVector(NewS,Index);//spocitame osovy vektor
	vector_KMul(NewS,Size);//vynasobime polovinou velikosti
	vector_Add(NewS,S);//a pricteme puvodni zacatek
}

