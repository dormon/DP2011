#ifndef _MYARG_H_
#define _MYARG_H_

#include"standard.h"

//pocet argumentu
#define myarg_NUM 6

/**
 * @brief Type argumetnu
 */
enum myarg_ArgType{
	ARG_WIDTH=0,
	ARG_HEIGHT,
	ARG_FULLSCREEN,
	ARG_SKYBOX,
	ARG_TERRAINTEXTURE,
	ARG_WATER
};

extern int myarg_Val[myarg_NUM];

/**
 * @brief Funkce nahraje argumenty
 *
 * @param Argc pocet argumentu
 * @param Argv[] argumenty
 */
void myarg_Load(int Argc,char*Argv[]);

#endif//_MYARG_H_
