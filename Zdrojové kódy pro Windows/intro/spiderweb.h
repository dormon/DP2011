#ifndef _SPIDERWEB_H_
#define _SPIDERWEB_H_

#include"object.h"

typedef struct{
	unsigned NumMainFiber;/// pocet hlavnich vlaken (odstredu)
	unsigned NumSecFiber;/// pocet vedlejsi vlaken (letokruhy)
	unsigned NumMainFiberSplit;/// pocet clanku na letokruzich mezi hlav. v.
	unsigned NumSecFiberSplit;/// pocet clanku na hlav. v mezi letokruhy
	float Translate[3];/// pozice
	float Rot[3];///rotacni vektor
	float A;///uhel natoceni
	float Scale[3];/// meritko
	SGPUTexture Texture;/// nepouziva se
	SGPUTexture BumpTexture;/// nepouziva se
	SCollide Collide;/// kolizni system
	SGPUShaderProgram*Program;/// kreslici program
}SSpiderWebTemplate;

/**
 * @brief Vygeneruje pavucinu
 *
 * @param SpiderWeb pavucina
 * @param Template sablona pavuciny
 */
void spiderweb_Init(SObject*SpiderWeb,SSpiderWebTemplate Template);

/**
 * @brief Vykresli pavucinu
 *
 * @param SpiderWeb pavucina
 */
void spiderweb_Draw(SObject*SpiderWeb);

void spiderweb_Free(SObject*Web);

#endif//_SPIDERWEB_H_
