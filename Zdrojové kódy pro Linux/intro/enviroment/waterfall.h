#ifndef _WATERFALL_H_
#define _WATERFALL_H_

#include"gpu.h"
#include"collide.h"

/**
 * @brief Reprezentuje vodopad
 */
typedef struct{
	float Emitor[3];/// pozice emitoru castic
	SRandRange Size;/// velikost
	float MaxTTL;/// maximalni delka zivota
	SGPUBuffer GPUData;/// data castice
	SGPUAttribute PositionAttribute;/// pozicni atribut
	SGPUAttribute SizeAttribute;/// velikostni atribut
	SGPUAttribute TTLAttribute;/// cas zivota atribut
	float*Velocity;/// rychlost castic
	SGPUTexture Particle;/// textura casti
	SGPUTextureUnit ParticleUnit;/// texturovaci jednotka
	GLuint MaxTTLUniform;/// uniform maximalniho casu zivota
	unsigned Num;/// pocet castic
	SCollide Collide;/// kolizni system
	float MinHeight;/// minimalni vyska pod kterou castice zanikaji
}SWaterFall;

/**
 * @brief Inicializuje vodopad
 *
 * @param Waterfall vodopad
 * @param Emitor emitor (pramen)
 * @param Texture textura
 * @param TextureUnit texturovaci jednotka
 * @param Num poce castic
 * @param Volume objem pro koliye
 * @param Normal onrmaly povrchu
 * @param Threshold prah pri kterem je objem
 * @param PositionAttribute pozicni atribut
 * @param SizeAttribute velikostni atribut
 * @param TTLAttribute cas zivota atribut
 * @param MaxTTLUniform uniform maximalniho casu zivota
 * @param Size Velikost
 * @param MinHeight minimalni vyska castic, pak zaniknou a obnovi se
 * @param MaxTTL maximalni delka zivota
 */
void waterfall_Init(
		SWaterFall*Waterfall,
		float*Emitor,
		SGPUTexture Particle,
		SGPUTextureUnit ParticleUnit,
		unsigned Num,
		SCollide CollideAttribute,
		GLuint PositionAtribute,
		GLuint SizeAttribute,
		GLuint TTLAttribute,
		GLuint MaxTTLUniform,
		SRandRange Size,
		float MinHeight,
		unsigned MaxTTL);

/**
 * @brief Aktualizuje vodopad
 *
 * @param Fall vodopad
 * @param Dt rozdil casu v milisekundach
 */
void waterfall_Update(SWaterFall*Fall,unsigned Dt);


/**
 * @brief Vykresli vodopad
 *
 * @param Fall vodopad
 */
void waterfall_Draw(SWaterFall*Fall);

/**
 * @brief Inicializuje castici
 *
 * @param W vodopad
 * @param Ptr ukazatel na pozici a velikost
 * @param p cislo castice
 * @param TTL 1 znaci maximalni zivot, 0 minimalni zivot
 */
void waterfall_InitParticle(SWaterFall*W,unsigned p,float TTL);

#endif//_WATERFALL_H_
