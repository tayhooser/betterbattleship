#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"
#include "thooser.h"
#include "cnavarro.h"

int isAttack = 0;
int isCapital = 1;
int isRepair = 2;

int shipt;
int orient;
int selectShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships, int shipToMove) {
    //printf("%d", shipToMove);
    for (int i = 0; i < GRIDDIM; i++) {
        for (int j = 0; j < GRIDDIM; j++) {
            if (grid[i][j].shipno == shipToMove)
                if (ship[shipToMove].type != SHIP_PLANET) {
                    {

                        if (grid[i][j].status == 1 && ship[shipToMove].type == SHIP_REPAIR) {
                            //printf("ship type repair:%d ", SHIP_REPAIR);
                            shipt = 2;
                            //printf("ship type %d just placed!", shipt);
                        }
                        if (grid[i][j].status == 1 && ship[shipToMove].type == SHIP_ATTACK) {
                            if (ship[shipToMove].orientation == 0) {
                                orient = 0;
                            }
                            if (ship[shipToMove].orientation == 1) {
                                orient = 1;
                            }
                            //printf("ship type attack:%d ", SHIP_ATTACK);
                            shipt = 0;
                            //printf("ship type %d just placed!", shipt);
                        }
                        if (grid[i][j].status == 1 && ship[shipToMove].type == SHIP_CAPITAL) {
                            if (ship[shipToMove].orientation == 0) {
                                orient = 0;
                            }
                            if (ship[shipToMove].orientation == 1) {
                                orient = 1;
                            }
                            //printf("ship type capital:%d ", SHIP_CAPITAL);
                            shipt = 1;
                            //printf("ship type %d just placed!", shipt);
                        }
                        //printf("Hello");
                        grid[i][j].shipno = 0;
                        grid[i][j].status = 0;
                    }
                }
        }
    }
    return shipt;
}

void moveShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships, int k, int l) {
    //int curShip = grid[k][l].shipno;
    //ship[shipT].type = ship[curShip].type;
    //printf("ship type %d just placed!", shipt);
    //printf("ship type %d just placed!", orient);
    if (shipt == 0) {
        if (orient == 1) {
            grid[k][l].status = 1;
            grid[k+1][l].status = 1;
        } else if (orient == 0) {
            grid[k][l].status = 1;
            grid[k][l+1].status = 1;
        }
    }
    if (shipt == 1) {
        if (orient == 1) {
            grid[k][l].status = 1;
            grid[k+1][l].status = 1;
            grid[k+2][l].status = 1;
        } else if (orient == 0) {
            grid[k][l].status = 1;
            grid[k][l+1].status = 1;
            grid[k][l+2].status = 1;
        }
    }
    if (shipt == 2) {
        grid[k][l].status = 1;
    }
    /*for (int i = 0; i < GRIDDIM; i++) {
		for (int j = 0; j < GRIDDIM; j++) {

        }
    }*/
    //grid[i][j].shipno = shipT;
    //if ()
}
void show_cecilio()
{
	printf ("Cecilio\n");
}

//Toggle when testing
/*
unsigned int cecilio_feature(unsigned int s) {
	s = s ^ 1;
	return s;
}*/

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
	
	Rect r;
	r.left = 40;
	r.bot  = yres - 50;
	r.center = 0;
	
	ggprint16(&r, 40, 0xffffff00, "Cecilio's Feature:");
	ggprint16(&r, 40, 0xffffff00, "Moving ships");

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

