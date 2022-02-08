#ifndef _MAP_H_
#define _MAP_H_

#include "adt.h"
#include "myfloat.h"

/**
 * mapovaci kousek
 */
typedef struct
{
	float From;//zacatek intervalu
	float Len;//delka intervalu
	float MapFrom;//premapovat From na tuto hodnotu
	float MapLen;//delka premapovaciho intervalu
}SMapPart;

/**
 * Mapovaci struktura
 */
typedef struct
{
	SList2 L;//seznam kousku pro premapovani
	float From;//zacatek
	float Len;//delka
}SMap;

/**
 * Inicializuje prechod
 */
void map_Init(SMap*Map);

/**
 * Funkce premapuje danou hodnotu
 * @param Value hodnota
 * @param Map typ mapovani
 * @vraci hodnotu v intervalu 0,1
 */
float map_Map(float Value,SMap Map);

/**
 * Vytvori mapovaci strukturu jako prechod mez dvemi jinymi
 * pokd je Val cislo vytvori se (pri mezere mezi prechody)
 * konstantni prechod,  v pripade ze Val neni cislo, vytvori
 * linearni prechod
 * @param Out vystupni prechod (mezi pocatkem a koncem)
 * @param From pocatecni prechod
 * @param To koncovy prechod
 * @param Val hodnota konstanty nebo necislo
 * @return vraci 0 v pripade uspechu
 */
int map_Between(SMap*Out,SMap From,SMap To,float Val);

/**
 * Spoji dva prechody
 * @param Out prechod vystupni; spojeny se vstupnim
 * @param In prechod vstupni
 * @param Val notanumber->linearni prechod, konst->konst. prechod v mezere
 * @return vraci 0 v pripade uspechu
 */
int map_Join(SMap*Out,SMap In,float Val);


/**
 * prevede prechod do novych mezi
 * @param M prechod
 * @param From novy zacatek
 * @param Len nova delka
 */
void map_Normalize(SMap*M,float From,float Len);

/**
 * Uvolni prechod
 * @param Map prechod
 */
void map_Free(SMap*Map);

/**
 * Nahraje data o prechodu
 * 4BYTE pocet casti
 * 4*float od, delka, mapovat od, mapovaci delka
 * @param Map prechod
 * @param Data pole dat
 * @return vraci pocet prectenych bytu nebo cislo mensi 0
 */
int map_Load(SMap*Map,unsigned char*Data);

/**
 * @brief Vytvori kanal pro obarvovaci prechod
 *
 * @param Map prechod
 * @param V hodnota stredu
 */
void map_GenerateColorization(SMap*Map,float V);

#endif//_MAP_H_
