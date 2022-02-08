#ifndef _FONT_H_
#define _FONT_H_

#include"standard.h"

enum ECharName{
 __=0,
 _A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P,_Q,_R,_S,_T,_U,_V,_W,_X,_Y,_Z,
 _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,
 _AA,_EE,_II,_OO,_UU,_YY,
 _ZZ,_SS,_CC,_RR,_DD,_TT,_NN,
 _EEE,_UUU,
 _SCR,
 _QUE,
 _DOT,
 _COL,
 _QUO,
 _COM
};

/**
 * Reprezentuje jeden znak
 */
typedef struct{
	float Point[8];/// rohy ctverce
	int GLTex;/// texturovaci index
}SFontChar;

/**
 * Vytvori font; nutno zavolat jednou na zacatku programu
 */
void font_Create();

/**
 * Uvolni font; zavolat jednou na konci programu
 */
void font_Delete();

/**
 * Vytiskne text
 * @param Text data textu
 */
void font_PrintLine(char*Text);

/**
 * Vytiskne text na danych souradnicich s urcenou velikosti a barvou
 * @param Text date textu
 * @param x souradnice x
 * @param y souradnice y
 * @param w velikost pisma
 * @param Color barva pisma
 */
void font_PrintLineEx(char*Text,float x,float y,float w,
											float Color[3]);

/**
 * Vycentruje text
 * @param Text text
 * @param W velikost pisma
 */
void font_CenterStart(char*Text,float W);
/** 
 * Konec centorvani textu
 * @param text Text
 * @param W velikost pisma
 */
void font_CenterEnd(char*Text,float W);

/**
 * Vycentruje text horizontalne
 * @param W velikost pisma
 */
void font_CenterYStart(float W);

/** 
 * Konec centorvani textu
 * @param W velikost pisma
 */
void font_CenterYEnd(float W);

/**
 * Sirka textu
 * @param Text text
 * @param Velikost pisma
 * @return vraci sirku textu
 */
float font_Width(char*Text,float W);

/**
 * Zjisti velikost pisma aby se veslo do delky
 * @param Text text
 * @param Len celkova delka
 * @return vraci velikost pisma
 */
float font_Size(char*Text,float Len);


#endif//_FONT_H_
