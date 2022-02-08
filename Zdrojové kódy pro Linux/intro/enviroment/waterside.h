#ifndef _WATERSIDE_H_
#define _WATERSIDE_H_

#include"gpu.h"
#include"colorbuffer.h"

/**
 * @brief Vygeneruje vyskovou mapu pobrezi
 *
 * @param HeightMap vyskova mapa pobrezi
 * @param Size velikost
 * @param Min minimalni vyska
 * @param Max maximalni vyska
 * @param Seed seminko
 */
void waterside_InitHeightMap(SColorBuffer*HeightMap,unsigned Size,
		float Min,float Max,unsigned Seed);

#endif//_WATERSIDE_H_
