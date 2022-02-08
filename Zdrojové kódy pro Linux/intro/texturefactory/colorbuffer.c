#include"colorbuffer.h"

/*
 * @brief Provede alokaci bufferu
 *
 * @param Out buffer
 * @param Channels pocet kanalu
 * @param Size rozmery v dimenzich
 */
void colorbuffer_AllocStruct(
		SColorBuffer*Out,
		unsigned Channels,
		SNSize*Size){
	nsize_Mov(&Out->Size,Size);//rozmery
	Out->Channels=Channels;//pocet kanalu
	Out->Data=(float**)mymem_Alloc(sizeof(float*)*Channels);
	Out->InterleavedData=(unsigned char*)mymem_Alloc(sizeof(unsigned char)*Channels*
			nsize_Size(Size));
}

void*colorbuffer_GenVoronoiParam(SNSize*Size,float H){
	unsigned Num=nsize_Size(Size)*H;//pocet bodu
	unsigned*Result=(unsigned*)mymem_Alloc(sizeof(unsigned)*(Num*Size->Dimension+1));
	Result[0]=Num;//pocet
	for(unsigned i=0;i<Num;++i)//projdeme body
		for(unsigned d=0;d<Size->Dimension;++d)
			Result[1+i*Size->Dimension+d]=round(Random(0,Size->Size[d]));
	return Result;//vracime vysledek
}


/**
 * @brief Alokuje data bufferu
 *
 * @param Out buffer
 *
 * @return vraci pocet pixelu na kanal
 */
unsigned colorbuffer_AllocData(SColorBuffer*Out){
	unsigned NumD=nsize_Size(&Out->Size);
	for(unsigned i=0;i<Out->Channels;++i)
		Out->Data[i]=(float*)mymem_Alloc(sizeof(float)*NumD);
	return NumD;
}

void colorbuffer_Alloc(
		SColorBuffer*Buf,
		unsigned Channels,
		SNSize*Size){
	colorbuffer_AllocStruct(Buf,Channels,Size);//alokujeme strukturu
	colorbuffer_AllocData(Buf);//alokujeme data
}


void colorbuffer_Init(
		SColorBuffer*Buf,
		enum EBaseBufferType Type,
		SNSize*Size,
		void*Parameters){
	switch(Type){
		case BB_EMPTY:
			colorbuffer_AllocStruct(Buf,
					(unsigned long)((unsigned long*)Parameters)
					,Size);
			colorbuffer_AllocData(Buf);
			break;
		case BB_MIDPOINT:
			colorbuffer_AllocStruct(Buf,1,Size);//alokace
			Buf->Data[0]=midpoint_Generate(
					&Buf->Size,
					1,//rozsah
					*(float*)Parameters);
			break;
		case BB_VORONOID:
			colorbuffer_AllocStruct(Buf,1,Size);//alokace
			Buf->Data[0]=VoronoiDiagram(
					&Buf->Size,
					((unsigned*)Parameters)+1,
					*(unsigned*)Parameters);
			break;
	}
}

void colorbuffer_Copy(SColorBuffer*Out,SColorBuffer*In){
	colorbuffer_AllocStruct(Out,In->Channels,&In->Size);
	colorbuffer_AllocData(Out);
	unsigned NumD=nsize_Size(&In->Size);
	for(unsigned c=0;c<In->Channels;++c)
		mymem_Cpy(Out->Data[c],In->Data[c],NumD*sizeof(float));
	mymem_Cpy(Out->InterleavedData,In->InterleavedData,NumD*In->Channels);
}


void colorbuffer_ToColor(
		SColorBuffer*ColorBuffer,
		SColorBuffer*BaseBuffer,
		SColorMap*Map){
	colorbuffer_AllocStruct(ColorBuffer,Map->Num,&BaseBuffer->Size);
	unsigned NumD=colorbuffer_AllocData(ColorBuffer);
	float Color[4];//barva
	for(unsigned i=0;i<NumD;++i){//projdeme pixely
		colormap_Map(Color,BaseBuffer->Data[0][i],Map);//zjistime barvu
		for(unsigned c=0;c<Map->Num;++c)//zapiseme barvu
			ColorBuffer->Data[c][i]=Color[c];
	}
}

void colorbuffer_Compose(
		SColorBuffer*Out,
		SColorBuffer**In,
		unsigned*S,
		unsigned*Channel,
		unsigned Num){
	colorbuffer_AllocStruct(Out,Num,&In[0]->Size);//alokujeme strukturu
	unsigned NumD=colorbuffer_AllocData(Out);//alokujeme data kanalu
	for(unsigned c=0;c<Num;++c)//projdeme kanaly
		mymem_Cpy(Out->Data[c],In[S[c]]->Data[Channel[c]],sizeof(float)*NumD);
}

void colorbuffer_Circle(SColorBuffer*InitBuffer,float Pow){
	SIndex Ind;//Index
	index_Init(&Ind,&InitBuffer->Size);//inicializujeme index
	do{//prevod do kompaktniho tvaru
		float r=0;//polemer
		for(unsigned i=0;i<Ind.Size.Dimension;++i){//projdeme dimenze
			float aa=(2.*Ind.Index[i]/Ind.Size.Size[i]-1);//prevod do<-1,1>
			r+=aa*aa;//mocnina
		}
		r=sqrt(r);//velikost
		r=1-pow(r,Pow);
		unsigned IIII=index_GetIndex(&Ind);
		InitBuffer->Data[0][IIII]*=r;
		InitBuffer->Data[0][IIII]=mymath_Clamp(InitBuffer->Data[0][IIII],0,1);
	}while(!index_Inc(&Ind));
	index_Free(&Ind);
}


void colorbuffer_Free(void*Buf){
	SColorBuffer*B=(SColorBuffer*)Buf;//pro snazsi pristup
	for(unsigned i=0;i<B->Channels;++i)mymem_Free((void**)&B->Data[i]);//slozky
	mymem_Free((void**)&B->Data);//uvolnime data
	mymem_Free((void**)&B->InterleavedData);
	nsize_Free(&B->Size);//uvolnime rozsah
}

void colorbuffer_KMul(SColorBuffer*Buf,float K){
	unsigned NumD=nsize_Size(&Buf->Size);//pocet bunek
	for(unsigned i=0;i<NumD;++i){
		for(unsigned c=0;c<Buf->Channels;++c)
			Buf->Data[c][i]*=K;
	}
}

void colorbuffer_Range(float*Min,float*Max,SColorBuffer*Buf,unsigned Channel){
	unsigned NumD=nsize_Size(&Buf->Size);
	*Min=1e100;
	*Max=-1e100; 
	for(unsigned i=0;i<NumD;++i){
		*Min=fmin(*Min,Buf->Data[Channel][i]);
		*Max=fmax(*Max,Buf->Data[Channel][i]);
	}
}

void colorbuffer_Mix(
		SColorBuffer*Out,
		SColorBuffer*A,
		SColorBuffer*B,
		enum EColorBufferMixType Type){

	//inicializace vystupu
	colorbuffer_AllocStruct(Out,A->Channels,&A->Size);//alokujeme buffer
	unsigned NumD=colorbuffer_AllocData(Out);//alokujeme data

	if(	Out->Channels<4&&
			(Type==MT_A_PLUS_B_BY_A_ALPHA||
			 Type==MT_A_PLUS_B_BY_B_ALPHA))
		Type=MT_ADD;//pokud neni alpha TODO mozna pryc

	for(unsigned i=0;i<NumD;++i){//projdeme pixely
		for(unsigned c=0;c<A->Channels;++c){//projdeme kanaly
			//unsigned Ind=i*Out->Channels;
			//unsigned I=Ind+c;
			switch(Type){
				case MT_ADD:
					Out->Data[c][i]=(A->Data[c][i]+B->Data[c][i])/2;
					break;
				case MT_SUB:
					Out->Data[c][i]=mymath_Clamp(A->Data[c][i]-B->Data[c][i],0,1);
					break;
				case MT_MUL:
					Out->Data[c][i]=A->Data[c][i]*B->Data[c][i];
					break;
				case MT_DIFF:
					Out->Data[c][i]=fabs(A->Data[c][i]-B->Data[c][i]);//TODO fabs
					break;
				case MT_A_PLUS_B_BY_A_ALPHA:
					Out->Data[c][i]=
						A->Data[c][i]*A->Data[3][i]+B->Data[c][i]*(1-A->Data[3][i]);
					break;
				case MT_A_PLUS_B_BY_B_ALPHA:
					Out->Data[c][i]=
						A->Data[c][i]*B->Data[3][i]+B->Data[c][i]*(1-B->Data[3][i]);
					break;
			}
		}
	}
}

void colorbuffer_Transform(
		SColorBuffer*Out,
		SColorBuffer*In,
		enum ETrasformType Type,
		unsigned Channel,
		void*Param){
	//inicializace vystupu
	colorbuffer_AllocStruct(Out,In->Channels,&In->Size);//alokujeme buffer
	unsigned NumD=colorbuffer_AllocData(Out);//alokujeme data

	//lokalni mapovani
	for(unsigned c=0;c<In->Channels;++c){//projdeme kanaly
		if(c==Channel){//tento kanal transformujeme
			if(Type<=TT_WHEEZING){
				SIndex I;//index na aktualni pixel
				index_Init(&I,&In->Size);//inicializace
				do{//projdeme body 
					Out->Data[Channel][index_GetIndex(&I)]=
						LocalTransformFce[Type](In->Data[Channel],&I,Param);
				}while(!index_Inc(&I));//dokud nejake body jsou
				index_Free(&I);//uvolneni
			}else{//globalni mapovani
				GlobalTransformFce[Type-TT_SMOOTH](Out->Data[Channel],
						In->Data[Channel],&In->Size,Param);
			}
		}else{
			mymem_Cpy(Out->Data[c],In->Data[c],
					sizeof(float)*nsize_Size(&In->Size));
		}
	}
	mymath_Normalize(Out->Data[Channel],NumD,0,1);
}

void colorbuffer_Normal(
		SColorBuffer*Out,
		SColorBuffer*A,
		unsigned Channel){
	unsigned Dimension=A->Size.Dimension;
	colorbuffer_Alloc(Out,Dimension,&A->Size);
	SIndex I;//index do dat
	index_Init(&I,&A->Size);//inicializujeme index
	float*DPtr=A->Data[Channel];//ukazazel na data
	float ValA,ValB;//okolni body
	do{
		float Suma=0;
		unsigned Ind=index_GetIndex(&I);
		for(unsigned d=0;d<Dimension;++d){
			index_Mov(&I,1,d,INDEX_REPEAT);//posuneme index dopredu
			ValB=DPtr[index_GetIndex(&I)];
			index_Mov(&I,-2,d,INDEX_REPEAT);//posuneme index dozadu
			ValA=DPtr[index_GetIndex(&I)];
			index_Mov(&I,1,d,INDEX_REPEAT);//vracime se
			float Part=ValB-ValA;
			Out->Data[d][Ind]=Part;//uhel v dane dimenzi
			Suma+=Part*Part;
		}
		Suma=sqrt(Suma);//velikost vektoru
		if(Suma!=0)
			for(unsigned d=0;d<Dimension;++d)
				Out->Data[d][Ind]=(Out->Data[d][Ind]/Suma+1)/2;//prevod do <0,1>
	}while(!index_Inc(&I));
	index_Free(&I);//uvolnime index
}

void colorbuffer_Normal2D3D(
		SColorBuffer*Out,
		SColorBuffer*A,
		unsigned Channel){
	colorbuffer_Alloc(Out,3,&A->Size);
	SIndex I;//index do dat
	A->Size.Dimension--;//pro vytvoreni 2D indexu
	unsigned Dimension=A->Size.Dimension;
	index_Init(&I,&A->Size);//inicializujeme index 2D
	A->Size.Dimension++;//zpet na 3D

	float G[2];//gradient
	float*DPtr=A->Data[Channel];//ukazazel na data
	float ValA,ValB;//okolni body
	for(unsigned z=0;z<A->Size.Size[2];++z){//projdeme vrstvy
		do{
			float Suma=0;
			unsigned Ind=index_GetIndex(&I);
			unsigned OffSet=z*A->Size.Size[0]*A->Size.Size[1];//vrstvy 
			Ind+=OffSet;
			for(unsigned d=0;d<Dimension;++d){
				index_Mov(&I,1,d,INDEX_REPEAT);//posuneme index dopredu
				ValB=DPtr[index_GetIndex(&I)+OffSet];
				index_Mov(&I,-2,d,INDEX_REPEAT);//posuneme index dozadu
				ValA=DPtr[index_GetIndex(&I)+OffSet];
				index_Mov(&I,1,d,INDEX_REPEAT);//vracime se
				float Part=ValB-ValA;
				G[d]=Part;//uhel v dane dimenzi
				Suma+=Part*Part;
			}
			Suma=sqrt(Suma);//velikost vektoru
			if(Suma!=0)
				for(unsigned d=0;d<Dimension;++d)
					G[d]=(G[d]/Suma+1)/2;//prevod do <0,1>

			for(unsigned c=0;c<Out->Channels;++c)
				Out->Data[c][Ind]=1;

			for(unsigned d=0;d<Dimension;++d){//projdeme uhly
				for(unsigned k=0;k<=d+1;++k){//roznasobime [cos(),..,cos(),sin()]
					float Angle=atan(G[d]*2-1);// /2;//vypocet uhlu z gradientu
					if(k>d)Out->Data[k][Ind]*=sin(Angle);//posledni prvek je sin
					else Out->Data[k][Ind]*=cos(Angle);//ostatni jsou cos
				}
			}
			for(unsigned c=0;c<Out->Channels;++c)//zapis normal
				Out->Data[c][Ind]=(Out->Data[c][Ind]+1)/2;//prevod do <0,1>

			float w=Out->Data[0][Ind];
			for(unsigned c=1;c<Out->Channels;++c)
				Out->Data[c-1][Ind]=-Out->Data[c][Ind];
			Out->Data[Out->Channels-1][Ind]=w;


		}while(!index_Inc(&I));
	}
	index_Free(&I);//uvolnime index

}

void colorbuffer_Bump(
		SColorBuffer*Bump,
		SColorBuffer*In){
	colorbuffer_Alloc(Bump,In->Channels+1,&In->Size);
	SIndex I;//index do dat
	index_Init(&I,&In->Size);//inicializujeme index
	do{
		unsigned Ind=index_GetIndex(&I);
		for(unsigned c=0;c<Bump->Channels;++c)
			Bump->Data[c][Ind]=1;
		for(unsigned d=0;d<In->Channels;++d){//projdeme uhly
			for(unsigned k=0;k<=d+1;++k){//roznasobime [cos(),..,cos(),sin()]
				float Angle=atan(In->Data[d][Ind]*2-1);// /2;//vypocet uhlu z gradientu
				if(k>d)Bump->Data[k][Ind]*=sin(Angle);//posledni prvek je sin
				else Bump->Data[k][Ind]*=cos(Angle);//ostatni jsou cos
			}
		}
		for(unsigned c=0;c<Bump->Channels;++c)//zapis normal
			Bump->Data[c][Ind]=(Bump->Data[c][Ind]+1)/2;//prevod do <0,1>

		float w=Bump->Data[0][Ind];
		for(unsigned c=1;c<Bump->Channels;++c)
			Bump->Data[c-1][Ind]=-Bump->Data[c][Ind];
		Bump->Data[Bump->Channels-1][Ind]=w;

	}while(!index_Inc(&I));
	index_Free(&I);//uvolnime index
}
/*
GLuint colorbuffer_ToTarget(unsigned N){
	GLuint Tex[]={GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D};
}
*/
void colorbuffer_Update(SColorBuffer*Out){
	unsigned NumD=nsize_Size(&Out->Size);
	for(unsigned i=0;i<NumD;++i)//prolozime pole
		for(unsigned c=0;c<Out->Channels;++c)
			Out->InterleavedData[i*Out->Channels+c]=Out->Data[c][i]*255;
}

float colorbuffer_GetValueRec(float*Data,SIndex*II,float*t,unsigned D){
	if(D==0){
		return Data[index_GetIndex(II)];
	}else{
		--D;
		float a=colorbuffer_GetValueRec(Data,II,t+1,D);
		index_Mov(II,1,D,INDEX_REPEAT);
		float b=colorbuffer_GetValueRec(Data,II,t+1,D);
		index_Mov(II,-1,D,INDEX_REPEAT);
		return Interpolation(a,b,t[0]);
	}
}

float colorbuffer_GetValue(SColorBuffer*B,float*P,unsigned C){
	SIndex Index;
	index_Init(&Index,&B->Size);
	float H[3];
	for(unsigned d=0;d<B->Size.Dimension;++d){
		H[d]=fmod(fmod(P[d],1)+1,1);
		H[d]*=B->Size.Size[d];
		Index.Index[d]=H[d];//orezeme desetinnou cast
		H[d]-=Index.Index[d];
	}
	for(unsigned d=0;d<B->Size.Dimension/2;++d){
		float z=H[d];
		H[d]=H[B->Size.Dimension-1-d];
		H[B->Size.Dimension-1-d]=z;
	}
	float Result=colorbuffer_GetValueRec(B->Data[C],&Index,H,B->Size.Dimension);
	//Result=B->Data[C][index_GetIndex(&Index)];
	index_Free(&Index);
	return Result;
}
