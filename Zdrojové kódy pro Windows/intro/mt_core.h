#ifndef _MT_CORE_H_
#define _MT_CORE_H_

#include"mymem.h"

/**
 * @brief Inicializuje (predpocita) pochodujici tetrahedron (1x na zacatku)
 */
void mt_Init();

/**
 * @brief Deinicializuje (uvolni) pochodujici tetrahedron (1x na konci)
 */
void mt_DeInit();

/**
 * @brief Ziska geometrii pro kostku
 *
 * @param Points vertexy
 * @param NumPoint pocet vertexu
 * @param Triangles trojuhelniky
 * @param NumTriangle pocet trojuhelniku
 * @param Corners bitova jednicka znaci ze v rohu je teleso
 * @param Start souradnice kostky
 * @param Size velikost kostky
 * @param CornerValue hodnoty v rozich pro aproximaci
 * @param Threshold Prah
 */
void mt_Get(
		float*Points,
		unsigned*NumPoint,
		unsigned*Triangles,
		unsigned*NumTriangle,
		unsigned char Corners,
		float*Start,
		float Size,
		float*CornerValue,
		float Threshold);

#endif//_MT_CORE_H_
