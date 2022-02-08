/**
 * 64kB OpenGL intro
 * Hlavickovy soubor pro texturovaci jednotku
 * @author Tomas Milet, xmilet01
 * @date 13.3.2012
 */

#ifndef _GPUTEXTUREUNIT_H_
#define _GPUTEXTUREUNIT_H_

#include"standard.h"

/**
 * @brief Reprezentuje texturovaci jednotku
 */
typedef struct{
	GLuint UnitUniform;/// texturovaci uniform
	unsigned UnitNumber;/// cislo texturovaci jednotky
}SGPUTextureUnit;

/**
 * @brief Inicializuje texturovaci jednotku
 *
 * @param Unit texturovaci jednotka
 * @param UnitUniform uniform texturovaci jednotky
 * @param UnitNumber cislo texturovaci jednotky
 * @param Target cil textury 1D/2D/3D/CUBE_MAP
 */
void gpu_InitTextureUnit(SGPUTextureUnit*Unit,GLuint UnitUniform,
		unsigned UnitNumber);

/**
 * @brief Nastavi texturovaci jednotku - spoji ji s texturou
 *
 * @param Unit texturovaci jednotka
 * @param Texture textura
 */
void gpu_SetTextureUnit(SGPUTextureUnit*Unit);

#endif//_GPUTEXTUREUNIT_H_
