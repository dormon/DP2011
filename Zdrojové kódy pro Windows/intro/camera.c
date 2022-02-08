/**
 * 64kB opengl intro
 * zdrojovy soubor pro kameru
 * @author Tomas Milet, xmilet01
 * @file camera.c
 * @date 23.12.2009
 * @version 0.0
 */

#include "camera.h"

void camera_ProjectionMatrix(float*M,float Fovy,unsigned Width,
		unsigned Height,float Near,float Far){
	float Aspect=(float)Width/Height;
	float Right=Aspect*Near*tan(Fovy/2);
	float Left=-Right;
	float Top=Right/Aspect;
	float Bottom=Left/Aspect;
	matrix_Null(M);//vynulujeme matici

	M[0]=2*Near/(Right-Left);
	//M[4]=0;
	M[8]=(Right+Left)/(Right-Left);
	//M[12]=0
	
	//M[1]=0;
	M[5]=2*Near/(Top-Bottom);
	M[9]=(Top+Bottom)/(Top-Bottom);
	//M[13]=0;

	//M[2]=0;
	//M[6]=0;
	M[10]=-(Far+Near)/(Far-Near);
	M[14]=-2*Far*Near/(Far-Near);

	//M[3]=0;
	//M[7]=0;
	M[11]=-1;
	//M[15]=0;
}

void camera_Init(SCam*C,float N,float F,float A,unsigned W,unsigned H){
	transform_Init(&C->Transform,TRANSTYPE_TRS);
	C->Far=F;
	C->Near=N;
	C->Angle=A;
	C->Height=H;
	C->Width=W;
	camera_ProjectionMatrix(C->PM,C->Angle,C->Width,C->Height,C->Near,C->Far);
}

