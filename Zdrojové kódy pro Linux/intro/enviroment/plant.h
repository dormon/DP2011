#ifndef _PLANT_H_
#define _PLANT_H_

#include"collide.h"
#include"elastic.h"
#include"gpu.h"
#include"glslinit.h"

/**
 * @brief Reprezentuje svesenou kytku
 */
typedef struct{
	SElastic Elastic;/// elasticke data
	SGPUBuffer GPUPositionData;/// pozice castic
	SGPUBuffer GPUSizeTTLData;/// velikost castic
	SGPUAttribute Position;/// pozicni atribut
	SGPUAttribute Size;/// velikosti atribut
	SGPUAttribute TTL;/// cas zivota atribut
	SGPUTexture Texture;/// textura listu
	unsigned Num;///pocet clanku kytky
	SGPUShaderProgram*Program;/// kreslici program
}SPlant;

/**
 * @brief Inicializuje kytku
 *
 * @param Plant kytka
 * @param Collide Kolizni system
 * @param Position pozice korenu
 * @param Length delka
 * @param Num pocet clanku kytky
 * @param Texture textura listu
 * @param Program kreslici program
 */
void plant_Init(SPlant*Plant,SCollide Collide,float*Position,float Length,
		unsigned Num,SGPUTexture Texture,SGPUShaderProgram*Program);

/**
 * @brief Vyresli kytku
 *
 * @param Plant kytka
 */
void plant_Draw(SPlant*Plant);

/**
 * @brief Prepocita kytku
 *
 * @param Plant kytka
 * @param Dt delta casu v milisekundach
 */
void plant_Update(SPlant*Plant,unsigned Dt);

/**
 * @brief Vytvori texturu kytky
 *
 * @param Texture textura
 * @param Size velikost
 */
void plant_InitTexture(SGPUTexture*Texture,unsigned Size);

/**
 * @brief Uvolni rostlinu
 *
 * @param Plant rostlina
 */
void plant_Free(SPlant*Plant);

#endif//_PLANT_H_
