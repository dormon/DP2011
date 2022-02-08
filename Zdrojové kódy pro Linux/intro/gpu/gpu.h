#ifndef _GPU_H_
#define _GPU_H_

#include"gpubuffer.h"
#include"gpuattribute.h"
#include"gputexture.h"
#include"gputextureunit.h"
#include"gpushaderprogram.h"
#include"transform.h"
#include"camera.h"

/**
 * @brief Nastavi matice (projekcni, kamera, scena)
 *
 * @param MatrixUniform matice
 * @param Scene transformace sceny
 * @param Cam kamera
 */
void gpu_SetMatrix(GLuint*MatrixUniform,STransform Scene,SCam Cam);

#endif//_GPU_H_
