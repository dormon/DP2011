/**
 * 64kB opengl intro
 * hlavickovy soubor pro prototypy funkci
 * @author Tomas Milet, xmilet01
 * @file adtfce.h
 * @date 23.12.2009
 * @version 0.0
 */

#ifndef _ADTFCE_H_
#define _ADTFCE_H_

#include <stdlib.h>
#include"mymem.h"

/**
 * Prototyp funkce pro uvolneni dat
 * @param Data data pro uvolneni
 */
typedef void(*ADT_FREEFCE)(void*Data);

/**
 * Prototyp funkce pro porovnani dat
 * @param A data A
 * @param B data B
 * @return pokud A<B -1,pokud A==B 0,pokud A>B 1
 */
typedef int(*ADT_COMPARE)(void*A,void*B);

/**
 * Prototyp funkce pro zkopirovani dat z In Do out
 * @param Out vystupni pamet
 * @param In vstupni pamet
 * @return vraci 0 v pripade uspechu
 */
typedef int(*ADT_COPY)(void*Out,void*In);

/**
 * Prototyp funkce pro porovnani dat
 * @param A data a
 * @param B data b
 * @return vraci 0 pri A==B; -1 A<B; 1 A>B
 */
typedef int(*ADT_CMP)(void*A,void*B);

/**
 * Prototyp pro mapovaci funkci
 * @param A data nad kterymi se rovede funkce 
 * @param D parametry funkce
 */
typedef int(*ADT_MAP)(void*A,void*D);

/**
 * Struktura udrzujici informace, jak pracovat s objekty
 */
typedef struct
{
	ADT_FREEFCE Free;/// uvolnovaci funkce
	ADT_COPY Copy;/// kopirovaci funkce
	ADT_CMP Cmp;/// porovnavaci funkce
	unsigned Size;/// velikost prvku
}SADTFCE;

/**
 * Uvolni data podle predane funkce
 * @param Data data
 * @param Fce standardni funkce pro praci
 */
void adt_FreeData(void*Data,SADTFCE*Fce);//testovano

/**
 * Zkopiruje data podle predane funkce
 * @param B kam kopiruje
 * @param A odkud kopiruje
 * @param F standardni funkce pro praci
 * @return vraci 0 v pripade uspechu
 */
int adt_CopyData(void*B,void*A,SADTFCE*F);//testovano

/**
 * Porovna data podle predane funkce
 * @param A data a
 * @param B data b
 * @param F standardni funkce pro praci
 * @return vraci 0 pri A==B,-1 A<B,1 A>B
 */
int adt_CmpData(void*A,void*B,SADTFCE*F);

/**
 * @brief Ziska velikost dat
 *
 * @param F struktura pro popis dat
 *
 * @return vraci F->Size nebo pokud je F==NULL sizeof(unsigned char)
 */
unsigned adt_ElemSize(SADTFCE*F);

#endif//_ADTFCE_H_
