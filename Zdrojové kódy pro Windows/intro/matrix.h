#ifndef _MATRIX_H_
#define _MATRIX_H_

#include"vector.h"
#include"mymem.h"

/**
 * @brief Vynuluje matici
 *
 * @param A matice
 */
void matrix_Null(float*A);

/**
 * @brief Vytvori jednotkovou matici
 *
 * @param A matice
 */
void matrix_Identity(float*A);

/**
 * @brief Vytvori translacni matici
 *
 * @param M matice
 * @param T vektor posunu
 */
void matrix_Translate(float*M,float*T);

/**
 * @brief Vytvori meritkovou matici
 *
 * @param M matice
 * @param S meritko
 */
void matrix_Scale(float*M,float*S);

/**
 * @brief Vytori rotacni matici kterou se musi nasobit zleva!
 *
 * @param M matice
 * @param RV rotacni vektor
 * @param a uhel
 */
void matrix_Rotate(float*M,float*RV,float a);

/**
 * @brief Presune matici
 *
 * @param A vstup
 * @param B vystup
 */
void matrix_Mov(float*A,float*B);

/**
 * @brief Secte matice
 *
 * @param A vysledna matice
 * @param B druha matice
 */
void matrix_Add(float*A,float*B);

/**
 * @brief Odecte matice
 *
 * @param A vysledna matice
 * @param B druha matice
 */
void matrix_Sub(float*A,float*B);


/**
 * @brief Vynasobi matice
 *
 * @param A prvni matice
 * @param B druha matice
 */
void matrix_Mul(float*A,float*B);

/**
 * @brief Vynasobi matici konstantou
 *
 * @param A Matice
 * @param K konstanta
 */
void matrix_KMul(float*A,float K);

/**
 * @brief Ziska radek z matice
 *
 * @param V vystupni radek (vektor3)
 * @param M matice
 * @param I cislo radku 0-3
 */
void matrix_GetRow(float*V,float*M,int I);

/**
 * @brief Ziska sloupec z matice
 *
 * @param V vystupni sloupec (vektor3)
 * @param M matice
 * @param I cislo sloupce 0-3
 */
void matrix_GetCol(float*V,float*M,int I);

/**
 * @brief Nastavi radek matice
 *
 * @param M matice
 * @param V radek (vektor3)
 * @param I cislo radku
 */
void matrix_SetRow(float*M,float*V,int I);

/**
 * @brief Nastavi sloupec matice
 *
 * @param M matice
 * @param V sloupec (vektor3)
 * @param I cislo sloupce
 */
void matrix_SetCol(float*M,float*V,int I);

/**
 * @brief Transponuje matici
 *
 * @param M matice
 */
void matrix_Transpose(float*M);

/**
 * @brief Vynasobim vektor matici
 *
 * @param O vystupni vektor
 * @param M matice
 * @param V vstupni vektor
 * @param W 4. komplonenta vektoru
 */
void matrix_VectorMul(float*O,float*M,float*V,float W);

/**
 * @brief Vypocita TRS matici (translate rotate scale)
 *
 * @param M vystupni matice
 * @param T translace
 * @param R rotacni vektor
 * @param A uhel
 * @param S meritko
 */
void matrix_TRSMatrix(float*M,float*T,float*R,float A,float*S);

#endif//_MATRIX_H_
