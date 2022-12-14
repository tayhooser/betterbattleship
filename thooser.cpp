//author: Taylor Hooser
//date: Fall 2022
//purpose: personal file for project


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
bool validateShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships, int shipTotals[])
{
	//printf("\nVALIDATE FUNCTION CALLED ------------ \n");
	
	int validated[MAXSHIPS] = { 0 };
	int v = 0;
	int toDelete[MAXSHIPS] = { 0 };
	int d = 0;
	
	for (int i = 0; i < 4; i++){
		shipTotals[i] = 0;
	}
	
	// shipTotals[0] -> attack
	// shipTotals[1] -> capital
	// shipTotals[2] -> repair
	// shipTotals[3] -> planet
	
	int curShip = 0;
	
	for (int i = 0 ; i < GRIDDIM; i++){
		for (int j = 0; j < GRIDDIM; j++){
			
			if (grid[i][j].status == 1){ //if ship exists at location
				curShip = grid[i][j].shipno;
				//printf("curShip = %d\n", curShip);
				
				// if ship not already validated
				if (std::find(validated, validated+MAXSHIPS, curShip) == validated+MAXSHIPS){
					
					validated[v] = curShip;
					v++;
					ship[curShip].pos[0] = i;
					ship[curShip].pos[1] = j;
					
					//printf("\tship %d found!\n", curShip);
					//printf("\t\tsize = %d\n",  ship[curShip].size);
					//printf("\t\ttype = %d\n",  ship[curShip].type);
					//printf("\t\tlocation = (%d,%d)\n", i, j);
					
					//find bottom left corner of ship, validate all possible shapes
					// attack : 2x1
					if (ship[curShip].type == SHIP_ATTACK){
						if (grid[i][j+1].shipno == curShip){
							ship[curShip].orientation = 0; //horizontal
							shipTotals[0] += 1;
						}else if (grid[i+1][j].shipno == curShip){
							ship[curShip].orientation = 1; //vetical
							shipTotals[0] += 1;
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
						
					// capital : 3x1
					}else if (ship[curShip].type == SHIP_CAPITAL){
						if ((grid[i][j+1].shipno == curShip) and 
							 grid[i][j+2].shipno == curShip){
							ship[curShip].orientation = 0; //horizontal
							shipTotals[1] += 1;
						}else if ((grid[i+1][j].shipno == curShip) and
								  (grid[i+2][j].shipno == curShip)){
							ship[curShip].orientation = 1; //vetical
							shipTotals[1] += 1;
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
					
					// repair : 1x1 ONLY 1
					}else if (ship[curShip].type == SHIP_REPAIR){
						if (shipTotals[2] >= 1){
							ship[curShip].type = SHIP_INVALID;
						}else{
							shipTotals[2] = 1;
						}
						
					// planet : 3x3 ONLY 1
					}else if (ship[curShip].type == SHIP_PLANET){
						if (shipTotals[3] < 1 and
						   (grid[i][j+1].shipno == curShip) and
						   (grid[i][j+2].shipno == curShip) and
						   (grid[i+1][j+0].shipno == curShip) and
						   (grid[i+1][j+1].shipno == curShip) and
						   (grid[i+1][j+2].shipno == curShip) and
						   (grid[i+2][j].shipno == curShip) and
						   (grid[i+2][j+1].shipno == curShip) and
						   (grid[i+2][j+2].shipno == curShip)){
							shipTotals[3] = 1;
						}else{
							ship[curShip].type = SHIP_INVALID;
						}
						
					}
						
					if (ship[curShip].type == SHIP_INVALID){
						//printf("\t\t!!SHIP %d INVALID!!\n", grid[i][j].shipno);
						toDelete[d] = curShip;
						d++;
					}else{
						//printf("\t\tship %d valid! orientation = %d\n", curShip, ship[curShip].orientation);
					}
				}
			}
		}
	}
	
	//printGrid(grid, GRIDDIM);
	
	// delete ships in delete list
	for (int i = 0 ; i < MAXSHIPS; i++){
		if (toDelete[i] != 0){
			deleteShip(grid, ship, GRIDDIM, toDelete[i], nships);
			//printf("\n\tShip %d deleted, ship %d renamed to %d\n", toDelete[i], nships, toDelete[i]);
		}
	}
	
	//printGrid(grid, GRIDDIM);
	
	if ((shipTotals[2] == 1)
		&& (shipTotals[3] == 1)
		&& ((shipTotals[0] + shipTotals[1]) >= 1)){
			//printf("VALID = TRUE\n\n");
			return true;
	}
	//printf("VALID = FALSE\n\n");
	return false;
	
	
}

// deletes given ship, called by validateShips()
void deleteShip(Grid grid[][16], Ship ship[], int GRIDDIM, int curShip, int nships)
{
	//update grid
	for (int i = 0; i < GRIDDIM; i++){
		for (int j = 0; j < GRIDDIM; j++){
			// delete given ship from grid
			if (grid[i][j].shipno == curShip){
				grid[i][j].shipno = 0;
				grid[i][j].status = 0;
				//printf("ship %d found, setting (%d, %d) to default/n", curShip, i, j);
			}
			// recycle ship
			else if ((grid[i][j].shipno == nships) && (curShip != nships)){
				grid[i][j].shipno = curShip;
				//printf("ship %d found, setting (%d, %d) to default\n", nships, i, j);
			}
			
			
		}
	}
	//recycle ship
	ship[curShip] = ship[nships];
	nships -= 1;
}

// prints grid and ship ids, used for debugging
void printGrid(Grid grid[][16], int GRIDDIM)
{
	for (int i = GRIDDIM - 1; i >= 0; i--){
		for (int j = 0; j < GRIDDIM; j++){
			printf("%d ", grid[i][j].shipno);
		}
		printf("\n");
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
	r.bot  = yres - 50;
	r.center = 0;
	
	ggprint16(&r, 40, 0xffffff00, "Taylor's Feature:");
	ggprint16(&r, 40, 0xffffff00, "Ship types and validation");
}

// function from lab 7
void showCredits(int xres, int yres, GLuint portraitTexture)
{
	Rect r;
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 380;
	int h = 300;
	
	//dim background
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(0, 0, 0, 0.6f);
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
	
	r.left = xcent;
	r.bot  = ycent + 250;
	ggprint16(&r, 80, 0xffffffff, "~~~ CREDITS ~~~");
	
	//display names
	r.left = xcent + 80;
	r.bot  = ycent + 130;
	ggprint16(&r, 80, 0xffffffff, "Taylor Hooser");
	ggprint16(&r, 80, 0xffffffff, "Jason Rodriguez");
	ggprint16(&r, 80, 0xffffffff, "Danny Simpson");
	ggprint16(&r, 80, 0xffffffff, "Cecilio Navarro");
	ggprint16(&r, 80, 0xffffffff, "Delaney Welch");
	
	//placeholder image used
	
	int imgdim = 64;
	int imgx;
	int imgy;
	
	//display taylor's portrait
	imgx = xcent - 100;
	imgy = ycent + 130 + 16;
	
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
	imgy = ycent + 130 + 16 - 80;
	
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
	imgy = ycent + 130 + 16 - 2*80;
	
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
	imgy = ycent + 130 + 16 - 3*80;
	
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
	imgy = ycent + 130 + 16 - 4*80;
	
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
