#ifndef _COLLIDE_H_
#define _COLLIDE_H_

#include"colorbuffer.h"

/**
 * @brief Reprezentuje system pro detekci kolizi
 */
typedef struct{
	SColorBuffer*Volume;/// objem
	SColorBuffer*Normal;/// normala objemu
	float Threshold;
}SCollide;


/**
 * @brief Inicializuje kolizni system
 *
 * @param Collide kolizni system
 * @param Volume objem
 * @param Normal normaly  objemu
 * @param Threshold prah Vzduch/Skala
 */
void collide_Init(
		SCollide*Collide,
		SColorBuffer*Volume,
		SColorBuffer*Normal,
		float Threshold);

/**
 * @brief Zjisti zda je bod v kolizi
 *
 * @param Collide kolizni system
 * @param Position pozice v objemu
 *
 * @return vraci 1 pri kolizi jinak 0
 */
int collide_GetVolume(SCollide*Collide,float*Position);

/**
 * @brief Ziska normalu v danem bode
 *
 * @param Normal normala
 * @param Collide kolizni system
 * @param Position pozice
 */
void collide_GetNormal(float*Normal,SCollide*Collide,float*Position);

/**
 * @brief Vypocita novou pozici a rychlost bodu
 *
 * @param Pos pozice (vstup i vystup)
 * @param Vel rychlost (vstup i vystup)
 * @param Dt delta casu
 * @param Collide kolizni system
 *
 * @return vraci 1 pri kolizi jinak 0
 */
int collide_GetNewPositionVelocity(float*Pos,float*Vel,unsigned Dt,
		SCollide*Collide);

/**
 * @brief Spocita novou pozici a rychlost pri kolizi
 *
 * @param NewPos I/O je nova pozice pro kterou zjistujeme kolizi
 * @param NewVel I/0 je nova rychlost pro kterou zjistujeme kolizi
 * @param OldPos predchazejici pozice
 * @param OldVel predchazejici rychlost
 * @param Collide kolizni system
 *
 * @return vraci 1 pri kolizi jinak 0
 */
int collide_GetNewPosVel(float*NewPos,float*NewVel,
		float*OldPos,float*OldVel,SCollide*Collide);

/**
 * @brief Ziska bod na hrane jeskyne a jeho normalu smerujici do jeskyne
 *
 * @param Pos bod na hrane
 * @param Normal normala
 * @param Collide kolizni system
 */
void collide_GetEdgePoint(float*Pos,float*Normal,SCollide*Collide);

/**
 * @brief Spocita hloubku danym smerem
 *
 * @param Pos pocatecni pozice
 * @param Vec smer
 * @param Collide kolizni system
 *
 * @return vraci hloubku
 */
float collide_ComputeDepth(float*Pos,float*Vec,SCollide*Collide);

#endif//_COLLIDE_H_
