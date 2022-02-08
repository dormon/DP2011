/**
 * 64kB opengl intro
 * zdrojovy soubor pro font
 * @author Tomas Milet, xmilet01
 * @file font.c
 * @date 9.4.2012
 * @version 0.2
 */

#include "font.h"

//pocet znaku
#define NUMCHAR 57
//sirka znaku
#define WIDTHCHAR 6
//vyska znaku
#define HEIGHTCHAR 12
//posun textury po x
#define XOFFSET 5
//posun textury po y
#define YOFFSET 2

GLuint FontTexture[NUMCHAR];
GLuint FontList;

const unsigned char Font[]={
	0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,68,248,112,207,23,112,209,247,132,127,24,57,129,19,62,132,67,80,
	33,241,59,207,247,32,143,35,56,79,248,125,142,67,252,15,23,62,68,244,57,2,
	34,8,0,17,69,66,22,4,83,68,36,81,16,102,81,16,69,16,17,17,84,68,4,145,49,
	64,8,54,17,17,22,7,68,20,17,1,36,196,25,145,5,17,0,0,0,128,124,97,16,5,65,
	20,17,72,18,132,89,20,68,17,68,68,10,165,17,66,100,24,208,15,69,68,4,125,1,
	17,69,132,0,5,81,68,100,65,36,32,0,0,0,81,20,68,65,16,69,4,82,4,101,21,5,
	81,2,17,145,98,59,4,17,25,4,148,64,17,17,121,81,64,68,17,65,64,65,22,17,85,
	16,9,8,0,0,128,244,4,209,247,101,31,129,12,65,89,69,79,244,56,68,20,169,4,
	65,84,6,130,37,216,132,56,145,246,17,81,68,16,78,240,68,68,245,69,2,4,8,0,
	160,68,65,20,4,65,68,32,69,176,86,81,20,69,1,17,69,42,163,32,149,1,57,202,
	243,32,81,164,4,68,20,41,72,16,68,17,81,5,145,0,2,0,0,40,81,16,5,65,16,17,
	72,18,172,83,20,70,81,64,68,81,170,108,88,100,64,16,19,4,88,20,41,1,17,69,
	17,20,4,81,68,76,65,36,136,0,32,1,76,36,100,65,48,68,4,18,5,243,20,69,81,
	20,16,81,20,78,17,20,25,25,196,4,3,20,69,78,64,68,81,4,5,66,148,17,83,16,
	9,34,0,72,0,241,112,207,247,113,209,231,132,65,28,57,143,243,120,95,20,133,
	113,244,59,199,243,32,159,247,57,14,241,125,78,20,253,30,247,60,95,244,85,
	2,7,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,16,4,65,
	16,8,129,16,4,65,16,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,32,8,130,32,8,165,80,138,162,40,10,1,0,0,0,0
};

unsigned char MyGetPixel(unsigned i,unsigned x,unsigned y){
	unsigned PixelIndex=y*NUMCHAR*WIDTHCHAR;//cele radky
	PixelIndex+=i*WIDTHCHAR;//znaky pred
	PixelIndex+=x;//pixely ve znaku
	if((Font[PixelIndex/8]>>(PixelIndex%8))&1)return 255;
	return 0;
}

void font_CreateTexture(){
	unsigned char TextureData[16*16*4];//data jedne textury

	glUseProgram(0);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(NUMCHAR,FontTexture);//vygenerujeme textury
	for(unsigned i=0;i<NUMCHAR;++i)//projde znaky
	{
		for(unsigned j=0;j<16*16*4;++j)TextureData[j]=0;//inicializujeme
		for(unsigned y=0;y<HEIGHTCHAR;++y)
		for(unsigned x=0;x<WIDTHCHAR;++x)
		for(unsigned c=0;c<4;++c)
			TextureData[((y+YOFFSET)*16+x+XOFFSET)*4+c]=MyGetPixel(i,x,y);
		glBindTexture(GL_TEXTURE_2D,FontTexture[i]);//svazeme gl s danym znakem
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,16,16,0,GL_RGBA,GL_UNSIGNED_BYTE,
			TextureData);
	}

}


void font_Create(){
	glUseProgram(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	font_CreateTexture();//vytvorime textury pro jednotlive znaky
	return;
	FontList=glGenLists(NUMCHAR);//vygenerujeme listy
	for(unsigned i=0;i<NUMCHAR;++i)
	{
		glNewList(FontList+i,GL_COMPILE);

			glBindTexture(GL_TEXTURE_2D,FontTexture[i]);
			glBegin(GL_QUADS);
				glTexCoord2f((float)XOFFSET/16,(float)YOFFSET/16);
				glVertex2f(0,0);

				glTexCoord2f((float)(XOFFSET+WIDTHCHAR)/16,(float)YOFFSET/16);
				glVertex2f(WIDTHCHAR,0);

				glTexCoord2f((float)(XOFFSET+WIDTHCHAR)/16,
					(float)(YOFFSET+HEIGHTCHAR)/16);
				glVertex2f(WIDTHCHAR,HEIGHTCHAR);

				glTexCoord2f((float)XOFFSET/16,(float)(YOFFSET+HEIGHTCHAR)/16);
				glVertex2f(0,HEIGHTCHAR);
			glEnd();
		glEndList();
	}
}

void font_Delete(){
	glDeleteLists(FontList,NUMCHAR);//uvolni listy
}

void font_PrintLine(char*Text){
	for(int i=0;i<Text[0];++i){
		if(Text[i+1]==0)continue;
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE0);
	glDisable(GL_CULL_FACE);

		glTranslatef(8*i,0,0);
		//if(Text[i+1]>0)glCallList(FontList+Text[i+1]-1);
		if(glIsTexture(FontTexture[23])==GL_FALSE)fprintf(stderr,"NENI TEXTURA!!!!\n");
		//glColor3f(1,1,1);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D,FontTexture[Text[i+1]-1]);
		glBegin(GL_QUADS);
				glTexCoord2f((float)XOFFSET/16,(float)YOFFSET/16);
				glVertex2f(0,0);

				glTexCoord2f((float)(XOFFSET+WIDTHCHAR)/16,(float)YOFFSET/16);
				glVertex2f(WIDTHCHAR,0);

				glTexCoord2f((float)(XOFFSET+WIDTHCHAR)/16,
					(float)(YOFFSET+HEIGHTCHAR)/16);
				glVertex2f(WIDTHCHAR,HEIGHTCHAR);

				glTexCoord2f((float)XOFFSET/16,(float)(YOFFSET+HEIGHTCHAR)/16);
				glVertex2f(0,HEIGHTCHAR);
		glEnd();
		glTranslatef(-8*i,0,0);
	}
}


void font_PrintLineEx(
		char*Text,
		float x,
		float y,
		float w,
		float Color[3]){

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(x,y,0);
	glScalef(w,w,w);
	glColor3fv(Color);
	font_PrintLine(Text);
	glScalef(1/w,1/w,1/w);
	glTranslatef(-x,-y,0);
}

void font_CenterStart(char*Text,float W){
	glTranslatef(-W*8*Text[0]/2.0,0,0);
}

void font_CenterEnd(char*Text,float W){
	glTranslatef(W*8*Text[0]/2.0,0,0);
}

void font_CenterYStart(float W){
	glTranslatef(0,-W*HEIGHTCHAR/2.0,0);
}

void font_CenterYEnd(float W){
	glTranslatef(0,W*HEIGHTCHAR/2.0,0);
}

float font_Width(char*Text,float W){
	return W*Text[0];
}

float font_Size(char*Text,float Len){
	return Len/(float)(Text[0]*8);
}

