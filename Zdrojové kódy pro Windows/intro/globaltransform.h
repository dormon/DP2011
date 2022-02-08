#ifndef _GLOBALTRANSFORM_H_
#define _GLOBALTRANSFORM_H_

#include"index.h"
#include"convolution.h"

/**
 * @brief Prototyp transformacni fukce
 *
 * @param Out vystupni data
 * @param In vstupni data
 * @param Size velikost dat
 * @param Param parametry transformace
 */
typedef void(*GLOBALTRANSFORMFCE)(float*Out,float*In,SNSize*Size,void*Param);

/**
 * @brief Tabulka transformacnich funkci
 */
extern const GLOBALTRANSFORMFCE GlobalTransformFce[];

/**
 * @brief Typ transformace
 */
enum EGlobalTransform{
	GT_SMOOTH,//prvni prvek
	// pod toto pridavame
	
	// nad toto pridavame
	GT_EDGE//posledni prvek
};

#endif//_GLOBALTRANSFORM_H_
