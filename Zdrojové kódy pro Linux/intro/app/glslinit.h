#ifndef _GLSLINIT_H_
#define _GLSLINIT_H_

#include"gpu.h"
#include"glslprogram.h"

/// GLSL programy
extern SGPUShaderProgram Shader0_Cave;
extern SGPUShaderProgram Shader1_Particle;
extern SGPUShaderProgram Shader2_Object;
extern SGPUShaderProgram Shader3_Water;
extern SGPUShaderProgram Shader4_Terrain;
extern SGPUShaderProgram Shader5_SkyBox;
extern SGPUShaderProgram Shader6_SpiderWeb;

enum EAttributes0{
	ATT0_POSITION=0,
	ATT0_NORMAL
};
enum EUniforms0{
	UNI0_PROJECTION=0,
	UNI0_CAMERA,
	UNI0_SCENE,
	UNI0_LIGHTPOSITION0,
	UNI0_SETAO,
	UNI0_CAUSTICT
};
enum ETextureUnits0{
	UNIT0_SIDE=0,
	UNIT0_SIDEBUMP,
	UNIT0_DOWN,
	UNIT0_DOWNBUMP,
	UNIT0_UP,
	UNIT0_UPBUMP,
	UNIT0_SEDIMENT,
	UNIT0_NORAMB,
	UNIT0_WET,
	UNIT0_CAUSTIC
};

enum EAttributes1{
	ATT1_POSITION=0,
	ATT1_SIZE,
	ATT1_TTL,
	ATT1_NORMAL
};
enum EUniforms1{
	UNI1_PROJECTIONMATRIX=0,
	UNI1_CAMERAMATRIX,
	UNI1_SCENEMATRIX,
	UNI1_MAXTTL,
	UNI1_SPRITE
};
enum ETextureUnits1{
	UNIT1_TEXTURE=0
};

enum EAttributes2{
	ATT2_POSITION=0,
	ATT2_NORMAL,
	ATT2_COORD
};
enum EUniforms2{
	UNI2_PROJECTIONMATRIX=0,
	UNI2_CAMERAMATRIX,
	UNI2_SCENEMATRIX,
	UNI2_LIGHTPOSITION,
	UNI2_SMOOTH
};
enum ETextureUnits2{
	UNIT2_TEXTURE=0,
	UNIT2_BUMPTEXTURE
};

enum EAttributes3{
	ATT3_POSITION=0
};
enum EUniforms3{
	UNI3_PROJECTIONMATRIX=0,
	UNI3_CAMERAMATRIX,
	UNI3_SCENEMATRIX,
	UNI3_LIGHTPOSITION0,
	UNI3_TIME,
	UNI3_LIGHTCOLOR
};
enum ETextureUnits3{
	UNIT3_CUBE=0,
	UNIT3_WAVE
};

enum EAttributes4{
	ATT4_POSITION=0,
	ATT4_NORMAL
};
enum EUniforms4{
	UNI4_PROJECTIONMATRIX=0,
	UNI4_CAMERAMATRIX,
	UNI4_SCENEMATRIX,
	UNI4_LIGHTPOSITION0,
	UNI4_MINHEIGHT,
	UNI4_MAXHEIGHT,
	UNI4_SEAHEIGHT,
	UNI4_MOUNTAINHEIGHT,
	UNI4_LIGHTCOLOR,
	UNI4_FOGCOLOR
};
enum ETextureUnits4{
	UNIT4_TERRAINBUMP=0,
	UNIT4_HEIGHTTEXTURE,
	UNIT4_ROCKTEXTURE
};

enum EAttributes5{
	ATT5_POSITION=0,
	ATT5_NORMAL
};
enum EUniforms5{
	UNI5_PROJECTIONMATRIX=0,
	UNI5_CAMERAMATRIX,
	UNI5_SCENEMATRIX
};
enum ETextureUnits5{
	UNIT5_SKYBOX=0
};

enum EAttributes6{
	ATT6_POSITION=0,
	ATT6_NORMAL,
	ATT6_COORD
};
enum EUniforms6{
	UNI6_PROJECTIONMATRIX=0,
	UNI6_CAMERAMATRIX,
	UNI6_SCENEMATRIX
};
enum ETextureUnits6{
	UNIT6_TEXTURE=0
};

/**
 * @brief Inicializuje komunikaci s glsl programy
 */
void InitGLSL();

/**
 * @brief Deinicializuje komunikaci s glsl programy
 */
void FreeGLSL();

#endif//_GLSLINIT_H_
