/**
 * 64kB opengl intro
 * zdrojovy soubor pro prototypy funkci
 * @author Tomas Milet, xmilet01
 * @file adtfce.c
 * @date 23.12.2009
 * @version 0.0
 */

#include "adtfce.h"

void adt_FreeData(void*D,SADTFCE*F)
{
	if(D)//existuji data?
	{
		if(F&&F->Free)F->Free(D);//ma to vnorenou strukturu -> uvolnime
		mymem_Free(&D);//uvolnime celkovy ukazatel na data
	}
}

int adt_CopyData(void*B,void*A,SADTFCE*F){
	if(F&&F->Copy){//existuje kopirovaci funkce
		if(F->Copy(B,A))return 1;//ma vnorenou strukturu
	}else//neexistuje - prekopiruje se byte po bytu
		mymem_Cpy(B,A,F->Size);
	return 0;
}

int adt_CmpData(void*A,void*B,SADTFCE*F){
	unsigned char*a=(unsigned char*)A,*b=(unsigned char*)B;//pomocne promenne pro zkraceny zapis
	unsigned i=0;//pomocna promenna pro cyklus
	if(F&&F->Cmp)return F->Cmp(A,B);//existuje porovnavaci funkce
	else while(i<F->Size&&a[i]==b[i])++i;//prochazi data pokud jsou stejna
	if(i==F->Size)return 0;//prosly se vsechny
	else if(a[i]<b[i])return -1;//TODO mozna mymem_Cmp
	return 1;
}

unsigned adt_ElemSize(SADTFCE*F){
	if(F)return F->Size;
	return sizeof(unsigned char);
}

