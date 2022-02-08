#ifndef _RELIST_H_
#define _RELIST_H_

#include"adtfce.h"
#include"standard.h"

/**
 * @brief Reprezentuje roztahovatelne pole
 */
typedef struct{
	void*Data;/// data
	SADTFCE*F;/// funkce pro praci
	unsigned Used;/// pocet pouzitych prvku v poli
	unsigned Size;/// celkova velikost pole
}SReList;

/**
 * @brief Inicializuje roztahovacne pole
 *
 * @param List roztahovacne pole
 * @param ElementSize velikost prvku
 */
void relist_Init(SReList*List,SADTFCE*F);

/**
 * @brief Uvolni list
 *
 * @param List list
 */
void relist_Free(void*List);

/**
 * @brief Provede realokaci
 *
 * @param SReList roztahovacne pole
 * @return vraci 0 pri uspechu
 */
int relist_Realloc(SReList*List);

/**
 * @brief Vlozime dalsi data na vrchol zasobniku
 *
 * @param List seznam
 * @param Data dalsi data
 *
 * @return vraci 0 pri uspechu
 */
int relist_Push(SReList*List,void*Data);

/**
 * @brief Ziskame data z vrcholu zasobniku (funkce data neuvolnuje!)
 *
 * @param List seznam
 *
 * @return vraci ukazatel na data
 */
void*relist_Pop(SReList*List);

/**
 * @brief Smaze prvek na i pozici a konec seznamu dosune aby prekyl mezeru
 *
 * @param List seznam
 * @param i cislo prvku
 *
 */
void relist_Delete(SReList*List,unsigned i);

/**
 * @brief Vlozi nova data na index i data za index i posune
 *
 * @param List seznam
 * @param i idenx
 * @param Data nova data
 */
void relist_Insert(SReList*List,unsigned i,void*Data);

/**
 * @brief Ziskame data
 *
 * @param List seznam
 * @param Index index
 *
 * @return data na danem indexu
 */
void*relist_GetData(SReList*List,unsigned Index);

#endif//_RELIST_H_
