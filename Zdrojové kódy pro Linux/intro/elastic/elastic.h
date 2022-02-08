/**
 * @author Tomas Milet, xmilet01
 * PGP projekt
 * Animace elastickeho objektu pomoci casticoveho systemu
 */

#ifndef _ELASTIC_H_
#define _ELASTIC_H_

#include"vector.h"
#include"mymem.h"
#include"collide.h"

/**
 * @brief Pokud budeme kompilovat s RUNGEKUTTOU
 */
#define RUNGEKUTTA

/**
 * @brief Pruzina
 */
typedef struct{
	float*A,*B;/// pruzina je nadazena mezi body A a B
	float K;/// koeficient tuhosti
	float Len;/// delka pruziny
	float Vec[3];/// vektor sily spoje
}SSpring;

/**
 * @brief Bod
 */
typedef struct{
	SSpring**Springs;/// pruziny spojene s timto bodem
	unsigned NumSpring;/// pocet pruzin
	float Velocity[3];/// vysledna rychlost
	int NotFixed;/// bod neni fixni a jda s nim hnout
	float Weight;/// hmotnost bodu
	float*Point;/// pozice bodu
	float CarryVelocity[3];/// zaloha rychlosti
	float CarryPoint[3];/// zaloha bodu
#ifdef RUNGEKUTTA
	float VelocityKN[4][3];/// k1,k2,k3,k4
	float PointKN[4][3];/// k1,k2,k3,k4
#endif
}SNode;

/**
 * @brief Elasticka data objektu
 */
typedef struct{
	SNode*Nodes;/// uzly
	SSpring*Springs;/// pruziny
	SCollide Collide;/// kolizni system
	unsigned NumNode;/// pocet uzlu
	unsigned NumSpring;/// pocet pruzin
	float*Points;/// body
	unsigned MaxConection;/// maximalni pocet propoju
}SElastic;

/**
 * @brief Prepocita elasticky system
 *
 * @param Elastic elasticky system
 * @param Dt delta casu v milisekundach
 */
void elastic_Update(SElastic*Elastic,unsigned Dt);

/**
 * @brief Vytvori elsticka data z geometrie
 *
 * @param Elastic elasticka data
 * @param Points body geometrie
 * @param NumPoint pocet bodu
 * @param Edges hrany geometrie
 * @param NumEdge pocet hran
 */
void elastic_Init(SElastic*Elastic,float*Points,unsigned NumPoint,
		unsigned*Edges,unsigned NumEdge,SCollide Collide);

/**
 * @brief Nastavi vsem bodum elasticitu
 *
 * @param Elastic alsticka data
 * @param E tuhost pruzin
 */
void elastic_SetElasticity(SElastic*Elastic,float E);

/**
 * @brief Zjisteni maximalni tuhosti pruzin
 *
 * @param Elastic elasticka data
 * @param Dt delta casu
 *
 * @return maximalni tuhost
 */
float elastic_MaxElasticity(SElastic*Elastic,unsigned Dt);

/**
 * @brief Uvolni elasticke data
 *
 * @param Elastic elasticke data
 */
void elastic_Free(void*Elastic);

#endif//_ELASTIC_H_
