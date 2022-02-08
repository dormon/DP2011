#include "map.h"

const SADTFCE MAPLISTITEM={
	/*.Free=*/NULL,
	/*.Copy=*/NULL,
	/*.Cmp=*/NULL,
	/*.Size=*/sizeof(SMapPart)
};

void map_Init(SMap*Map)
{
	list2_Init(&Map->L,(SADTFCE*)&MAPLISTITEM);
}

float map_Map(float V,SMap Map)
{
	struct SList2Item*Carry=Map.L.Border[0];//pro pruchod listem
	SMapPart*P=(SMapPart*)Carry->Data;//pomocna prom. pro zkraceny zapis
	while(V>(P->From+P->Len))//dokud je V mimo rozsah casti
	{
		Carry=Carry->NextTo[1];//prejdi na dalsi cast
		if(Carry)P=(SMapPart*)Carry->Data;//nove data
		else break;
	}
	if(!Carry)//pokud se prechod prekrocil
	{
		P=(SMapPart*)Map.L.Border[1]->Data;
		return P->MapFrom+P->MapLen;//vraceni posledni hodnoty
	}
	float t=(V-P->From)/P->Len;//zjisteni pomeru mezi zacatkem a koncem
	return P->MapFrom+t*P->MapLen;//navrat hodnoty
}

int map_Between(SMap*Out,SMap From,SMap To,float Val)
{
	SMapPart*P,*F,*T;//pomocne ukazatele pro kratsi zapis
	map_Init(Out);//inicializujeme vystup
	if(list2_InsertData(&Out->L,0))return 1;//vlozime prvek
	Out->From=From.From+From.Len;//zacatek je konec zacatku
	Out->Len=To.From-Out->From;//delka ja zacatek konce -  konec zacatku
	P=(SMapPart*)Out->L.Border[0]->Data;//ziskame ukazatel jedinneho prvku
	P->From=Out->From;//zacatek intervalu
	P->Len=Out->Len;//delka intervalu
	if(isnan(Val))
	{
		F=(SMapPart*)From.L.Border[1]->Data;//prirazeni pom. ukazatelum
		T=(SMapPart*)To.L.Border[0]->Data;//prirazeni pom. ukazatelum
		P->MapFrom=F->MapFrom+F->MapLen;//hodnota zacatku mapovani
		P->MapLen=T->MapFrom-P->MapFrom;//delka mapovani
	}
	else
	{
		P->MapFrom=Val;//hodnota je konstanta
		P->MapLen=0;//s zadnou zmenou
	}
	return 0;
}

int map_Join(SMap*Out,SMap In,float Val)
{
	SMapPart*P;//pomocny ukazatel pro zkraceny zapis
	if(list2_IsEmpty(Out->L))
	{//seznam Out je prazdny, nakopirujeme do nej In
		Out->From=In.From;//zacatek
		Out->Len=In.Len;//delka
		return list2_Copy(&Out->L,&In.L);//nakopirovani
	}
	else if(!list2_IsEmpty(In.L))
	{
		if(float_Eq(Out->From+Out->Len,In.From))
		{//konec out konci na zacatku in, staci spojit seznamy
			Out->Len+=In.Len;//mapova delka je soucet
			P=(SMapPart*)Out->L.Border[1]->Data;//prirazeni ukazatele
			P->Len=In.From-P->From;//pro odstraneni chyby
			return list2_Join(&Out->L,In.L);//spoji seznamy
		}
		else
		{//mezi prechody je mezera
			SMap Space;//mezera
			if(map_Between(&Space,*Out,In,Val))return 1;//vyplnime mezeru
			if(map_Join(Out,Space,Val)||map_Join(Out,In,Val))
			{
				map_Free(&Space);
				return 1;
			}
			map_Free(&Space);//uvolnime mezeru
		}
	}
	return 0;//konec
}

void map_Normalize(SMap*M,float From,float Len)
{
	SMapPart*P;//pomocny ukazatel pro zkraceny zapis
	struct SList2Item*Carry=M->L.Border[0];//prom. pro pruchod seznamem
	float Scale=Len/M->Len;//meritko
	while(Carry)//projdeme prvky
	{
		P=(SMapPart*)Carry->Data;
		P->From-=M->From;//odecteme zacatek
		P->From*=Scale;//vynasobime meritkem
		P->From+=From;//pricteme novy zacatek
		P->Len*=Scale;//vynasobime meritkem
		Carry=Carry->NextTo[1];//nasledujici prvek
	}
	M->From=From;//novy zacatek
	M->Len=Len;//nova delka
}

void map_Free(SMap*Map)
{
	list2_Free(&Map->L);
}

int map_Load(SMap*Map,unsigned char*Data)
{
	SMapPart*P;//pomocna prom. pro zkraceny zapis
	unsigned Cont=*((unsigned*)Data);//kontrolni cislo
	unsigned Len=0;//pocet radku
	map_Init(Map);//inicializujeme prechod
	Len=Cont&0x7fffffff;//delka
	Data+=4;//posuneme se v datech
	for(unsigned i=0;i<Len;++i)//projde casti
	{
		if(list2_InsertData(&Map->L,1))//vlozi prvek
		{//chyba pri alokaci
			map_Free(Map);//uvolni doposud alokovane prvky
			return-1;//konec
		}
		P=(SMapPart*)Map->L.Border[1]->Data;//prvek
		P->From=*((float*)Data);//nacteme zacatek
		Data+=4;//posuneme se v datech
		P->Len=*((float*)Data);//nacteme delku
		Data+=4;//posuneme se v datech
		if(Cont&0x80000000)
		{//nacitaji se byte
			P->MapFrom=(*Data)/255.;//priradime data
			++Data;//posuneme se v datech
			P->MapLen=(*Data)/255.;//priradime data
			++Data;//posuneme se v datech
		}
		else
		{//nacitaji se float
			P->MapFrom=*((float*)Data);//nacteme zacatek
			Data+=4;//posuneme se v datech
			P->MapLen=*((float*)Data);//nacteme delku
			Data+=4;//posuneme se v datech
		}
		P->MapLen-=P->MapFrom;//vypocitame delku
	}
	P=(SMapPart*)Map->L.Border[0]->Data;
	Map->From=P->From;
	P=(SMapPart*)Map->L.Border[1]->Data;
	Map->Len=P->From+P->Len-Map->From;
	return 4+Len*(10+6*(!(Cont>>31)));
}

void map_GenerateColorization(SMap*Map,float V){
	SMapPart*P;//pomocna prom. pro zkraceny zapis
	map_Init(Map);//inicializujeme
	Map->From=0;//od nuly
	Map->Len=1;//po jednicku
	list2_InsertData(&Map->L,1);//vlozime na konec prvek
	P=(SMapPart*)Map->L.Border[1]->Data;
	P->From=0;
	P->Len=.5;
	P->MapFrom=0;
	P->MapLen=V;
	list2_InsertData(&Map->L,1);//vlozime na konec prvek
	P=(SMapPart*)Map->L.Border[1]->Data;
	P->From=.5;
	P->Len=.5;
	P->MapFrom=V;
	P->MapLen=1-V;
}

