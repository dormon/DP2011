#ifndef _MYMEM_H_
#define _MYMEM_H_

#include"standard.h"

/**
 * @brief Provede alokaci pameti
 *
 * @param NumByte pocet byte pro alokaci
 *
 * @return vraci ukazatel na alokovanou pamet
 */
void*mymem_Alloc(unsigned NumByte);

/**
 * @brief zmeni velikost
 *
 * @param D data
 * @param Num nova velikost
 *
 * @return vraci pole se zmenenou velikosti
 */
void*mymem_Realloc(void*D,unsigned Num);

/**
 * @brief Provede uvolneni pameri
 *
 * @param Mem pamet
 */
void mymem_Free(void**Mem);

/**
 * @brief Provede prekopirovani Num byte z S do D
 *
 * @param D destinace
 * @param S zdroj
 * @param Num pocet byte
 */
void mymem_Cpy(void*D,void*S,unsigned Num);

/**
 * @brief Provede porovnani Num byte mezi D a S
 *
 * @param D jedny data
 * @param S druhe data
 * @param Num pocet dat pro porovnani
 *
 * @return vraci -1,0,1 D<S D==S D>S
 */
int mymem_Cmp(void*D,void*S,unsigned Num);

/**
 * @brief Nastavi data na danou hodnotu
 *
 * @param D data
 * @param Val hodnota
 * @param Num pocet byte
 */
void mymem_Set(void*D,unsigned char Val,unsigned Num);

#endif//_MYMEM_H_
