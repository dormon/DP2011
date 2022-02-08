/**
 * 64kB opengl intro
 * hlavickovy soubor pro generovani midpoint sumu
 * @author Tomas Milet, xmilet01
 * @file midpoint.h
 * @date 11.11.2011
 * @version 0.0
 */

#ifndef _MIDPOINT_H_
#define _MIDPOINT_H_

#include "standard.h"
#include "mymath.h"
#include"mymem.h"
#include"ndimension.h"

/**
 * funkce vygeneruje N rozmerny fraktal
 * funkce pouziva puleni intervalu
 * @param NSize velikost sumu
 * @param RandInt zakladni interval pro nahodu <-RandInt,RandInt>
 * @param Smooth vyhlazovani timto se deli RandInt v kazdem kroku
 * @return vraci pole o velikosti sizeof(float)*product(Level)
 */
float*midpoint_Generate(SNSize*NSize,
												float RandInt,float Smooth);//testovano

#endif//_MIDPOINT_H_
