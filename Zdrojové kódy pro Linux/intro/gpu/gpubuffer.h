#ifndef _GPUBUFFER_H_
#define _GPUBUFFER_H_

#include"standard.h"
#include"mymem.h"

/**
 * @brief Reprezentruje GPU buffer
 */
typedef struct{
	GLenum Target;/// cil (element/vertex/...)
	GLuint Buffer;/// gpu buffer
	GLsizeiptr Size;/// velikost dat v bytech
	GLenum Usage;/// pouzivani
	void*Data;/// CPU data
	int DataChanged;/// priznak zmeny dat
}SGPUBuffer;

/**
 * @brief Vytvori buffer
 *
 * @param Buffer buffer
 * @param Size velikost bufferu v byte
 * @param Target typ bufferu
 * @param Usage pouziti bufferu
 *
 * @return vraci ukazatel na gpu data pro primy pristup
 */
void gpu_InitBuffer(SGPUBuffer*Buffer,unsigned Size,GLenum Target,GLenum Usage);

/**
 * @brief Uvolni buffer
 *
 * @param Buffer buffer
 */
void gpu_FreeBuffer(SGPUBuffer*Buffer);

/**
 * @brief Pribinduje buffer
 *
 * @param Buffer buffer
 */
void gpu_BindBuffer(SGPUBuffer*Buffer);

/**
 * @brief Nastavi priznak ze se data zmenily
 *
 * @param Buffer buffer
 */
void gpu_ChangeBuffer(SGPUBuffer*Buffer);

/**
 * @brief Ziskame ukazatel na data na grafice
 *
 * @param Buffer buffer
 *
 * @return vraci prozatimni ukazatel na data v grafice
 */
void*gpu_MapBuffer(SGPUBuffer*Buffer);

/**
 * @brief Ukonci mapovani (zrusi ukazatel na data na grafice)
 *
 * @param Buffer buffer
 */
void gpu_UnmapBuffer(SGPUBuffer*Buffer);

#endif//_GPUBUFFER_H_
