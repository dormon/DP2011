#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include"gpu.h"
#include"glslinit.h"

/**
 * @brief Reprezentuje skybox
 */
typedef struct{
	SGPUBuffer GPUData;/// GPU data
	SGPUBuffer GPUQuads;/// steny krychle
	SGPUAttribute Normal;/// normal atribut
	SGPUAttribute Position;/// pozicni atribut
	SGPUTexture SkyBox;/// textura skyboxu
	SGPUShaderProgram*Program;/// kreslici program
}SSkyBox;

/**
 * @brief Vygeneruje skybox
 *
 * @param SkyBox skybox
 * @param Texture textura skyboxu
 * @param Program kreslici program
 */
void skybox_Init(
		SSkyBox*SkyBox,
		SGPUTexture Texture,
		SGPUShaderProgram*Program);

/**
 * @brief Vykresli skybox
 *
 * @param SkyBox skybox
 */
void skybox_Draw(SSkyBox*SkyBox);

/**
 * @brief Uvolni skybox
 *
 * @param SkyBox skybox
 */
void skybox_Free(SSkyBox*SkyBox);

#endif//_SKYBOX_H_
