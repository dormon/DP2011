#ifndef _STDMATH_H_
#define _STDMATH_H_

#include<math.h>
#include"standard.h"
#include"myfloat.h"

#define MY_PI 3.14159265358979323846
#define MY_LN2 0.69314718055994530942

/**
 * @brief Oreze hodnotu do intervalu
 *
 * @param X hodnota
 * @param Min minimum
 * @param Max maximum
 *
 * @return vraci X pokud X>=Min && X<=Max; Min pokud X<Min; Max pokud X>Max
 */
float mymath_Clamp(float X,float Min,float Max);

/**
 * @brief Zjisti minimalni a maximalni hodnotu pole
 *
 * @param Min minimum
 * @param Max maximum
 * @param F data
 * @param Len delka dat
 */
void mymath_MinMax(float*Min,float*Max,float*F,unsigned Len);

/**
 * provede normalizaci dat do hranic
 * @param F data
 * @param Len delka dat
 * @param Min minimalni hodnota
 * @param Max maximalni hodnota
 */
void mymath_Normalize(float*F,unsigned Len,float Min,float Max);



/*
#ifndef sin
#define IMPLEMENTsin
**
 * @brief funkce sin
 *
 * @param Alpha uhel v radianech
 *
 * @return vraci funkcni hodnotu
 *
float sin(float Alpha);
#endif//sin
*/
#endif//_STDMATH_H_
