#ifndef _LAKE_H_
#define _LAKE_H_

#include"gpu.h"
#include"glslinit.h"

/**
 * @brief Reprezentuje jezero
 */
typedef struct{
	SGPUBuffer GPUPoints;/// pozice
	SGPUAttribute Position;/// pozicni atribut
	SGPUTexture Wave;/// textura vln
	SGPUTexture Cube;/// textura okoli
	SGPUShaderProgram*Program;/// kreslici program
}SLake;

/**
 * @brief Inicializuje jezero
 *
 * @param Lake jezero
 * @param Pos pozice stredu ctverce jezera
 * @param A velikost hrany ctverce jezera
 * @param CubeSize velikost CubeMapy
 * @param WaveSize velikost textury vln
 * @param Program kreslici program
 */
void lake_Init(SLake*Lake,float*Pos,float A,unsigned CubeSize,unsigned WaveSize,
		SGPUShaderProgram*Program);

void lake_Inita(SLake*Lake,float*Pos,float A,unsigned WaveSize,SGPUTexture CubeMap,SGPUShaderProgram*Program);

/**
 * @brief Uvolni jezero
 *
 * @param Lake jezero
 */
void lake_Free(SLake*Lake);

/**
 * @brief Vykresli jezero
 *
 * @param Lake jezero
 */
void lake_Draw(SLake*Lake);

#endif//_LAKE_H_
