#ifndef _BOX_SWARM_H_
#define _BOX_SWARM_H_

#include"object.h"

/**
 * @brief Sablona hejna beden
 */
typedef struct{
	unsigned Seed;/// seminko
	unsigned Num;/// pocet beden
	float S[3];/// stred kruznice ve ktere se generuji bedny
	float r;/// polomer kruznice ve ktere se generuji bedny
	SRandRange Size[3];/// velikost bedny
	SGPUTexture Texture;/// textura beden
	SGPUTexture BumpTexture;/// bump mapa
	SCollide Collide;/// kolizni system
	SGPUShaderProgram*Program;/// kreslici program
}SBoxSwarmTemplate;

/**
 * @brief Inicializuje hejno beden
 *
 * @param BoxSwarm hejno beden
 * @param Template sablona beden
 */
void boxswarm_Init(SObject*BoxSwarm,SBoxSwarmTemplate Template);

#endif//_BOX_SWARM_H_
