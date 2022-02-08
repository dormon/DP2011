#ifndef _GPUFRAMEBUFFER_H_
#define _GPUFRAMEBUFFER_H_

#include"standard.h"

/**
 * @brief Reprezentuje GPU frame buffer
 */
typedef struct{
	GLuint FrameBuffer;/// frame buffer
	GLenum Target;/// cil (hloubka/barva/stencil)
}SGPUFrameBuffer;

/**
 * @brief Pribinduje buffery do kterych budeme kreslit
 *
 * @param Num pocet bufferu
 * @param ... buffery
 */
void gpu_BindFrameBuffer(unsigned Num,...);

#endif//_GPUFRAMEBUFFER_H_
