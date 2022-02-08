#include"matrix.h"

void matrix_Null(float*A){
	for(int i=0;i<16;++i)A[i]=0;
}

void matrix_Identity(float*A){
	for(int i=0;i<16;++i)A[i]=(i%4)==(i/4);
}

void matrix_Translate(float*M,float*V){
	matrix_Identity(M);//idenity
	vector_Mov(M+12,V);//zapiseme posun
}

void matrix_Scale(float*M,float*S){
	matrix_Identity(M);//identity
	M[0]=S[0];//meritko
	M[5]=S[1];
	M[10]=S[2];
}

void matrix_Rotate(float*M,float*RV,float a){
	float ca=cos(a);
	float mca=1-ca;
	float sa=sin(a);
	matrix_Identity(M);
	M[0]=RV[0]*RV[0]*mca+ca;
	M[1]=RV[0]*RV[1]*mca-RV[2]*sa;
	M[2]=RV[0]*RV[2]*mca+RV[1]*sa;
//	M[3]=0;

	M[4]=RV[1]*RV[0]*mca+RV[2]*sa;
	M[5]=RV[1]*RV[1]*mca+ca;
	M[6]=RV[1]*RV[2]*mca-RV[0]*sa;
//	M[7]=0;

	M[8]=RV[0]*RV[2]*mca-RV[1]*sa;
	M[9]=RV[1]*RV[2]*mca+RV[0]*sa;
	M[10]=RV[2]*RV[2]*mca+ca;
//	M[11]=0;

//	M[12]=0;
//	M[13]=0;
//	M[14]=0;
//	M[15]=1;
}

void matrix_Mov(float*A,float*B){
	for(int i=0;i<16;++i)A[i]=B[i];
}

void matrix_Add(float*A,float*B){
	for(int i=0;i<16;++i)A[i]+=B[i];
}

void matrix_Sub(float*A,float*B){
	for(int i=0;i<16;++i)A[i]-=B[i];
}

void matrix_Mul(float*A,float*B){
	float C[16];
	matrix_Null(C);//vynulujeme vystup
	for(int i=0;i<16;++i)
		for(int j=0;j<4;++j)
			C[i]+=A[(i&0xc)+j]*B[i%4+j*4];
	matrix_Mov(A,C);
}

void matrix_KMul(float*A,float K){
	for(int i=0;i<16;++i)A[i]*=K;
}

void matrix_GetRow(float*V,float*M,int i){
	vector_Mov(V,M+i*4);//radek
}

void matrix_GetCol(float*V,float*M,int i){
	V[0]=M[i];//sloupec
	V[1]=M[i+4];
	V[2]=M[i+8];
}

void matrix_SetRow(float*M,float*V,int i){
	vector_Mov(M+i*4,V);//radek
}

void matrix_SetCol(float*M,float*V,int i){
	M[i]=V[0];//sloupec
	M[i+4]=V[1];
	M[i+8]=V[2];
}

void matrix_Transpose(float*M){
	float C;//pomocna promenna pro swap
	for(int i=1;i<4;++i)
	for(int j=0;j<i;++j){
		C=M[j*4+i];
		M[j*4+i]=M[i*4+j];
		M[i*4+j]=C;
	}
}

void matrix_VectorMul(float*O,float*M,float*V,float W){
	for(unsigned i=0;i<3;++i){
		O[i]=M[i*4+3]*W;
		for(unsigned j=0;j<3;++j)
			O[i]+=M[i*4+j]*V[j];
	}
}

void matrix_TRSMatrix(float*M,float*T,float*R,float A,float*S){
	float N[16];
	matrix_Identity(M);

	matrix_Translate(N,T);
	matrix_Transpose(N);
	matrix_Mul(M,N);//T

	matrix_Rotate(N,R,A);
	matrix_Mul(M,N);//R

	matrix_Scale(N,S);
	matrix_Mul(M,N);//S
}

