#include"bargraph.h"

float TATAC[3]=//{1,1,0};
{1.*0x14/0xff, 1.*0x4b/0xff, 1.*0x03/0xff};
unsigned char NAME_DP[]={37,
	_G,_R,_A,_F,_I,_C,_K,_EE,
	__,
	_I,_N,_T,_R,_O,
	__,
	_6,_4,_K,_B,
	__,
	_S,
	__,
	_P,_O,_U,_ZZ,_I,_T,_I,_M,
	__,
	_O,_P,_E,_N,_G,_L
};

unsigned char AUTHOR_DP[]={15,
	_B,_C,_DOT,
	__,
	_T,_O,_M,_AA,_SS,
	__,
	_M,_I,_L,_E,_T
};

unsigned char WHERE_DP[]={27,
	_V,_Y,_S,_O,_K,_EE,
	__,
	_U,_CC,_E,_N,_II,
	__,
	_T,_E,_C,_H,_N,_I,_C,_K,_EE,
	__,
	_B,_R,_N,_O,
};

unsigned char FACULTY_DP[]={32,
	_F,_A,_K,_U,_L,_T,_A,
	__,
	_I,_N,_F,_O,_R,_M,_A,_CC,_N,_II,_C,_H,
	__,
	_T,_E,_C,_H,_N,_O,_L,_O,_G,_I,_I
};

unsigned char NAMENAME_DP[]={17,
	_P,_RR,_II,_R,_O,_D,_N,_II,
	__,
	_S,_C,_E,_N,_EE,_R,_I,_E
};



void DrawBar(){
	//return;
	static unsigned Counter=0;
	float MaxT=77.;//TODO upravit podle konecneho poctu volani
	float t=Counter/MaxT;
	float y=0.1;
	float Width=.8;
	float Height=.01;

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(0);

	glDisable(GL_TEXTURE_2D);
	//glColor3f(1,1,0);
	glColor3f(1.*0x4d/0xff,1.*0xc6/0xff,1.*0x28/0xff);
	glBegin(GL_QUADS);
	glVertex2f(-Width,-1+y*2);
	glVertex2f(-Width,-1+Height*2+y*2);
	glVertex2f((t*2-1)*Width,-1+Height*2+y*2);
	glVertex2f((t*2-1)*Width,-1+y*2);
	glEnd();
	glLineWidth(2);
	glDisable(GL_DEPTH_TEST);
	//glColor3f(.4,.4,0);
	glColor3f(1.*0x14/0xff, 1.*0x4b/0xff, 1.*0x03/0xff);

	glBegin(GL_LINE_STRIP);
	glVertex2f(-Width,-1+y*2);
	glVertex2f(-Width,-1+Height*2+y*2);
	glVertex2f(Width,-1+Height*2+y*2);
	glVertex2f(Width,-1+y*2);
	glVertex2f(-Width,-1+y*2);
	glEnd();
	
	glColor3f(1,1,0);
	font_CenterStart((char*)WHERE_DP,0.003);
	font_PrintLineEx((char*)WHERE_DP,0,.8,0.003,TATAC);
	font_CenterEnd((char*)WHERE_DP,0.003);

	font_CenterStart((char*)FACULTY_DP,0.003);
	font_PrintLineEx((char*)FACULTY_DP,0,.7,0.003,TATAC);
	font_CenterEnd((char*)FACULTY_DP,0.003);



	font_CenterStart((char*)NAME_DP,0.003);
	font_PrintLineEx((char*)NAME_DP,0,-.2,0.003,TATAC);
	font_CenterEnd((char*)NAME_DP,0.003);

	font_CenterStart((char*)AUTHOR_DP,0.003);
	font_PrintLineEx((char*)AUTHOR_DP,0,-.3,0.003,TATAC);
	font_CenterEnd((char*)AUTHOR_DP,0.003);

	font_CenterStart((char*)NAMENAME_DP,0.005);
	font_PrintLineEx((char*)NAMENAME_DP,0,.3,0.005,TATAC);
	font_CenterEnd((char*)NAMENAME_DP,0.005);


	winwindow_SwapBuffers();
	Counter++;

	//fprintf(stderr,"BAR: %u\n",Counter);
}
