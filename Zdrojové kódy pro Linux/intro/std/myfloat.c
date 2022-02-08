/**
 * 64kB opengl intro
 * zdrojovy soubor pro porovnani floatu
 * @author Tomas Milet, xmilet01
 * @file myfloat.h
 * @date 17.2.2010
 * @version 0.0
 */

#include "myfloat.h"

//float FLOAT_DIFF=0.001;
float FLOAT_DIFF=0.01;

int float_Eq(float A,float B){
	return fabs(A-B)<FLOAT_DIFF;
}
