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
int GridMax = NGRDS;
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
void FeatureBox(int xres, int yres)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3f(0.5, 0.0, 1.0);
	glColor4f(0.5, 0.0, 1.0, 0.5); 
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

void get_grid_center(const int t, const int g, const int i, const int j, int xres, int yres, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = BoardDim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	//
	//This determines the center for each teir we are on 
	switch(t) {
		case 1:
			s0 = xres/4;
			s1 = yres/6;
			//set info to base left
			//need to update s1 with y dims
			break;
		case 2:
			s0 = xres/4;
			s1 = yres/4 * 3;
			//set info to mid left
			//need to update s1 with y dims
			break;
		case 3:
			s0 = xres/4;
			s1 = yres/2;
			//set info to atmo left
			//need to update s1 with y dims
			break;
		case 4:
			s0 = xres/4 * 3;
			s1 = yres/6;
			//set info to base right
			//need to update s1 with y dims
			break;
		case 5:
			s0 = xres/4 * 3;
			s1 = yres/4 * 3;
			//set info to mid right
			//need to update s1 with y dims
			break;
		case 6:
			s0 = xres/4 * 3;
			s1 = yres/2;
			//set info to atmo right
			//need to update s1 with y dims
			break;
	}
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
	int bq = (BoardDim / GridDim);
	qsz = bq / 2 - 1;
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
//	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
	int cent[2];
	int xcent = xres / 2;		
	int ycent = yres / 2;	
	int w = 10;
	int h = 10;

	//tester init matrices 
	//not sure if this will do anything I want it to 
	//tbd
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	glColor3f(0.8f, 0.6f, 0.2f);
	//


	//draw grid left #1
		// ...each grid square is drawn
		//
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//	glEnable(GL_BLEND);
		for (int h=0; h<GridMax; h++) {
			for (int i=0; i<GridDim; i++) {
				for (int j=0; j<GridDim; j++) {
					get_grid_center(h,1,i,j,xres,yres,cent);
					//glColor3f(0.5f, 0.1f, 0.1f);
					glColor4f(20.0f, 51.0f, 13.3f, 111.5f);
					//if (gridleft1[i][j].over) {
					if (1) {
						glColor3f(1.0f, 1.0f, 0.0f);
					}
					if (gridleft2[i][j].over) {
						glColor3f(1.0f, 1.0f, 0.0f);
					}
					if (gridleft3[i][j].over) {
						glColor3f(1.0f, 1.0f, 0.0f);
					}
					glBindTexture(GL_TEXTURE_2D, 0);

					//if (gridleft1[i][j].status==1)
					if (1)
						glBindTexture(GL_TEXTURE_2D, xTexture);
					if (gridleft2[i][j].status==1)
						glBindTexture(GL_TEXTURE_2D, xTexture);
					if (gridleft3[i][j].status==1)
						glBindTexture(GL_TEXTURE_2D, xTexture);

					glBegin(GL_QUADS);

					//			glTexCoord2f(0.0f, 0.0f);
					glVertex2f(cent[0]-w+i, cent[1]-h+j);
					//			glTexCoord2f(0.0f, 0.2f);
					glVertex2f(cent[0]-w+i, cent[1]+h-j);
					//			glTexCoord2f(1.0f, 0.0f);
					glVertex2f(cent[0]+w-i, cent[1]+h-j);
					//			glTexCoord2f(1.0f, 0.2f);
					glVertex2f(cent[0]+w-i, cent[1]-h+j);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);

					/*
					//if (grid1[i][j].status==2)
					//	glBindTexture(GL_TEXTURE_2D, explosionTexture);
					//grid left 1
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2i(cent[0]-qsz,cent[1]-qsz);
					glTexCoord2f(0.0f, 0.2f);
					glVertex2i(cent[0]-qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2i(cent[0]+qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.2f);
					glVertex2i(cent[0]+qsz,cent[1]-qsz);
					glEnd();
					//grid left 2
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.4f);
					glVertex2i(cent[0]-qsz,cent[1]-qsz);
					glTexCoord2f(0.0f, 0.6f);
					glVertex2i(cent[0]-qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.4f);
					glVertex2i(cent[0]+qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.6f);
					glVertex2i(cent[0]+qsz,cent[1]-qsz);
					glEnd();
					//grid left 3
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.8f);
					glVertex2i(cent[0]-qsz,cent[1]-qsz);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2i(cent[0]-qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 0.8f);
					glVertex2i(cent[0]+qsz,cent[1]+qsz);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2i(cent[0]+qsz,cent[1]-qsz);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			}
	//	glDisable(GL_BLEND);
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
			//if (grid2[i][j].statu s==1)
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
	glDisable(GL_BLEND);*/
			}
		}
	}
}
//credits and creditibility 
void deeCred(int xres, int yres)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 0.0, 0.5);
	int w = 20;
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, 0);
		glVertex2i(w, w);
		
		glVertex2i(0, yres);
		glVertex2i(w, yres-w);
			
		glVertex2i(xres, yres);
		glVertex2i(xres-w, yres-w);
			
		glVertex2i(xres, 0);
		glVertex2i(xres-w, w);
			
		glVertex2i(0, 0);
		glVertex2i(w, w);
	glEnd();
	glDisable(GL_BLEND);
	
	Rect r;
	r.left = 40;
	r.bot  = yres - 40;
	r.center = 0;
	
	ggprint16(&r, 40, 0xffffff00, "Delaney's Feature:");
	ggprint16(&r, 40, 0xffffff00, "Grid System Updates");
}

void showGameOver(int xres, int yres, GLuint overTexture)
{
	int xcent = xres / 2;
	int ycent = yres / 2;
	int imgdim = 640;
	int imgx;
	int imgy;

	imgx = xcent;
	imgy = ycent;
	
	glBindTexture(GL_TEXTURE_2D, overTexture);
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

