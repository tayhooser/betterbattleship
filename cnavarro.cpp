#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"

void show_cecilio()
{
	printf ("Cecilio\n");
}

unsigned int cecilio_feature(unsigned int s) {
	s = s ^ 1;
	return s;
}

void cecilio_feature(int xres, int yres) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3f(1.0, 0.0, 0.0);
	glColor4f(1.0, 0.0, 0.0, 0.5);
	int w = 20;
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0, 0);
		glVertex2f(0+w, w);

		glVertex2f(0, yres);
		glVertex2f(0+w, yres-w);

		glVertex2f(xres, yres);
		glVertex2f(xres-w, yres-w);
		
		glVertex2f(xres, 0);
		glVertex2f(xres-w, w);

		glVertex2f(0, 0);
		glVertex2f(0+w, w);

	glEnd();
	glDisable(GL_BLEND);

}

void showIntro(int xres, int yres, GLuint capitalTexture)
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
	imgx = xcent;
        imgy = ycent + 150 + 16;

        glBindTexture(GL_TEXTURE_2D, capitalTexture);
        glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(0, 0);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0, ycent*2);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(xcent*2, ycent*2);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(xcent*2, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
	
	r.left = xcent;
	r.bot  = 70;
	r.center = 50;
	ggprint16(&r, 50, 0xffffffff, "Press spacebar to begin");
	
}

