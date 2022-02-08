#ifndef _COLOR_H_
#define _COLOR_H_

#include"vector.h"
#include"mymath.h"

/**
 * @brief Prevod barvy na sedou
 */
extern const float ToGray[];

/**
 * @brief Spocita hodnotu odstinu
 *
 * @param Color vystupni barva
 * @param Angle uhel ve stupnich
 */
void color_Hue(float*Color,float Angle);

/**
 * @brief Prevede HSV na RGB
 *
 * @param Color Vystupni barva v RGB
 * @param Angle Uhel Hue
 * @param Saturaion saturace
 * @param Value hodnota
 */
void color_HSV(float*Color,float Angle,float Saturaion,float Value);

#endif//_COLOR_H_
