#include"fade.h"

void fade_Draw(float T){
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0,0,0,T);
	glBegin(GL_QUADS);
	glVertex2f(-1,-1);
	glVertex2f(+1,-1);
	glVertex2f(+1,+1);
	glVertex2f(-1,+1);
	glEnd();
	glEnable(GL_TEXTURE_2D);

}

