#include <iostream>
#include <stdio.h>
#include <string>
//#ifndef t_grid
//typedef struct t_grid {
//	int status;  //status of unit 0=empty 1=shipunit 2=damagedunit
//	int shipno;  //ship ID placed there -> 0 = no ship
//	int over;
//	float color[4];
//} Grid;
//#endif
static int gamemode = 0;
extern int nships;
extern int nshipssunk;
extern int nbombs;
extern std::string gameOver;
extern int planetID;
enum {
	MODE_READY=0,
	MODE_PLACE_SHIPS,
	MODE_FIND_SHIPS,
	MODE_GAMEOVER
};

extern int show_danny();
extern unsigned int manage_state(unsigned int s);
extern void PauseScreen(int xres, int yres);
extern int LaunchMissile(int x, int y, int cent[], int qsize, 
                        int missileType);//, int grid2[16][16]);
extern void FeatureBorder(int xres, int yres);
extern void ExplosionAnimation(int x, int y, int width, int m);
extern void make_particle(int x, int y, int width);
extern void explosion_physics();


