/**
 * 64kB opengl intro
 * zdrojovy soubor pro generovani midpoint sumu
 * @author Tomas Milet, xmilet01
 * @file midpoint.c
 * @date 11.11.2011
 * @version 0.2
 */

#include "midpoint.h"


float*midpoint_Generate(SNSize*NSize,float RandInt,float Smooth)
{
	float*Data=NULL;//polo pro generovani
	float E;//soucet 1/(1+vzdalenosti soucenich bodu)
	float RI;//interva
	unsigned Size;//velikost pole
	unsigned*P;//misto pro nasobky
	unsigned*NIndex;//index v danem rozmeru
	unsigned Index;//prubezny index
	unsigned j;//pomocna promenna pro cyklus
	unsigned Base;//tam kde jsou jednicky tam jsou sousede v N prostoru
	unsigned Num1;//pocet jenicek
	unsigned r=1;//aktualni velikost poloviny
	int l;//pomocna promenna cyklu
	Size=nsize_Size(NSize);
	unsigned D=NSize->Dimension;
	unsigned*Level=NSize->Size;
	if((P=(unsigned*)mymem_Alloc(sizeof(unsigned)*D))==NULL)goto FR0;
	if((NIndex=(unsigned*)mymem_Alloc(sizeof(unsigned)*D))==NULL)goto FR1;
	if((Data=(float*)mymem_Alloc(Size*sizeof(float)))==NULL)goto FR2;
	Size=0;//vynulujeme velikost
	for(unsigned i=0;i<D;++i)P[i]=1;//nastavime jednicky pro roznasobeni
	for(j=0;j<D;++j)
	for(unsigned i=j+1;i<D;++i)P[i]*=Level[j];//spocita nasobky rozmeru
	for(unsigned d=0;d<D;++d)if(Size<Level[d])Size=Level[d];//nejvet. velikost
	
	//while(r<Size)r<<=1;//najde prvni vyssi mocninu
	//r>>=1;//v r je nejvetsi polovina
	r=(unsigned)pow(2,ilogb(Size));//v r je nejvetsi polovina
	Data[0]=Random(-RandInt,RandInt);//vygeneruje pocatecni bod
	RandInt/=2;
	while(r>0)//prochazi jednotlive pulky
	{
		for(unsigned i=1;i<=D;++i)//prochazi i rozmerne elementy
		{
			Base=(1<<i)-1;//inicializujeme
			for(;;)//prochazi rozmery i rozmernych elementu
			{
				for(j=0;j<D;++j)NIndex[j]=((Base>>j)&1)*r;
				for(;;)
				{
					RI=RandInt;
					Index=0;
					for(unsigned k=0;k<D;++k)
					{
						if(NIndex[k]>=Level[k])goto konec;//preteklo -> konec
						Index+=NIndex[k]*P[k];//Spocita index
					}
					Data[Index]=0;
					E=(float)i/(1+r);//vzdalenost sousedu s mensim indexem
					for(unsigned e=0;e<D;++e)//spocitame E
						if((Base>>e)&1)
						{
							if((NIndex[e]+r)<Level[e])E+=1./(1+r);//nepresahuje
							else
							{
								E+=1./(1+Level[e]-NIndex[e]);//presahuje
								if(r>1+Level[e]-NIndex[e])
								RI=fminf(RI,RandInt/pow(Smooth,//spocita minimalni rozsah
									(unsigned)(log(r-1-Level[e]+NIndex[e])/MY_LN2)));
							}
						}
					for(unsigned e=0;e<D;++e)//pripocteme vahove nasobene sousedy
						if((Base>>e)&1)
						{
							Data[Index]+=Data[Index-r*P[e]]/(1+r)/E;//mensi index
							if((NIndex[e]+r)<Level[e])//soused nepresahuje
								Data[Index]+=Data[Index+r*P[e]]/(1+r)/E;
							else Data[Index]+=Data[Index-NIndex[e]*P[e]]/
								(1+Level[e]-NIndex[e])/E;//soused presahuje
						}
					Data[Index]+=Random(-RI,RI);//pripocteme nahodu
konec:		for(j=0;j<D;++j)//prochazi jednotlive indexy
					{//a pokousime se zvysit 
						if((NIndex[j]+2*r)>=Level[j])NIndex[j]=((Base>>j)&1)*r;//offset
						else//pricteme
						{
							NIndex[j]+=2*r;//zvysime index v danem rozmeru
							j=D+1;//konec inkrementace
						}
					}
					if(j==D)break;//po spravne inkrementaci ma byt nastevano na D+1
				}
				l=D-1;
				while((l>=0)&&((1<<l)&Base))--l;//najdeme nejnizsi 1 vlevo
				if((unsigned)l==D-i-1)break;//vsechny 1 jsou vlevo -> konec (11100)
				else if((unsigned)l==D-1)
				{//nejvyssi bit neni 1 (00101)
					l=1;
					while(Base>>l)++l;
					--l;//v l je ted index nejvyssi jednicky
					Base+=1<<l;//posuneme
				}
				else
				{//nejvyssi bit je 1, v l je index pozice prvni nuly (11001)
					Num1=D-l-1;//pocet jednicek vlevo
					while(!(Base&(1<<l)))--l;//najde pozici nejvyssi 1 ktera neni vlevo
					Base+=1<<l;//posuneme (11010)
					Base=(Base&((1<<(D-Num1))-1))|(((1<<Num1)-1)<<(l+2));//(01110)
				}
			}
		}
		RandInt/=Smooth;//vydelime interval nahody
		r/=2;//rozpulime polovinu
	}
	mymath_Normalize(Data,nsize_Size(NSize),0,1);//normalizace
FR2:
	mymem_Free((void**)&NIndex);//uvolnime nasobky
FR1:
	mymem_Free((void**)&P);
FR0:
	return Data;//v pripade chyby vraci NULL
}

