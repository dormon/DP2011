#ifndef _CAMERACONTROL_H_
#define _CAMERACONTROL_H_

#include"camera.h"
#include"standard.h"
#include"relist.h"

/**
 * @brief Reprezentuje kontrolni system pro kameru
 */
typedef struct{
	SCam*Camera;/// kamera
	SReList Key;/// klicove body
	unsigned NumKey;/// pocet klicovych bodu
	int Circular;/// kruhova cesta
	float MaxTime;/// delka drahy v sekundach
	float Time;/// aktualni cas
}SCameraControl;

/**
 * @brief Inicializuje kontrolni system pro kameru
 *
 * @param CameraControl kontorlni system pro kameru
 * @param Camera kamera
 * @param Key klicovy bod (3 pozice,3 vektor pohledu,3 vektor nahoru)
 * @param NumKey pocet klicovych bodu
 * @param MaxTime maximalni cas
 * @param Circular Cyklicka cesta
 */
void cameracontrol_Init(
		SCameraControl*CameraControl,
		SCam*Camera,
		float*Key,
		unsigned NumKey,
		float MaxTime,
		int Circular);

/**
 * @brief Prepocita system
 *
 * @param CameraControl kontrolni system kamery
 * @param Dt delta casu v minisekundach
 */
void cameracontrol_Update(SCameraControl*CameraControl,unsigned Dt);

void cameracontrol_GetControl(float*D,SCameraControl*Control);

/**
 * @brief Uvolni kontrolni system kamery
 *
 * @param CameraControl kontrolni system kamery
 */
void cameracontrol_Free(SCameraControl*CameraControl);

void cameracontrol_Draw(SCameraControl*CameraControl);

void cameracontrol_Save(const char*File,const char*FilePath,const char*Name,SCameraControl*CameraControl);


#endif//_CAMERACONTROL_H_
