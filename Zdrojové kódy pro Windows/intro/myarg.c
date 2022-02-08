#include"myarg.h"

int myarg_Val[myarg_NUM]={
	1366,
	766,
	1,
	512,
	2048,
	64
};

void myarg_Load(int Argc,char*Argv[]){
	Argc=fmin(Argc-1,myarg_NUM);//nacteme jen tolik argumentu kolik muzeme
	for(int i=1;i<=Argc;++i)myarg_Val[i-1]=StrToInt(Argv[i]);//nacitame
}

