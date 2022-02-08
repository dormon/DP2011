/**
 * 64kB opengl intro
 * hlavickovy soubor pro vektorove operace
 * @author Tomas Milet, xmilet01
 * @file mymath.h
 * @date 11.10.2011
 * @version 0.0
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include"stdmath.h"
#include"myfloat.h"

extern const float NullVector[];
extern const float MaxVector[];
extern float Gravity[];
extern const float OneVector[];
extern const float XVector[];
extern const float YVector[];
extern const float ZVector[];

#define PRINTVEC(V)  fprintf(stderr,#V": %f %f %f\n", V[0],V[1],V[2])


/**
 * secte vektory
 * @param a vysledny vektor a
 * @param b druhy vektor
 */
void vector_Add(float*a,float*b);

/**
 * odecte vektory
 * @param a vysledny vektor a
 * @param b druhy vektor
 */
void vector_Sub(float*a,float*b);

/**
 * vynasobi vektory
 * @param a vysledny vektor a
 * @param b druhy vektor
 */
void vector_Mul(float*a,float*b);

/**
 * skalarne vynasobi vektory
 * @param a vysledny vektor a
 * @param b druhy vektor
 * @return skralarni nasobek
 */
float vector_AMul(float*a,float*b);

/**
 * vynasobi vektor konstantou
 * @param a vysledny vektor a
 * @param K konstanta
 */
void vector_KMul(float*a,float K);

/**
 * velikost vektoru
 * @param a vektor
 * @return vraci velikost vektoru
 */
float vector_Len(float*a);

/**
 * vytvori kolmy vektor na 2 vektory
 * @param c vysledny vektor
 * @param a vektor 1
 * @param b vektor 2
 */
void vector_Ortho(float*c,float*a,float*b);

/**
 * @brief Spocita cross product
 *
 * @param c vystup
 * @param a jeden vektor
 * @param b druhy vektor
 */
void vector_Cross(float*c,float*a,float*b);

/**
 * znormalizuje vektor
 * @param a vysledny vektor
 */
void vector_Nor(float*a);

/**
 * spocita vektor na minus prvni
 * @param a vektor
 */
void vector_Rot(float*a);

/**
 * Vlozi vektor do druheho
 * @param a vysledny vektor
 * @param b druhy vektor
 */
void vector_Mov(float*a,float*b);

/**
 * vygeneruje nahodny jednotkovy vektor
 * @param a vektor
 */
void vector_Rnd(float*a);

/**
 * vytvori jednotkovy linearne nezavisly vektor b k a
 * @param a vektor a
 * @param b linearne nezavisly vektor
 */
void vector_LN(float*b,float*a);

/**
 * Spocita kosinus uhlu mezi vektory
 * @param u vektor u
 * @param v vektor v
 * @return cosinus uhlu
 */
float vector_Cos(float*u,float*v);

/**
 * Vytvori vektor mezi body A a B
 * @param O vystupni vektor
 * @param A bod zacateku vektoru
 * @param B bod konce vektoru
 */
void vector_Make(float*O,float*A,float*B);

/**
 * @brief Provede reflexi vektoru (uhel odrazu == uhel dopadu)
 *
 * @param O vystupni vektor
 * @param V vstupni vektor
 * @param N normala povrchu
 */
void vector_Reflect(float*O,float*V,float*N);

/**
 * @brief Zkontroluje na identitu
 *
 * @param A vektor a
 * @param B vektor b
 *
 * @return vraci A==B
 */
int vector_Eq(float*A,float*B);

/**
 * @brief Zjisti do ktere subkostky v octtree bod X patri
 *
 * @param S souradnice rohu kostky
 * @param Size velikost kostky
 * @param X Bod
 *
 * @return vraci index 0-7 - cislo subkostky
 */
int vector_Index(float*S,float Size,float*X);

/**
 * @brief Vytvori vektor z Indexu 7=111 (1,1,1); 5=101 (1,0,1)
 *
 * @param V vektor
 * @param Index index
 */
void vector_IndexToVector(float*V,int Index);

/**
 * @brief Spocita novy zacatek subkostky
 *
 * @param NewS novy zacatek
 * @param S puvodni zacatek
 * @param Size Velikost puvodni kostky
 * @param Index index subkostky
 */
void vector_CubeStart(float*NewS,float*S,float Size,int Index);

#endif//_VECTOR_H_
