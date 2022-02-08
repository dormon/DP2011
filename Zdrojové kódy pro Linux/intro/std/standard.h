/**
 * 64kB opengl intro
 * hlavickovy soubor pro standardni funkce
 * @author Tomas Milet, xmilet01
 * @file standard.h
 * @date 23.12.2009
 * @version 0.0
 */

#ifndef _STANDARD_H_
#define _STANDARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include "../GL/gl.h"
#include "../GL/glext.h"
/*
#include<iostream>
using namespace std;
*/
//#include <stdio.h>
/*
#include "../GL/glu.h"
#define glError() {\
GLenum err=glGetError();\
	while(err!=GL_NO_ERROR){\
	fprintf(stderr,"glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
	err=glGetError();\
	}\
}
*/

#if defined(__WIN32__)
	#include <windows.h>
#else
	#include <SDL/SDL.h>
#endif//__LINUX__

//GL extensions
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM1FPROC glUniform1f;

#if defined(__WIN32__)
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
#endif

extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;


extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

extern GLenum ToTextureTarget[];
extern GLuint ToTextureFormat[];


extern unsigned myseed;

/**
 * @param Min hodnota od
 * @param Max hodnota do
 * @return nahodne cislo z rozsahu
 */
float Random(float Min,float Max);

/**
 * @param Seed seed
 */
void RandomSeed(unsigned Seed);

/**
 * @brief Zisti delku textu
 *
 * @param Text text
 *
 * @return delka textu
 */
int StrLen(char*Text);

/**
 * @brief Prevede text na cislo cislo ocekavano v desitkove soustave
 *
 * @param Text text pro prevod
 *
 * @return prevedene cislo
 */
int StrToInt(char*Text);


#endif//_STANDARD_H_
