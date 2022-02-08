#ifndef _TUNNEL_H_
#define _TUNNEL_H_

#include"colorbuffer.h"

/**
 * @brief Reprezentuje tunel
 */
typedef struct{
	unsigned Seed;/// seminko
	unsigned Size;/// velikost
}STunnelTemplate;

/**
 * @brief Vygeneruje tunel
 *
 * @param Tunnel objem tunelu
 * @param Template sablona tunelu
 */
void tunnel_Init(SColorBuffer*Tunnel,STunnelTemplate Template);

#endif//_TUNNEL_H_
