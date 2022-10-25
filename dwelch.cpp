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


// ----- resizing and tinkering teirs
	#define MXGRD 16
	#define GRDDM 10
	#define NGRDS 6

typedef struct t_grid {
	int status;
	int shipno;
	int over;
	float color[4];
} Grid;
Grid gridleft1[MXGRD][MXGRD];
Grid gridleft2[MXGRD][MXGRD];
Grid gridleft3[MXGRD][MXGRD];
Grid gridright1[MXGRD][MXGRD];
Grid gridright2[MXGRD][MXGRD];
Grid gridright3[MXGRD][MXGRD];
int GridDim = GRDDM;
int BoardDim;
int qsz;
int dun=0;
	
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
	
void ResetGrids(void)
{
	//restart the game...
	int i,j;
	for (i=0; i<GridDim; i++) {
		for (j=0; j<GridDim; j++) {
			gridleft1[i][j].status=0;
			gridleft2[i][j].status=0;
			gridleft3[i][j].status=0;

			gridright1[i][j].status=0;
			gridright2[i][j].status=0;
			gridright3[i][j].status=0;

			gridleft1[i][j].shipno=0;
			gridleft2[i][j].shipno=0;
			gridleft3[i][j].shipno=0;

			gridright1[i][j].shipno=0;
			gridright2[i][j].shipno=0;
			gridright3[i][j].shipno=0;
		}
	}
}

void get_grid_center(const int g, const int i, const int j, int xres, int yres, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = BoardDim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	//
	//This determines the center of each grid.
	switch(g) {
		case 1:
			s0 = xres/4;
			break;
		case 2:
			s0 = xres/4 * 3;
			break;
	}
	//quad upper-left corner
	int quad[2];
	//bq is the width of one grid section
	int bq = (b2 / GridDim);
	//-------------------------------------
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i);
}

void showTeir(int xres, int yres, GLuint xTexture)
{
    printf("delaney\n");
	int cent[2];
	//draw grid left #1
		// ...each grid square is drawn
		//
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		for (int i=0; i<GridDim; i++) {
			for (int j=0; j<GridDim; j++) {
				get_grid_center(1,i,j,xres,yres,cent);
				//glColor3f(0.5f, 0.1f, 0.1f);
				glColor4f(1.3f, 1.0f, 1.3f, 1.5f);
				if (gridleft1[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				if (gridleft2[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				if (gridleft3[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			
				if (gridleft1[i][j].status==1)
					glBindTexture(GL_TEXTURE_2D, xTexture);
				if (gridleft2[i][j].status==1)
					glBindTexture(GL_TEXTURE_2D, xTexture);
				if (gridleft3[i][j].status==1)
					glBindTexture(GL_TEXTURE_2D, xTexture);
			
				//if (grid1[i][j].status==2)
				//	glBindTexture(GL_TEXTURE_2D, explosionTexture);
			
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2i(cent[0]-qsz,cent[1]-qsz);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2i(cent[0]-qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2i(cent[0]+qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2i(cent[0]+qsz,cent[1]-qsz);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glDisable(GL_BLEND);
	//
	//draw grid right #1
	// ...each grid square is drawn
	//
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	for (int i=0; i<GridDim; i++) {
		for (int j=0; j<GridDim; j++) {
			get_grid_center(2,i,j,xres,yres,cent);
			//glColor3f(0.6f, 0.4f, 0.1f);
			glColor4f(1.1f, 1.8f, 1.5f, 1.6f);
			if (gridright1[i][j].over) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			if (gridright2[i][j].over) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			if (gridright3[i][j].over) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			//if (grid2[i][j].status==1)
		}
	}
}

void showGameOver(int xres, int yres)
{
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
	r.bot  = ycent + 80;
	r.center = 50;
	ggprint16(&r, 50, 0xffffffff, "Game Over");
}

