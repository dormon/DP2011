/**
 * 64kB opengl intro
 * zdrojovy soubor barevny prechod
 * @author Tomas Milet, xmilet01
 * @file colormap.c
 * @date 24.12.2009
 * @version 0.0
 */

#include "colormap.h"

void colormap_Free(SColorMap*Map)
{
	for(unsigned i=0;i<Map->Num;++i)
		map_Free(Map->Map+i);//uvolnime barvy
	mymem_Free((void**)&Map->Map);//uvolnime pole barev
	Map->Num=0;//zadne barvy
}


int colormap_Load(SColorMap*Map,unsigned char*Data)
{
	Map->Num=*((unsigned char*)Data);//nacteme pocet barev
	++Data;//posuneme se v datech
	int Read=0;//pocet prectenych bytu u barvy
	if((Map->Map=(SMap*)mymem_Alloc(sizeof(SMap)*Map->Num))==NULL)return 1;//chyba
	for(unsigned i=0;i<Map->Num;++i)//projdeme barvy
	{
		if((Read+=map_Load(Map->Map+i,Data+Read))<0)//pokud nastala chyba
		{
			colormap_Free(Map);
			return-1;
		}
	}
	return Read+4;//pocet prectenych bytu
}

void colormap_Normalize(SColorMap*M,float From,float Len)
{
	for(unsigned i=0;i<M->Num;++i)
		map_Normalize(M->Map+i,From,Len);
}

void colormap_Map(float*Pixel,float Value,SColorMap*Map)
{
	for(unsigned i=0;i<Map->Num;++i)
		Pixel[i]=map_Map(Value,Map->Map[i]);

}

void colormap_GenerateColorization(SColorMap*Map,float Hue){
	float Color[3];//barva stredu
	color_Hue(Color,Hue);//vybereme barvu z hue
	Map->Map=(SMap*)mymem_Alloc(sizeof(SMap)*3);//alokujeme kanaly
	for(unsigned i=0;i<3;++i)//projdeme slozky barvy
		map_GenerateColorization(Map->Map+i,Color[i]);//generejume pro slozku
	Map->Num=3;//tri barvy
}

void colormap_Generate(
		SColorMap*Map,
		SColorMapTemplate*Template){
/*		unsigned Num,
		float StartHue,
		float MaxStep){*/
	float StartHue=Template->StartHue;
	myseed=Template->Seed;//nastavime seed
	Map->Num=3;

	Map->Map=(SMap*)mymem_Alloc(sizeof(SMap)*Map->Num);
	float*Pos=(float*)mymem_Alloc(sizeof(float)*(Template->Num+1));
	float*Color=(float*)mymem_Alloc(sizeof(float)*3*(Template->Num+1));


	//vypocitame hue
	for(unsigned i=0;i<=Template->Num;++i){
		color_HSV(
				Color+i*3,
				StartHue,
				pow(Random(0,1),6),
				Random(0,(i+1.)/(Template->Num+1)));
		StartHue+=Random(-1,1)*Template->MaxStep;
	}

	//vypocitame pozice
	Pos[0]=0;
	for(unsigned i=1;i<=Template->Num;++i)
		Pos[i]=Pos[i-1]+Random(0.001,1);
	for(unsigned i=1;i<=Template->Num;++i)
		Pos[i]/=Pos[Template->Num];

	for(unsigned i=0;i<Map->Num;++i){
		SMap*M=Map->Map+i;
		M->Len=1;
		M->From=0;
		map_Init(M);
		for(unsigned k=0;k<Template->Num;++k){
				list2_InsertData(&M->L,1);
				SMapPart*P=(SMapPart*)M->L.Border[1]->Data;
				P->From=Pos[k];
				P->Len=Pos[k+1]-Pos[k];
				P->MapFrom=Color[k*3+i];
				P->MapLen=Color[(k+1)*3+i]-P->MapFrom;
				/*fprintf(stderr,"A: %f %f %f %f\n",
						P->From,P->Len,
						P->MapFrom,P->MapLen
						
						);*/
			}
	}
	colormap_Normalize(Map,0,1);
	mymem_Free((void**)&Color);
	mymem_Free((void**)&Pos);
}

void colormap_SimpleLoad(SColorMap*Map,unsigned char*Key,unsigned NumKey){
	Map->Num=3;//tri barvy
	Map->Map=(SMap*)mymem_Alloc(sizeof(SMap)*Map->Num);
	map_Init(Map->Map+0);
	map_Init(Map->Map+1);
	map_Init(Map->Map+2);
	float ActTime=0;
	SMapPart*P;
	for(unsigned i=0;i<NumKey;++i){//projdeme klice
		for(unsigned c=0;c<3;++c){//projdeme barvu
			list2_InsertData(&Map->Map[c].L,1);//vlozime do barvy nakonec dilek
			P=(SMapPart*)Map->Map[c].L.Border[1]->Data;//data dilku
			P->From=ActTime;
			P->Len=(1-ActTime)*Key[i*7+0]/255;
			P->MapFrom=Key[i*7+1+c]/255.;
			P->MapLen= Key[i*7+4+c]/255.-P->MapFrom;
		}
		ActTime+=P->Len;
	}
	for(unsigned c=0;c<3;++c){
		Map->Map[c].From=0;
		Map->Map[c].Len=1;
	}
	colormap_Normalize(Map,0,1);
}


