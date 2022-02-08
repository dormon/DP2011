#ifndef _GPUTEXTURE_H_
#define _GPUTEXTURE_H_

#include"standard.h"
#include"colorbuffer.h"
#include"bargraph.h"
#include"gputextureunit.h"

/**
 * @brief Reprezentuje texturu
 */
typedef struct{
	GLuint Texture;/// textura
	GLenum Target;/// cil
	SColorBuffer Data[6];/// data textury
	int CubeMap;/// jedna se o texturu CUBEMAP
	int DataChanged;/// zmenily se data?
}SGPUTexture;

/**
 * @brief Inicializuje texturu
 *
 * @param Texture textura
 * @param MipMap 0 vypina mipmapping
 * @param CubeMap 1 zapina cubemap
 * @param ... data SColorBuffer jeden nebo 6
 */
void gpu_InitTexture(
		SGPUTexture*Texture,
		int MipMap,
		int CubeMap,
		...);

/**
 * @brief Nastavi texturu
 *
 * @param Texture textura
 */
void gpu_SetTexture(SGPUTexture*Texture);

/**
 * @brief Uvolni texturu
 *
 * @param Texture textura
 */
void gpu_FreeTexture(SGPUTexture*Texture);

void gpu_SaveTexture(const char*Name,SGPUTexture*Texture);

#endif//_GPUTEXTURE_H_
