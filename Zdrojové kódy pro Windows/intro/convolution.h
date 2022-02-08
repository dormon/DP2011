#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include"nsize.h"
#include"ndimension.h"
#include"stdmath.h"

/**
 * @brief Provede konvoluci dat s konvolucnim jadrem
 *
 * @param Out vystup
 * @param Data vstup
 * @param M konvolucni matice
 * @param DataSize velikost dat
 * @param MSize velikost konvolucni masky
 * @param Type typ chovani na okraji (opakovani prodlouzeni)
 */
void texturefactory_Convolution(float*Out,float*Data,float*M,
		SNSize*DataSize,SNSize*MSize,enum EIndex Type);

#endif//_CONVOLUTION_H_
