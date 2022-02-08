/**
 * @author Tomas Milet, xmilet01
 * PGP projekt
 * Animace elastickeho objektu pomoci casticoveho systemu
 */

#include"elastic.h"

#define DEFAULT_K 1000.0/3.0
#define DEFAULT_WEIGHT .1
#define ELASCALE 30. //TODO nekam jinam

/**
 * @brief Spocita sily na pruzinach
 *
 * @param Elastic elasticky system
 */
void elastic_ComputeSprings(SElastic*Elastic){
	SSpring*Spring;//aktualni pruzina
	for(unsigned i=0;i<Elastic->NumSpring;++i){//projdeme pruziny
		Spring=Elastic->Springs+i;//vyber pruziny
		vector_Make(Spring->Vec,Spring->A,Spring->B);//vytvorime vektor
		vector_KMul(Spring->Vec,ELASCALE);//meritko pruziny
		float NewLen=vector_Len(Spring->Vec);//delka vektoru
		if(NewLen!=0){//pokud neni delka nula
			float Coef=(NewLen-Spring->Len*ELASCALE)/NewLen*Spring->K;//rozdil delek
			vector_KMul(Spring->Vec,Coef);//nasobime tuhosti
		}
	}
}

void elastic_ComputeAcceleration(float*A,SNode*Node){
	vector_Mov(A,(float*)Gravity);//g
	vector_KMul(A,Node->Weight);//FG=m*g
	for(unsigned s=0;s<Node->NumSpring;++s)//projdeme pruziny
		if(Node->Springs[s]->A==Node->Point)//pokud je bod A shodny s uzlem
			vector_Add(A,Node->Springs[s]->Vec);//pricteme silu
		else//jinak
			vector_Sub(A,Node->Springs[s]->Vec);//odecteme silu
	vector_KMul(A,1/Node->Weight);//ziskame zrychleni a=F/m
}

void elastic_Update(SElastic*Elastic,unsigned Dt){
	float dt=Dt*0.001;//prepocet na sekundy
	SNode*Node;//aktualni uzel
	float A[3];//zrychleni
	float V[3];//rychlost

	for(unsigned n=0;n<Elastic->NumNode;++n){//zaloh. rychlost a pozici (0)
		Node=Elastic->Nodes+n;//aktualni uzel
		vector_Mov(Node->CarryVelocity,Node->Velocity);//zaloha rychlosti
		vector_Mov(Node->CarryPoint,Node->Point);//zaloha polohy
	}

#ifdef RUNGEKUTTA
	for(unsigned k=0;k<4;++k){//projdeme k1,k2,k3,k4
#endif
		elastic_ComputeSprings(Elastic);//spocitame sily na pruzinach

		for(unsigned i=0;i<Elastic->NumNode;++i){//projdeme uzly
			Node=Elastic->Nodes+i;//aktualni uzel
			if(Node->NotFixed){//pro uzly se kterymi lze hnout

				elastic_ComputeAcceleration(A,Node);//spocitame zrychleni

				vector_KMul(A,dt);//rychlost
#ifdef RUNGEKUTTA
				vector_Mov(Node->VelocityKN[k],A);//kn

				vector_Mov(Node->Velocity,Node->VelocityKN[k]);
				vector_KMul(Node->Velocity,.5+.5*(k>=2));
				vector_Add(Node->Velocity,Node->CarryVelocity);
				vector_Mov(V,Node->Velocity);
				vector_KMul(V,.98/ELASCALE);//tlumeni

				vector_KMul(V,dt);//pozice
				vector_Mov(Node->PointKN[k],V);//kn
				vector_Mov(Node->Point,Node->PointKN[k]);
				vector_KMul(Node->Point,.5+.5*(k>=2));
				vector_Add(Node->Point,Node->CarryPoint);
#else
				vector_Add(Node->Velocity,A);//pricteme prirustek rychlosti
				vector_Mov(V,Node->Velocity);
				vector_KMul(V,.98);//tlumeni

				vector_KMul(V,1./ELASCALE);

				vector_KMul(V,dt);
				vector_Add(Node->Point,V);
#endif
				/*
					 vector_KMul(Node->Velocity,1./ELASCALE);
				//ted je cas spocitat novou pozici a vektor rychlosti podle ?kolize?
				if(collide_GetNewPositionVelocity(Node->Point,
				Node->Velocity,Dt,&Elastic->Collide))
				vector_KMul(Node->Velocity,.8);//ztraty při odrazu
				vector_KMul(Node->Velocity,ELASCALE);
				*/

			}
		}
#ifdef RUNGEKUTTA
	}
#endif

	for(unsigned i=0;i<Elastic->NumNode;++i){//projdeme uzly
		Node=Elastic->Nodes+i;//aktualni uzel
		if(Node->NotFixed){//pro uzly se kterymi lze hnout
#ifdef RUNGEKUTTA

			vector_Mov(Node->Velocity,Node->VelocityKN[1]);//k2
			vector_Add(Node->Velocity,Node->VelocityKN[2]);//k2+k3
			vector_KMul(Node->Velocity,2);//2*(k1+k2)
			vector_Add(Node->Velocity,Node->VelocityKN[0]);//k1+2*(k2+k3)
			vector_Add(Node->Velocity,Node->VelocityKN[3]);//k1+2*(k2+k3)+k4
			vector_KMul(Node->Velocity,1./6);//(k1+2*(k2+k3)+k4)/6
			vector_Add(Node->Velocity,Node->CarryVelocity);

			vector_Mov(Node->Point,Node->PointKN[1]);//k2
			vector_Add(Node->Point,Node->PointKN[2]);//k2+k3
			vector_KMul(Node->Point,2);//2*(k1+k2)
			vector_Add(Node->Point,Node->PointKN[0]);//k1+2*(k2+k3)
			vector_Add(Node->Point,Node->PointKN[3]);//k1+2*(k2+k3)+k4
			vector_KMul(Node->Point,1./6);//(k1+2*(k2+k3)+k4)/6
			vector_Add(Node->Point,Node->CarryPoint);
#endif
			vector_KMul(Node->Velocity,1./ELASCALE);
			vector_KMul(Node->CarryVelocity,.1/ELASCALE);
			collide_GetNewPosVel(Node->Point,Node->Velocity,
						Node->CarryPoint,Node->CarryVelocity,&Elastic->Collide);
			vector_KMul(Node->Velocity,ELASCALE);


/*
			vector_Make(V,Node->Point,Node->CarryPoint);
			vector_Nor(V);

			if(Node->Point[1]<-2){
				vector_Make(V,Node->CarryPoint,Node->Point);
				float t=(-2-Node->CarryPoint[1])/V[1];

				vector_KMul(Node->CarryPoint,1-t);
				vector_KMul(Node->Point,t);
				vector_Add(Node->Point,Node->CarryPoint);

				vector_KMul(Node->CarryVelocity,1-t);
				vector_KMul(Node->Velocity,t);
				vector_Add(Node->Velocity,Node->CarryVelocity);

				float N[3]={0,1,0};
				vector_Reflect(V,Node->Velocity,N);
				vector_Mov(Node->Velocity,V);
			}
*/
		}
	}


}

void elastic_Init(
		SElastic*Elastic,
		float*Points,
		unsigned NumPoint,
		unsigned*Edge,
		unsigned NumEdge,
		SCollide Collide){
	Elastic->Collide=Collide;
	Elastic->Points=Points;//predame ukazatel na body

	//ted je cas vytvorit pruziny a body
	Elastic->Nodes=(SNode*)mymem_Alloc(sizeof(SNode)*NumPoint);//alokujeme body
	Elastic->Springs=(SSpring*)mymem_Alloc(sizeof(SSpring)*NumEdge);
	Elastic->NumSpring=NumEdge;
	Elastic->NumNode=NumPoint;

	for(unsigned n=0;n<NumPoint;++n)//projdeme body
		Elastic->Nodes[n].NumSpring=0;//zatim nemame pripojene zadne pruziny

	for(unsigned e=0;e<NumEdge;++e){//projdeme hrany
		SSpring*Spring=Elastic->Springs+e;//toto je pruzina k hrane asociovana
		Spring->K=DEFAULT_K;//defaultni tuhost
		Spring->A=Points+Edge[e*2+0]*3;//ukazatel na prvni bod
		Spring->B=Points+Edge[e*2+1]*3;//ukazatel na druhy bod
		float V[3];//vektor mezi body
		vector_Make(V,Spring->A,Spring->B);//vytvorime jej
		Spring->Len=vector_Len(V);//pocatecni velikost
		Elastic->Nodes[Edge[e*2+0]].NumSpring++;//uzlu s A bodem zvysime poc. pruzin
		Elastic->Nodes[Edge[e*2+1]].NumSpring++;//to seme s B
	}

	for(unsigned p=0;p<NumPoint;++p){//projdeme body
		SNode*Node=Elastic->Nodes+p;//toto je uzel asociovany s bodem
		Node->Springs=(SSpring**)mymem_Alloc(sizeof(SSpring*)*Node->NumSpring);//alokujeme
		Node->NumSpring=0;//znovu vynulujeme pocet pruzin u uzlu
		Node->NotFixed=1;//zatim zadny uzel neni fixni
		Node->Weight=DEFAULT_WEIGHT;//defaultni hmotnost
		Node->Point=Points+p*3;//ukazatel na bod
		vector_Mov(Node->Velocity,(float*)NullVector);//nulova pocatecni rychlost
	}

	for(unsigned e=0;e<NumEdge;++e)//projdeme hrany (pruziny)
		for(unsigned p=0;p<2;++p){//projdeme body hrany
			SNode*Node=Elastic->Nodes+Edge[e*2+p];//uzel asociovany bodem
			Node->Springs[Node->NumSpring++]=Elastic->Springs+e;//pripojime pruzinu
		}

	Elastic->MaxConection=0;//maximalni pocet spoju z uzlu
	for(unsigned n=0;n<Elastic->NumNode;++n)//projdeme uzly
		if(Elastic->MaxConection<Elastic->Nodes[n].NumSpring)//vice spoju
			Elastic->MaxConection=Elastic->Nodes[n].NumSpring;//priradime vice spoju
	elastic_SetElasticity(Elastic,elastic_MaxElasticity(Elastic,10));
}

void elastic_Free(
		void*E){
	SElastic*Elastic=(SElastic*)E;//pro snazsi pristup
	for(unsigned n=0;n<Elastic->NumNode;++n)
		mymem_Free((void**)&Elastic->Nodes[n].Springs);
	mymem_Free((void**)&Elastic->Nodes);
	mymem_Free((void**)&Elastic->Springs);
}

void elastic_SetElasticity(SElastic*Elastic,float E){
	for(unsigned s=0;s<Elastic->NumSpring;++s)
		Elastic->Springs[s].K=E;
}

float elastic_MaxElasticity(SElastic*Elastic,unsigned Dt){
	/* F=k*x
	 * F=m*a
	 * a=l/t^2
	 * k*x=m*l/t^2
	 * x*(N*k*t^2/m)=l
	 */
	return 2.0/(Dt*Dt*1e-6/Elastic->Nodes[0].Weight*Elastic->MaxConection);
}

