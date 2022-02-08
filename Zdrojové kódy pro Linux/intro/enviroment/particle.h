#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"gpu.h"
#include"glslinit.h"
#include"collide.h"
#include"mymath.h"

/**
 * @brief Reprezentuje casticovy system
 *
 */
typedef struct{
	unsigned Num;/// pocet castic
	float Emitor[3];/// pozice emitoru castic
	float MaxTTL;/// maximalni delka zivota
	float*Velocity;/// rychlost castic
	float Damping;/// tlumeni pri narazu
	float IsSprite;/// znaci pokud se ploska nataci ke kamere
	SRandRange Size;/// velikost
	SRandRange Speed;/// velikost pocatecni rychlosti castic
	SRandRange BraunSpeed;/// braunova rychlost
	SGPUBuffer GPUDataPosition;///data pozice
	SGPUBuffer GPUDataNormal;/// data normal
	SGPUBuffer GPUDataSize;/// data velikosti
	SGPUBuffer GPUDataTTL;/// data casu zivota
	SGPUAttribute PositionAttribute;/// pozicni atribut
	SGPUAttribute NormalAttribute;/// normala castice
	SGPUAttribute SizeAttribute;/// velikostni atribut
	SGPUAttribute TTLAttribute;/// cas zivota atribut
	SGPUTexture Particle;/// textura casti
	SCollide Collide;/// kolizni system
	SGPUShaderProgram*Program;/// kreslici program
}SParticleSystem;

/**
 * @brief Inicializuje casticovy system
 *
 * @param ParticleSystem casticovy system
 * @param Num pocet castic
 * @param Emitor pozice emitoru
 * @param Damping tlumeni pri narazu
 * @param MaxTTL maximalni delka zivota castic v minisekundach
 * @param Size velikost castic
 * @param Speed velikost pocatecni rychlosti castic
 * @param BraunSpeed velikost braunovy rychlosti pro castici
 * @param Particle textura castic
 * @param ParticleUnit texturovaci jednotka castic
 * @param Collide kolizni system
 */
void particlesystem_Init(
		SParticleSystem*ParticleSystem,
		unsigned Num,
		float*Emitor,
		float Damping,
		unsigned MaxTTL,
		SRandRange Size,
		SRandRange Speed,
		SRandRange BraunSpeed,
		SGPUTexture Particle,
		SCollide Collide,
		SGPUShaderProgram*Program);

/**
 * @brief Inicializuje castici
 *
 * @param ParticleSystem casticovy system
 * @param Particle cislo castice
 * @param TTL cas zivota 1 - maximalni zivot 0 minimalni zivot
 */
void particlesystem_InitParticle(
		SParticleSystem*ParticleSystem,
		unsigned Particle,
		float TTL);

/**
 * @brief Vykresli castice
 *
 * @param ParticleSystem casticovy system
 */
void particlesystem_Draw(SParticleSystem*ParticleSystem);

/**
 * @brief Prepocita casticovy system
 *
 * @param ParticleSystem casticovy system
 * @param Dt delta castu v milisekundach
 */
void particlesystem_Update(SParticleSystem*ParticleSystem,unsigned Dt);

/**
 * @brief Uvolni casticovy system
 *
 * @param ParticleSystem casticovy system
 */
void particlesystem_Free(SParticleSystem*ParticleSystem);

/**
 * @brief Zanecha mokrou stopu v texture
 *
 * @param Wet mokra textura
 * @param PS casticovy system
 * @param Stride kolikatou castici omokrit mokrou texturu
 * @param Val zapisovana hodnota
 */
void particlesystem_DrawWet(SColorBuffer Wet,SParticleSystem*PS,
		unsigned Stride,float Val);

/**
 * @brief Inicializuje texturu
 *
 * @param Texture textura
 * @param Size velikost
 */
void particlesystem_InitTexture(SGPUTexture*Texture,unsigned Size);

#endif//_PARTICLE_H_
