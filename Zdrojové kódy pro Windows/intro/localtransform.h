#ifndef _LOCALTRANSFORM_H_
#define _LOCALTRANSFORM_H_

#include"index.h"
#include"mymath.h"

/**
 * @brief Prototyp transformacni funkce
 *
 * @param Data data bufferu
 * @param Index transformovany pixel
 * @param Param parametry transformace
 *
 * @return vraci transformovanou hodnotu
 */
typedef float(*LOCALTRANSFORMFCE)(float*Data,SIndex*Index,void*Param);

/**
 * @brief Tabulka transformacnich funkci
 */
extern const LOCALTRANSFORMFCE LocalTransformFce[];


/**
 * @brief Typ transformace
 */
enum ELocalTransform{
	PT_INVERT=0,//prvni prvek
	// pod toto pridavame

	PT_FLOWERS,
	PT_SCRAP,
	PT_THRESHOLD,

	// nad toto pridavame
	PT_WHEEZING//posledni prvek
};

#endif//_LOCALTRANSFORM_H_
