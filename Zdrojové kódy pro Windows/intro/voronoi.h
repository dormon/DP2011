#ifndef _VORONOI_H_
#define _VORONOI_H_

#include"ndimension.h"
#include"mymem.h"
#include"index.h"
#include"stdmath.h"

/**
 * @brief Spocita Voronoi Diagram
 *
 * @param Size Velikost diagramu
 * @param Points Vstupni body Points={(x,y,...),(x,y,...),....}
 * @param NumPoint pocet vstupnich bodu NumPoint=|Points|
 *
 * @return Vraci Voronoi Diagram
 */
float*VoronoiDiagram(SNSize*Size,unsigned*Points,unsigned NumPoint);

#endif//_VORONOI_H_
