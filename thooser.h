//author: Taylor Hooser
//date: Spring 2022
//purpose: include file for project

#include <stdio.h>

// copied from bship.cpp
typedef struct t_grid {
	int status;  //status of unit 0=empty 1=shipunit 2=damagedunit
	int shipno;  //ship ID placed there -> 0 = no ship
	int over;
	float color[4];
} Grid;


// Taylor's work below -----------------------------------------------


enum ShipStatus {
	SHIP_HEALTHY=0,
	SHIP_DAMAGED,
	SHIP_SUNK
};

enum ShipType {
	SHIP_ATTACK=0,
	SHIP_CAPITAL,
	SHIP_REPAIR,
	SHIP_PLANET,
	SHIP_INVALID
};

// Ship class refers to multi-unit structure.
// For single grid units, see grid structure in bship.cpp

// if grid[i][j].shipno = 1, it refers to the ship ship[1]
// if grid[i][j].shipno = 0, it has no ship. ship[0] is a dummy ship

class Ship {
public:
	int pos[2]; //position of lower left corner of ship
	int orientation = 0; // 0 = horizontal, 1 = vertical
	ShipStatus status = SHIP_HEALTHY;
	ShipType type = SHIP_INVALID;
	int size = 0;
	Ship();
	
	void updateType(void)
	{
		if (size == 1){
			type = SHIP_REPAIR;
		}else if (size == 2){
			type = SHIP_ATTACK;
		}else if (size == 3){
			type = SHIP_CAPITAL;
		}else if (size == 9){
			type = SHIP_PLANET;
		}else{
			type = SHIP_INVALID;
		}
	}
	
	void updateStatus(int s)
	{
		if (s == 0){
			status = SHIP_HEALTHY;
		}else if (s == 1){
			status = SHIP_DAMAGED;
		}else if (s == 2){
			status = SHIP_SUNK;
		}
	}
	
};

extern void show_taylor();

extern void showCredits(int xres, int yres, GLuint portraitTexture);

extern bool validateShips(Grid grid[][16], Ship ship[], int GRIDDIM, int MAXSHIP, int nships, int shipTotals[]);

extern void deleteShip(Grid grid[][16], Ship ship[], int GRIDDIM, int curShip, int nships);

extern void taylorFeatureOverlay(int xres, int yres);

extern void printGrid(Grid grid[][16], int GRIDDIM);
