
// Individual source file
// Author: Jason Rodriguez
// Update Date : October 4 2022
#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"

int  show_jason() {

	printf("Jason\n");
	return 0;
}

unsigned int toggle(unsigned int toggle) {

	toggle = toggle ^ 1;
	return toggle;
}

void game_log(int xres, int yres) {

	int xcent = xres / 2;		
	int ycent = yres / 2;		
	int w = 90;
	int h = 190;
    glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-h);
        glVertex2f(xcent-w, ycent+h);
        glVertex2f(xcent+w, ycent+h);
        glVertex2f(xcent+w, ycent-h);
    glEnd();
}

void feature_border(int xres, int yres) {

	// Triangle strip border
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3f(0.2, 1.0, 0.2);
	glColor4f(0.2, 1.0, 0.2, 0.5);
	int w = 20;
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, 0);
		glVertex2i(w, w);

		glVertex2i(0, yres);
		glVertex2i(0+w, yres-w);

		glVertex2i(xres, yres);
		glVertex2i(xres-w, yres-w);

		glVertex2i(xres, 0);
		glVertex2i(xres-w, w);
		
		glVertex2i(0, 0);
		glVertex2i(w, w);

	glEnd();
	glDisable(GL_BLEND);

}

void show_help( int xres, int yres) {

    Rect r;
    int xcent = xres / 2;
    int ycent = yres / 2;
    int w = 350;
    int h = 220;
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(xcent-w, ycent-h);
        glVertex2f(xcent-w, ycent+h);
        glVertex2f(xcent+w, ycent+h);
        glVertex2f(xcent+w, ycent-h);
    glEnd();
    
    r.left = xcent;
    r.bot = ycent + 120;
    r.center = 50;
        
    ggprint16(&r, 50, 0xffffffff, " --- HELP ---");
    ggprint16(&r, 25, 0xffffffff, " press 's' for Danny");
    ggprint16(&r, 25, 0xffffffff, " press 'g' for Cecilio");
    ggprint16(&r, 25, 0xffffffff, " press 'd' for Delaney");
    ggprint16(&r, 25, 0xffffffff, " press 't' for Taylor");
    ggprint16(&r, 25, 0xffffffff, " press 'a' for Jason");
    
    ggprint16(&r, 25, 0xffffffff, "");
    
    ggprint16(&r, 25, 0xffffffff, " press 'c' for Credits");
    ggprint16(&r, 25, 0xffffffff, " press 'p' for Pause");
    ggprint16(&r, 25, 0xffffffff, " press 'o' for Game Over");
    ggprint16(&r, 25, 0xffffffff, " press 'F1' for Help");
    ggprint16(&r, 25, 0xffffffff, " press 'i' for Intro");
    ggprint16(&r, 25, 0xffffffff, " press 'Esc' to quit game");

}

