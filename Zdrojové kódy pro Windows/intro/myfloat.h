/**
 * 64kB opengl intro
 * hlavickovy soubor pro porovnani floatu
 * @author Tomas Milet, xmilet01
 * @file myfloat.h
 * @date 17.2.2010
 * @version 0.0
 */

#ifndef _MYFLOAT_H_
#define _MYFLOAT_H_

#include <math.h>

/**
 * Hranice pod kterou se musi nachazet rozdil dvou cisel
 * pro prohlaseni ze jsou stejna -> pro nepresnost float
 */
#define FLOAT_MAX 10e100
#define FLOAT_MIN -10e100

extern float FLOAT_DIFF;

/**
 * Provna dve cisla float na shodu
 * @param A cislo a
 * @param B cislo b
 * @return vraci A==B
 */
int float_Eq(float A,float B);//testovano

#endif//_MYFLOAT_H_
