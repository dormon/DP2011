#ifndef _BUBBLE_H_
#define _BUBBLE_H_

typedef struct{
	float Emitor[3];/// pozice emitoru castic
	float InitSize;/// pocatecni velikost
	float SizeDeviation;/// deviace velikosti
	float MaxTTL;/// maximalni zivodnost
	unsigned Num;/// pocet castic
	SGPUBuffer GPUData;/// data castice
	SGPUAttribute Position;/// pozicni atribut
	SGPUAttribute Size;/// velikostni atribut
	SGPUAttribute TTL;/// cas zivota atribut
	SGPUTexture Particle;/// textura castice
	SGPUTextureUnit ParticleUnit;/// texturovaci jednotka
	SCollide Collide;/// kolizni system
	float MaxHeight;/// maximalni vyska nad kterou castice zanikaji
}SBubble;

#endif//_BUBBLE_H_
