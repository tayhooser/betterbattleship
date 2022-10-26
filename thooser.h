//taylor's include files 

#include <stdio.h>

//#define GRIDDIM 10

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
	
	void updateStatus(void)
	{
		
	}
	
};

extern void show_taylor();

extern void showCredits(int xres, int yres, GLuint portraitTexture);

extern void validateShips(Grid grid[][16], Ship ship[], int grid_dim);

extern void deleteShip(Grid grid[][16], Ship ship[]);

extern void taylorFeatureOverlay(int xres, int yres);
