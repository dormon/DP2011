/**
 * 64kB opengl intro
 * hlavickovy soubor pro vytvoreni a spravu okna
 * @author Tomas Milet, xmilet01
 * @file winwindow.h
 * @date 23.12.2009
 * @version 1.1
 */

#ifndef _WINWINDOW_H_
#define _WINWINDOW_H_

#include "standard.h"


extern int winwindow_K[256];// klavesa zmacknuta
extern int winwindow_M[256];//klavesa zmacknuta
extern unsigned winwindow_Height;//vyska
extern unsigned winwindow_Width;//sirka
extern float winwindow_DTime;//delta casu ve float
extern int winwindow_TimeDelta;//delta casu v int
extern unsigned winwindow_TimeOffset;
typedef void(*TIMERFCE)();//prototyp funkce pro obsluhu casovace
typedef void(*WINFCE)();//prototyp funkce pro obsluhu programu
typedef void(*MOUSEMOTION)(int x,int y);//prototyp funkce pro mys
void winwindow_ActivateExtensions();
void winwindow_Quit();
//pokud je stara verze OpenGL

/**
 * Nastavi funkci ktera se vola pri vykreslovani
 * @param F funkce
 */
void winwindow_SetFce(WINFCE F);

/**
 * Nastavi funkci ktera se bude volat pri uplynuti casu
 * @param F ukazatel na funkci
 */
void winwindow_SetTimerFce(TIMERFCE F);

/**
 * @brief Spusti casovac
 */
void winwindow_StartTimer();

/**
 * Nastavi rozdil casu
 * @param Dt cas po lehoz uplynuti se vola funkce
 */
void winwindow_SetTimerDt(int Dt);

/**
 * Nastavi funkci ktera se vola pri pohybu mysi
 * @param M funkce
 */
void winwindow_SetMouseMotionFce(MOUSEMOTION M);

/**
 * Vytvori okno
 * @param Width sirka
 * @param Height vyska
 * @param FullScreen fullscreen?
 */ 
void winwindow_Create(unsigned Width,unsigned Height,int FullScreen);

/**
 * Odstrani okno
 */
void winwindow_Kill();

/**
 * Prehodi framebuffery
 */
void winwindow_SwapBuffers();

/**
 * Hlavni smycka
 */
void winwindow_MainLoop();

/**
 * Vycentruje pozici mysi
 */
void winwindow_MouseCenter();




#endif//_WINWINDOW_H_
