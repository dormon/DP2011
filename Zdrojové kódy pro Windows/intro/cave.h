#ifndef _CAVE_H_
#define _CAVE_H_

#include"gpu.h"
#include"glslinit.h"
#include"pack.h"
#include"mt_core.h"
#include"standard.h"
#include"bargraph.h"
#include"collide.h"

typedef struct{
	SGPUBuffer GPUPoints;/// pozice
	SGPUBuffer GPUTriangles;/// trojuhelniky
	SGPUAttribute Position;/// pozicni atribut
	SGPUTexture Up;/// textura nahore
	SGPUTexture UpBump;/// bumpmapa nahore
	SGPUTexture Down;/// textura dole
	SGPUTexture DownBump;/// bumpmapa dole
	SGPUTexture Side;/// textura naboku
	SGPUTexture SideBump;/// bumpama naboku
	SGPUTexture Sediment;/// sedimenty textura
	SGPUTexture NorAmb;/// normaly a AO
	SGPUTexture Wet;/// mokra textura
	SGPUTexture Caustic;/// kaustiky
	SGPUShaderProgram*Program;/// kreslici program
	SColorBuffer Volume;/// objem
	SColorBuffer NormalAmbient;/// normaly a AO
}SCave;

/**
 * @brief Vytvori jeskyni
 *
 * @param Cave jeskyne
 * @param Size velikost jeskyne 
 * @param Threshold prah
 * @param AmbientMask velikost masky AO
 * @param TextureSize velikost textur
 * @param Program kreslici program
 */
void cave_Init(
		SCave*Cave,
		SColorBuffer Volume,
		float Threshold,
		unsigned AmbientMask,
		unsigned TextureSize,
		SGPUShaderProgram*Program);

/**
 * @brief Vykresli jeskyni
 *
 * @param Cave jeskyne
 */
void cave_Draw(SCave*Cave);

/**
 * @brief Vytvori buffer pro vytvoreni jeskyne a pro kolize
 *
 * @param Volume objem
 * @param Size velikost kostky s jeskyni
 */
void cave_BuildVolume(SColorBuffer*Volume,unsigned Size);

/**
 * @brief Vytvori normalovy a AO buffer
 *
 * @param NorAmb vystup
 * @param Volume objem
 * @param AmbientMask velikost AO masky
 * @param Threshold prah vzduch/skala
 */
void cave_BuildNorAmb(SColorBuffer*NorAmb,SColorBuffer*Volume,
		unsigned AmbientMask,float Threshold);


/**
 * @brief Prevede volume na trojuhelniky
 *
 * @param Points body
 * @param Triangles trojuhelniky
 * @param Volume objem
 * @param Threshold prah vzduch/skala
 */
void cave_VolumeToTriangles(
		SGPUBuffer*Points,
		SGPUBuffer*Triangles,
		SColorBuffer*Volume,
		float Threshold);

/**
 * @brief Vytvore textury jeskyne
 *
 * @param Cave jeskyne
 * @param Size velikost textur
 */
void cave_BuildTextures(SCave*Cave,unsigned Size);

/**
 * @brief Prepocita svetelny tok
 *
 * @param Cave jeskyne
 * @param Collide kolizni system
 * @param LightPos pozice svetla
 */
void cave_ComputeLight(SCave*Cave,SCollide*Collide,float*LightPos);

/**
 * @brief Uvolni jeskyni/tunel
 *
 * @param Cave jeskyne tunel
 */
void cave_Free(SCave*Cave);

#endif//_CAVE_H_
