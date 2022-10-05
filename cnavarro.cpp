#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"

void show_cecilio()
{
	printf ("Cecilio\n");
}

unsigned int showIntro(unsigned int s) {
	s = s ^ 1;
	return s;
}

void showIntro(int xres, int yres, GLuint gameoverTexture)
{
    int imgdim = 64;
    int imgx;
    int imgy;

	Rect r;
	int xcent = xres / 2;
	int ycent = yres / 2;
	//int w = 350;
	//int h = 220;
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, yres);
		glVertex2f(xres, yres);
		glVertex2f(xres, 0);
	glEnd();
	r.left = xcent;
	r.bot  = ycent + 80;
	r.center = 50;
	ggprint16(&r, 50, 0xffffffff, "Hello");
	ggprint16(&r, 50, 0xffffffff, "Press: 'I' to continue");
	//ggprint16(&r, 50, 0xffffffff, "Press: 'F1' for help screen");
	
	//Display img
	imgx = xcent - 100;
        imgy = ycent + 150 + 16;

        glBindTexture(GL_TEXTURE_2D, gameoverTexture);
        glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(imgx-imgdim, imgy-imgdim);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(imgx-imgdim, imgy+imgdim);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(imgx+imgdim, imgy+imgdim);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(imgx+imgdim, imgy-imgdim);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
	
}
