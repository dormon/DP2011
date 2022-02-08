#ifndef _NSIZE_H_
#define _NSIZE_H_

#include"mymem.h"
#include<stdarg.h>

/**
 * @brief Reprezentuje N-tou dimenzi rozmeru
 */
typedef struct{
	unsigned Dimension;///dimenze
	unsigned*Size;///rozmery v dimenzich
}SNSize;

/**
 * @brief Inicializuje N dimenzionalni rozmer
 *
 * @param S rozmer
 * @param Dimension dimenze
 * @param Size rozmery v dimenzich
 */
//void nsize_Init(SNSize*S,unsigned Dimension,unsigned*Size);

void nsize_Inita(SNSize*S,unsigned Dimesion,...);

/**
 * @brief Inicializuje N dimenzionalni rozmer
 *
 * @param S rozmer
 * @param N vstupni rozmer
 */
void nsize_Mov(SNSize*S,SNSize*N);

/**
 * @brief Uvolni N dimenzionalni rozmer
 *
 * @param S rozmer
 */
void nsize_Free(void*S);

/**
 * @brief Spocita celkovou velikost
 *
 * @param Size rozmery v dimenzich
 *
 * @return vraci Size0*Size1*Size2*...
 */
unsigned nsize_Size(SNSize*Size);


#endif//_NSIZE_H_
