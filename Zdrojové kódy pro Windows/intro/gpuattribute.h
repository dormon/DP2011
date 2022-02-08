#ifndef _GPUATTRIBUTE_H_
#define _GPUATTRIBUTE_H_

#include"standard.h"

/**
 * @brief Reprezentuje atribut
 */
typedef struct{
	GLuint Index;/// atribut
	GLint Size;/// pocet slozek jednech dat
	GLenum Type;/// typ dat
	GLboolean Normalized;/// normalizacni znacka
	GLsizei Stride;/// prokladani
	GLvoid*Pointer;/// ukazatel na prvni prvek
}SGPUAttribute;

/**
 * @brief Inicializuje atribut
 *
 * @param Attribute atribut
 * @param Index identifikator promenne v shaderu
 * @param Size pocet slozek
 * @param Type typ slozek
 * @param Normalized normalizujeme
 * @param Stride prokladani
 * @param Pointer ukazatel na prvni prvek
 */
void gpu_InitAttribute(SGPUAttribute*Attribute,GLuint Index,GLint Size,
		GLuint Type,GLboolean Normalized,GLsizei Stride,GLvoid*Pointer);

/**
 * @brief Nastavi atribut
 *
 * @param Attribut atribut
 */
void gpu_SetAttribute(SGPUAttribute*Attribute);

#endif//_GPUATTRIBUTE_H_
