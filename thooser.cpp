//author: Taylor Hooser
//date: Spring 2022
//purpose: personal file for project
//

#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"



void show_taylor()
{
	printf("Taylor Hooser\n");
}

void showCredits(int xres, int yres, GLuint portraitTexture)
{
	
	Rect r;
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 400;
	int h = 280;
	int imgdim = 64;
	int imgx;
	int imgy;
	
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
	
	//credits rectangle
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-h);
		glVertex2f(xcent-w, ycent+h);
		glVertex2f(xcent+w, ycent+h);
		glVertex2f(xcent+w, ycent-h);
	glEnd();
	
	//display names
	r.left = xcent + 100;
	r.bot  = ycent + 150;
	ggprint16(&r, 80, 0xffffffff, "Taylor Hooser");
	ggprint16(&r, 80, 0xffffffff, "Jason Rodriguez");
	ggprint16(&r, 80, 0xffffffff, "Danny Simpson");
	ggprint16(&r, 80, 0xffffffff, "Cecilio Navarro");
	ggprint16(&r, 80, 0xffffffff, "Delaney Welch");
	

	//display taylor's portrait
	imgx = xcent - 100;
	imgy = ycent + 150 + 16;
	
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
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
	
	//display jason's portrait
	imgx = xcent - 260;
	imgy = ycent + 150 + 16 - 80;
	
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
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
	
	//display danny's portrait
	imgx = xcent - 100;
	imgy = ycent + 150 + 16 - 2*80;
	
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
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
	
	//display cecilio's portrait
	imgx = xcent - 260;
	imgy = ycent + 150 + 16 - 3*80;
	
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
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
	
	//display delaney's portrait
	imgx = xcent - 100;
	imgy = ycent + 150 + 16 - 4*80;
	
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
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
