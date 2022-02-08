#ifndef _CARPET_H_
#define _CARPET_H_

#include"object.h"

/**
 * @brief Sablona koberce
 */
typedef struct{
	unsigned NumX;/// pocet dilku+1 na X
	unsigned NumY;/// pocet dilku+1 na Y
	float Translate[3];///pozice
	float Rot[3];///rotacni vektor
	float A;///uhel natoceni
	float Scale[3];///velikost
	SGPUTexture Texture;/// textura koberce
	SGPUTexture BumpTexture;/// bump mapa koberce
	SCollide Collide;/// kolizni system
	SGPUShaderProgram*Program;/// kreslici program
}SCarpetTemplate;

/**
 * @brief Vygeneruje koberec
 *
 * @param Carpet koberec
 * @param Template sablona koberce
 */
void carpet_Init(SObject*Carpet,SCarpetTemplate Template);

/**
 * @brief Inicializuje texturu
 *
 * @param Texture textura
 * @param BumpTexture bumptextura
 * @param Size velikost
 */
void carpet_InitTexture(SGPUTexture*Texture,SGPUTexture*BumpTexture,unsigned Size,unsigned C);

#endif//_CARPET_H_
