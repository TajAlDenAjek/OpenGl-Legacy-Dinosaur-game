#include "dinosaur.h"

dinosaur::dinosaur(float a,float b,float c,float d):x(a), y(b),img(c),sz(d){}


void dinosaur::drawDinosaur()
{
	glEnable(GL_TEXTURE_2D);
	int setDownOnTheGround=-0.8;
	glBindTexture(GL_TEXTURE_2D,img);
	glColor3d(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex2d(x,y+setDownOnTheGround);
	glTexCoord2d(1,0);
	glVertex2d(x+sz,y+setDownOnTheGround);
	glTexCoord2d(1,1);
	glVertex2d(x+sz,y+sz);
	glTexCoord2d(0,1);
	glVertex2d(x,y+sz);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}