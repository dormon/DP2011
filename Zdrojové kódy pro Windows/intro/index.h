#ifndef _INDEX_H_
#define _INDEX_H_

#include"nsize.h"

/**
 * @brief Reprezentuje N-dimenzionalni index
 */
typedef struct{
	unsigned*Index;/// slozky indexu
	SNSize Size;/// rozmery v dimenzich
}SIndex;

/**
 * @brief Opakovani od zacatku (REPEAT), opakovani posledni hodnoty (CLAMP)
 */
enum EIndex{
	INDEX_REPEAT=0,
	INDEX_CLAMP
};

/**
 * @brief Inicializuje index
 *
 * @param I Index
 * @param Size Rozmery v dimenzich
 */
void index_Init(SIndex*I,SNSize*Size);

/**
 * @brief Uvolni N-dimensionalni index
 *
 * @param I Index
 */
void index_Free(SIndex*I);

/**
 * @brief Posune N-dimensionalni index
 *
 * @param Index Index
 * @param Mov Delta pohybu
 * @param DimID Cislo dimenze ve ktere se pohybujeme
 * @param Type Typ reseni problemu s presahy (navrat na zacatek a pod)
 */
void index_Mov(SIndex*Index,int Mov,unsigned DimID,enum EIndex Type);

/**
 * @brief Prevede index pro zakladni pruchod na zigzag pruchod
 *
 * @param ZigZag vystup v zigzag
 * @param In vstup
 */
void index_ZigZag(SIndex*ZigZag,SIndex*In);

/**
 * @brief Inkrementuje index nejprve nejnizssi dimenze, pri preteceni vyssi
 *
 * @param Index index
 *
 * @return vraci 1 pokud doslo k preteceni indexu (pretekly vsechny casti)
 */
int index_Inc(SIndex*Index);

/**
 * @brief Spocita index do 1D pole
 *
 * @param I Index
 *
 * @return vraci hodnotu ... + I2*S1*S0 + I1*S0 + I0 (I slozka,S velikost)
 */
unsigned index_GetIndex(SIndex*I);

/**
 * @brief Ziska ukazatel na zacetek dat na danem indexu
 *
 * @param Data pole dat
 * @param I N-dimenzionalni index
 * @param ElemSize velikost prvku dat (sizeof(float))
 *
 * @return vraci ukazatel na zacek indexovaneho prvku
 */
void*index_GetData(void*Data,SIndex*I,unsigned ElemSize);

/**
 * @brief Nastavi index podle druheho indexu
 *
 * @param O druhy vystupni index
 * @param I prvni vstupni index
 */
void index_Set(SIndex*O,SIndex*I);

#endif//_INDEX_H_
