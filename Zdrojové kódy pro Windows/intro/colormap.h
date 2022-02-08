/**
 * 64kB opengl intro
 * hlavickovy soubor barevny prechod
 * @author Tomas Milet, xmilet01
 * @file colormap.h
 * @date 24.12.2009
 * @version 0.0
 */

#ifndef _COLORMAP_H_
#define _COLORMAP_H_

#include"map.h"
#include"color.h"

/**
 * Sestava nekolika slozek
 */
typedef struct{
	SMap*Map;/// jednotlive slozky
	unsigned Num;/// pocet slozek
}SColorMap;

/**
 * @brief Reprezentuje sablonu pro vygenerovani barevneho prechodu
 */
typedef struct{
	unsigned Seed;/// pocatecni seminko generatoru
	unsigned Num;/// pocet kroku v prechodu
	float StartHue;/// pocatecni obarveni
	float MaxStep;/// maximalni velikost kroku
}SColorMapTemplate;

/**
 * Nacte data z pole o prechodu
 * @param Map barevny prechod
 * @param Data pole s daty
 * @return vraci pocet prectenych bytu nebo cislo < 0
 */
int colormap_Load(SColorMap*Map,unsigned char*Data);

/**
 * @brief Jednoduchy nahravac prechodu
 *
 * @param Map barevny prechod RGB,
 * @param Key barevne klice (pomerova delka zbytku,fromR,fromG,fromB,toR,toG,toB)
 * @param NumKey pocet intervalu
 */
void colormap_SimpleLoad(SColorMap*Map,unsigned char*Key,unsigned NumKey);

/**
 * @brief Vygeneruje barevny prechod pro obarveni
 *
 * @param Map barevny prechod
 * @param Hue hodnota barvy HUE
 */
void colormap_GenerateColorization(SColorMap*Map,float Hue);

/**
 * @brief Vygeneruje barevny prechod podle sablony
 *
 * @param Map barvny prechod
 * @param Template sablona
 */
void colormap_Generate(SColorMap*Map,SColorMapTemplate*Template);

/**
 * Premapuje hodtu na barvu
 * @param Pixel vystup slozek barvy (RGB)
 * @param Value hodnota ze ktere se mapuje
 * @param Map barevny prechod prechod
 */
void colormap_Map(float*Pixel,float Value,SColorMap*Map);

/**
 * Prevede barevny prechod do novych mezi
 * @param M barevny prechod
 * @param From novy zacatek
 * @param Len nova delka
 */
void colormap_Normalize(SColorMap*M,float From,float Len);

/**
 * Uvolni barevny prechod z pameti
 * @param Map barevny prechod
 */
void colormap_Free(SColorMap*Map);

#endif//_COLORMAP_H_
