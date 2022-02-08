#ifndef _GPUSHADERPROGRAM_H_
#define _GPUSHADERPROGRAM_H_

#include"standard.h"
#include"mymem.h"
#include"gputextureunit.h"
/**
 * @brief Data shaderu
 */
typedef struct{
	GLuint*Attributes;/// atributy
	GLuint*Uniforms;/// uniformy
	SGPUTextureUnit*TextureUnits;/// texturovaci jednoty
	unsigned NumAttribute;/// pocet atributu
	unsigned NumUniform;/// pocet uniformu
	unsigned NumTextureUnit;/// pocet texturovacich jednotek
	GLuint Shader[3];/// shadery
	unsigned NumShader;/// pocet dilcich shaderu
	GLuint ShaderProgram;/// shader program
}SGPUShaderProgram;

/**
 * @brief Vytvori shader
 *
 * @param Text text shaderu
 * @param Type typ shader
 *
 * @return vraci ID shaderu
 */
GLuint gpu_CreateShader(char*Text,GLuint Type);

/**
 * @brief Vytvori shader program
 *
 * @param Shader pole shaderu
 * @param NumShader pocet shaderu
 *
 * @return vraci shader program
 */
GLuint gpu_CreateShaderProgram(GLuint*Shader,unsigned NumShader);

/**
 * @brief Inicializuje shader strukturu
 *
 * @param Shader struktura
 * @param Vertex vertex shader (NULL) ignoruje
 * @param Geometry geometry shader -||-
 * @param Fragment fragmet shader -||-
 * @param Attributes seznam atributu
 * @param NumAttributes pocet atribudu
 * @param Uniforms seznam uniformu
 * @param NumUniforms pocet uniformu
 * @param TextureUnits seznam texturovacich jednotek
 * @param Pocet Texturovacich jednotek
 */
void gpu_InitShaderProgram(SGPUShaderProgram*Shader,
		char*Vertex,char*Geometry,char*Fragment,
		char**Attributes,unsigned NumAttributes,
		char**Uniforms,unsigned NumUniforms,
		char**TextureUnits,unsigned NumTextureUnits);

/**
 * @brief Uvolni shader program
 *
 * @param Shader shader program
 */
void gpu_FreeShaderProgram(SGPUShaderProgram*Shader);

#endif//_GPUSHADERPROGRAM_H_
