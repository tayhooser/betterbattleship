//delaney welch 
//source code for battle ship group project
//date 9/13/22

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "fonts.h"
#include "dwelch.h"


// ----- resizing and tinkering teirs
	#define MXGRD 450
	#define GRDDM 10
	#define NGRDS 6

typedef struct t_grid {
	int status;
	int shipno;
	int over;
	float color[4];
} Grid;
Grid gridleft1[MXGRD][MXGRD];
Grid gridright1[MXGRD][MXGRD];
int GridDim = GRDDM;
int GridMax = NGRDS;
int BoardDim = 400;
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
			gridright1[i][j].status=0;

			gridleft1[i][j].shipno=0;
			gridright1[i][j].shipno=0;
		}
	}
}
void get_grid_center(const int t, const int i, const int j, int xres, int yres, int cent[2]);
void check_dims(const int t, const int x, const int y, const int i, const int j, int xres, int yres, int cent[2])
{
        for(int i=0; i<GridDim; i++) {
                for (int j=0; j<GridDim; j++) {
                        gridleft1[i][j].over=0;
                        gridright1[i][j].over=0;
                }
        }
        for (int i=0; i<GridDim; i++) {
                for (int j=0; j<GridDim; j++) {
                        if (t > 0 && t < 3) {
                                get_grid_center(t,i,j,xres,yres,cent);
                                if (x >= cent[0]-qsz &&
                                        x <= cent[0]+qsz &&
                                        y >= cent[1]-qsz &&
                                        y <= cent[1]+qsz) {
                                        gridleft1[i][j].over=1;
                                        break;
                                }
                        }
                        if (t > 3 && t < 6) {
                                get_grid_center(t,i,j,xres,yres,cent);
                                if (x >= cent[0]-qsz &&
                                        x <= cent[0]+qsz &&
                                        y >= cent[1]-qsz &&
                                        y <= cent[1]+qsz) {
                                        gridright1[i][j].over=1;
                                        break;
                                }
                                if (x >= cent[0]-qsz &&
                                        x <= cent[0]+qsz &&
                                        y >= cent[1]-qsz &&
                                        y <= cent[1]+qsz) {
                                        gridright1[i][j].over=1;
                                        gridright1[i + 1][j].over=1;
                                        gridright1[i - 1][j].over=1;
                                        gridright1[i][j + 1].over=1;
                                        gridright1[i][j - 1].over=1;
                                        break;
                                }
                        }
               }
                if (gridleft1[i][j].over) break;
                if (gridright1[i][j].over) break;
        }
}

void get_grid_center(const int t, const int i, const int j, int xres, int yres, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = BoardDim/2;
	int screen_center[2]= {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	//
	//This determines the center for each teir we are on 
	switch(t) {
		case 1:
			s0 = xres/4;
			//s1 = yres/6;
			//set info to base left
			//need to update s1 with y dims
			break;
		case 2:
			s0 = xres/4;
			s1 = yres/4 * 3.15;
			//set info to atmo left
			//need to update s1 with y dims
			break;
		case 3:
			s0 = xres/4*3;
			//set info to base right
			//need to update s1 with y dims
			break;
		case 4:
			s0 = xres/4*3;
			s1 = yres/4*3.15;
			//set info to mid right
			//need to update s1 with y dims
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
	cent[0] += (bq * i);
	cent[1] += (bq * j);
}

void single(int, float, float);
void showTeir(int xres, int yres, GLuint xTexture)
{
//	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
	int cent[2];
	int xcent = xres / 2;		
	int ycent = yres / 2;	
	int width = 10;
	int height = 10;

	//tester init matrices 
	//not sure if this will do anything I want it to 
	//tbd
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	glColor3f(0.8f, 0.6f, 0.2f);
	//

	for (int h=1; h<3; h++){
	    for (int y=0; y<5; y++){
			for (int x=0; x<10; x++){
				if (gridleft1[x][y].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				glBindTexture(GL_TEXTURE_2D, 0);

				if (gridleft1[x][y].status==1)
					glBindTexture(GL_TEXTURE_2D, xTexture);

				get_grid_center(h,x,y,xres,yres,cent);
				single(h,cent[0]+qsz, cent[1]+qsz);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
	    }
	}

	//draw grid right #1
	// ...each grid square is drawn
	//
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	for (int h=3; h<5; h++){
		for (int i=0; i<5; i++) {
			for (int j=0; j<10; j++) {
				get_grid_center(h,j,i,xres,yres,cent);
				if (gridright1[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				single(h,cent[0]+qsz, cent[1]+qsz);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		for (int i=0; i<5; i++) {
			for (int j=0; j<10; j++) {
				get_grid_center(h,j,i,xres,yres,cent);
				if (gridright1[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				single(h,cent[0]+qsz, cent[1]+qsz);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
	//glDisable(GL_BLEND);
}
//credits and creditibility
void single(int h,float x, float y)
{
	if (h ==1)
		glColor4f(0.3f, 1.0f, 0.9f, 0.6f);
	if (h ==2)
		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
	if (h ==3)
		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	if (h ==4)
		glColor4f(0.5f, 0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+32,y);
		glVertex2f(x+32,y+32);
		glVertex2f(x,y+32);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}
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

