/**
 * 64kB opengl intro
 * hlavickovy soubor pro kameru
 * @author Tomas Milet, xmilet01
 * @file camera.h
 * @date 23.12.2009
 * @version 0.0
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "standard.h"
#include "mymath.h"
#include"transform.h"

typedef struct
{
	STransform Transform;
	float PM[16];//projekcni matice
	float Angle;//uhel zaberu
	float Near;//nejblizsi viditelne objekty
	float Far;//nezvzalenejsi viditelne objekty
	unsigned Width;//sirka
	unsigned Height;//vyska
}SCam;

/**
 * @brief Provede inicializaci kamery
 *
 * @param C Kamera
 * @param N near rovina
 * @param F far rovina
 * @param A uhel
 * @param W sirka
 * @param H vyska
 */
void camera_Init(SCam*C,float N,float F,float A,unsigned W,unsigned H);


/**
 * @brief Vytvori projekcni matici
 *
 * @param M matice
 * @param Fovy zorny uhel
 * @param Width sirka
 * @param Height vyska
 * @param Near vzdalenost k nejblizsi orezavaci rovine
 * @param Far vzdalenost k nejvzdalenejsi orezavaci rovine
 */
void camera_ProjectionMatrix(float*M,float Fovy,unsigned Width,
		unsigned Height,float Near,float Far);

#endif//_CAMERA_H_
