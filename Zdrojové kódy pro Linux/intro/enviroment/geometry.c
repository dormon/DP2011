#include"geometry.h"

void geometry_ComputeNormal(
		float*Normal,
		unsigned NormalStride,
		float*Points,
		unsigned PointStride,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles){
	for(unsigned i=0;i<NumPoints;++i)//vynulujeme
		vector_Mov(Normal+i*NormalStride,(float*)NullVector);
	for(unsigned t=0;t<NumTriangles;++t){//projdeme trojuhelniky
		float A[3];
		float B[3];
		float C[3];
		float xx[3];
		float yy[3];
		float kk[3];

		unsigned AI=Triangles[t*3+0];
		unsigned BI=Triangles[t*3+1];
		unsigned CI=Triangles[t*3+2];

		vector_Mov(A,Points+AI*PointStride);
		vector_Mov(B,Points+BI*PointStride);
		vector_Mov(C,Points+CI*PointStride);

		vector_Make(xx,A,B);
		vector_Nor(xx);
		vector_Make(yy,A,C);
		vector_Nor(yy);
		vector_Ortho(kk,xx,yy);

		vector_Nor(kk);

		vector_Add(Normal+AI*NormalStride,kk);
		vector_Add(Normal+BI*NormalStride,kk);
		vector_Add(Normal+CI*NormalStride,kk);
	}

	for(unsigned i=0;i<NumPoints;++i){
		vector_Nor(Normal+i*NormalStride);
		vector_KMul(Normal+i*NormalStride,-1);
	}
}

void geometry_ComputeJoints(
		unsigned**Joints,
		unsigned*NumJoints,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles,
		unsigned Full){
	if(Full){
		*NumJoints=NumPoints*(NumPoints-1)/2;
		*Joints=(unsigned*)mymem_Alloc(sizeof(unsigned)*(*NumJoints)*2);
		*NumJoints=0;
		for(unsigned i=0;i<NumPoints;++i)
			for(unsigned j=i+1;j<NumPoints;++j){
				(*Joints)[(*NumJoints)*2+0]=i;
				(*Joints)[(*NumJoints)*2+1]=j;
				++(*NumJoints);
			}
	}else{
		unsigned*NumTrianglesPerPoint=//pocet troj. na bod
			(unsigned*)mymem_Alloc(sizeof(unsigned)*NumPoints);
		unsigned*NumEdgePerPoint=// pocet hran na bod
			(unsigned*)mymem_Alloc(sizeof(unsigned)*NumPoints);
		unsigned**PointToTriangle=//bod na trojuhelniky
			(unsigned**)mymem_Alloc(sizeof(unsigned*)*NumPoints);
		unsigned**PointToEdge=//bod na hrany
			(unsigned**)mymem_Alloc(sizeof(unsigned*)*NumPoints);

		//inicializujeme pocet trojuhelniku na bod
		for(unsigned p=0;p<NumPoints;++p){//projdeme body
			NumTrianglesPerPoint[p]=0;
			NumEdgePerPoint[p]=0;
		}
		//spocitame pocet trojuhelniku na bod
		for(unsigned t=0;t<NumTriangles;++t)//projdeme pres trojuhelniky
			for(int k=0;k<3;++k)//body troj.
				NumTrianglesPerPoint[Triangles[t*3+k]]++;//citac
		//inicializujeme trojuhelniky na body
		for(unsigned p=0;p<NumPoints;++p){//projdeme body
			PointToTriangle[p]=
				(unsigned*)mymem_Alloc(sizeof(unsigned)*NumTrianglesPerPoint[p]);
			PointToEdge[p]=
				(unsigned*)mymem_Alloc(sizeof(unsigned)*(NumTrianglesPerPoint[p]*2));
			NumTrianglesPerPoint[p]=0;
		}
		//prida cislo trojuhelniku bodum
		for(unsigned t=0;t<NumTriangles;++t){//projdeme trojuhelniky
			for(int k=0;k<3;++k){//body na trojuhelniku
				unsigned Point=Triangles[t*3+k];//bod
				PointToTriangle[Point][NumTrianglesPerPoint[Point]++]=t;
			}
		}
		//prida cisla sousednich bodu
		for(unsigned p=0;p<NumPoints;++p){//projdeme body
			for(unsigned t=0;t<NumTrianglesPerPoint[p];++t){//jejich trojhuhelniky
				for(int k=0;k<3;++k){//body trojuhelniku
					unsigned q=Triangles[PointToTriangle[p][t]*3+k];//bod q
					if(q==p)continue;//stejny neberem
					int Uniq=1;//je hrana unikatni v bodu
					for(unsigned e=0;e<NumEdgePerPoint[p];++e)
						if(PointToEdge[p][e]==q){//tento uz tam je
							Uniq=0;//neni unikatni
							break;//konec testovani
						}
					if(Uniq)PointToEdge[p][NumEdgePerPoint[p]++]=q;//zapiseme a zvysime
				}
			}
		}
		*NumJoints=0;
		for(unsigned p=0;p<NumPoints;++p){//body
			for(unsigned e=0;e<NumEdgePerPoint[p];++e){//hrany
				unsigned q=PointToEdge[p][e];//druhy pod hrany
				if(q>p){//spoj
					(*NumJoints)++;
				}
			}
		}
		*Joints=(unsigned*)mymem_Alloc(sizeof(unsigned)*(*NumJoints)*2);
		*NumJoints=0;
		for(unsigned p=0;p<NumPoints;++p){//body
			for(unsigned e=0;e<NumEdgePerPoint[p];++e){//hrany
				unsigned q=PointToEdge[p][e];//druhy pod hrany
				if(q>p){//spoj
					(*Joints)[(*NumJoints)*2+0]=p;
					(*Joints)[(*NumJoints)*2+1]=q;
					(*NumJoints)++;
				}
			}
		}
		for(unsigned p=0;p<NumPoints;++p){
			mymem_Free((void**)&PointToTriangle[p]);
			mymem_Free((void**)&PointToEdge[p]);
		}
		mymem_Free((void**)&NumTrianglesPerPoint);
		mymem_Free((void**)&NumEdgePerPoint);
		mymem_Free((void**)&PointToTriangle);
		mymem_Free((void**)&PointToEdge);
	}
}


