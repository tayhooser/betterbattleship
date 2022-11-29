//Author: Cecilio Navarro
//Date: Fall 2022
//Purpose: Include files for project
#include <stdio.h>

//Function prototypes

extern void coord(int i, int j);
extern int selectShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships, int shipToMove);
extern void moveShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIPS, int nships, int i, int j);
extern void show_cecilio();
extern void cecilio_feature(int xres, int yres);
extern void showIntro(int xres, int yres, GLuint capitalTexture);
