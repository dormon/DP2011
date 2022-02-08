/**
 * 64kB opengl intro
 * zdrojovy soubor pro standardni funkce
 * @author Tomas Milet, xmilet01
 * @file standard.c
 * @date 23.12.2009
 * @version 0.0
 */

#include "standard.h"

#define MYRAND_MAX 2147483647
unsigned myseed=1;

GLenum ToTextureTarget[]={
	GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D
};

GLuint ToTextureFormat[]={
	GL_LUMINANCE,GL_LUMINANCE,GL_RGB,GL_RGBA
};

int randrand();

float Random(float Min,float Max)
{
	return Min+(Max-Min)*randrand()/(float)MYRAND_MAX;
}

void RandomSeed(unsigned Seed)
{
	myseed=Seed;
}

/*
 * http://ftp.gnu.org/gnu/glibc/glibc-2.9.tar.gz
 * stdlib/rand_r.c - int rand_r(unsigned int*seed)
 * @return vraci pseoudonahodne cislo
 */
int randrand ()
{
	unsigned int next = myseed;
	int result;

	next *= 1103515245;
	next += 12345;
	result = (unsigned int) (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	myseed = next;

	return result;
}

int StrLen(char*Text){
	int Length=0;//delka
	while(Text[Length]!='\0')++Length;//dokud nejsme na konci zvysujeme
	return Length;//navrat delky
}

int StrCmp(char*A,char*B){
	int i=0;//prave zpracovavany znak
	do{//provadej porovnavani
		if(A[i]<B[i])return-1;//znak z A je mensi => -1
		else if(A[i]>B[i])return 1;//znak z B je mensi => 1
	}while(A[i]!='\0'||B[i]!='\0');//dokud nejsme na konci
	if(A[i]=='\0'){//A skoncilo
		if(B[i]=='\0')return 0;//i B skonci => 0
		else return-1;//B neskoncilo => -1
	}
	return 1;//A neskoncilo => 1
}

int StrToInt(char*Text){
	int Length=StrLen(Text);//delka textu
	int Result=0;//vysledek
	for(int i=0;i<Length;++i){//projdemene znaky
		Result*=10;//posuneme predchazejici vysledek
		Result+=Text[i]-'0';//pricteme dalsi digit
	}
	return Result;//navrat vysledku
}

//GL extensions
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;

PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;

PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM1FPROC glUniform1f;

#if defined(__WIN32__)
PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
#endif

PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
PFNGLPOINTPARAMETERFPROC glPointParameterf;


PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

