/**
 * 64kB opengl intro
 * hlavickovy soubor pro transformace
 * @author Tomas Milet, xmilet01
 * @file transform.h
 * @date 4.4.2010
 * @version 0.0
 */

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "standard.h"
#include "mymath.h"
#include"mymem.h"

/**
 * @brief Typ transformace poradi pismen(R,T,S) znaci nasobeni
 */
enum ETransformType{
	// NIC=0,T=1,R=2,S=3
	TRANSTYPE_RTS=0x312,//rotace translace scale
	TRANSTYPE_TRS=0x321,//translace rotace scale
	TRANSTYPE_RS=0x32,//rotace scale
	TRANSTYPE_T=0x1,//translace
	TRANSTYPE_R=0x2,//rotace
	TRANSTYPE_S=0x3,//scale
	TRANSTYPE_N=0x0//nic
};

/**
 * Transformace
 */
typedef struct
{
	float T[3];/// posun
	float R[16];/// rotace
	float S[3];/// meritko
	enum ETransformType Type;/// typ transformace
}STransform;


/**
 * @brief Ziska modelview matici
 *
 * @param M matice
 * @param T transformace
 */
void transform_GetModelViewMatrix(float*M,STransform*T);

/**
 * Inicializace transformace
 * @param T transformace
 * @param Type typ transformace
 */
void transform_Init(STransform*T,enum ETransformType Type);

/**
 * Zmeni meritko
 * @param T transformace
 * @param S meritko
 */
void transform_Scale(STransform*T,float*S);

/**
 * Posune
 * @param T transformace
 * @param S meritko
 */
void transform_Translate(STransform*T,float*t);

/**
 * @brief Orotuje transformaci
 *
 * @param T transformace
 * @param V vektor kolem ktereho rotujeme
 * @param Angle uhel o kolik rotujeme
 */
void transform_Rotate(STransform*T,float*V,float Angle);

/**
 * @brief Nastavi transformaci Out podle T
 *
 * @param Out transformace Out //vysupt
 * @param T transformace T
 */
void transform_Mov(STransform*Out,STransform*T);

/**
 * @brief Provede rotaci podobnou jako v 3D hrach
 *
 * @param T transformace
 * @param R rotacni uhly kolem os
 */
void transform_YXZRotate(STransform*T,float*R);

/**
 * @brief Provede rotaci(Rx,Ry) podle (-y,x) pak z podle z
 *
 * @param T Transformace
 * @param R rotacni uhly kole os
 */
void transform_3DRotate(STransform*T,float*R);

/**
 * @brief Zameri objekt na urcity bod (neco jako LookAt funkce)
 *
 * @param T transformace
 * @param P bod
 * @param V vektor nahoru
 */
void transform_Track(STransform*T,float*P,float*V);

/**
 * @brief Posune se v transformaci podle osy
 *
 * @param T transformace
 * @param XYZ x,y nebo z osa
 * @param DeltaX posun na ose transformace
 */
void transform_GoXYZ(STransform*T,int XYZ,float DeltaX);

/**
 * Nastavi souradnice
 * @param T transformace
 * @param M pozice
 */
void transform_Move(STransform*T,float*M);

#endif//_TRANSFORM_H_
