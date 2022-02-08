#ifndef _PACK_H_
#define _PACK_H_

#include"mymem.h"
#include"ntree.h"
#include"relist.h"
#include"vector.h"

//#define PACK_USE_TREE

typedef struct{
	SReList Point;/// body
	SReList Triangle;/// trojuhelniky
}SPack;

/**
 * @brief Inicializuje balik s geometrii 
 *
 * @param Pack Balik
 * @param S souradnice prostoru
 * @param Size velikost zabiraneho prostoru
 */
void pack_Init(SPack*Pack);

/**
 * @brief Uvolni balik
 *
 * @param Pack balik
 */
void pack_Free(void*Pack);

/**
 * @brief Vlozi geometrii do baliku
 *
 * @param Pack balik
 * @param Points body
 * @param NumPoint pocet bodu
 * @param Triangles trojuhleniky
 * @param NumTriangle pocet trojuhelniku
 */
void pack_Insert(SPack*Pack,
		float*Points,unsigned NumPoint,
		unsigned*Triangles,unsigned NumTriangle);

#endif//_PACK_H_
