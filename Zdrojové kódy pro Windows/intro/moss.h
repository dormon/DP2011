#ifndef _MOSS_H_
#define _MOSS_H_

#include"gpu.h"
#include"glslinit.h"
#include"collide.h"

/**
 * @brief Reprezentuje mech (mozna)
 */
typedef struct{
	SGPUBuffer GPUData;/// pozice castic
	SGPUAttribute Position;/// pozicni atribut
	SGPUAttribute Normal;/// normalovy atribut
	SGPUAttribute Size;/// velikostni atribut
	SGPUAttribute TTL;/// cas zivota castice
	SGPUTexture Texture;/// textura mechu
	unsigned Num;/// pocet mechu
	SGPUShaderProgram*Program;/// kreslici program
}SMoss;

/**
 * @brief Inicializuje mech
 *
 * @param Moss mech
 * @param Num pocet mechu
 * @param Collide kolizni system
 * @param Texture textura mechu
 * @param Program kreslici program
 */
void moss_Init(SMoss*Moss,unsigned Num,SCollide*Collide,SGPUTexture Texture,
		SGPUShaderProgram*Program);

/**
 * @brief Vykresli mech
 *
 * @param Moss mech
 * @param ParticleUnit texturovaci jednotka
 */
void moss_Draw(SMoss*Moss);

/**
 * @brief Uvolni mech
 *
 * @param Moss mech
 */
void moss_Free(SMoss*Moss);

#endif//_MOSS_H_
