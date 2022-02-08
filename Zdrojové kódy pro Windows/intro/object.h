#ifndef _OBJECT_H_
#define _OBJECT_H_

#include"gpu.h"
#include"elastic.h"
#include"glslinit.h"
#include"geometry.h"

/**
 * @brief Reprezentuje objekt
 */
typedef struct{
	SElastic Elastic;/// elasticke data
	float Smooth;/// hladky povrch
	SGPUBuffer GPUDataPosition;/// pozice castic
	SGPUBuffer GPUDataNormal;/// normaly
	SGPUBuffer GPUDataCoord;/// tex. koordinaty
	SGPUBuffer GPUDataIndex;/// indexy
	SGPUAttribute PositionAttribute;/// pozicni atribut
	SGPUAttribute NormalAttribute;/// normaly atributy
	SGPUAttribute CoordAttribute;/// tex. koord atribut
	SGPUTexture Texture;/// textura objektu
	SGPUTexture BumpTexture;/// bump mapa
	SGPUShaderProgram*Program;/// kreslici program
}SObject;

/**
 * @brief Inicializuje objekt
 *
 * @param Object objekt
 * @param Collide kolizni system
 * @param Smooth hladky povrch (normaly se vyhlazuji)
 * @param Points body
 * @param Coords tex. koordinaty
 * @param NumPoints pocet bodu
 * @param Index indexy
 * @param NumIndex pocet indexu
 * @param Joints spoje
 * @param NumJoints pocet spoju
 * @param Texture textura
 * @param Program kreslici program
 */
void object_Init(SObject*Object,
		SCollide Collide,
		float Smooth,
		float*Points,float*Coords,unsigned NumPoints,
		unsigned*Index,unsigned NumIndex,
		unsigned*Joints,unsigned NumJoints,
		SGPUTexture Texture,SGPUTexture BumpTexture,
		SGPUShaderProgram*Program);

/**
 * @brief Vykresli objekt
 *
 * @param Object objekt
 */
void object_Draw(SObject*Object);

/**
 * @brief Prepocita objekt
 *
 * @param Object objekt
 * @param Dt delta casu
 */
void object_Update(SObject*Object,unsigned Dt);

/**
 * @brief Prepocita normaly objektu
 *
 * @param Object objekt
 */
void object_ComputeNormal(SObject*Object);

/**
 * @brief Uvolni objekt
 *
 * @param Object objekt
 */
void object_Free(SObject*Object);

#endif//_OBJECT_H_
