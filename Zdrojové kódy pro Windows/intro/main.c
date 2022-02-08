/*
 * 64kB opengl intro
 * zdrojovy kod - hlavni cast programu
 * @author Tomas Milet, xmilet01
 * @file main.c
 * @date 22.3.2012
 * @version 9.2
 */

#include"main.h"

void Init();//funkce pro inicializaci
void DeInit();//funkce pro deinicializaci
void Draw();//funkce pro vykresleni

SCam Cam;//kamera


float CausticT=0;

void GENCUBE(SGPUTexture Texture,float*P);

float Thresholdd=0.775;
float PLAKE[3]={.5,.35,.5};

void Timer();


void BuildMountain();
void BuildWaterSide();
void BuildCarpet();
void BuildMountainSkyBox();
void BuildWaterSideSkyBox();

void BuildBuble();
void BuildWaterFall();
void BuildPlant();
void BuildMoss();//TODO
void BuildSpiderWeb();
void BuildBoxSwarm();
void BlowUp();

//hory//////////////////////////////////////////////////
STerrain Mountain;
SSkyBox MountainSkyBox;

//POBREZI///////////////////////////////////////////////
STerrain WaterSide;
SSkyBox WaterSideSkyBox;
SLake WaterSideLake;

//tunel/////////////////////////////////////////////////
SCave Tunnel;
SCollide TunnelCollide;
SMoss Moss;
SObject SpiderWeb;

//jeskyne///////////////////////////////////////////////
SCave Cave;
SCollide CaveCollide;
SParticleSystem WWW0;
SParticleSystem WWW1;
SParticleSystem WWW2;
SParticleSystem Fall0;
SParticleSystem Fall1;
SParticleSystem Fall2;
SParticleSystem Fall3;
SLake CaveLake;
SGPUTexture CaveSkyBox;
GLuint LakeCarry;
SObject Box;
SObject Carpet[3];
#define NUMPLANTS 4
SPlant Plants[NUMPLANTS];
int ScreenShot=0;



#define NUMSCENE 8
float SceneLen[NUMSCENE]={90,45,30,70,5,5,5,1000};
float SceneTime[NUMSCENE]={
	SceneLen[0],
	SceneLen[0]+SceneLen[1],
	SceneLen[0]+SceneLen[1]+SceneLen[2],
	SceneLen[0]+SceneLen[1]+SceneLen[2]+SceneLen[3],
	SceneLen[0]+SceneLen[1]+SceneLen[2]+SceneLen[3]+SceneLen[4],
	SceneLen[0]+SceneLen[1]+SceneLen[2]+SceneLen[3]+SceneLen[4]+SceneLen[5],
	SceneLen[0]+SceneLen[1]+SceneLen[2]+SceneLen[3]+SceneLen[4]+SceneLen[5]+SceneLen[6],
	SceneLen[0]+SceneLen[1]+SceneLen[2]+SceneLen[3]+SceneLen[4]+SceneLen[5]+SceneLen[6]+SceneLen[7]		
};
float GlobalTime=0;//cas
enum EScene{
	SCENE_MOUNTAIN=0,
	SCENE_WATERSIDE,
	SCENE_TUNNEL,
	SCENE_CAVE,
	SCENE_EXP0,
	SCENE_EXP1,
	SCENE_EXP2,
	SCENE_OUTRO
};

SCameraControl CameraCon[NUMSCENE];
enum EScene ActScene=SCENE_MOUNTAIN;//SCENE_CAVE;//SCENE_MOUNTAIN;//scena
unsigned char OUTRO_DP[]={29,
	_P,_R,_O,_G,_R,_A,_M,
	__,
	_U,_K,_O,_N,_CC,_II,_T,_E,
	__,
	_K,_L,_AA,_V,_E,_S,_O,_U,
	__,
	_E,_S,_C
};


GLuint FBO;//framebuffer object


int Blown=0;
unsigned TimeDT=20;



int main(int Argc,char*Argv[]){	
	myarg_Load(Argc,Argv);//nacteme argumenty programu
/*	
	myarg_Val[ARG_SKYBOX]=32;
	myarg_Val[ARG_TERRAINTEXTURE]=32;
	myarg_Val[ARG_WATER]=32;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(100,100,24,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL);

	winwindow_ActivateExtensions();
	InitGLSL();
	mt_Init();


	mt_DeInit();
	FreeGLSL();
	SDL_Quit();

	return 0;
*/
	winwindow_SetFce(Draw);//nastavi vykreslovaci fce
	winwindow_SetMouseMotionFce(NULL);//nastavi mys fce
	winwindow_SetTimerDt(TimeDT);
	winwindow_SetTimerFce(Timer);

	winwindow_Create(//vytvori okno
			myarg_Val[ARG_WIDTH],//sirka
			myarg_Val[ARG_HEIGHT],//vyska
			myarg_Val[ARG_FULLSCREEN]);//fullscreen

	music_Init(outro00_v2m);
	music_Play();
	Init();//funkce pro inicializaci
/*
	DeInit();
	winwindow_Quit();
	exit(0);
*/
	music_DeInit();
	music_Init(vesela01_v2m);
	music_Play();

	winwindow_StartTimer();//pustime casovac

	//Draw();

	winwindow_MainLoop();//hlavni smycka

	music_DeInit();
	DeInit();
	winwindow_Quit();
	return 0;
}


void Draw(){
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if(ScreenShot){
		PLAKE[0]=0.521927;PLAKE[1]=0.560819;PLAKE[2]=0.151121;
		GENCUBE(CaveLake.Cube,PLAKE);
		glViewport(0,0,myarg_Val[ARG_WIDTH],myarg_Val[ARG_HEIGHT]);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		ScreenShot=0;
	}

	STransform Scene;
	transform_Init(&Scene,TRANSTYPE_RTS);

	//SKYBOX////////////////////////////////////////////////////////////////
	glUseProgram(Shader5_SkyBox.ShaderProgram);
	gpu_SetMatrix(Shader5_SkyBox.Uniforms+UNI1_PROJECTIONMATRIX,Scene,Cam);
	glDisable(GL_CULL_FACE);
	if(ActScene==SCENE_MOUNTAIN)skybox_Draw(&MountainSkyBox);
	if(ActScene==SCENE_WATERSIDE)skybox_Draw(&WaterSideSkyBox);

	//TUNNEL CAVE///////////////////////////////////////////////////////
	glUseProgram(Shader0_Cave.ShaderProgram);
	gpu_SetMatrix(Shader0_Cave.Uniforms+UNI1_PROJECTIONMATRIX,Scene,Cam);
	float LightPosition0[3]={1,2,0};
	glUniform3fv(Shader0_Cave.Uniforms[UNI0_LIGHTPOSITION0],1,LightPosition0);
	glUniform1f(Shader0_Cave.Uniforms[UNI0_CAUSTICT],CausticT);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glUniform1f(Shader0_Cave.Uniforms[UNI0_SETAO],0);
	if(ActScene>=SCENE_CAVE)cave_Draw(&Cave);//hloubka
	if(ActScene==SCENE_TUNNEL)cave_Draw(&Tunnel);
	//
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glUniform1f(Shader0_Cave.Uniforms[UNI0_SETAO],0);//TTTOODODODODODODODOD
	glEnable(GL_BLEND);
	if(ActScene>=SCENE_CAVE)cave_Draw(&Cave);//barva
	if(ActScene==SCENE_TUNNEL)cave_Draw(&Tunnel);

	//MECH VODOPADY BUBLINKY ROSTLINY////////////////////////////////////////
	glUseProgram(Shader1_Particle.ShaderProgram);
	gpu_SetMatrix(Shader1_Particle.Uniforms+UNI1_PROJECTIONMATRIX,Scene,Cam);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if(ActScene==SCENE_TUNNEL)moss_Draw(&Moss);
	glDisableVertexAttribArray(Shader1_Particle.Attributes[ATT1_NORMAL]);
	if(ActScene>=SCENE_CAVE){
		for(int i=0;i<NUMPLANTS;++i)
			plant_Draw(Plants+i);

		particlesystem_Draw(&Fall0);
		particlesystem_Draw(&Fall1);
		particlesystem_Draw(&Fall2);
		particlesystem_Draw(&Fall3);

		particlesystem_Draw(&WWW0);
		particlesystem_Draw(&WWW1);
		particlesystem_Draw(&WWW2);
	}




	// TERENY///////////////////////////////////////////////////////////////
	glUseProgram(Shader4_Terrain.ShaderProgram);
	gpu_SetMatrix(Shader4_Terrain.Uniforms+UNI4_PROJECTIONMATRIX,Scene,Cam);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	LightPosition0[0]=100;LightPosition0[1]=10;LightPosition0[2]=100;
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_LIGHTPOSITION0],1,LightPosition0);
	float LightColor[3]={1.5,1.5,0};
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_LIGHTCOLOR],1,LightColor);
	float FogColor[3]={.5,.1,0};
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_FOGCOLOR],1,FogColor);

	if(ActScene==SCENE_WATERSIDE)
		terrain_Draw(&WaterSide);

	LightPosition0[0]=100;LightPosition0[1]=100;LightPosition0[2]=100;
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_LIGHTPOSITION0],1,LightPosition0);
	LightColor[0]=1.5;LightColor[1]=1.5;LightColor[2]=1.5;
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_LIGHTCOLOR],1,LightColor);
	FogColor[0]=.5;FogColor[1]=.5;FogColor[2]=1;
	glUniform3fv(Shader4_Terrain.Uniforms[UNI4_FOGCOLOR],1,FogColor);

	if(ActScene==SCENE_MOUNTAIN)
		terrain_Draw(&Mountain);

	glFrontFace(GL_CCW);


	// VODA ///////////////////////////////////////////////////////////////
	glUseProgram(Shader3_Water.ShaderProgram);
	gpu_SetMatrix(Shader3_Water.Uniforms+UNI3_PROJECTIONMATRIX,Scene,Cam);

	glDisable(GL_CULL_FACE);
	glUniform1f(Shader3_Water.Uniforms[UNI3_TIME],CausticT);

	glUniform3fv(Shader3_Water.Uniforms[UNI3_LIGHTPOSITION0],1,LightPosition0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(ActScene==SCENE_WATERSIDE){
		LightColor[0]=1;LightColor[1]=.7;LightColor[2]=.2;
		glUniform3fv(Shader3_Water.Uniforms[UNI3_LIGHTCOLOR],1,LightColor);
	}
	if(ActScene>=SCENE_TUNNEL){
		LightColor[0]=1;LightColor[1]=1;LightColor[2]=1;
		glUniform3fv(Shader3_Water.Uniforms[UNI3_LIGHTCOLOR],1,LightColor);
	}

	if(ActScene==SCENE_WATERSIDE)lake_Draw(&WaterSideLake);
	if(ActScene>=SCENE_TUNNEL)lake_Draw(&CaveLake);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);


	// BEDNY KOBERCE
	glUseProgram(Shader2_Object.ShaderProgram);
	gpu_SetMatrix(Shader2_Object.Uniforms+UNI2_PROJECTIONMATRIX,Scene,Cam);


	glDisable(GL_CULL_FACE);
	glUniform3fv(Shader2_Object.Uniforms[UNI2_LIGHTPOSITION],1,LightPosition0);

	if(ActScene>=SCENE_CAVE){
		object_Draw(&Box);
		for(int i=0;i<3;++i)
			object_Draw(Carpet+i);
	}


	// PAVUCINA ////////////////////////////////////////////////////////////
	glUseProgram(Shader6_SpiderWeb.ShaderProgram);
	gpu_SetMatrix(Shader6_SpiderWeb.Uniforms+UNI6_PROJECTIONMATRIX,Scene,Cam);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if(ActScene==SCENE_TUNNEL)spiderweb_Draw(&SpiderWeb);




	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//glTranslatef(.6,.6,.6);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	if(ActScene==SCENE_OUTRO){
		for(unsigned i=0;i<16;++i){
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
			
		glColor3f(1,1,1);
		float TATAC[3]={1,1,1};
		font_CenterStart((char*)OUTRO_DP,0.003);
		font_PrintLineEx((char*)OUTRO_DP,0,.8,0.003,TATAC);
		font_CenterEnd((char*)OUTRO_DP,0.003);
	}

	float ttt=fmax(0,GlobalTime-SceneTime[(int)ActScene]+3)/3;
	fade_Draw(fmax(0,ttt));

	winwindow_SwapBuffers();
}

void Init(){
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);
	glClearStencil(0);

	font_Create();
	DrawBar();

	camera_Init(&Cam,0.01,40,MY_PI/4,myarg_Val[ARG_WIDTH],myarg_Val[ARG_HEIGHT]);
	Cam.Transform.T[0]=-.5;
	Cam.Transform.T[1]=-.5;
	Cam.Transform.T[2]=-.5;
	cameracontrol_Init(CameraCon+0,&Cam,(float*)ccmountain,num_ccmountain,SceneLen[0],0);
	cameracontrol_Init(CameraCon+1,&Cam,(float*)ccwaterside,num_ccwaterside,SceneLen[1],0);
	cameracontrol_Init(CameraCon+2,&Cam,(float*)cctunnel,num_cctunnel,SceneLen[2],0);
	cameracontrol_Init(CameraCon+3,&Cam,(float*)cccave,num_cccave,SceneLen[3],0);
	cameracontrol_Init(CameraCon+4,&Cam,(float*)ccexp0,num_ccexp0,SceneLen[4],0);
	cameracontrol_Init(CameraCon+5,&Cam,(float*)ccexp1,num_ccexp1,SceneLen[5],0);
	cameracontrol_Init(CameraCon+6,&Cam,(float*)ccexp2,num_ccexp2,SceneLen[6],0);

	RandomSeed(0);//time(NULL));//seed

	InitGLSL();//inicializace komunikace s shadery

	DrawBar();

	mt_Init();
	DrawBar();

	//tunel
	SColorBuffer TunnelVolume;
	STunnelTemplate TunnelTemplate={0,32};
	tunnel_Init(&TunnelVolume,TunnelTemplate);
	cave_Init(&Tunnel,TunnelVolume,Thresholdd,1,128,&Shader0_Cave);

	//jeskyne
	SColorBuffer CaveVolume;
	cave_BuildVolume(&CaveVolume,32);
	cave_Init(&Cave,CaveVolume,Thresholdd,1,128*4,&Shader0_Cave);

	collide_Init(&CaveCollide,&Cave.Volume,&Cave.NorAmb.Data[0],Thresholdd);
	collide_Init(&TunnelCollide,&Tunnel.Volume,&Tunnel.NorAmb.Data[0],Thresholdd);
	DrawBar();

	BuildMountain();
	BuildWaterSide();

	BuildMountainSkyBox();
	BuildWaterSideSkyBox();
	lake_Inita(&WaterSideLake,PLAKE,10,myarg_Val[ARG_WATER],WaterSideSkyBox.SkyBox,&Shader3_Water);

	skybox_Empty(&CaveSkyBox,512);
	lake_Inita(&CaveLake,PLAKE,10,myarg_Val[ARG_WATER],CaveSkyBox,&Shader3_Water);


	DrawBar();

	BuildWaterFall();
	DrawBar();

	BuildBuble();
	DrawBar();

	BuildPlant();
	DrawBar();

	BuildMoss();

	BuildBoxSwarm();

	BuildCarpet();

	BuildSpiderWeb();


	glGenFramebuffers(1,&FBO);
	GLenum Status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(Status!=GL_FRAMEBUFFER_COMPLETE)fprintf(stderr,"NEKOMPLETNI!\n");


	cameracontrol_Update(CameraCon+0,0);
}


float CAN=0;
void Timer(){

	GlobalTime+=winwindow_TimeOffset*1e-3;
	if(GlobalTime>=SceneTime[ActScene]){
		ActScene=(enum EScene)((int)ActScene+1);
		if(ActScene==SCENE_TUNNEL){
			music_DeInit();
			music_Init(final00_v2m);
			music_Play();
		}
		if(ActScene==SCENE_OUTRO){
			music_DeInit();
			music_Init(outro00_v2m);
			music_Play();
		}
		if(ActScene==SCENE_CAVE){
			ScreenShot=1;
		}
		if(ActScene>SCENE_OUTRO)
			ActScene=(enum EScene)((int)ActScene-1);
	}
	switch(ActScene){
		case SCENE_MOUNTAIN:
			cameracontrol_Update(CameraCon+0,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_WATERSIDE:
			cameracontrol_Update(CameraCon+1,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_TUNNEL:
			cameracontrol_Update(CameraCon+2,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_CAVE:
			cameracontrol_Update(CameraCon+3,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_EXP0:
			cameracontrol_Update(CameraCon+4,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_EXP1:
			cameracontrol_Update(CameraCon+5,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_EXP2:
			cameracontrol_Update(CameraCon+6,winwindow_TimeOffset);//TimeDT);
			break;
		case SCENE_OUTRO:
			break;
	}
	for(int i=0;i<3;++i)
		if(GlobalTime>SceneTime[3+i]-5&&Blown==i){
			Blown++;
			BlowUp();
			BlowUp();
		}
	CAN+=.001;
	CausticT=10*CAN;

	if(ActScene>=SCENE_TUNNEL){
		particlesystem_DrawWet(Cave.Wet.Data[0],&Fall0,100,-.1);
		particlesystem_DrawWet(Cave.Wet.Data[0],&Fall1,100,-.1);
		particlesystem_DrawWet(Cave.Wet.Data[0],&Fall2,100,-.1);
		particlesystem_DrawWet(Cave.Wet.Data[0],&Fall3,100,-.1);


		unsigned NumD=nsize_Size(&Cave.Wet.Data[0].Size);
		for(unsigned i=0;i<NumD;++i)
			Cave.Wet.Data[0].Data[0][i]=
				mymath_Clamp(Cave.Wet.Data[0].Data[0][i]+0.0005,0,1);


		Cave.Wet.DataChanged=1;

		particlesystem_Update(&Fall0,1);
		particlesystem_Update(&Fall1,1);
		particlesystem_Update(&Fall2,1);
		particlesystem_Update(&Fall3,1);
		for(int i=0;i<NUMPLANTS;++i)
			plant_Update(Plants+i,10);


		Gravity[1]=9.81;

		particlesystem_Update(&WWW0,1);
		particlesystem_Update(&WWW1,1);
		particlesystem_Update(&WWW2,1);

		Gravity[1]=-9.81;


		object_Update(&Box,10);
		object_ComputeNormal(&Box);
		for(int i=0;i<3;++i){
			object_Update(&Carpet[i],10);
			object_ComputeNormal(&Carpet[i]);
		}

		object_Update(&SpiderWeb,10);
	}
}

void GENCUBE(SGPUTexture T,float*P){
	//fprintf(stderr,"BOX BOX BOX\n");
	SCam CubeCam;
	STransform CubeScene;
	camera_Init(//nastavi kameru
			&CubeCam,//camera
			0.0001,//nejblizsi rovina
			4,//nejvzdalenejsi rovina
			MY_PI/2,//fovy 90
			T.Data[0].Size.Size[0],//sirka
			T.Data[0].Size.Size[0]);//vyska
	float N[16];//matice
	glViewport(0,0,T.Data[0].Size.Size[0],T.Data[0].Size.Size[0]);

	vector_Mov(CubeCam.Transform.T,P);
	vector_KMul(CubeCam.Transform.T,-1);

	glUseProgram(Shader0_Cave.ShaderProgram);
	transform_Init(&CubeScene,TRANSTYPE_RTS);
	transform_GetModelViewMatrix(N,&CubeScene);
	glUniformMatrix4fv(Shader0_Cave.Uniforms[UNI0_SCENE],1,GL_FALSE,N);
	transform_GetModelViewMatrix(N,&(CubeCam.Transform));
	glUniformMatrix4fv(Shader0_Cave.Uniforms[UNI0_CAMERA],1,GL_FALSE,N);

	glUniformMatrix4fv(Shader0_Cave.Uniforms[UNI0_PROJECTION],1,GL_FALSE,CubeCam.PM);


	float LightPosition0[3]={1,2,0};

	glUniform3fv(Shader0_Cave.Uniforms[UNI0_LIGHTPOSITION0],1,LightPosition0);

	glUniform1f(Shader0_Cave.Uniforms[UNI0_CAUSTICT],CausticT);


	for(unsigned i=0;i<6;++i){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		matrix_Identity(CubeCam.Transform.R);

		float RR[6][3]={
			{0,MY_PI/2,MY_PI},
			{0,-MY_PI/2,MY_PI},
			{MY_PI/2,0,MY_PI},
			{-MY_PI/2,0,MY_PI},
			{0,0,MY_PI},
			{0,MY_PI,MY_PI}
		};
		transform_YXZRotate(&CubeCam.Transform,RR[i]);

		transform_GetModelViewMatrix(N,&(CubeCam.Transform));
		glUniformMatrix4fv(Shader0_Cave.Uniforms[UNI0_CAMERA],1,GL_FALSE,N);

		glBindFramebuffer(GL_FRAMEBUFFER,FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,T.Texture,0);
		GLenum H[]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2,H);

		glDisable(GL_CULL_FACE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glDepthMask(GL_TRUE);
		glStencilMask(GL_FALSE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDepthFunc(GL_LESS);
		glFrontFace(GL_CW);

		glUniform1f(Shader0_Cave.Uniforms[UNI0_SETAO],0);
		cave_Draw(&Cave);//hloubka

		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glDepthMask(GL_FALSE);
		glStencilMask(GL_FALSE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);//DORMON
		glDepthFunc(GL_LEQUAL);
		glFrontFace(GL_CW);
		glUniform1f(Shader0_Cave.Uniforms[UNI0_SETAO],0);
		cave_Draw(&Cave);//barva
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		//winwindow_SwapBuffers();
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void BuildMountain(){
	SColorBuffer MountainHeightMap;
	mountain_InitHeightMap(&MountainHeightMap,256,0.500084,0.595608,1621956440);
	SGPUTexture MountainBumpTexture;
	terrain_InitTexture(&MountainBumpTexture,0,myarg_Val[ARG_TERRAINTEXTURE]);
	SGPUTexture MountainHeightTexture;
	SGPUTexture MountainRockTexture;
	SColorMap MHT;
	SColorMap MRT;
	const unsigned char MHTT[]={
		10,0,0,0,25,17,2,
		7,25,17,2,51,34,4,
		10,51,34,4,36,39,4,
		22,36,39,4,21,45,4,
		29,21,45,4,66,65,22,
		31,66,65,22,112,86,40,
		14,112,86,40,107,90,57,
		14,107,90,57,102,95,74,
		16,102,95,74,105,98,76,
		12,105,98,76,109,102,78,
		15,109,102,78,136,132,120,
		12,136,132,120,163,163,163,
		30,163,163,163,209,209,209,
		255,209,209,209,255,255,255
	};
	const unsigned char MRTT[]={
		45,0,0,0,18,12,2,
		43,18,12,2,37,25,4,
		16,37,25,4,59,41,6,
		19,59,41,6,80,56,8,
		31,80,56,8,104,91,68,
		34,104,91,68,127,127,127,
		129,127,127,127,74,74,74,
		89,74,74,74,22,22,22,
		149,22,22,22,107,107,107,
		255,107,107,107,191,191,191
	};
	colormap_SimpleLoad(&MHT,(unsigned char*)MHTT,sizeof(MHTT)/7);
	colormap_SimpleLoad(&MRT,(unsigned char*)MRTT,sizeof(MRTT)/7);

	terrain_GradientTexture(&MountainHeightTexture,128,MHT);
	terrain_GradientTexture(&MountainRockTexture,128,MRT);
	colormap_Free(&MHT);
	colormap_Free(&MRT);
	STerrainTemplate MountainTemplate={
		MountainBumpTexture,
		MountainHeightTexture,MountainRockTexture,
		MountainHeightMap,.28,.65,&Shader4_Terrain};
	terrain_Init(&Mountain,MountainTemplate);
	colorbuffer_Free(&MountainHeightMap);
}

void BuildWaterSide(){
	SColorBuffer WaterSideHeightMap;
	waterside_InitHeightMap(&WaterSideHeightMap,256,0.3,0.4,1621956440);
	SGPUTexture WaterSideBumpTexture;
	terrain_InitTexture(&WaterSideBumpTexture,0,myarg_Val[ARG_TERRAINTEXTURE]);
	SGPUTexture WaterSideHeightTexture;
	SGPUTexture WaterSideRockTexture;
	SColorMap MHT;
	SColorMap MRT;
	const unsigned char MHTT[]={
		60,7,5,51,24,36,81,
		33,24,36,81,40,67,112,
		31,40,67,112,88,132,163,
		24,88,132,163,135,196,214,
		23,135,196,214,155,196,131,
		14,155,196,131,174,196,48,
		15,174,196,48,132,159,46,
		12,132,159,46,89,121,43,
		45,89,121,43,54,89,23,
		255,54,89,23,18,58,3
	};
	const unsigned char MRTT[]={
		45,0,0,0,18,12,2,
		43,18,12,2,37,25,4,
		16,37,25,4,59,41,6,
		22,59,41,6,80,56,8,
		21,80,56,8,59,52,16,
		60,59,52,16,37,47,24,
		55,37,47,24,38,47,28,
		255,38,47,28,40,47,32
	};
	colormap_SimpleLoad(&MHT,(unsigned char*)MHTT,sizeof(MHTT)/7);
	colormap_SimpleLoad(&MRT,(unsigned char*)MRTT,sizeof(MRTT)/7);

	terrain_GradientTexture(&WaterSideHeightTexture,128,MHT);
	terrain_GradientTexture(&WaterSideRockTexture,128,MRT);
	colormap_Free(&MHT);
	colormap_Free(&MRT);
	STerrainTemplate WaterSideTemplate={
		WaterSideBumpTexture,
		WaterSideHeightTexture,WaterSideRockTexture,
		WaterSideHeightMap,.28,1,&Shader4_Terrain};
	terrain_Init(&WaterSide,WaterSideTemplate);
	colorbuffer_Free(&WaterSideHeightMap);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BuildCarpet(){
	// koberce
	unsigned CarpetColor[3]={100,220,0};
	float CarpetPos[9]={0.314918, 0.487361, 0.844145,0.248172, 0.496989, 0.774242,0.282147, 0.488564, 0.807271};
	float CarpetAngle[3]={-MY_PI/8,-MY_PI/3,-MY_PI/3};
	for(int i=0;i<3;++i){
		SGPUTexture CarpetT,CarpetB;
		carpet_InitTexture(&CarpetT,&CarpetB,256,CarpetColor[i]);
		SCarpetTemplate CarpetTemplate={10,10,
			{CarpetPos[i*3+0],CarpetPos[i*3+1],CarpetPos[i*3+2]},
			{0,1,0},
			CarpetAngle[i],
			{.03,.12,1},
			CarpetT,CarpetB,
			CaveCollide,
			&Shader2_Object};
		carpet_Init(Carpet+i,CarpetTemplate);
		Carpet[i].Elastic.Nodes[90].NotFixed=0;
		Carpet[i].Elastic.Nodes[99].NotFixed=0;
	}
}

////////////////////////////////////////////////////////////////////////////////

void BuildMountainSkyBox(){
	SGPUTexture SkyBoxTexture;
	SSkyTemplate SkyTemplate={{1,1,1},{0,0,.5},{1,1,1},{.5,.5,1},{1,1,1},.5};
	SSunTemplate SunTemplate={{1,1,1},{1,1,1},.04,1,10};
	SCloudTemplate CloudTemplate0={{1e4,1e4},.005,1,.15,8,.5,.1,{1,1,1}};
	SCloudTemplate CloudTemplate1={{1e4,1e4},.015,1,.65,8,.5,.1,{.8,.8,.8}};
	SSkyBoxElement SkyBoxElement[]={
		{&SkyTemplate,sky_GetColor}
		,{&CloudTemplate0,cloud_GetColor}
		,{&CloudTemplate1,cloud_GetColor}
		,{&SunTemplate,sun_GetColor}
	};
	SSkyBoxTemplate SkyBoxTemplate={myarg_Val[ARG_SKYBOX],SkyBoxElement,
		sizeof(SkyBoxElement)/sizeof(SSkyBoxElement)};
	skybox_Generate(&SkyBoxTexture,SkyBoxTemplate);
	skybox_Init(&MountainSkyBox,SkyBoxTexture,&Shader5_SkyBox);
}

void BuildWaterSideSkyBox(){
	SGPUTexture SkyBoxTexture;
	SSkyTemplate SkyTemplate={{1,.1,1},{0,0,.1},{1,1,1},{0,0,0},{1,.5,0},.5};
	SSunTemplate SunTemplate={{1,.1,1},{1,.8,.3},.05,1,100};
	SCloudTemplate CloudTemplate0={{1e4,1e4},.015,1,.65,8,.5,.2,{.3,.1,0}};
	SCloudTemplate CloudTemplate1={{1e4,1e4},.015,1,.65,8,.5,.2,{.9,.5,0}};
	SSkyBoxElement SkyBoxElement[]={
		{&SkyTemplate,sky_GetColor}
		,{&SunTemplate,sun_GetColor}
		,{&CloudTemplate0,cloud_GetColor}
		,{&CloudTemplate1,cloud_GetColor}
	};
	SSkyBoxTemplate SkyBoxTemplate={myarg_Val[ARG_SKYBOX],SkyBoxElement,
		sizeof(SkyBoxElement)/sizeof(SSkyBoxElement)};
	skybox_Generate(&SkyBoxTexture,SkyBoxTemplate);
	skybox_Init(&WaterSideSkyBox,SkyBoxTexture,&Shader5_SkyBox);
}

/////////////////////////////////////////////////////////////////////////////////////

void BuildBuble(){
	SRandRange BUBLESIZE={.001,.0009,Linear};
	SRandRange BRAUN={.0044,.0036,Linear};
	SRandRange SPEED={.044,.036,Linear};

	SNSize BubleSize;
	nsize_Inita(&BubleSize,2,128,128);
	SIndex BUIndex;
	index_Init(&BUIndex,&BubleSize);

	SGPUTexture BUBLE[3];
	SColorBuffer BU[3];

	for(int i=0;i<3;++i){
		colorbuffer_Init(BU+i,BB_EMPTY,&BubleSize,(void*)4);
		do{
			unsigned Index=index_GetIndex(&BUIndex);
			BU[i].Data[0][Index]=1;
			BU[i].Data[1][Index]=1;
			BU[i].Data[2][Index]=1;
			float MR=BubleSize.Size[0]/2.;
			float r=sqrt(pow(BUIndex.Index[0]-MR,2)+pow(BUIndex.Index[1]-MR,2));
			if(r>MR)r=0;
			BU[i].Data[3][Index]=pow(r/MR,2)*.2;
		}while(!index_Inc(&BUIndex));
		gpu_InitTexture(BUBLE+i,1,0,BU[i]);
	}

	float PS0[3]={0.542120,0.190468,0.354811};
	float PS1[3]={0.656982,0.193811,0.439683};
	float PS2[3]={0.562680,0.155245,0.527283};

	particlesystem_Init(&WWW0,100,PS0,.8,170,BUBLESIZE,SPEED,BRAUN,BUBLE[0],CaveCollide,&Shader1_Particle);
	particlesystem_Init(&WWW1,100,PS1,.4,500,BUBLESIZE,SPEED,BRAUN,BUBLE[1],CaveCollide,&Shader1_Particle);
	particlesystem_Init(&WWW2,100,PS2,.8,170,BUBLESIZE,SPEED,BRAUN,BUBLE[2],CaveCollide,&Shader1_Particle);
	index_Free(&BUIndex);
	nsize_Free(&BubleSize);
}

///////////////////////////////////////////////////////////////////////////////////////////

void BuildWaterFall(){
	SRandRange WaterFallSize={.001,.0009,Linear};
	SRandRange BRAUN={.0044,.0036,Linear};
	SRandRange SPEED={.044,.036,Linear};

	float EM0[3]={0.483830,0.837524,0.341953};
	float EM1[3]={0.651155,0.677634,0.710820};
	float EM2[3]={0.351986, 0.614859, 0.884118};
	float EM3[3]={0.209896, 0.712944, 0.644604};
	SGPUTexture lParticleTexture[4];
	for(int i=0;i<4;++i)particlesystem_InitTexture(lParticleTexture+i,128);

	particlesystem_Init(&Fall0,7500,EM0,.8,500,WaterFallSize,SPEED,BRAUN,lParticleTexture[0],CaveCollide,&Shader1_Particle);
	particlesystem_Init(&Fall1,7500,EM1,.8,300,WaterFallSize,SPEED,BRAUN,lParticleTexture[1],CaveCollide,&Shader1_Particle);
	particlesystem_Init(&Fall2,7500,EM2,.8,500,WaterFallSize,SPEED,BRAUN,lParticleTexture[2],CaveCollide,&Shader1_Particle);
	particlesystem_Init(&Fall3,7500,EM3,.8,500,WaterFallSize,SPEED,BRAUN,lParticleTexture[3],CaveCollide,&Shader1_Particle);

}

//////////////////////////////////////////////////////////////////////////////////////////

void BuildPlant(){
	float P[]={
		0.450581, 0.640559, 0.900980,
		0.292918, 0.600699, 0.675688,
		0.400653, 0.840466, 0.452814,
		0.639223, 0.609363, 0.634463};
	for(int i=0;i<NUMPLANTS;++i){
		SGPUTexture PlantTexture;
		plant_InitTexture(&PlantTexture,255);
		plant_Init(Plants+i,CaveCollide,P+i*3,.05,20,PlantTexture,&Shader1_Particle);
	}

}

////////////////////////////////////////////////////////////////////////////////////////

void BuildMoss(){
	SGPUTexture lMossTexture;
	SColorBuffer MossBuffer;
	colorbuffer_Copy(&MossBuffer,Plants[0].Texture.Data+0);
/*
	SNSize NAA;nsize_Inita(&NAA,2,2,2);
	colorbuffer_Init(&MossBuffer,BB_EMPTY,&NAA,(void*)3);
	nsize_Free(&NAA);*/
	gpu_InitTexture(&lMossTexture,1,0,MossBuffer);
	moss_Init(&Moss,1000,&TunnelCollide,lMossTexture,&Shader1_Particle);
}

///////////////////////////////////////////////////////////////////////////////////////

void BuildSpiderWeb(){
	SSpiderWebTemplate SpiderWebTemplate={8,4,1,1,{0.197532,0.635446,0.351294},{0,1,0},MY_PI/2,{.04,.04,.04},
		Cave.Up,Cave.UpBump,TunnelCollide,&Shader6_SpiderWeb};
	spiderweb_Init(&SpiderWeb,SpiderWebTemplate);
	//for(unsigned i=0;i<1000;++i)object_Update(&SpiderWeb,10);
}

void BuildBoxSwarm(){
	SGPUTexture BoxTex;
	SGPUTexture BoxBump;
	SColorBuffer BoxTexC,BoxBumpC;

	colorbuffer_Copy(&BoxTexC,Cave.Side.Data+0);
	gpu_InitTexture(&BoxTex,1,0,BoxTexC);

	colorbuffer_Copy(&BoxBumpC,Cave.SideBump.Data+0);
	gpu_InitTexture(&BoxBump,1,0,BoxBumpC);

	SBoxSwarmTemplate BoxSwarmTemplate={
		0,100,{0.340488,0.502355,0.751892},.01,{{.01,0.005,Linear},{.01,0.005,Linear},{.01,0.005,Linear}},
		BoxTex,BoxBump,CaveCollide,&Shader2_Object};
	boxswarm_Init(&Box,BoxSwarmTemplate);


}

////////////////////////////////////////////////////////////////////////////////////////

void BlowUp(){
	float ADV[3]={0,1,0};

	for(unsigned i=0;i<Box.Elastic.NumNode;++i){
		if(!(i%8)){vector_Rnd(ADV);vector_KMul(ADV,15);}
		vector_Add(
				Box.Elastic.Nodes[i].Velocity,ADV);
	}
	for(int j=0;j<3;++j){
		vector_Rnd(ADV);vector_KMul(ADV,10);
		for(unsigned i=0;i<Carpet[j].Elastic.NumNode;++i)
			vector_Add(Carpet[j].Elastic.Nodes[i].Velocity,ADV);
	}
	float Center[3]={0.340488,0.502355,0.751892};

	SParticleSystem F[]={Fall0,Fall1,Fall2,Fall3};
	for(int j=0;j<4;++j){
		for(unsigned i=0;i<F[j].Num;++i){
			float*ptr=(float*)F[j].GPUDataPosition.Data;
			float V[3];
			vector_Make(V,Center,ptr+i*3);
			float len=vector_Len(V);
			vector_Nor(V);
			vector_KMul(V,5*(1-len));
			vector_Add(F[j].Velocity+i*3,V);
		}
	}

	for(int j=0;j<NUMPLANTS;++j){
		for(unsigned i=0;i<Plants[j].Elastic.NumNode;++i){
			float V[3];
			vector_Make(V,Center,Plants[j].Elastic.Nodes[i].Point);
			float len=vector_Len(V);
			vector_Nor(V);
			vector_KMul(V,5*(1-len));
			vector_Add(Plants[j].Elastic.Nodes[i].Velocity,V);
		}
	}


}

void DeInit(){

	cave_Free(&Cave);

	cave_Free(&Tunnel);

	mt_DeInit();

	font_Delete();
	glDeleteFramebuffers(1,&FBO);
	skybox_Free(&MountainSkyBox);
	skybox_Free(&WaterSideSkyBox);
	terrain_Free(&Mountain);
	terrain_Free(&WaterSide);

	
	object_Free(Carpet+0);
	object_Free(Carpet+1);
	object_Free(Carpet+2);

	particlesystem_Free(&WWW0);
	particlesystem_Free(&WWW1);
	particlesystem_Free(&WWW2);
	particlesystem_Free(&Fall0);
	particlesystem_Free(&Fall1);
	particlesystem_Free(&Fall2);
	particlesystem_Free(&Fall3);
	
	for(unsigned i=0;i<NUMPLANTS;++i)
		plant_Free(Plants+i);

	spiderweb_Free(&SpiderWeb);
	moss_Free(&Moss);
	lake_Free(&WaterSideLake);
	lake_Free(&CaveLake);
	gpu_FreeTexture(&CaveSkyBox);
	
	object_Free(&Box);

	for(unsigned i=0;i<7;++i)
		cameracontrol_Free(CameraCon+i);

	FreeGLSL();

}
