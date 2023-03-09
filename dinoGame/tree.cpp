#include "tree.h"

tree::tree(float a,float b):x(a),y(b){}


void tree::drawTree()
{
	glColor3f(0.4f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2d(x,y);
	glVertex2d(x+0.2,y);
	glVertex2d(x+0.2,y+1);
	glVertex2d(x,y+1);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2d(x+0.6,y+0.8);
	glVertex2d(x-0.4,y+0.8);
	glVertex2d(x+0.1,y+2);
	glEnd();
}
