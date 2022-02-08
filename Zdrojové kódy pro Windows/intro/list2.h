/**
 * 64kB opengl intro
 * hlavickovy soubor pro dvousmerny seznam
 * @author Tomas Milet, xmilet01
 * @file list2.h
 * @date 23.12.2009
 * @version 0.1
 */

#ifndef _LIST2_H_
#define _LIST2_H_

#include <stdlib.h>
#include "adtfce.h"
#include "standard.h"

/**
 * Zakladni funkce pro seznam
 */
extern const SADTFCE LIST2_ADTFCE;

/**
 * Prvek dvousmerneho seznalu
 */
struct SList2Item
{
	struct SList2Item*NextTo[2];/// levy a pravy soused
	void*Data;/// data
};

/**
 * Dvousmerny seznam
 */
typedef struct
{
	struct SList2Item*Border[2];/// zacatek a konec
	SADTFCE*F;/// funkce pro praci
	unsigned Lenght;/// delka
}SList2;

/**
 * Inicializuje seznam
 * @param L list
 * @param F funkce pro praci
 */
void list2_Init(SList2*L,SADTFCE*F);//testovano

/**
 * Vlozi prvek na kraj seznamu
 * @param L seznam
 * @param B pokud je B 0 zacatek pokud je B 1 konec
 * @return vraci 0 pri uspechu
 */
int list2_Insert(SList2*L,int B);//testovano

/**
 * Vlozi prvek na kraj seznamu + alokuje misto pro data
 * @param L seznam
 * @param B pokud je B 0 zacatek pokud je B 1 konec
 * @return vraci 0 pri uspechu
 */
int list2_InsertData(SList2*L,int B);//testovano

/**
 * Vloti prvek do seznamu s razenim
 * @param L seznam
 * @param Data data noveho prvku
 * @return vraci 0 pri uspechu
 */
int list2_OrderInsert(SList2*L,void*Data);

/**
 * Spoji dva seznamy
 * @param L jeden vystupni seznam
 * @param B druhy seznam
 * @return vraci 0 v pripade uspechu
 */
int list2_Join(SList2*L,SList2 B);//testovano

/**
 * Uvolni prvek na kraji seznamu
 * @param L seznam
 * @param B pokudj e B 0 zacatek pokud je B 1 konec 
 */
void list2_Delete(SList2*L,int B);//testovano

/**
 * Testuje na prazdnost seznamu
 * @param L seznam
 * @return vraci 1 pri prazdnosti
 */
int list2_IsEmpty(SList2 L);//testovano

/**
 * Uvolni seznam z pamet
 * @param L seznam
 * @param FreeFce funkce pro uvolneni dat
 */
void list2_Free(void*L);//testovano

/**
 * Zkopiruje seznam
 * @param L novy seznam
 * @param B seznma ze ktereho se cte
 * @return vraci 0 v pripade uspechu
 */
int list2_Copy(void*L,void*B);//testovano

/**
 * Provna dva seznamy
 * @param A data a
 * @param B data b
 * @return vraci 0 pri A==B,-1 A<B, 1 A>B
 */
int list2_Cmp(void*A,void*B);

/**
 * Aplikuje funkci Map na seznam
 * @param L seznam
 * @param Map mapovaci funkce
 * @param Arg argumenty pro mapovaci funkci
 * @return vraci 1 v pripade ze funkce Map aspon jednou vratila 1
 */
int list2_Map(SList2*L,ADT_MAP Map,void*Arg);

#endif//_LIST2_H_
