#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include"collide.h"

typedef void(*PHYSICS_UPDATE)(void*Fce,void*Data,unsigned Dt);

/**
 * @brief Reprezentuje fyzikalni system
 */
typedef struct{
	float Gravity[3];/// gravitace
	SCollide Collide;/// kolizni system
	float*Velocity;/// data rychlosti
	float*Position;/// ukazatel na pozice
	void*PhyData;/// data pro vypocet pohybu (elasticky system,...)
	PHYSICS_UPDATE Update;/// aktualizacni funkce
}SPhysicsSystem;

/**
 * @brief Prepocita pozice
 *
 * @param Physics fyzikalni system
 * @param Dt delta casu
 */
void physics_Update(SPhysicsSystem*Physics,unsigned Dt);

#endif//_PHYSICS_H_
