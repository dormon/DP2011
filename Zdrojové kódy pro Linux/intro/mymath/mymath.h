/**
 * 64kB opengl intro
 * hlavickovy soubor pro vlastni matematiku
 * @author Tomas Milet, xmilet01
 * @file mymath.h
 * @date 23.12.2009
 * @version 0.0
 */

#ifndef _MYMATH_H_
#define _MYMATH_H_

#include"standard.h"
#include"stdmath.h"
#include"vector.h"
#include"matrix.h"


/**
 * @brief prototyp jednoparmerove float funkce
 *
 * @param A vstup
 *
 * @return vraci funkcni hodnotu v bode A
 */
typedef float(*FCEF1F1)(float A);

/**
 * @brief Reprezentuje nahodnou hodnotu v rozsahu
 */
typedef struct{
	float Mean;/// stredni hodnota
	float Deviation;/// rozptyl
	FCEF1F1 Fce;/// fonkce rozptylu
}SRandRange;

/**
 * @brief Vytvori nahodnou hodnotu v rozsahu
 *
 * @param RandRange Rozsah
 *
 * @return vraci hodnotu v <Mean-Deviation,Mean+Deviation>
 */
float RandRange(SRandRange*RandRange);


/**
 * @brief Udrzuje hodnotu vraci 0
 *
 * @param t parametr
 *
 * @return vraci 0
 */
float Hold(float t);

/**
 * @brief Linearni funkce
 *
 * @param t vstup
 *
 * @return t
 */
float Linear(float t);

/**
 * @brief Hermituv vyhlazovaci polynom
 *
 * @param t vstup
 *
 * @return vraci 3t^2 - 2t^3
 */
float Hermite(float t);

/**
 * @brief Vyhlazovaci polynom 5 radu
 *
 * @param t vstup
 *
 * @return vraci 6t^5 - 15t^4 + 10t^3
 */
float FifthDegreePoly(float t);


extern FCEF1F1 AproxFce[];

/**
 * @brief Linearni interpolace
 *
 * @param a hodnota A
 * @param b hodnota B
 * @param t pomer hodno 0 => A, 1 => B
 *
 * @return vraci hodnotu a*(1-f(t))+b*f(t)
 */
float Interpolation(float a,float b,float t);

/**
 * @brief Vypocita koeficienty B-spline
 *
 * @param C vystupni koeficienty
 * @param t pomer <0,1>
 */
void CatmullRomKoef(float*C,float t);

/**
 * @brief CatmullRom interpolace
 *
 * @param P 4 konrolni body
 * @param t paramter <0,1>
 *
 * @return interpolovana hodnota
 */
float CatmullRom(float*P,float t);

/**
 * @brief CatmullRom interpolace vektorova
 *
 * @param R vystup
 * @param A kontrolni bod
 * @param B kontrolni bod
 * @param C kontrolni bod
 * @param D kontrolni bod
 *
 * @param t parametr <0,1>
 */
void vector_CatmullRom(float*R,float*A,float*B,float*C,float*D,float t);

/**
 * @brief Provede interpolaci mezi dvema vektory
 *
 * @param c vystup
 * @param a vektor a
 * @param b vektor b
 * @param t pomer hodnot
 */
void vector_Interpolation(float*c,float*a,float*b,float t);

/**
 * Spocita obsah trojuhelniku
 * @param a delka a
 * @param b delka b
 * @param c delka c
 * @return obsah trojuhelni se stranami a,b,c
 */
float triangle_area_base(float a,float b,float c);

/**
 * Spocita obsah trojuhelniku
 * @param A bod A
 * @param B bod B
 * @param C bod C
 * @return obsah trojuhelniku
 *
 */
float triangle_area(float*A,float*B,float*C);


#endif//_MYMATH_H_
