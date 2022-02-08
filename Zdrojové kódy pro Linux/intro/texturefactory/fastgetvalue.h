#ifndef _FASTGETVALUE_H_
#define _FASTGETVALUE_H_

#include"vector.h"

/**
 * @brief Ziska aproximovanou hodnotu z 3D pole
 *
 * @param Data data pole
 * @param Size rozmer pole
 * @param P float index v rozsahu <0,1> X <0,1> X <0,1>
 *
 * @return vraci aproximovanou hodnotu z okoli P
 */
float colorbuffer_FastGetValue3D(float*Data,unsigned*Size,float*P);

/**
 * @brief Pricte hodnotu na aproximovanou pozici
 *
 * @param Data data pole
 * @param Size rozmer pole
 * @param P float index v rozsahu <0,1> X <0,1> X <0,1>
 * @param V hodnota
 */
void colorbuffer_FastAddValue3D(float*Data,unsigned*Size,float*P,float V);

#endif//_FASTGETVALUE_H_
