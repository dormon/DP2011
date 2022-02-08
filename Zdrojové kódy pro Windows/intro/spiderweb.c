#include"spiderweb.h"

void spiderweb_Init(SObject*SpiderWeb,SSpiderWebTemplate T){
	unsigned NumCirc=T.NumMainFiber+T.NumMainFiber*T.NumMainFiberSplit;
	unsigned NumLet=T.NumSecFiber+T.NumSecFiber*T.NumSecFiberSplit;
	float*PP=(float*)mymem_Alloc(sizeof(float)*3*(NumCirc*NumLet+1));
	float*CC=(float*)mymem_Alloc(sizeof(float)*2*(NumCirc*NumLet+1));
	unsigned*II=(unsigned*)mymem_Alloc(sizeof(unsigned)*2*
			(T.NumSecFiber*(NumCirc)+T.NumMainFiber*NumLet));
	float M[16];//matice soustavy
	matrix_TRSMatrix(M,T.Translate,T.Rot,T.A,T.Scale);
	for(unsigned i=0;i<NumCirc;++i){
		float Angle=2*MY_PI*i/NumCirc;//uhel vlakna
		for(unsigned j=0;j<NumLet;++j){
			float Dist=(j+1.)/NumLet;//vzdalenost od stredu
			float Pos[3];
			Pos[0]=Dist*cos(Angle);
			Pos[1]=Dist*sin(Angle);
			Pos[2]=0;
			matrix_VectorMul(PP+(i*NumLet+j)*3,M,Pos,1);
			CC[(i*NumLet+j)*2+0]=0;//coordinaty se nepouzivaji
			CC[(i*NumLet+j)*2+1]=0;
		}
	}
	matrix_VectorMul(PP+(NumCirc*NumLet)*3,M,(float*)NullVector,1);
	CC[NumCirc*NumLet*2+0]=0;
	CC[NumCirc*NumLet*2+1]=0;


	unsigned ActEdge=0;
	//propojime hlavni vlakna
	for(unsigned m=0;m<T.NumMainFiber;++m)//hlavni vlakna
		for(unsigned j=0;j<NumLet;++j){//po kouscich
			if(j<NumLet-1){
				II[ActEdge*2+0]=m*(1+T.NumMainFiberSplit)*NumLet+j;
				II[ActEdge*2+1]=m*(1+T.NumMainFiberSplit)*NumLet+j+1;
				++ActEdge;
			}else{//posledni dame ze stredu k prvnimu
				II[ActEdge*2+0]=NumCirc*NumLet;
				II[ActEdge*2+1]=m*(1+T.NumMainFiberSplit)*NumLet;
				++ActEdge;
			}
		}
	//propojime vedlejsi vlakna (letokruhy)
	for(unsigned i=0;i<NumCirc;++i)//po vsech hlavnich vlaknech
		for(unsigned l=0;l<T.NumSecFiber;++l){//po letokruzich
			II[ActEdge*2+0]=i*NumLet+l*(T.NumSecFiberSplit+1)+T.NumSecFiberSplit;
			II[ActEdge*2+1]=((i+1)%NumCirc)*NumLet+l*(T.NumSecFiberSplit+1)+T.NumSecFiberSplit;
			++ActEdge;
		}
	object_Init(SpiderWeb,T.Collide,1,PP,CC,NumCirc*NumLet+1,II,ActEdge*2,
			II,ActEdge,T.Texture,T.BumpTexture,T.Program);

	//nastavime krajni body na fixni
	for(unsigned i=0;i<T.NumMainFiber;++i)
		SpiderWeb->Elastic.Nodes
			[i*(T.NumMainFiberSplit+1)*NumLet+NumLet-1].NotFixed=0;
	elastic_SetElasticity(&SpiderWeb->Elastic,600);

	mymem_Free((void**)&PP);
	mymem_Free((void**)&CC);
	mymem_Free((void**)&II);
}

void spiderweb_Draw(SObject*Object){
	gpu_SetTextureUnit(Object->Program->TextureUnits+UNIT6_TEXTURE);
	gpu_SetTexture(&Object->Texture);
	gpu_BindBuffer(&Object->GPUDataPosition);
	gpu_SetAttribute(&Object->PositionAttribute);
	gpu_BindBuffer(&Object->GPUDataIndex);
	glDrawElements(
			GL_LINES,
			Object->GPUDataIndex.Size/sizeof(unsigned),
			GL_UNSIGNED_INT,
			NULL);
}

void spiderweb_Free(SObject*Object){
	gpu_FreeBuffer(&Object->GPUDataPosition);
	gpu_FreeBuffer(&Object->GPUDataNormal);
	gpu_FreeBuffer(&Object->GPUDataCoord);
	gpu_FreeBuffer(&Object->GPUDataIndex);
	elastic_Free(&Object->Elastic);
}

