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

/*
void over(){
	Rect r;
	ggprint16(&r, 50, 0xffffffff, "Game Over ");
}
extern class Show_Over show_over;

Show_Over::~Show_Over() { delete [] data; }

Show_Over::Show_Over(const char *fname) {
	if (fname[0] == '\0')
		return;
	char name[40];
	strcpy(name, fname);
	int slen = strlen(name);
	name[slen-4] = '\0';
	char ppmname[80];
	sprintf(ts, "convert %s %s" fname, ppmname);
	system(ts);
	FILE *fpi = fopen(ppmname, "r");
	if (fpi) { 
		char line[200];
		fgets(line, 200, fpi);
		fgets(line, 200, fpi);

		while (line[0] == '#' || strlen(line) < 2)
			fgets(line, 200, fpi);
		sscanf(line, "%i %i", &width, &height);
		fgets(line, 200, fpi);

		int n = width * height * 3;
		data = new unsigned char[n];
		for (int i=0; i<n; i++)
			data[i] = fgetc(fpi);
		fclose(fpi);
	}else {
		printf("ERROR opening image: %s\n" ppmname);
		exit(0);
	}
	unlink(ppmname);
}
*/
