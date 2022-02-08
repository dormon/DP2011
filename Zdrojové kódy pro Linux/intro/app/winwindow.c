/**
 * 64kB opengl intro
 * zdrojovy soubor soubor pro vytvoreni a spravu okna
 * @author Tomas Milet, xmilet01
 * @file winwindow.c
 * @date 23.12.2009
 * @version 1.1
 */


#include "winwindow.h"

int winwindow_Dt=1000;/// delta casu
TIMERFCE Fce=NULL;/// funkce 
MOUSEMOTION MouseMotion=NULL;/// funkce pri pobyhu mysi
WINFCE WinFce=NULL;
int winwindow_K[256]={0};// klaves zmacknuta
int winwindow_M[256]={0};// tlacitko zmacknuto
unsigned winwindow_Height=600;//vyska
unsigned winwindow_Width=800;//sirka
int Running=1;//bezi?
float winwindow_DTime=0.01;
int winwindow_TimeDelta=10;
int TimerRunning=0;//casovac bezi
unsigned winwindow_TimeOffset=20;

#define MY_TIMER 0

void winwindow_SetFce(WINFCE F)
{
	WinFce=F;
}

void winwindow_SetMouseMotionFce(MOUSEMOTION M)
{
	MouseMotion=M;
}

void winwindow_SetTimerFce(TIMERFCE F)
{
	Fce=F;//nastavi funkci
}

void winwindow_SetTimerDt(int Time)
{
	#if defined(__WIN32__)
	winwindow_Dt=Time;//nastavi cas
	winwindow_DTime=Time*0.001;
	winwindow_TimeDelta=Time;
	winwindow_TimeOffset=Time;
	#else
	winwindow_Dt=Time;//nastavi cas
	winwindow_DTime=Time*0.001;
	winwindow_TimeDelta=Time;
	winwindow_TimeOffset=Time;
	#endif
}

void winwindow_Kill()
{
	Running=0;
}

//funkce pro ziskavani adres rozsireni
#if defined(__WIN32__)
	#define winwindow_GetProc wglGetProcAddress
#else
	#define winwindow_GetProc SDL_GL_GetProcAddress
#endif


void winwindow_ActivateExtensions(){
	//GLSL shader
	glCreateShader=(PFNGLCREATESHADERPROC)
		winwindow_GetProc("glCreateShader");
	glCompileShader=(PFNGLCOMPILESHADERPROC)
		winwindow_GetProc("glCompileShader");
	glShaderSource=(PFNGLSHADERSOURCEPROC)
		winwindow_GetProc("glShaderSource");
	glAttachShader=(PFNGLATTACHSHADERPROC)
		winwindow_GetProc("glAttachShader");
	glLinkProgram=(PFNGLLINKPROGRAMPROC)
		winwindow_GetProc("glLinkProgram");
	glCreateProgram=(PFNGLCREATEPROGRAMPROC)
		winwindow_GetProc("glCreateProgram");
	glUseProgram=(PFNGLUSEPROGRAMPROC)
		winwindow_GetProc("glUseProgram");
	glGetShaderiv=(PFNGLGETSHADERIVPROC)
		winwindow_GetProc("glGetShaderiv");
	glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC)
		winwindow_GetProc("glGetShaderInfoLog");
	glDeleteShader=(PFNGLDELETESHADERPROC)
		winwindow_GetProc("glDeleteShader");
	glDeleteProgram=(PFNGLDELETEPROGRAMPROC)
		winwindow_GetProc("glDeleteProgram");

	//GLSL vytazeni atributu a uniformu operation
	glGetAttribLocation=(PFNGLGETATTRIBLOCATIONPROC)
		winwindow_GetProc("glGetAttribLocation");
	glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)
		winwindow_GetProc("glGetUniformLocation");

	//GLSL io operace
	glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC)
		winwindow_GetProc("glUniformMatrix4fv");

	glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)
		winwindow_GetProc("glEnableVertexAttribArray");
	glDisableVertexAttribArray=(PFNGLDISABLEVERTEXATTRIBARRAYPROC)
		winwindow_GetProc("glDisableVertexAttribArray");
	glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)
		winwindow_GetProc("glVertexAttribPointer");

	//buffer
	glGenBuffers=(PFNGLGENBUFFERSPROC)
		winwindow_GetProc("glGenBuffers");
	glBindBuffer=(PFNGLBINDBUFFERPROC)
		winwindow_GetProc("glBindBuffer");
	glBufferData=(PFNGLBUFFERDATAPROC)
		winwindow_GetProc("glBufferData");
	glDeleteBuffers=(PFNGLDELETEBUFFERSPROC)
		winwindow_GetProc("glDeleteBuffers");
	glBufferSubData=(PFNGLBUFFERSUBDATAPROC)
		winwindow_GetProc("glBufferSubData");
	glMapBuffer=(PFNGLMAPBUFFERPROC)
		winwindow_GetProc("glMapBuffer");
	glUnmapBuffer=(PFNGLUNMAPBUFFERPROC)
		winwindow_GetProc("glUnmapBuffer");

	//uniformy
	glUniform1i=(PFNGLUNIFORM1IPROC)winwindow_GetProc("glUniform1i");
	glUniform1f=(PFNGLUNIFORM1FPROC)winwindow_GetProc("glUniform1f");
	glUniform3fv=(PFNGLUNIFORM3FVPROC)winwindow_GetProc("glUniform3fv");

#if defined(__WIN32__)
	glTexImage3D=(PFNGLTEXIMAGE3DPROC)winwindow_GetProc("glTexImage3D");
	glActiveTexture=(PFNGLACTIVETEXTUREPROC)winwindow_GetProc("glActiveTexture");
	glTexSubImage3D=(PFNGLTEXSUBIMAGE3DPROC)winwindow_GetProc("glTexSubImage3D");
#endif

	//mipmapy
	glGenerateMipmap=(PFNGLGENERATEMIPMAPPROC)
		winwindow_GetProc("glGenerateMipmap");

	//point sprite
	glPointParameterf=(PFNGLPOINTPARAMETERFPROC)
		winwindow_GetProc("glPointParameterf");
	glPointParameterfv=(PFNGLPOINTPARAMETERFVPROC)
		winwindow_GetProc("glPointParameterfv");

	glGenFramebuffers=(PFNGLGENFRAMEBUFFERSPROC)
		winwindow_GetProc("glGenFramebuffers");
	glBindFramebuffer=(PFNGLBINDFRAMEBUFFERPROC)
		winwindow_GetProc("glBindFramebuffer");
	glDeleteFramebuffers=(PFNGLDELETEFRAMEBUFFERSPROC)
		winwindow_GetProc("glDeleteFramebuffers");
	glFramebufferTexture2D=(PFNGLFRAMEBUFFERTEXTURE2DPROC)
		winwindow_GetProc("glFramebufferTexture2D");
	glDrawBuffers=(PFNGLDRAWBUFFERSPROC)
		winwindow_GetProc("glDrawBuffers");
	glCheckFramebufferStatus=(PFNGLCHECKFRAMEBUFFERSTATUSPROC)
		winwindow_GetProc("glCheckFramebufferStatus");
		
}

#if defined(__WIN32__)
/***********************/
/*cast pro windows     */
/***********************/

HDC hDc;
HGLRC		hRc;
HWND		hWnd;
HINSTANCE   hInstance;
WNDCLASSA    wc;
DEVMODEA 	dmSS;
RECT		Rect;
int active=1;

typedef struct _Timer {
  __int64 frequency;
  float resolution;
  unsigned long mm_timer_start;
  unsigned long mm_timer_elapsed;
  bool performance_timer;
  __int64 performance_timer_start;
  __int64 performance_timer_elapsed;
} Timer;

Timer BestResTimer;
float LastSeenTime=0;

void InitStateChanging(void) {
	memset(&BestResTimer, 0, sizeof(BestResTimer));

	if(!QueryPerformanceFrequency((LARGE_INTEGER *) &BestResTimer.frequency)) {
    BestResTimer.performance_timer = FALSE;
    BestResTimer.mm_timer_start = timeGetTime();
    BestResTimer.resolution = 1.0f/1000.0f;
    BestResTimer.frequency = 1000;
    BestResTimer.mm_timer_elapsed = BestResTimer.mm_timer_start;
	} else {
    QueryPerformanceCounter((LARGE_INTEGER *) &BestResTimer.performance_timer_start);
    BestResTimer.performance_timer = TRUE;
    BestResTimer.resolution = (float) (((double)1.0f)/((double)BestResTimer.frequency));
    BestResTimer.performance_timer_elapsed = BestResTimer.performance_timer_start;
  }
}

float GetCurrentState(void) {
  __int64 time;
  if(BestResTimer.performance_timer) {
    QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - BestResTimer.performance_timer_start) * BestResTimer.resolution)*1000.0f;
  } else {
    return ( (float) ( timeGetTime() - BestResTimer.mm_timer_start) * BestResTimer.resolution)*1000.0f;
	}
}

void winwindow_StartTimer(){
	InitStateChanging();//inicializuje casovac
	LastSeenTime=GetCurrentState();//naposledy videny cas
	TimerRunning=1;
}


#define GetProc wghGetProcc
LRESULT CALLBACK WndProc(HWND mhWnd,UINT	uMsg,WPARAM	wParam,LPARAM lParam)
{
	POINT mp;
	switch (uMsg)
	{
		/*case WM_ACTIVATE:
			if(!HIWORD(wParam))active=TRUE;
			else active=FALSE;
			break;
		case WM_SYSCOMMAND:
			switch(wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					break;
			}
			break;*/
		case WM_MOUSEMOVE:
			GetCursorPos(&mp);
			if(MouseMotion)MouseMotion((int)mp.x,(int)mp.y);//obsluha mysi
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if(wParam==VK_ESCAPE)winwindow_Kill();
			if(wParam>='A'&&wParam<='Z')
			winwindow_K[wParam-'A'+'a']=1;
			else
			winwindow_K[wParam]=1;
			break;
		case WM_KEYUP:
			if(wParam>='A'&&wParam<='Z')
			winwindow_K[wParam-'A'+'a']=0;
			else
			winwindow_K[wParam]=0;
			break;
		case WM_LBUTTONDOWN:
			winwindow_M[1]=1;
			break;
		case WM_LBUTTONUP:
			winwindow_M[1]=0;
			break;
		case WM_RBUTTONDOWN:
			winwindow_M[3]=1;
			break;
		case WM_RBUTTONUP:
			winwindow_M[3]=0;
			break;
		case WM_MBUTTONDOWN:
			winwindow_M[2]=1;
			break;
		case WM_MBUTTONUP:
			winwindow_M[2]=0;
			break;
		case WM_TIMER:
			SetTimer(mhWnd,MY_TIMER,winwindow_Dt,NULL);
			break;
	}
	return DefWindowProc(mhWnd,uMsg,wParam,lParam);
}

void winwindow_WndKill()
{
	KillTimer(hWnd,MY_TIMER);
	ChangeDisplaySettings(NULL,0);
	ShowCursor(TRUE);
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRc);
	ReleaseDC(hWnd,hDc);
	DestroyWindow(hWnd);
	UnregisterClass("Form1",hInstance);
}

void winwindow_Create(unsigned Width,unsigned Height,int FullScreen)
{
	winwindow_Width=Width;
	winwindow_Height=Height;
	unsigned depth;
	dmSS.dmBitsPerPel=24;
	dmSS.dmPelsWidth=winwindow_Width;
	dmSS.dmPelsHeight=winwindow_Height;
	if(FullScreen)
	{
		ChangeDisplaySettingsA(&dmSS,CDS_FULLSCREEN);
	}
	else
	{
		DEVMODEA dm;
		EnumDisplaySettings(0,ENUM_REGISTRY_SETTINGS,&dm);
		winwindow_Width=
			(winwindow_Width>dm.dmPelsWidth)?dm.dmPelsWidth:winwindow_Width;
		winwindow_Height=
			(winwindow_Height>dm.dmPelsHeight)?dm.dmPelsHeight:winwindow_Height;
		depth=dm.dmBitsPerPel;

	}

	Rect.left=(long)0;
	Rect.right=(long)winwindow_Width;
	Rect.top=(long)0;
	Rect.bottom=(long)winwindow_Height;
	if(!FullScreen)
	{
		AdjustWindowRectEx(&Rect,
				WS_CAPTION|WS_POPUPWINDOW,FALSE,WS_EX_APPWINDOW);
	}

	hInstance=GetModuleHandle(NULL);

	wc.style=CS_OWNDC;
	wc.lpfnWndProc=(WNDPROC)WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=NULL;
	wc.lpszMenuName=NULL;
	wc.lpszClassName="Form1";
	RegisterClass(&wc);
	ShowCursor(FALSE);
	hWnd=CreateWindowEx(WS_EX_APPWINDOW,"Form1","",
			(FullScreen) ? (WS_POPUP) : (WS_POPUPWINDOW | WS_CAPTION),
			0,0,
			Rect.right-Rect.left,
			Rect.bottom-Rect.top,
			NULL,
			NULL,
			hInstance,
			NULL);

	hDc=GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(pfd));
	pfd.nSize=sizeof(pfd);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=24;
	pfd.cDepthBits=32;
	pfd.iLayerType=PFD_MAIN_PLANE;
	int iForm=ChoosePixelFormat(hDc,&pfd);
	SetPixelFormat(hDc,iForm,&pfd);

	hRc=wglCreateContext(hDc);
	wglMakeCurrent(hDc,hRc);

	ShowWindow(hWnd,SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	SetTimer(hWnd,MY_TIMER,winwindow_Dt,NULL);
	winwindow_ActivateExtensions();

}

void winwindow_MainLoop(){
	MSG msg;
	while(Running)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)Running=0;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			if(TimerRunning){//casovac bezi
				float ActTime=GetCurrentState();//aktualni cas
				if(ActTime-LastSeenTime>winwindow_Dt){//uplynula perioda
					winwindow_TimeOffset=(ActTime-LastSeenTime);
					//LastSeenTime+=winwindow_Dt;//nastavime posledni videny cas
					LastSeenTime=ActTime;

					if(Fce)Fce();//vyvolame obsluhu casovace
				}
//				fprintf(stderr,"ActTime: %f\n",ActTime);
			}

			WinFce();
		}
	}
	winwindow_WndKill();
	KillTimer(hWnd,MY_TIMER);
	ChangeDisplaySettings(NULL,0);
	ShowCursor(TRUE);
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRc);
	ReleaseDC(hWnd,hDc);
	DestroyWindow(hWnd);
	UnregisterClass("Form1",hInstance);
}
void winwindow_Quit(){

}

void winwindow_SwapBuffers(){
  SwapBuffers(hDc);
}

void winwindow_MouseCenter(){
	SetCursorPos(winwindow_Width/2,winwindow_Height/2);
}

#else
/***********************/
/*cast pro linux a jine*/
/***********************/
void winwindow_StartTimer(){
	TimerRunning=1;
}


SDL_TimerID sdl_timer = NULL;///casovas

Uint32 TimerCallback(Uint32 interval, void* param)
{
	param=param;
	if(TimerRunning&&Fce)Fce();
	return interval;
}

void winwindow_Create(unsigned Width,unsigned Height,int FullScreen)
{
	winwindow_Height=Height;
	winwindow_Width=Width;
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);//STENCIL TODO to WINDOWS
	if(FullScreen)
		SDL_SetVideoMode(Width,Height,24,
				SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL|SDL_FULLSCREEN);
	else
		SDL_SetVideoMode(Width,Height,24,
				SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL);
	//SDL_ShowCursor(0);
	sdl_timer=SDL_AddTimer(winwindow_Dt,TimerCallback,NULL);
	//atexit(SDL_Quit);
	winwindow_ActivateExtensions();
}

void winwindow_SwapBuffers()
{
	SDL_GL_SwapBuffers();
}

void winwindow_MouseCenter()
{
	SDL_WarpMouse(winwindow_Width/2,winwindow_Height/2);
}

void winwindow_MainLoop()
{
	SDL_Event E;//udalost
	while(Running)
	{
		while(SDL_PollEvent(&E))
		{
			switch(E.type)
			{
				case SDL_KEYDOWN:
					if(E.key.keysym.sym==SDLK_ESCAPE)Running=0;
					winwindow_K[E.key.keysym.sym%255]=1;
					break;
				case SDL_KEYUP:
					winwindow_K[E.key.keysym.sym%255]=0;
					break;
				case SDL_MOUSEMOTION:
					if(MouseMotion)MouseMotion(E.motion.x,E.motion.y);//obsluha mysi
					break;
				case SDL_MOUSEBUTTONDOWN:
					winwindow_M[E.button.button]=1;
					break;
				case SDL_MOUSEBUTTONUP:
					winwindow_M[E.button.button]=0;
					break;
				case SDL_QUIT:
					Running=0;
					break;
			}
		}
		WinFce();//obsluha vykreslovani
	}
	SDL_RemoveTimer(MY_TIMER);
}
void winwindow_Quit(){
	SDL_Quit();
}


#endif//if defined(__WIN32__)
