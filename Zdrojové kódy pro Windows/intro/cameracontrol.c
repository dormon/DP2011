#include"cameracontrol.h"

SADTFCE CCL={/*.Free=*/NULL,/*.Copy=*/NULL,/*.Cmp=*/NULL,/*.Size=*/sizeof(float)*10};

void cameracontrol_Init(
		SCameraControl*CameraControl,
		SCam*Camera,
		float*Key,
		unsigned NumKey,
		float MaxTime,
		int Circular){
	relist_Init(&CameraControl->Key,&CCL);
	for(unsigned i=0;i<NumKey;++i)
		relist_Push(&CameraControl->Key,Key+i*10);
	CameraControl->NumKey=NumKey;
	CameraControl->Circular=Circular;
	CameraControl->Camera=Camera;
	CameraControl->MaxTime=MaxTime;
	CameraControl->Time=0;
	cameracontrol_Update(CameraControl,0);
}

void cameracontrol_GetControl(float*D,SCameraControl*CC){
	unsigned Divider=CC->Circular?CC->NumKey:CC->NumKey-1;
	float l=CC->Time/CC->MaxTime;
	unsigned Index[4];
	Index[1]=((unsigned)(l*Divider))%CC->NumKey;
	if(CC->Circular){
		Index[0]=Index[1]?Index[1]-1:CC->NumKey-1;
		Index[2]=(Index[1]+1==CC->NumKey)?0:Index[1]+1;
		Index[3]=(Index[2]+1==CC->NumKey)?0:Index[2]+1;
	}else{
		Index[0]=Index[1]?Index[1]-1:0;
		Index[2]=(Index[1]+1==CC->NumKey)?Index[1]:Index[1]+1;
		Index[3]=(Index[2]+1==CC->NumKey)?Index[2]:Index[2]+1;
	}
	/*
	for(int i=0;i<4;++i)
		fprintf(stderr,"%u ",Index[i]);
	fprintf(stderr," %f/ %f/ %f \n",CC->Time,CC->MaxTime,fmod(fmod(CC->Time,CC->MaxTime)+CC->MaxTime,CC->MaxTime));*/
	//float t=(CC->Time-CC->MaxTime*Index[1]/Divider)/(CC->MaxTime/Divider);
	float t=Divider*CC->Time/CC->MaxTime-Index[1];



	float Coef[4];//koeficienty catmullrom
	CatmullRomKoef(Coef,t);
	//float y[3],x[3],z[3];
	float*DPtr=(float*)CC->Key.Data;
	for(unsigned i=0;i<3;++i){
		D[9]=D[0+i]=D[3+i]=D[6+i]=0;
		for(unsigned k=0;k<4;++k){
			D[0+i]+=Coef[k]*DPtr[Index[k]*10+i];
			D[3+i]+=Coef[k]*DPtr[Index[k]*10+3+i];
			D[6+i]+=Coef[k]*DPtr[Index[k]*10+6+i];
			D[9]+=Coef[k]*DPtr[Index[k]*10+9];
		}
	}
	vector_Nor(D+3);
	vector_Nor(D+6);
}

void cameracontrol_Update(SCameraControl*CC,unsigned Dt){
	if(CC->NumKey<1)return;
	float dt=Dt*1e-3;
	CC->Time+=dt;
	if(CC->Circular){
		CC->Time=fmod(fmod(CC->Time,CC->MaxTime)+CC->MaxTime,CC->MaxTime);
	}else{
		if(CC->Time>CC->MaxTime)CC->Time=CC->MaxTime;
	}
	float D[10],X[3];
	cameracontrol_GetControl(D,CC);
	vector_Ortho(X,D+6,D+3);
	for(unsigned i=0;i<3;++i){
		CC->Camera->Transform.T[i]=-D[i];
		CC->Camera->Transform.R[i*4+0]=X[i];
		CC->Camera->Transform.R[i*4+1]=D[6+i];
		CC->Camera->Transform.R[i*4+2]=D[3+i];
	}
	CC->Camera->Angle=D[9];
	camera_ProjectionMatrix(CC->Camera->PM,CC->Camera->Angle,CC->Camera->Width,
			CC->Camera->Height,CC->Camera->Near,CC->Camera->Far);;
}

void cameracontrol_Free(SCameraControl*CameraControl){
	relist_Free(&CameraControl->Key);
}

void cameracontrol_Draw(SCameraControl*CC){
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(CC->Camera->PM);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float M[16];
	transform_GetModelViewMatrix(M,&CC->Camera->Transform);
	glMultMatrixf(M);
	glColor3f(1,0,0);
	glPointSize(10);

	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	for(unsigned i=0;i<16;++i){
	glActiveTexture(GL_TEXTURE0+i);
	glDisable(GL_TEXTURE_2D);
	}

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);


	float*DPtr=(float*)CC->Key.Data;
	for(unsigned i=0;i<CC->NumKey;++i){
		glBegin(GL_POINTS);
		glVertex3fv(DPtr+i*10);
		glEnd();
		glBegin(GL_LINES);
		float c=DPtr[i*10+9]/MY_PI;

		glColor3f(1-c,c,0);
		glVertex3fv(DPtr+i*10);
		vector_KMul(DPtr+i*10+3,.02/c);
		vector_Sub(DPtr+i*10,DPtr+i*10+3);
		glColor3f(c,1-c,0);
		glVertex3fv(DPtr+i*10);
		vector_Add(DPtr+i*10,DPtr+i*10+3);
		vector_KMul(DPtr+i*10+3,50*c);

		glColor3f(1,0,1);
		glVertex3fv(DPtr+i*10);
		vector_KMul(DPtr+i*10+6,.0025);
		vector_Add(DPtr+i*10,DPtr+i*10+6);
		glVertex3fv(DPtr+i*10);
		vector_Sub(DPtr+i*10,DPtr+i*10+6);
		vector_KMul(DPtr+i*10+6,400);


		glEnd();
	}
	if(!CC->NumKey)return;
	glColor3f(0,1,0);
	float D[10];
	cameracontrol_GetControl(D,CC);
	glBegin(GL_POINTS);
		glVertex3fv(D);
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(D);
		vector_KMul(D+3,.01);
		vector_Sub(D,D+3);
		glVertex3fv(D);
		vector_Sub(D,D+3);
		vector_KMul(D+3,100);
	glEnd();
	glPointSize(2);
	float ttt=CC->Time;
	glColor4f(1,1,1,1);
	glBegin(GL_POINTS);
	unsigned NumI=1000;
		for(unsigned i=0;i<NumI;++i){
			CC->Time=i*CC->MaxTime/NumI;
			cameracontrol_GetControl(D,CC);
			glVertex3fv(D);
		}
	glEnd();
	CC->Time=ttt;
}

void cameracontrol_Save(const char*File,const char*FilePath,const char*Name,SCameraControl*CC){
	char fileh[256],filec[256];
	unsigned k=0,l=0;
	k=0;
	while(FilePath[k]!='\0'){
		filec[l]=fileh[l]=FilePath[k];
		++k;
		++l;
	}
	filec[l]=fileh[l]='/';
	l++;
	k=0;
	while(File[k]!='\0'){
		filec[l]=fileh[l]=File[k];
		++k;
		++l;
	}
	filec[l]=fileh[l]='.';
	++l;
	filec[l]='c';
	fileh[l]='h';
	++l;
	filec[l]='\0';
	fileh[l]='\0';
	//fprintf(stderr,"%s : %s \n",filec,fileh);return;
	FILE*f=fopen(filec,"w");
	fprintf(f,"#include\"%s.h\"\n",File);
	fprintf(f,"const float %s[]={\n",Name);
	float*Ptr=(float*)CC->Key.Data;
	for(unsigned i=0;i<CC->NumKey-1;++i){
		for(unsigned j=0;j<10;++j)
			fprintf(f,"%f,",Ptr[i*10+j]);
		fprintf(f,"\n");
	}
	for(unsigned j=0;j<9;++j)
		fprintf(f,"%f,",Ptr[(CC->NumKey-1)*10+j]);
	fprintf(f,"%f\n};\n",Ptr[(CC->NumKey-1)*10+9]);
	fprintf(f,"const unsigned num_%s=%u;\n",Name,CC->NumKey);
	fclose(f);

	f=fopen(fileh,"w");
	fprintf(f,"#ifndef _%s_h_\n",Name);
	fprintf(f,"#define _%s_h_\n",Name);
	fprintf(f,"extern const float %s[];\n",Name);
	fprintf(f,"extern const unsigned num_%s;\n",Name);
	fprintf(f,"#endif//_%s_h_",Name);
	fclose(f);
}

