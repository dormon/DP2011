#include"fastgetvalue.h"

float colorbuffer_FastGetValue3D(float*D,unsigned*S,float*P){
	float H[3];
	float R[3];
	unsigned II;
	int V[3]={1,S[0],S[0]*S[1]};
	unsigned B=0;
	for(unsigned d=0;d<3;++d){
		H[d]=P[d]*S[d];
		II=H[d];
		H[d]-=II;
		R[d]=1-H[d];
		B+=II*V[d];
		if(II>=S[d]-1)V[d]*=-II;
	}
	return 
		((D[B]          *R[0] + D[B+V[0]]          *H[0])*R[1]+
		 (D[B+V[1]]     *R[0] + D[B+V[0]+V[1]]     *H[0])*H[1])*R[2]+
		((D[B+V[2]]     *R[0] + D[B+V[0]+V[2]]     *H[0])*R[1]+
		 (D[B+V[1]+V[2]]*R[0] + D[B+V[0]+V[1]+V[2]]*H[0])*H[1])*H[2];
}

void colorbuffer_FastAddValue3D(float*D,unsigned*S,float*P,float Val){
	float H[3];//TODO VYTKNOUT
	float R[3];
	unsigned II;
	int V[3]={1,S[0],S[0]*S[1]};
	unsigned B=0;
	for(unsigned d=0;d<3;++d){
		H[d]=fmod(P[d],1)*S[d];
		II=H[d];
		H[d]-=II;
		R[d]=1-H[d];
		B+=II*V[d];
		if(II>=S[d]-1)V[d]*=-II;
	}
/*	D[B]								=Val*R[0]*R[1]*R[2];
	D[B+V[0]]						=Val*H[0]*R[1]*R[2];
	D[B+V[1]]						=Val*R[0]*H[1]*R[2];
	D[B+V[1]+V[0]]			=Val*H[0]*H[1]*R[2];
	D[B+V[2]]						=Val*R[0]*R[1]*H[2];
	D[B+V[2]+V[0]]			=Val*H[0]*R[1]*H[2];
	D[B+V[2]+V[1]]			=Val*R[0]*H[1]*H[2];
	D[B+V[2]+V[1]+V[0]]	=Val*H[0]*H[1]*H[2];
*/
	D[B]							 =mymath_Clamp(	D[B]								+Val*R[0]*R[1]*R[2],0,1);
	D[B+V[0]]					 =mymath_Clamp(	D[B+V[0]]						+Val*H[0]*R[1]*R[2],0,1);
	D[B+V[1]]					 =mymath_Clamp(	D[B+V[1]]						+Val*R[0]*H[1]*R[2],0,1);
	D[B+V[1]+V[0]]		 =mymath_Clamp(	D[B+V[1]+V[0]]			+Val*H[0]*H[1]*R[2],0,1);
	D[B+V[2]]					 =mymath_Clamp(	D[B+V[2]]						+Val*R[0]*R[1]*H[2],0,1);
	D[B+V[2]+V[0]]		 =mymath_Clamp(	D[B+V[2]+V[0]]			+Val*H[0]*R[1]*H[2],0,1);
	D[B+V[2]+V[1]]		 =mymath_Clamp(	D[B+V[2]+V[1]]			+Val*R[0]*H[1]*H[2],0,1);
	D[B+V[2]+V[1]+V[0]]=mymath_Clamp(	D[B+V[2]+V[1]+V[0]]	+Val*H[0]*H[1]*H[2],0,1);

}

