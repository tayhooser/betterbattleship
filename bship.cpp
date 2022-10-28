//cs335 Spring 2014 - 2018
//program: bship.c
//         refactored to bship.cpp
//author:  gordon griesel
//purpose: framework for a battleship game.
//
//This program needs further refactoring.
//Maybe a global class.

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "log.h"
#include "fonts.h"
#include "dsimpson.h"
#include "jrodriguez4.h"
#include "dwelch.h"
#include "thooser.h"

//macros
#define rnd() (double)rand()/(double)RAND_MAX
//prototypes
void init();
void init_opengl();
void render();
void get_grid_center(const int g, const int i, const int j, int cent[2]);
int xres=1200;
int yres=800;

// -----------GRID STRUCTURE ------------------------------------------

//#define MAXGRID 16
const int MAXGRID = 16;
//#define GRIDDIM 10
const int GRIDDIM = 10;
//#define NGRIDS 2
const int NGRIDS = 2;

// copied to thooser.h, use include headers to use
/*

typedef struct t_grid {
	int status;  //status of unit 0=empty 1=shipunit 2=damagedunit
	int shipno;  //ship ID placed there -> 0 = no ship
	int over;
	float color[4];
} Grid;
*/

Grid grid1[MAXGRID][MAXGRID];
Grid grid2[MAXGRID][MAXGRID];
int grid_dim = GRIDDIM;
int board_dim;
int qsize;
int done=0;
int lbutton=0;
int rbutton=0;
#define MAXBUTTONS 8
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;
Button button[MAXBUTTONS];
int nbuttons=0;
//
//
class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert img.jpg img.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};
//Image img[3] = {"./x.ppm", "./explosion.ppm", "./bship.ppm"};

Image img[5] = {"./x.png", "./explosion.png", "./bship.png", "./portraitPlaceholder.png", "./capitalshipcombat.png"};

//
//
GLuint xTexture;
GLuint explosionTexture;
GLuint bshipTexture;
GLuint portraitTexture;
GLuint capitalTexture;
Image *xImage = NULL;
Image *explosionImage = NULL;
Image *bshipImage = NULL;
Image *portraitImage = NULL;
Image *capitalImage = NULL;

// -----------LOG STRUCTURE------------------------------------------

#define MAXQUEUE

std::string Event;
Queue<std::string> logQueue(MAXQUEUE);


// -----------SHIP STRUCTURE------------------------------------------

#define MAXSHIPS 10

// original ship structure, updated and moved to thooser.h
/*

typedef struct t_ship {
	int status;
	int pos[16][2];
	int n;
	int hv;
} Ship;
*/

class Ship Ship;
class Ship ship[MAXSHIPS];
int nships=0;
int nshipssunk=0;
int nbombs=0;

int missileType = 0;
int feature_mode = 0;

//
//modes:
//0 game is at rest
//1 place ships on left grid
//2 search for ships on right grid
//3 game over
enum {
	MODE_READY=0,
	MODE_PLACE_SHIPS,
	MODE_FIND_SHIPS,
	MODE_GAMEOVER
};

static int gamemode=0;
bool credits = false; //off on startup
bool intro = true; // plays on startup
unsigned int pause_screen = 0; //off on startup
int help = 0; // off on startup
int jason_feature = 0; // off on start up
unsigned int game_over = 0; //off on startup
bool dee_feature = false; //off on startup
bool taylorFeature = false; //off on startup, turns on during ship place



class X11_wrapper {
private:
	//X Windows variables
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(xres, yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask |
						KeyPressMask |
						KeyReleaseMask |
						ButtonPressMask |
						ButtonReleaseMask |
						PointerMotionMask |
						StructureNotifyMask |
						SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);

	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "CS335 - OpenGL Animation Template Under XWindows");
	}
	void setup_screen_res(const int w, const int h) {
		xres = w;
		yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		//
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, xres, 0, yres, -1, 1);
		set_title();
		init();
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != xres || xce.height != yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
} x11;

void check_keys(XEvent *e);
void check_mouse(XEvent *e);
void physics();
int check_connecting_quad(int i, int j, int gridno);
int check_for_sink(int s);


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

int main()
{
	logOpen();
	//initXWindows();
	init_opengl();
	initialize_fonts();
	init();
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
		}
		//
		//Below is a process to apply physics at a consistent rate.
		//1. Get the time right now.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while(physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		render();
		x11.swapBuffers();
	}
	//cleanupXWindows();
	cleanup_fonts();
	logClose();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void init_opengl(void)
{
	int h,w;
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	//
	//choose one of these
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	//Htexture = loadBMP("H.bmp");
	//Vtexture = loadBMP("V.bmp");
	//bshipTexture = loadBMP("bship.bmp");
	//
	//load the images file into a ppm structure.
	//
//	xImage          = ppm6GetImage("./x.ppm");
//	explosionImage  = ppm6GetImage("./explosion.ppm");
//	bshipImage      = ppm6GetImage("./bship.ppm");
	xImage          = &img[0];
	explosionImage  = &img[1];
	bshipImage      = &img[2];
	portraitImage 	= &img[3];
	capitalImage 	= &img[4];
	//
	//allocate opengl texture identifiers
	glGenTextures(1, &xTexture);
	glGenTextures(1, &explosionTexture);
	glGenTextures(1, &bshipTexture);
	glGenTextures(1, &portraitTexture);
	glGenTextures(1, &capitalTexture);
	//
	//load textures into memory
	//-------------------------------------------------------------------------
	//H
	w = xImage->width;
	h = xImage->height;
	glBindTexture(GL_TEXTURE_2D, xTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
								GL_RGB, GL_UNSIGNED_BYTE, xImage->data);
	//-------------------------------------------------------------------------
	//V
	w = explosionImage->width;
	h = explosionImage->height;
	glBindTexture(GL_TEXTURE_2D, explosionTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
								GL_RGB, GL_UNSIGNED_BYTE, explosionImage->data);
	//-------------------------------------------------------------------------
	//bship
	w = bshipImage->width;
	h = bshipImage->height;
	glBindTexture(GL_TEXTURE_2D, bshipTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
								GL_RGB, GL_UNSIGNED_BYTE, bshipImage->data);
	//-------------------------------------------------------------------------
	//portrait
	w = portraitImage->width;
	h = portraitImage->height;
	glBindTexture(GL_TEXTURE_2D, portraitTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
								GL_RGB, GL_UNSIGNED_BYTE, portraitImage->data);
	//-------------------------------------------------------------------------

	//portrait
	w = capitalImage->width;
	h = capitalImage->height;
	glBindTexture(GL_TEXTURE_2D, capitalTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
								GL_RGB, GL_UNSIGNED_BYTE, capitalImage->data);
	//-------------------------------------------------------------------------

	glBindTexture(GL_TEXTURE_2D, 0);
	//printf("tex: %i %i\n",Htexture,Vtexture);
}

void reset_grids(void)
{
	//restart the game...
	int i,j;
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			grid1[i][j].status=0;
			grid2[i][j].status=0;
			grid1[i][j].shipno=0;
			grid2[i][j].shipno=0;
		}
	}
	gamemode = MODE_READY;
	nships = 0;
}

void init(void)
{
	board_dim = 400;
	//quad upper-left corner
	//bq is the width of one grid section
	int bq = (board_dim / grid_dim);
	qsize = bq / 2 - 1;
	//
	//initialize buttons...
	nbuttons=0;
	//
	//Quit button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = 10;
	button[nbuttons].r.bot = 10;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx =
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery =
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	//
	//Credits button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = 10;
	button[nbuttons].r.bot = 70;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx =
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery =
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Credits");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	//
	//Reset button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 50;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx =
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery =
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Reset Grids");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
}


// -------- Function prototypes, move to individual .h files ---------


extern int show_dwelch();
extern int show_jason();
extern int show_danny();
//extern void show_taylor();
extern void show_cecilio();

extern void showCredits(int xres, int yres, GLuint portraitTexture);
extern void showIntro(int xres, int yres);
extern void showGameOver(int xres, int yres);
extern void showTeir(int xres, int yres, GLuint xTexture);
extern void FeatureBox(int xres, int yres);
extern void showIntro(int xres, int yres, GLuint capitalTexture);

extern void showGameOver(int xres, int yres);

void check_keys(XEvent *e)
{
	static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return;
	}
	if (e->type == KeyPress) {
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return;
		}
	} else {
		return;
	}
	switch(key) {
		//if (k1 == GLFW_KEY_F2) {
		case XK_Escape:
			done=1;
			break;
		case XK_q:
			done=1;
			break;
		case XK_F2:
			gamemode++;
			taylorFeature = false;
			feature_mode = 0;
			if (gamemode == MODE_PLACE_SHIPS){
				taylorFeature = true;
			}
			if (gamemode == MODE_FIND_SHIPS) {
				feature_mode = 1;
				nshipssunk = 0;
				nbombs = 10;
			}
			if (gamemode > MODE_GAMEOVER) {
				gamemode = MODE_READY;
			}
			break;
		case XK_s:
			show_danny();
			break;
		case XK_g:
			show_cecilio();
			break;
		case XK_d:
			//show_dwelch(); <--- prints delaney to terminal 
			dee_feature = !dee_feature;
			break;
		case XK_t:
			show_taylor();
			break;
		case XK_a:
		//	show_jason(); <--- prints jason in terminal
			jason_feature = toggle(jason_feature);
			break;
		case XK_c:
			credits = !credits;
			break;
		case XK_p:
			pause_screen = manage_state(pause_screen);
			break;
		case XK_F1:
			help = toggle(help);
			break;
		case XK_i:
			intro = !intro;
			break;
		case XK_o:
			game_over = manage_over_state(game_over);
			break;
		case XK_v:
			if (gamemode == MODE_PLACE_SHIPS)
				printf("\ncalling validate function...\n");
				validateShips(grid1, ship, grid_dim);
        break;
		case XK_m:
			missileType ^=1;
			break;
		case XK_f:
			feature_mode = manage_state(feature_mode);
			if (feature_mode) {
				gamemode = MODE_FIND_SHIPS;
			}
			break;
	}
}

void mouse_click(int ibutton, int action, int x, int y)
{
	int con;
	if (action == 1) {
		int i,j,k,m=0;
		//center of a grid
		int cent[2];
		//
		//buttons?
		for (i=0; i<nbuttons; i++) {
			if (button[i].over) {
				button[i].down = 1;
				button[i].click = 1;
				if (i==0) {
					//user clicked QUIT
					done = 1;
				}
				if (i==1) {
					//user clicked credits
					credits = !credits;
				}
				if (i==2) {
					//user clicked reset
					reset_grids();
				}
			}
		}
		
		// placing ships
		
		for (i=0; i<grid_dim; i++) {
			for (j=0; j<grid_dim; j++) {
				// place ships gamemode ----------------------------------------------------
				if (gamemode == MODE_PLACE_SHIPS) {
					get_grid_center(1,i,j,cent);
					if (x >= cent[0]-qsize &&
						x <= cent[0]+qsize &&
						y >= cent[1]-qsize &&
						y <= cent[1]+qsize) {
						
						// if user clicked in left grid
						// TODO: recycle ships
						if (ibutton == 1) {
							//does this quad have any connecting quads?
							if (nships != 0){
							con = check_connecting_quad(i,j,1);
							} else {
								con = 0;
								printf("placing ships...\n");
							}
							if (con != 0) {
								//same ship continued
								grid1[i][j].status=1;
								grid1[i][j].shipno=nships;
								ship[nships].size += 1;
								ship[nships].updateType();
								printf("\t\tship %d updated! type %d\n", grid1[i][j].shipno, ship[nships].type);
							} else {
								if (nships < MAXSHIPS) {
									//new ship being placed!
									grid1[i][j].status = 1;
									nships++;
									grid1[i][j].shipno = nships;
									ship[nships].size = 1;
									ship[nships].updateType();
									printf("\tship %d just placed! nships = %d\n", grid1[i][j].shipno, nships);
								}
							}
						}
						//if (ibutton == GLFW_MOUSE_BUTTON_RIGHT)
						//grid1[i][j].status=2;
						m=1;
						break;
					}
				}
				// find ships gamemode ----------------------------------------------------
				if (gamemode == MODE_FIND_SHIPS) {
					get_grid_center(2,i,j,cent);
					if (x >= cent[0]-qsize &&
						x <= cent[0]+qsize &&
						y >= cent[1]-qsize &&
						y <= cent[1]+qsize) {
						
						// if user clicked in right grid
						if (ibutton == 1) { 
							nbombs--;
							if (grid1[i][j].status) {
								int s = grid1[i][j].shipno;
								grid2[i][j].status = 2;
								if (feature_mode)
									make_particle(cent[0], cent[1], qsize);
								{
									//is this ship sunk?
									if (check_for_sink(s)) {
										nshipssunk++;
										nbombs += 5;
										if (nshipssunk >= nships) {
											gamemode = MODE_GAMEOVER;
										}
									}
								}
							}
							if (feature_mode != 0) {
								if (missileType != 0){
									int radar = 0;
									if (grid1[i][j].status) {
										int s = grid1[i][j].shipno;
										//int s1 = grid1[i+1][j].shipno;
										//int s2 = grid1[i-1][j].shipno;
										//int s3 = grid1[i][j+1].shipno;
										//int s4 = grid1[i][j-1].shipno;
										grid2[i][j].status = 2;
										//grid2[i+1][j].status = 2;
										//grid2[i-1][j].status = 2;
										//grid2[i][j+1].status = 2;
										//grid2[i][j-1].status = 2;
										make_particle(cent[0], cent[1], qsize);
										{
											//is this ship sunk?
											if (check_for_sink(s)) {
												radar++;
												nshipssunk++;
												nbombs += 5;
												if (nshipssunk >= nships) {
													gamemode = MODE_GAMEOVER;
												}
											}
										}
									}
									if (grid1[i + 1][j].status == 1) {
										get_grid_center(2,(i + 1),j,cent);
										int s = grid1[i + 1][j].shipno;
										grid2[i+1][j].status = 2;
										make_particle(cent[0], cent[1], qsize);
										{
											//is this ship sunk?
											if (check_for_sink(s)) {
												if (!(radar >= 1)){
													nshipssunk++;
													nbombs += 5;
												}
												radar++;
												if (nshipssunk >= nships) {
													gamemode = MODE_GAMEOVER;
												}
											}
										}
									}
									if (grid1[i - 1][j].status == 1) {
										get_grid_center(2,(i - 1),j,cent);
										int s = grid1[i-1][j].shipno;
										grid2[i-1][j].status = 2;
										make_particle(cent[0], cent[1], qsize);
										{
											//is this ship sunk?
											if (check_for_sink(s)) {
												if (!(radar >= 1)){
													nshipssunk++;
													nbombs += 5;
												}
												radar++;
												if (nshipssunk >= nships) {
													gamemode = MODE_GAMEOVER;
												}
											}
										}
									}
									if (grid1[i][j + 1].status == 1) {
										get_grid_center(2,i,(j + 1),cent);
										int s = grid1[i][j + 1].shipno;
										grid2[i][j+1].status = 2;
										make_particle(cent[0], cent[1], qsize);
										{
											//is this ship sunk?
											if (check_for_sink(s)) {
												if (!(radar >= 1)){
													nshipssunk++;
													nbombs += 5;
												}
												radar++;
												if (nshipssunk >= nships) {
													gamemode = MODE_GAMEOVER;
												}
											}
										}
									}
									if (grid1[i][j - 1].status == 1) {
										get_grid_center(2,i,(j - 1),cent);
										int s = grid1[i][j-1].shipno;
										grid2[i][j-1].status = 2;
										make_particle(cent[0], cent[1], qsize);
										{
											if (check_for_sink(s)) {
												if (!(radar >= 1)){
													nshipssunk++;
													nbombs += 5;
												}
												radar++;
												if (nshipssunk >= nships) {
													gamemode = MODE_GAMEOVER;
												}
											}
										}
									}
								}
							}
							if (nbombs <= 0) {
								gamemode = MODE_GAMEOVER;
							}
						}
						//if (ibutton == 2)
						//	grid2[i][j].status=2;
						m=1;
						break;
					}
				}
			}
			if (m) break;
		}
	}
	//
	if (action == 2) {
		int i;
		for (i=0; i<nbuttons; i++) {
			button[i].down = 0;
			button[i].click = 0;
		}
	}
}


void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	//Log("check_mouse()...\n");
	int x,y;
	int i,j,k;
	int cent[2];
	lbutton=0;
	rbutton=0;
	if (e->type == ButtonRelease) {
		mouse_click(e->xbutton.button, 2, e->xbutton.x, e->xbutton.y);
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
		}
	}
	//Log("e->xbutton.x: %i e->xbutton.y: %i\n",e->xbutton.x,e->xbutton.y);
	x = e->xbutton.x;
	y = e->xbutton.y;
	//reverse the y position
	y = yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	//Log("xy: %i %i\n",x,y);
	if (x == savex && y == savey)
		return;
	savex=x;
	savey=y;	
	//
	//is the mouse over any grid squares?
	//
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			grid1[i][j].over=0;
			grid2[i][j].over=0;
		}
	}
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			if (gamemode == MODE_PLACE_SHIPS) {
				get_grid_center(1,i,j,cent);
				if (x >= cent[0]-qsize &&
					x <= cent[0]+qsize &&
					y >= cent[1]-qsize &&
					y <= cent[1]+qsize) {
					grid1[i][j].over=1;
					break;
				}
			}
			if (gamemode == MODE_FIND_SHIPS) {
				get_grid_center(2,i,j,cent);
				if (feature_mode != 0) {
					if (x >= cent[0]-qsize &&
						x <= cent[0]+qsize &&
						y >= cent[1]-qsize &&
						y <= cent[1]+qsize && 
						missileType == 0) {
						grid2[i][j].over=1;
						break;
					}
					if (x >= cent[0]-qsize &&
						x <= cent[0]+qsize &&
						y >= cent[1]-qsize &&
						y <= cent[1]+qsize && 
						missileType == 1) {
						grid2[i][j].over=1;
						grid2[i + 1][j].over=1;
						grid2[i - 1][j].over=1;
						grid2[i][j + 1].over=1;
						grid2[i][j - 1].over=1;
						break;
					}
				}
				if (x >= cent[0]-qsize &&
					x <= cent[0]+qsize &&
					y >= cent[1]-qsize &&
					y <= cent[1]+qsize && 
					     missileType == 0) {
					grid2[i][j].over=1;
					break;
				}
				if (x >= cent[0]-qsize &&
					x <= cent[0]+qsize &&
					y >= cent[1]-qsize &&
					y <= cent[1]+qsize && 
					     missileType == 1) {
					grid2[i][j].over=1;
					grid2[i + 1][j].over=1;
					grid2[i - 1][j].over=1;
					grid2[i][j + 1].over=1;
					grid2[i][j - 1].over=1;
					break;
				}
			}
			//if (feature_mode != 0) {
				//if (!LaunchMissile(x, y, cent, qsize, missileType)) {
					//grid2[i][j].over=1;
					//break;
				//}
				//if (LaunchMissile(x, y, cent, qsize, missileType)) {
					//grid2[i][j].over=1;
					//grid2[i + 1][j].over=1;
					//grid2[i - 1][j].over=1;
					//grid2[i][j + 1].over=1;
					//grid2[i][j - 1].over=1;
					//break;
				//}
			//}
		}
		if (grid1[i][j].over) break;
		if (grid2[i][j].over) break;
	}
	//
	//is the mouse over any buttons?
	//
	//Log("check over %i buttons\n", nbuttons);
	for (i=0; i<nbuttons; i++) {
		button[i].over=0;
		button[i].down=0;
		//Log("xy: %i %i\n",x,y);
		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
			button[i].over=1;
			//Log("over button: %i\n", i);
			break;
		}
	}
	if (lbutton)
		mouse_click(1, 1,x,y);
	if (rbutton)
		mouse_click(2, 1,x,y);
}

void physics()
{
	if (feature_mode == 1) {
		explosion_physics();
	}
}

int check_connecting_quad(int i, int j, int gridno)
{
	int l = j-1;
	int t = i-1;
	int r = j+1;
	int b = i+1;
	int s = nships;
	if (gridno==1) {
		//if (grid1[t][l].shipno==s) return 1;
		if (grid1[t][j].shipno==s) return 1;
		//if (grid1[t][r].shipno==s) return 1;
		if (grid1[i][l].shipno==s) return 1;
		if (grid1[i][r].shipno==s) return 1;
		//if (grid1[b][l].shipno==s) return 1;
		if (grid1[b][j].shipno==s) return 1;
		//if (grid1[b][r].shipno==s) return 1;
	}
	return 0;
}

int check_for_sink(int s)
{
	int i,j,sunk=1;
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			if (grid1[i][j].shipno == s &&
				grid2[i][j].status != 2) {
				sunk=0;
				break;
			}
		}
	}
	return sunk;
}



void get_grid_center(const int g, const int i, const int j, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = board_dim/2;
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
	int bq = (board_dim / grid_dim);
	//-------------------------------------
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i);
}

void render(void)
{
	int i,j;
	Rect r;
	//--------------------------------------------------------
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	int b2 = board_dim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	//quad upper-left corner
	int quad[2], saveq0;
	//center of a grid
	int cent[2];
	//bq is the width of one grid section
	int bq = (board_dim / grid_dim);
	int bp;
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, xres, yres);
	//clear color buffer
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	glColor3f(0.8f, 0.6f, 0.2f);
	//
	//show screen background...
	glBindTexture(GL_TEXTURE_2D, bshipTexture);
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,    0);
		glTexCoord2f(0.0f, 0.25f); glVertex2i(0,    yres);
		glTexCoord2f(1.0f, 0.25f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f);  glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//draw the game
	//
	if (gamemode != MODE_FIND_SHIPS) {
		//draw grid #1
		// ...each grid square is drawn
		//
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		for (i=0; i<grid_dim; i++) {
			for (j=0; j<grid_dim; j++) {
				get_grid_center(1,i,j,cent);
				//glColor3f(0.5f, 0.1f, 0.1f);
				glColor4f(0.3f, 1.0f, 0.3f, 0.5f);
				if (grid1[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				if (grid1[i][j].status==1)
					glBindTexture(GL_TEXTURE_2D, xTexture);
				//if (grid1[i][j].status==2) {
					//glBindTexture(GL_TEXTURE_2D, explosionTexture);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2i(cent[0]-qsize,cent[1]-qsize);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2i(cent[0]-qsize,cent[1]+qsize);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2i(cent[0]+qsize,cent[1]+qsize);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2i(cent[0]+qsize,cent[1]-qsize);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glDisable(GL_BLEND);
	}
	//
	//draw grid #2
	// ...each grid square is drawn
	//
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			get_grid_center(2,i,j,cent);
			//glColor3f(0.6f, 0.4f, 0.1f);
			glColor4f(1.0f, 0.8f, 0.5f, 0.6f);
			if (grid2[i][j].over) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			//if (grid2[i][j].status==1)
			//	glBindTexture(GL_TEXTURE_2D, xTexture);
			if (grid2[i][j].status==2) {
				glBindTexture(GL_TEXTURE_2D, explosionTexture);
				if (feature_mode == 1)
						ExplosionAnimation(cent[0],cent[1], qsize, 0);
			}
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2i(cent[0]-qsize,cent[1]-qsize);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2i(cent[0]-qsize,cent[1]+qsize);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2i(cent[0]+qsize,cent[1]+qsize);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2i(cent[0]+qsize,cent[1]-qsize);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDisable(GL_BLEND);
	//
	//show text above each grid...
	//
	{
		unsigned int color = 0x00ffffdd;
		Rect r;
		r.left = xres/4;
		r.bot  = yres-100;
		r.center = 1;
		ggprint16(&r, 0, color, "Grid #1");
		r.left = xres/4*3;
		ggprint16(&r, 0, color, "Grid #2");
		//
		r.bot  = yres-50;
		r.left = xres/2;
		ggprint16(&r, 50, 0x0088aaff, "BATTLESHIP GAME STARTER KIT");
		ggprint16(&r, 0, 0x0088aaff, "**PRESS F1 FOR HELP**");
	}
	
	//
	r.left = 4;
	r.bot  = 200;
	r.center = 0;
	switch(gamemode) {
		case MODE_READY:
			ggprint16(&r, 0, 0x00ffffff, "Press F2 to place ships!");
			break;
		case MODE_PLACE_SHIPS:
			ggprint16(&r, 0, 0x00ffffff,
				"Press F2 when finished placing ships.");
			break;
		case MODE_FIND_SHIPS:
			ggprint16(&r, 0, 0x00ffffff, "Search for ships on grid!");
			break;
		case MODE_GAMEOVER:
			ggprint16(&r, 0, 0x00ffffff, "Game over!");
			break;
	}
	r.left = 4;
	r.bot  = 180;
	r.center = 0;
	ggprint16(&r, 20, 0x00ffff00, "Press M to toggle missile type!");
	r.left = 4;
	r.bot  = 160;
	r.center = 0;
	ggprint16(&r, 20, 0x00ffff00, "nships placed: %i",nships);
	ggprint16(&r, 20, 0x00ffff00, "nships sunk: %i",nshipssunk);
	ggprint16(&r, 20, 0x00ffff00, "nbombs left: %i",nbombs);
	
	if (credits) { // put before buttons to allow button presses during credits
		showCredits(xres, yres, portraitTexture);
	}
	
	//
	//draw all buttons
	//
	for (i=0; i<nbuttons; i++) {
		if (button[i].over) {
			glColor3f(1.0f, 0.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
				glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
				glVertex2i(button[i].r.left-2,  button[i].r.top+2);
				glVertex2i(button[i].r.right+2, button[i].r.top+2);
				glVertex2i(button[i].r.right+2, button[i].r.bot-2);
				glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
			glEnd();
			glLineWidth(1);
		}
		if (button[i].down) {
			glColor3fv(button[i].dcolor);
		} else {
			glColor3fv(button[i].color);
		}
		glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		ggprint16(&r, 0, button[i].text_color, button[i].text);

	}

	if (jason_feature) {
	
		feature_border(xres,yres);
		//game_log(xres,yres);

		/* Not yet working
		logText(logQueue,Event);
		printText(logQueue,xres,yres); 
		*/
		logFrame(xres,yres);

	}

	if (pause_screen != 0) {
        PauseScreen(xres, yres);
	}
	
	if (help) {
		show_help(xres,yres);
	}
	
	if (intro) {
		showIntro(xres, yres, capitalTexture);
	}
	
	if (game_over) {
		showGameOver(xres, yres);
	}

	if (taylorFeature){
		taylorFeatureOverlay(xres, yres);
	}	
	if (dee_feature) {
		FeatureBox(xres,yres);
		showTeir(xres, yres, xTexture);
	}	

	if (feature_mode == 1) {
		FeatureBorder(xres, yres);
	}


}


