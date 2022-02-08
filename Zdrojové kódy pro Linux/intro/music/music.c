#include"music.h"

#if defined(__WIN32__)
/***********************/
/*cast pro windows     */
/***********************/

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include"v2mplayer.h"
#include"libv2.h"

static V2MPlayer player;

void music_Init(unsigned char*Music){
  player.Init();//JASNY POUZITELNY
  player.Open(Music); //JASNY POUZITELNY
  dsInit(player.RenderProxy,&player,GetForegroundWindow());
}

void music_Play(){
  player.Play();
  sInt startTicks = GetTickCount();
}

void music_DeInit(){
  dsClose();
  player.Close();
}

#else
/***********************/
/*cast pro linux a jine*/
/***********************/

void music_Init(unsigned char*Music){}

void music_Play(){}

void music_DeInit(){}

#endif//if defined(__WIN32__)
