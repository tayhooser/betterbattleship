//delaney welch 
//source code for battle ship group project
//date 9/13/22

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <GL/glx.h>
#include "fonts.h"
#include "dwelch.h"

unsigned int manage_over_state(unsigned int o)
{
	o = o ^ 1;
	return o;
}	
int show_dwelch()
{
    printf("delaney\n");
    
    return 0;

}

void showGameOver(int xres, int yres, GLuint gameoverTexture)
{
	Rect r;
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 350;
	int h = 220;

	//dim background
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

	//starting rectangle
	glBindTexture(GL_TEXTURE_2D, gameoverTexture);
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);  glVertex2f(0,     0);
		glTexCoord2f(0.0f, 0.25f); glVertex2f(0,     yres);
		glTexCoord2f(1.0f, 0.25f); glVertex2f(xres,  yres);
		glTexCoord2f(1.0f, 1.0f);  glVertex2f(xres,  0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	r.left = xcent;
	r.bot  = ycent + 80;
	r.center = 50;



}
