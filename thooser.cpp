//author: Taylor Hooser
//date: Spring 2022
//purpose: personal file for project
//

#include <stdio.h>
#include <GL/glx.h>
#include <algorithm>
#include "fonts.h"
#include "thooser.h"

extern class ShipClass ShipClass;

// ship constructor
Ship::Ship()
{
	status = SHIP_HEALTHY;
	type = SHIP_INVALID;
	size = 0;
}

// function from lab 4
void show_taylor()
{
	printf("Taylor Hooser\n");
}

// checks # of ships and shapes are valid
// TODO: apply texture over valid ships
void validateShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships)
{
	int validated[MAXSHIPS] = { 0 };
	int v = 0;
	int curShip;
	bool repairExists = false;
	bool planetExists = false;
	
	for (int i = 0 ; i < GRIDDIM; i++){
		for (int j = 0; j < GRIDDIM; j++){
			
			if (grid[i][j].status == 1){ //if ship exists at location
				curShip = grid[i][j].shipno;
				printf("\tcurShip = %d\n", curShip);
				
				// if ship not already validated
				if (std::find(validated, validated+MAXSHIPS, curShip) == validated+MAXSHIPS){
					
					validated[v] = curShip;
					v++;
					ship[curShip].pos[0] = i;
					ship[curShip].pos[1] = j;
					
					printf("\tship %d found!\n", curShip);
					printf("\t\tsize = %d\n",  ship[curShip].size);
					printf("\t\ttype = %d\n",  ship[curShip].type);
					printf("\t\tlocation = (%d,%d)\n", i, j);
					
					//find bottom left corner of ship, validate all possible shapes
					
					// attack : 2x1
					if (ship[curShip].type == SHIP_ATTACK){
						if (grid[i][j+1].shipno == curShip){
							ship[curShip].orientation = 0; //horizontal
						}else if (grid[i+1][j].shipno == curShip){
							ship[curShip].orientation = 1; //vetical
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
						
					// capital : 3x1
					}else if (ship[curShip].type == SHIP_CAPITAL){
						if ((grid[i][j+1].shipno == curShip) and 
							 grid[i][j+2].shipno == curShip){
							ship[curShip].orientation = 0; //horizontal
						}else if ((grid[i+1][j].shipno == curShip) and
								  (grid[i+2][j].shipno == curShip)){
							ship[curShip].orientation = 1; //vetical
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
					
					// repair : 1x1 ONLY 1
					}else if (ship[curShip].type == SHIP_REPAIR){
						if (repairExists){
							ship[curShip].type = SHIP_INVALID;
						}else{
							repairExists = true;
						}
						
					// planet : 3x3 ONLY 1
					}else if (ship[curShip].type == SHIP_PLANET){
						if (!planetExists and
						   (grid[i][j+1].shipno == curShip) and
						   (grid[i][j+2].shipno == curShip) and
						   (grid[i+1][j+0].shipno == curShip) and
						   (grid[i+1][j+1].shipno == curShip) and
						   (grid[i+1][j+2].shipno == curShip) and
						   (grid[i+2][j].shipno == curShip) and
						   (grid[i+2][j+1].shipno == curShip) and
						   (grid[i+2][j+2].shipno == curShip)){
							planetExists = true;
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
						
					}
						
					if (ship[curShip].type == SHIP_INVALID){
						printf("\t\t!!SHIP %d INVALID!!\n", grid[i][j].shipno);
						deleteShip(grid, ship, GRIDDIM, curShip, nships);
						printf("\n\tShip %d deleted, ship %d renamed to %d\n", curShip, nships + 1, curShip);
					}else{
						printf("\t\tship %d valid! orientation = %d\n", curShip, ship[curShip].orientation);
					}
				}
			}
		}
	}
}

// deletes given ship, called by validateShips()
void deleteShip(Grid grid[][16], Ship ship[], int GRIDDIM, int curShip, int nships)
{
	
	//recycle ship
	for (int i = 1; i <= nships; i++){
		if (i == curShip){
			ship[i] = ship[nships];
			nships -= 1;
		}
	}

	//update grid
	for (int i = 0 ; i < GRIDDIM; i++){
		for (int j = 0; j < GRIDDIM; j++){
			// grid refers to now deleted ship
			if (grid[i][j].shipno = curShip){
				grid[i][j].shipno = 0; //no ship
				grid[i][j].status = 0; //empty grid space
			}
			// grid refers to previously ship[nships]
			if (grid[i][j].shipno = nships+1){
				grid[i][j].shipno = curShip;
			}
		}
	}
}

// overlay during ship placement phase
void taylorFeatureOverlay(int xres, int yres)
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
	
	ggprint16(&r, 40, 0xffffff00, "Taylor's Feature:");
	ggprint16(&r, 40, 0xffffff00, "Ship types, textures, and validation");
	ggprint16(&r, 40, 0xffffff00, "Press V to validate ships");
}

// function from lab 7
// credits overlay with 128x128 pixel images
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
