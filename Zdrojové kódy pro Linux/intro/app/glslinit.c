#include"glslinit.h"

SGPUShaderProgram Shader0_Cave;
SGPUShaderProgram Shader1_Particle;
SGPUShaderProgram Shader2_Object;
SGPUShaderProgram Shader3_Water;
SGPUShaderProgram Shader4_Terrain;
SGPUShaderProgram Shader5_SkyBox;
SGPUShaderProgram Shader6_SpiderWeb;

const char*Attributes0[]={
	"Position",
	"Normal"
};
const char*Uniforms0[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
	"LightPosition0",
	"SetAO",
	"CausticT"
};
const char*TextureUnits0[]={
	"Side",
	"SideBump",
	"Down",
	"DownBump",
	"Up",
	"UpBump",
	"Sediment",
	"NorAmb",
	"Wet",
	"Caustic",
};

const char*Attributes1[]={
	"Position",
	"Size",
	"TTL",
	"Normal"
};
const char*Uniforms1[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
	"MaxTTL",
	"p"
};
const char*TextureUnits1[]={
	"t"
};

const char*Attributes2[]={
	"Position",
	"Normal",
	"Coord"
};
const char*Uniforms2[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
	"LightPosition",
	"Smooth"
};
const char*TextureUnits2[]={
	"Texture",
	"BumpTexture"
};

const char*Attributes3[]={
	"Position"
};
const char*Uniforms3[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
	"LightPosition0",
	"Time",
	"LightColor"
};
const char*TextureUnits3[]={
	"Cube",
	"Wave"
};

const char*Attributes4[]={
	"Position",
	"Normal"
};
const char*Uniforms4[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
	"LightPosition0",
	"MinHeight",
	"MaxHeight",
	"SeaHeight",
	"MountainHeight",
	"LightColor",
	"FogColor"
};
const char*TextureUnits4[]={
	"TerrainBump",
	"HeightTexture",
	"RockTexture"
};

const char*Attributes5[]={
	"Position",
	"Normal"
};
const char*Uniforms5[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix",
};
const char*TextureUnits5[]={
	"SkyBox"
};

const char*Attributes6[]={
	"Position",
	"Normal",
	"Coord"
};
const char*Uniforms6[]={
	"ProjectionMatrix",
	"CameraMatrix",
	"SceneMatrix"
};
const char*TextureUnits6[]={
	"Texture"
};


void InitGLSL(){
	gpu_InitShaderProgram(&Shader0_Cave,
			(char*)vertex0_vp,NULL,(char*)fragment0_fp,
			(char**)Attributes0,sizeof(Attributes0)/sizeof(char*),
			(char**)Uniforms0,sizeof(Uniforms0)/sizeof(char*),
			(char**)TextureUnits0,sizeof(TextureUnits0)/sizeof(char*));

	gpu_InitShaderProgram(&Shader1_Particle,
			(char*)vertex1_vp,(char*)geometry1_gp,(char*)fragment1_fp,
			(char**)Attributes1,sizeof(Attributes1)/sizeof(char*),
			(char**)Uniforms1,sizeof(Uniforms1)/sizeof(char*),
			(char**)TextureUnits1,sizeof(TextureUnits1)/sizeof(char*));

	gpu_InitShaderProgram(&Shader2_Object,
			(char*)vertex2_vp,(char*)geometry2_gp,(char*)fragment2_fp,
			(char**)Attributes2,sizeof(Attributes2)/sizeof(char*),
			(char**)Uniforms2,sizeof(Uniforms2)/sizeof(char*),
			(char**)TextureUnits2,sizeof(TextureUnits2)/sizeof(char*));

	gpu_InitShaderProgram(&Shader3_Water,
			(char*)vertex3_vp,NULL,(char*)fragment3_fp,
			(char**)Attributes3,sizeof(Attributes3)/sizeof(char*),
			(char**)Uniforms3,sizeof(Uniforms3)/sizeof(char*),
			(char**)TextureUnits3,sizeof(TextureUnits3)/sizeof(char*));

	gpu_InitShaderProgram(&Shader4_Terrain,
			(char*)vertex4_vp,NULL,(char*)fragment4_fp,
			(char**)Attributes4,sizeof(Attributes4)/sizeof(char*),
			(char**)Uniforms4,sizeof(Uniforms4)/sizeof(char*),
			(char**)TextureUnits4,sizeof(TextureUnits4)/sizeof(char*));

	gpu_InitShaderProgram(&Shader5_SkyBox,
			(char*)vertex5_vp,NULL,(char*)fragment5_fp,
			(char**)Attributes5,sizeof(Attributes5)/sizeof(char*),
			(char**)Uniforms5,sizeof(Uniforms5)/sizeof(char*),
			(char**)TextureUnits5,sizeof(TextureUnits5)/sizeof(char*));

	gpu_InitShaderProgram(&Shader6_SpiderWeb,
			(char*)vertex2_vp,(char*)geometry6_gp,(char*)fragment6_fp,
			(char**)Attributes6,sizeof(Attributes6)/sizeof(char*),
			(char**)Uniforms6,sizeof(Uniforms6)/sizeof(char*),
			(char**)TextureUnits6,sizeof(TextureUnits6)/sizeof(char*));

}

void FreeGLSL(){//TODO nekde to volat
	gpu_FreeShaderProgram(&Shader0_Cave);
	gpu_FreeShaderProgram(&Shader1_Particle);
	gpu_FreeShaderProgram(&Shader2_Object);
	gpu_FreeShaderProgram(&Shader3_Water);
	gpu_FreeShaderProgram(&Shader4_Terrain);
	gpu_FreeShaderProgram(&Shader5_SkyBox);
	gpu_FreeShaderProgram(&Shader6_SpiderWeb);
}

