#ifndef _COLORBUFFER_H_
#define _COLORBUFFER_H_

#include"nsize.h"
#include"index.h"
#include"colormap.h"
#include"voronoi.h"
#include"midpoint.h"
#include"stdmath.h"
#include"mymath.h"
#include"localtransform.h"
#include"globaltransform.h"
#include"fastgetvalue.h"

/**
 * @brief Typ zakladniho bufferu
 */
enum EBaseBufferType{
	BB_EMPTY=0,
	BB_MIDPOINT,
	BB_VORONOID
};

enum EColorBufferMixType{
	MT_ADD=0,
	MT_MUL,
	MT_DIFF,
	MT_A_PLUS_B_BY_A_ALPHA,
	MT_A_PLUS_B_BY_B_ALPHA,

	MT_SUB
};

enum ETrasformType{
	TT_INVERT=0,//prvni prvek pixelovych transformace
	TT_FLOWERS,
	TT_SCRAP,
	TT_THRESHOLD,
	TT_WHEEZING,//posledni prvek pixelovych transformaci
	TT_SMOOTH,//prvni prvek globalnich transformaci
	TT_EDGE//posledni prvek globalnich transformaci
};

/**
 * @brief Vygeneruje parametry voronoi diagramu
 *
 * @param Size velikost diagramu
 * @param H hustota bodu
 *
 * @return vraci parametry pro vygenerovani
 */
void*colorbuffer_GenVoronoiParam(SNSize*Size,float H);

/**
 * @brief Barevny buffer pro textury
 */
typedef struct{
	unsigned Channels;///pocet kanalu
	SNSize Size;///rozmery v dimenzich
	float**Data;///data rozdelena do kanalu
	unsigned char*InterleavedData;///data s prokladanymi kanaly
}SColorBuffer;

/**
 * @brief Inicializuje zakladni buffer
 *
 * @param Buf buffer
 * @param Type typ bufferu
 * @param Size rozmery v dimenzich
 * @param Parameters parametry pro metodu vytvarejici obsah bufferu
 */
void colorbuffer_Init(SColorBuffer*Buf,enum EBaseBufferType Type,
		SNSize*Size,void*Parameters);

/**
 * @brief Prekopiruje buffer
 *
 * @param Out vystupni buffer
 * @param In vstupni buffer
 */
void colorbuffer_Copy(SColorBuffer*Out,SColorBuffer*In);


/**
 * @brief Alokuje buffer po inicializovan (chceme prazdny buffer)
 *
 * @param Buf buffer
 * @param Size rozmery v dimenzich
 * @param Channels pocet kanalu
 */
void colorbuffer_Alloc(SColorBuffer*Buf,unsigned Channels,SNSize*Size);

/**
 * @brief Provede kolorizaci zakladniho bufferu dle typu
 *
 * @param ColorBuffer barevny buffer
 * @param BaseBuffer zakladni buffer
 * @param Map barevny prechod
 */
void colorbuffer_ToColor(SColorBuffer*ColorBuffer,SColorBuffer*BaseBuffer,
		SColorMap*Map);


/**
 * @brief Provedee smichani bufferu
 *
 * @param Out vystup
 * @param A vstup A
 * @param B vstup B
 * @param Type typ smichani
 */
void colorbuffer_Mix(SColorBuffer*Out,SColorBuffer*A,SColorBuffer*B,
		enum EColorBufferMixType Type);

/**
 * @brief Vynasobi hodnoty v bufferu konstantou
 *
 * @param Buf buffer
 * @param K konstanta
 */
void colorbuffer_KMul(SColorBuffer*Buf,float K);

/**
 * @brief Ziska rozsah hodnot v bufferu v danem kanalu
 *
 * @param Min minimalni hodnota
 * @param Max maximalni hodnota
 * @param Buf buffer
 * @param Channel kanal
 */
void colorbuffer_Range(float*Min,float*Max,SColorBuffer*Buf,unsigned Channel);

/**
 * @brief Provede transformaci (aplikuje funkci)
 *
 * @param Out vysup
 * @param In vstup
 * @param Type typ transformace
 * @param Channel ktery kanal transformujeme
 * @param Param parametry transformace
 */
void colorbuffer_Transform(SColorBuffer*Out,SColorBuffer*In,
		enum ETrasformType Type,unsigned Channel,void*Param);

/**
 * @brief Sestavi novy buffer ze stavajicich
 *
 * @param Out vystupni buffer
 * @param In vstupni buffery
 * @param Source ze ktereho vstupniho se berou data pro dany kanal
 * @param Channel ze ktereho kanalu vstupniho bufferu se berou data
 * @param Num pocet kanalu vystupniho bufferu
 */
void colorbuffer_Compose(SColorBuffer*Out,SColorBuffer**In,
		unsigned*Source,unsigned*Channel,unsigned Num);

/**
 * @brief Omezi data do kruhu
 *
 * @param Buffer buffer
 */
void colorbuffer_Circle(SColorBuffer*Buffer,float Pow);

/**
 * @brief Presype data do prokladaneho pole
 *
 * @param Out buffer
 */
void colorbuffer_Update(SColorBuffer*Out);

/**
 * @brief Vytvori normalovou mapu
 *
 * @param Out normalova mala
 * @param Buf vstupni buffer
 * @param Channel ze ktereho kanalu vstupniho bufferu pocitam
 */
void colorbuffer_Normal(SColorBuffer*Out,SColorBuffer*Buf,unsigned Channel);

/**
 * @brief Vytvori bumb mapu
 *
 * @param Bump vystupni bumb mapa
 * @param I vstupni buffer generovany colorbuffer_Normal funkci
 */
void colorbuffer_Bump(SColorBuffer*Bump,SColorBuffer*I);

/**
 * @brief Spocita sekvenci 2D bump-map v 3D texture
 *
 * @param Out vystupni sekvence 2D bump-map (animovana bumpama)
 * @param A vstupni vychyleni
 * @param Channel ktery kanal vstupu se bere pro vychyleni normal
 */
void colorbuffer_Normal2D3D(SColorBuffer*Out,SColorBuffer*A,unsigned Channel);

/**
 * @brief Ziska hodnotu na souradnicich + bikubicke vyhlazeni
 *
 * @param B ColorBuffer
 * @param P Pozice
 * @param Channel kanal
 *
 * @return vraci interpolovanou hodnotu na danych souradnicich
 */
float colorbuffer_GetValue(SColorBuffer*B,float*P,unsigned Channel);

/**
 * @brief Spocita light mapu pro ambient occlusion pracuje na 0. kanale
 *
 * @param Out vystupni lightmapa
 * @param In vstupni buffer hustoty
 * @param Num pocet paprsku
 * @param Threshold Hodnota prahu telesa
 */
void colorbuffer_AA(SColorBuffer*Out,SColorBuffer*In,
		unsigned Num,float Threshold);

/**
 * @brief Uvolni buffer
 *
 * @param Buf buffer
 */
void colorbuffer_Free(void*Buf);

#endif//_COLORBUFFER_H_
