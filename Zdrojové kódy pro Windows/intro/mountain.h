#ifndef _MOUNTAIN_H_
#define _MOUNTAIN_H_

#include"gpu.h"
#include"colorbuffer.h"

/**
 * @brief Inicializuje vyskvou mapu pro hory
 *
 * @param HeightMap vyskova mapa
 * @param Size velkost vyskove mapy
 * @param Min minimalni vyska
 * @param Max maxmalni vyska
 * @param Seed seminko
 */
void mountain_InitHeightMap(SColorBuffer*HeightMap,unsigned Size,
		float Min,float Max,unsigned Seed);

#endif//_MOUNTAIN_H_
