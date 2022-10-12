//my source code file
// Created by: Danny Simpson
// Date: September 13, 2022
#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"
unsigned int manage_state(unsigned int s)
{
	s=s^1;
	return s;
}
int show_danny()
{
    printf("Danny Simpson \n");
    return 0;
}
void PauseScreen(int xres, int yres)
{
	Rect r;
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 350;
	int h = 220;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(0, 0, 0, 0.4f);
	glBegin(GL_QUADS);
		glVertex2f(0, yres);
		glVertex2f(xres, yres);
		glVertex2f(xres, 0);
		glVertex2f(0, 0);
		glEnd();
	glDisable(GL_BLEND);
	r.left = xcent;
	r.bot  = ycent + 80;
	r.center = 50;
	ggprint16(&r, 50, 0xffffffff, "--Pause--");
	ggprint16(&r, 50, 0xffffffff, "Press(F1) For Help");
}
int LaunchMissile(int x, int y, int cent[], int qsize, int i, int j, int missileType)//int grid2[16][16])
{
	if (x >= cent[0]-qsize &&
		x <= cent[0]+qsize &&
		y >= cent[1]-qsize &&
		y <= cent[1]+qsize && 
		missileType == 0) {
		return 0;
	}
	if (x >= cent[0]-qsize &&
		x <= cent[0]+qsize &&
		y >= cent[1]-qsize &&
		y <= cent[1]+qsize && 
		missileType == 1) {
		return 1;
	}
	return 0;
}
