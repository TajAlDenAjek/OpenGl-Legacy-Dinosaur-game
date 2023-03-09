#include "land.h"

void land::drawLand()
{
	glColor4f(0.0f, 0.8f, 0.0f,255);
	glBegin(GL_QUADS);
	glVertex3d(-250,0,-20);
	glVertex3d(500,0,-20);
	glVertex3d(500,-300,-20);
	glVertex3d(-250,-300,-20);
	glEnd();
	glColor4f(0.0f, 0.8f, 0.8f,0);
	glBegin(GL_QUADS);
	glVertex2d(-250,0);
	glVertex2d(500,0);
	glVertex2d(500,+300);
	glVertex2d(-250,+300);
	glEnd();
}