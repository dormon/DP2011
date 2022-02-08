/**
 * 64kB opengl intro
 * zdrojovy soubor pro dvousmerny list
 * @author Tomas Milet, xmilet01
 * @file list2.c
 * @date 23.12.2009
 * @version 0.0
 */

#include "list2.h"

const SADTFCE LIST2_ADTFCE=
{
	/*.Free=*/list2_Free,
	/*.Copy=*/list2_Copy,
	/*.Cmp=*/list2_Cmp,
	/*.Size=*/sizeof(SList2)
};

void list2_Init(SList2*L,SADTFCE*F)
{
	L->Border[0]=L->Border[1]=NULL;
	L->F=F;
	L->Lenght=0;
}

int list2_Insert(SList2*L,int B)
{
	struct SList2Item*Carry;//Novy prvek
	if(!(Carry=(SList2Item*)mymem_Alloc(sizeof(struct SList2Item))))return 1;//alokace
	Carry->Data=NULL;//vlozeni dat
	Carry->NextTo[!B]=L->Border[B];//svazani se seznamem
	Carry->NextTo[B]=NULL;//zarazka
	if(!L->Border[B])L->Border[!B]=Carry;//prvni vlozeny prvek
	else L->Border[B]->NextTo[B]=Carry;//navazeme
	L->Border[B]=Carry;
	++L->Lenght;
	return 0;
}

void list2_Delete(SList2*L,int B)
{
	if(!L->Border[B])return;//seznam je prazdny
	struct SList2Item*Carry=L->Border[B]->NextTo[!B];//zaloha vedlejsiho prvku
	adt_FreeData(L->Border[B]->Data,L->F);//uvolnime data
	mymem_Free((void**)&L->Border[B]);//uvolni se prvek
	L->Border[B]=Carry;//provaze se seznam
	if(!Carry)L->Border[!B]=NULL;//nastavi se hranice
	else Carry->NextTo[B]=NULL;//nema jednoho souseda
	--L->Lenght;
}

int list2_InsertData(SList2*L,int B)
{
	if(list2_Insert(L,B))return 1;//vytvorime prvek
	if((L->Border[B]->Data=mymem_Alloc(L->F->Size))==NULL)//alokujeme data
	{
		list2_Delete(L,B);//uvolnime vytvoreny prvek
		return 1;//chyba
	}
	return 0;//proslo
}

int list2_OrderInsert(SList2*L,void*Data)
{
	struct SList2Item*Carry;//novy prvek
	struct SList2Item*Carry1;//prvek pro pruchod seznamem
	if(!(Carry=(SList2Item*)calloc(1,sizeof(struct SList2Item))))return 1;//alokace
	Carry->Data=Data;//priradime data
	if(list2_IsEmpty(*L))//pokud neni zadny prvek
	{
		L->Border[0]=L->Border[1]=Carry;//priradime zacatku i konci
	}
	else
	{
		Carry1=L->Border[0];//zacatek seznamu
		while(Carry1!=NULL&&L->F->Cmp(Carry1->Data,Carry->Data)<0)
			Carry1=Carry1->NextTo[1];
		if(Carry1==NULL)//prosly se vsechny prvky
		{
			Carry->NextTo[0]=L->Border[1];
			L->Border[1]->NextTo[1]=Carry;
			L->Border[1]=Carry;
		}
		else
		{
			Carry->NextTo[1]=Carry1;
			Carry->NextTo[0]=Carry1->NextTo[0];
			Carry1->NextTo[0]=Carry;
			if(Carry->NextTo[0])Carry->NextTo[0]->NextTo[1]=Carry;
			else L->Border[0]=Carry;
		}
	}
	++L->Lenght;
	return 0;//proslo
}

int list2_Join(SList2*L,SList2 B)
{
	SList2 C;//pomocny seznam
	if(list2_Copy(&C,&B))return 1;//zkopirujeme si seznam B
	C.Border[0]->NextTo[0]=L->Border[1];//navazeme predchudce
	L->Border[1]->NextTo[1]=C.Border[0];//navazeme naslednika
	L->Border[1]=C.Border[1];//posledni prvek je posledni prvek B
	return 0;//konec
}

int list2_IsEmpty(SList2 L)
{
	return L.Lenght==0;
}

void list2_Free(void*L)
{
	while(!list2_IsEmpty(*(SList2*)L))list2_Delete((SList2*)L,0);
	((SList2*)L)->Lenght=0;
}

int list2_Copy(void*L,void*B)
{
	struct SList2Item*Carry=((SList2*)B)->Border[0];//zacatek
	list2_Init((SList2*)L,((SList2*)B)->F);//inicializace
	while(Carry)//dokud existuje prvek
	{
		if(list2_InsertData((SList2*)L,1)||
			adt_CopyData(((SList2*)L)->Border[1]->Data,Carry->Data,((SList2*)B)->F))
		{//nastala chyba
			list2_Free(L);
			return 1;
		}
		Carry=Carry->NextTo[1];//nasledujici prvek
	}
	return 0;//konec
}

int list2_Cmp(void*A,void*B)
{
	SList2*a=(SList2*)A,*b=(SList2*)B;//pomocne promenne pro zkraceni zapisu
	struct SList2Item*ai,*bi;//prvky seznamu
	if(a->Lenght<b->Lenght)return-1;//seznam a je kratsi
	else if(a->Lenght>a->Lenght)return 1;//seznam a je delsi
	else//seznamy jsou stejne dlouhe
	{
		ai=a->Border[0];bi=b->Border[0];//zacatky seznamu
		while(ai&&!adt_CmpData(ai,bi,a->F))//prochazi prvky seznamu
		{
			ai=ai->NextTo[1];//nasledujici prvek prvniho seznam
			bi=bi->NextTo[1];//nasledujici prvek druheho seznamu
		}
		if(ai)return a->F->Cmp(ai,bi);//nektere prvky nejsou stejne
	}
	return 0;//vsechno je stejne
}

int list2_Map(SList2*L,ADT_MAP Map,void*Arg)
{
	struct SList2Item*Carry=L->Border[0];//prvni prvek
	while(Carry)//dokud je prvek
	{
		if(Map(Carry->Data,Arg))return 1;//namapuj funkci
		Carry=Carry->NextTo[1];//posun se v seznamu
	}
	return 0;
}


