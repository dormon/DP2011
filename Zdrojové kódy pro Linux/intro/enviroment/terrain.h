#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include"gpu.h"
#include"glslinit.h"
#include"geometry.h"

/**
 * @brief Reprezentuje sablonu terenu
 */
typedef struct{
	SGPUTexture BumpTexture;/// bumpmapa
	SGPUTexture HeightTexture;/// vyskova textura
	SGPUTexture RockTexture;/// textura skaly
	SColorBuffer Terrain;/// vyskova mapa
	float SeaHeight;/// vyska more
	float MountainHeight;/// vyska hor
	SGPUShaderProgram*Program;/// kreslici program
}STerrainTemplate;

/**
 * @brief Reprezentuje teren
 */
typedef struct{
	SGPUBuffer GPUData;/// pozice normaly, koordinaty
	SGPUBuffer GPUTriangles;/// trojuhleniky
	SGPUAttribute Position;/// pozicni atribut
	SGPUAttribute Normal;/// normala
	SGPUTexture BumpTexture;/// bump mapa
	SGPUTexture HeightTexture;/// vyskova textura
	SGPUTexture RockTexture;/// textura skal
	SGPUShaderProgram*Program;/// shader program
	float MaxHeight;/// maximalni vyska
	float SeaHeight;/// vyska more
	float MountainHeight;/// vyska hor
	float MinHeight;/// minimalni vyska
}STerrain;

/**
 * @brief Inicializuje teren
 *
 * @param Terrain teren
 * @param Template sablona terenu
 */
void terrain_Init(STerrain*Terrain,STerrainTemplate Template);

/**
 * @brief Inicializuje textury terenu
 *
 * @param BumpTexture bump mapa
 * @param Template sablona textury
 */
void terrain_InitTexture(SGPUTexture*BumpTexture,unsigned Seed,unsigned Size);

/**
 * @brief Vytvori gradientni texturu aka barvny prechod
 *
 * @param Texture textura 1D
 * @param Size velikost textury
 * @param ColorMap barevny prechod
 */
void terrain_GradientTexture(SGPUTexture*Texture,unsigned Size,SColorMap ColorMap);

/**
 * @brief Vykresli dno more
 *
 * @param Terrain dno more
 */
void terrain_Draw(STerrain*Terrain);

/**
 * @brief Uvolni dno more
 *
 * @param Terrain dno more
 */
void terrain_Free(STerrain*Terrain);

#endif//_TERRAIN_H_
