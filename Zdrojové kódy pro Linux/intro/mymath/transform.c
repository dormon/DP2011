/**
 * 64kB opengl intro
 * zdrojovy soubor pro transformace
 * @author Tomas Milet, xmilet01
 * @file transform.c
 * @date 4.4.2010
 * @version 0.0
 */

#include "transform.h"

void transform_GetModelViewMatrix(float*M,STransform*T){
	float N[16];//pomocna matice
	matrix_Identity(M);
	enum ETransformType CurrentType;
	for(int i=0;i<3;++i){//projdeme typy transformaci
		CurrentType=(ETransformType)((((int)T->Type)>>(i*4))&0xf);//aktualni typ
		switch(CurrentType){
			case TRANSTYPE_T://translace
				matrix_Translate(N,T->T);//vytvorime translacni matici
				matrix_Mul(M,N);//vynasobime
				break;
			case TRANSTYPE_R://rotace
				matrix_Mul(M,T->R);//vynosobime rotacni matici
				break;
			case TRANSTYPE_S://meritko
				matrix_Scale(N,T->S);//vytvorime meritkovou matici
				matrix_Mul(M,N);//vynasobime
				break;
			default:break;//nic jineho
		}
	}
}

void transform_Init(STransform*T,enum ETransformType Type){
	T->Type=Type;//typ transformace
	matrix_Identity(T->R);//identita do rotacni matice
	vector_Mov(T->S,(float*)OneVector);
	vector_Mov(T->T,(float*)NullVector);
}

void transform_Scale(STransform*T,float*S){
	vector_Mov(T->S,S);
}

void transform_Translate(STransform*T,float*t){
	vector_Add(T->T,t);
}

void transform_Rotate(STransform*T,float*V,float Angle){
	float N[16];
	matrix_Rotate(N,V,Angle);
	matrix_Mul(T->R,N);
}

void transform_Mov(STransform*Out,STransform*T){
	mymem_Cpy(Out->R,T->R,sizeof(float)*16);//rotace
	vector_Mov(Out->T,T->T);//translace
	vector_Mov(Out->S,T->S);//scale
}

void transform_YXZRotate(STransform*T,float*R){
	R[0]=mymath_Clamp(R[0],-MY_PI/2,MY_PI/2);//nechceme se preklopit
	matrix_Identity(T->R);//identita

	float V[3];//vektor kolem ktereho tocime
	float RM[16];//rotacni matice

	matrix_GetCol(V,T->R,1);//Y vektor
	matrix_Rotate(RM,V,R[1]);
	matrix_Mul(RM,T->R);
	matrix_Mov(T->R,RM);

	matrix_GetCol(V,T->R,0);//X vektor
	matrix_Rotate(RM,V,R[0]);
	matrix_Mul(RM,T->R);
	matrix_Mov(T->R,RM);

	//matrix_Mul(T->R,RM);

	matrix_GetCol(V,T->R,2);//Z vektor
	matrix_Rotate(RM,V,R[2]);
	matrix_Mul(RM,T->R);
	matrix_Mov(T->R,RM);

	//matrix_Mul(T->R,RM);
}

void transform_3DRotate(STransform*T,float*R){
	float X[3];//osa X
	float Y[3];//osa Y
	float Angle=sqrt(R[0]*R[0]+R[1]*R[1]);
	if(Angle>0){//muzeme rotovat
		matrix_GetCol(X,T->R,0);//x osa
		matrix_GetCol(Y,T->R,1);//y osa
		vector_KMul(X,R[1]);//X*Ry
		vector_KMul(Y,R[0]);//Y*Rx
		vector_Add(X,Y);//X*Ry+Y*Rx
		vector_Nor(X);
		transform_Rotate(T,X,Angle);//rotujeme podel spocteneho vektoru
	}
	matrix_GetCol(X,T->R,2);//z osa
	transform_Rotate(T,X,R[2]);//rotujeme kolem osy z
}


void transform_Track(STransform*T,float*P,float*v){
	float x[3];
	float y[3];
	float z[3];
	vector_Make(z,P,T->T);
	vector_Mov(y,v);
	vector_Nor(z);
	vector_Nor(y);
	vector_Ortho(x,y,z);
	vector_Ortho(y,z,x);
	matrix_Identity(T->R);//identity
	T->R[2]=z[0];
	T->R[6]=z[1];
	T->R[10]=z[2];

	T->R[1]=y[0];
	T->R[5]=y[1];
	T->R[9]=y[2];

	T->R[0]=x[0];
	T->R[4]=x[1];
	T->R[8]=x[2];
}

void transform_Move(STransform*T,float*M){
	vector_Mov(T->T,M);
}

void transform_GoXYZ(STransform*T,int XYZ,float DeltaX){
	float V[3];
	matrix_GetCol(V,T->R,XYZ);
	vector_KMul(V,DeltaX);
	vector_Add(T->T,V);
}
