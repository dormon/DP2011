#ifndef _SKYBOXGENERATE_H_
#define _SKYBOXGENERATE_H_

#include"gpu.h"

/**
 * @brief Reprezentuje oblohu
 */
typedef struct{
	float SunVector[3];/// pozice slunce
	float BaseColor[3];/// zakladni barva oblohy
	float StartColor[3];/// startovni barva oblohy
	float EndColor[3];/// koncova barva oblohy
	float HorizontColor[3];/// barva na horizonzu
	float Exp;/// exponent oblohy kolem slunce
}SSkyTemplate;

/**
 * @brief Funkce ziska barvu oblohy
 *
 * @param Color barva oblohy
 * @param Vector vektor pohledu
 * @param Template sablona oblohy
 */
void sky_GetColor(float*Color,float*Vector,void*Template);

/**
 * @brief Reprezentuje slunce
 */
typedef struct{
	float SunVector[3];/// vektor ke slunci
	float SunColor[3];/// barva slunce
	float SunSize;/// velikost slunce v radianech
	float SunBloom;/// korona
	float SunExp;/// sila hranice slunce
}SSunTemplate;

/**
 * @brief Funkce ziska barvu slunce 
 *
 * @param Color vystupni barva
 * @param Vector vektor pohledu
 * @param Template sablona slunce
 */
void sun_GetColor(float*Color,float*Vector,void*Template);

/**
 * @brief Reprezentuje mrak
 */
typedef struct{
	float Start[2];/// pocatecni pozice
	float Frequency;/// frekvence
	float Amplitude;/// amplituda
	float Persistence;/// perzistence
	unsigned Num;/// pocet poscitani
	float Density;/// hustota
	float Coverage;/// kryti
	float Color[3];/// barva mraku
}SCloudTemplate;

/**
 * @brief Ziska barvu mraku
 *
 * @param Color vystupni barva
 * @param Vector vektor pohledu
 * @param Template sablona mraku
 */
void cloud_GetColor(float*Color,float*Vector,void*Template);

/**
 * @brief Reprezentuje skybox element
 */
typedef struct{
	void*Template;/// sablona elementu
	void(*GetColor)(float*Color,float*Vector,void*Template);/// ziska barvu
}SSkyBoxElement;

/**
 * @brief Reprezentuje sablonu skyboxu
 */
typedef struct{
	unsigned Size;/// velikost textury
	SSkyBoxElement*SkyBoxElement;/// skybox element
	unsigned NumElement;/// poce skybox elementu
}SSkyBoxTemplate;

/**
 * @brief Vykresli skybox
 *
 * @param B skybox
 * @param Template skybox sablona
 */
void skybox_Render(SGPUTexture*B,SSkyBoxTemplate Template);

/**
 * @brief Vygeneruje skybox
 *
 * @param SkyBox textura skyboxu
 * @param Template sablona skyboxu
 */
void skybox_Generate(SGPUTexture*SkyBox,SSkyBoxTemplate Template);

/**
 * @brief Vytvori prazdny skybox
 *
 * @param SkyBox prazdny skybox
 * @param Size velikost prazdneho skyboxu
 */
void skybox_Empty(SGPUTexture*SkyBox,unsigned Size);

#endif//_SKYBOXGENERATE_H_
