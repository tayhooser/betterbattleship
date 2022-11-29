// My source code file
// Created by: Danny Simpson
// Date: Fall 2022
// Purpose: Final Project for Software Engineering
#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"

const int MAX_PARTICLES = 1000;
int x_value = 0;
int y_value = 0;
int width = 0;
int maxGrid = 16;
int goneAlready[16][2];
//some structures

class Box {
public:
    float w;
        float dir;
    float vel[2];
       	float pos[2];
    Box() {
        w = 20.0f;
        dir = 25.0f;
        pos[0] = 600/3.0f;
        pos[1] = 400/1.5f;
        vel[0] = vel[1] = 0.0; 
    }
    Box(float wid, float d, float p0, float p1) {
        w = wid;
        dir = d;
        pos[0] = p0;
        pos[1] = p1;
        vel[0] = vel[1] = 0.0f;
    }


} particle(2.0, 0.0, 600/2.0, 400/4.0*3.0);

Box particles[MAX_PARTICLES];
int n = 0;
void ExplosionAnimation(int x, int y, int width, int m);
//make particles for animation
void make_particle(int x, int y, int wid)
{
    for (int i = 0; i < 20; i++) {
    	
        if (n>=MAX_PARTICLES)
            return;
        particles[n].w = 3.0;
        particles[n].pos[0] = x;
        particles[n].pos[1] = y;
        particles[n].vel[0]=particles[n].vel[1] = 0.0;
        ++n;
    }
//  ExplosionAnimation(x, y, width, n);
}
// To manage state
unsigned int manage_state(unsigned int s)
{
    s=s^1;
    return s;
}
// Function to print my name to the terminal
int show_danny()
{
    printf("Danny Simpson \n");
    return 0;
}
//Function pause the game
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
//prototype for the missile for mouse_click
/*int LaunchMissile(int x, int y, int cent[], int qsize, int missileType)
*                                                    //int grid2[16][16])
*{
*    if (x >= cent[0]-qsize &&
*        x <= cent[0]+qsize &&
*        y >= cent[1]-qsize &&
*        y <= cent[1]+qsize && 
*        missileType == 0) {
*        return 0;
*    }
*    if (x >= cent[0]-qsize &&
*        x <= cent[0]+qsize &&
*        y >= cent[1]-qsize &&
*        y <= cent[1]+qsize && 
*        missileType == 1) {
*        return 1;
*    }
*    return 0;
*}
*/
void FeatureBorder(int xres, int yres)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor3f(1.0, 1.0, 0.0);
    glColor4f(1.0, 1.0, 0.0, 0.5); 
    int wid = 40;
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2i(0, 0);
        glVertex2i(wid, wid);
        glVertex2i(0, yres);
        glVertex2i(0 + wid, yres - wid);
        glVertex2i(xres, yres);
        glVertex2i(xres - wid, yres - wid);
        glVertex2i(xres, 0);
        glVertex2i(xres - wid, wid);
        glVertex2i(0, 0);
        glVertex2i(wid, wid);
    glEnd();
    glDisable(GL_BLEND);
    Rect r;
    r.left = 40;
    r.bot  = yres - 50;
    r.center = 0;
    ggprint16(&r, 40, 0xffffff00, "Danny's Feature:");
    ggprint16(&r, 40, 0xffffff00, "Different missile types + 
                                                   animations");
	
	
	
	
}
//Physics for the particle animation
void explosion_physics()
{ 
    int k = 0;
    for (int i = 0; i<n; i++) {
        k++;
        if (k == 1) {
            particles[i].vel[1] += 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 2) {
            particles[i].vel[0] += 0.01;
            particles[i].vel[1] += 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 3) {
            particles[i].vel[0] += 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 4) {
            particles[i].vel[0] += 0.01;
            particles[i].vel[1] -= 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 5) {
            particles[i].vel[1] -= 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 6) {
            particles[i].vel[0] -= 0.01;
            particles[i].vel[1] -= 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 7) {
            particles[i].vel[0] -= 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
        }
        if (k == 8) {
            particles[i].vel[0] -= 0.01;
            particles[i].vel[1] += 0.01;
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
            k=0;
        }
        //
        //check for collision
        
       	if (particles[i].pos[0] >= (particles[i].pos[0] + width) &&
            particles[i].pos[1] >= (particles[i].pos[1] + width)) { 
            particles[i]= particles[n - 1];
            --n;
        }
        if (particles[i].pos[0] <= (particles[i].pos[0] - width) &&
            particles[i].pos[1] <= (particles[i].pos[1] - width)) { 
            particles[i]= particles[n - 1];
            --n;
        }
        if (particles[i].pos[0] >= (x_value + width) &&
            particles[i].pos[1] <= (y_value - width)) { 
            particles[i]= particles[n - 1];
            --n;
        }
        if (particles[i].pos[0] <= (x_value - width) &&
            particles[i].pos[1] >= (y_value + width)) { 
            particles[i]= particles[n - 1];
           --n;
        }
    }
}
//Function to render the particles
void ExplosionAnimation(int xx, int yy, int wid, int m)
{
    x_value = xx;
    y_value = yy;
    width = wid;
    //draw particles
    for (int i = 0; i<n; i++) {
        glPushMatrix();
        if (i % 2 == 0)
       	   	glColor3ub(255, 0, 0);
       	else
       	    glColor3ub(255, 215, 0);
        glTranslatef(particles[i].pos[0], particles[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
           	glVertex2f(-particles[i].w, -particles[i].w);
            glVertex2f(-particles[i].w,  particles[i].w);
            glVertex2f( particles[i].w,  particles[i].w);
            glVertex2f( particles[i].w, -particles[i].w);
       	glEnd();
        glPopMatrix();
    }
    glColor4f(1.0f, 0.8f, 0.5f, 0.6f);
}
