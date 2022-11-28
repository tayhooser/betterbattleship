// Purpose : Personal code file
// Author : Jason R.
// Update Date : November 22  2022

#include <iostream> // <--- output/input functions
#include <string>	// <--- string functions
#include <stdio.h>	// <--- output/input functions 
#include <stdlib.h>	
#include <GL/glx.h>	// <--- OpenGl functions and utilities
#include "fonts.h"
#include "jrodriguez4.h"


//|=========================|
//| Check if queue is empty |
//|=========================|
bool Queue::isEmpty() {
	
	if ( front == -1 && rear == -1 )
		return true;
	else
		return false;
}

//|========================|
//| Check if queue is full |
//|========================|
bool Queue::isFull() {

	if (rear == 4 )
		return true;
	else
		return false;
}

//|==========================|
//| Insert string into queue |
//|==========================|
void Queue::enqueue(string action) {
	if ( isFull() ) {
		// cout << " FULL " << endl
		dequeue();
		enqueue(action);
	} else if ( rear == -1 && front == -1 ) {
		rear = front = 0;
		arr[rear] = action;
	} else {
		rear++;
		arr[rear] = action;
	}
}


//|==========================|
//| Delete string from queue |
//|==========================|
void Queue::dequeue() {
	string temp = " ---- ";
	if ( isEmpty() ) {
		cout << "Can't dequeue an empty queue dummy" << endl;
	} else if ( isFull() ) {
		for ( int i=0; i <= 3; i++) {
			arr[i] = arr[i+1];
		}
		arr[rear] = temp;
		rear--;
	} else {
		arr[rear] = temp;
		rear--;
	}
}

//|=====================|
//| Check size of queue |
//|=====================|
int Queue::size() {

	return (rear - front + 1);
}

//|==============================|
//| Shows values inside of queue |
//|==============================|
void Queue::showQueue() {

	cout << "All values in the queue are - " << endl;
	for ( int i=0; i<5; i++) {
		cout << arr[i] << " \n";
	}
}

//|=============================================|
//| allows for toggling of int variables on/off |
//|=============================================|
unsigned int toggle(unsigned int toggle) {

	toggle = toggle ^ 1;
	return toggle;
}

//|===================================|
//| Border to show feature mode is on |
//|===================================|
void feature_border(int xres, int yres) {

	// Triangle strip border
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3f(0.2, 1.0, 0.2);
	glColor4f(0.2, 1.0, 0.2, 0.5);
	int w = 20;
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, 0);
		glVertex2i(w, w);

		glVertex2i(0, yres);
		glVertex2i(0+w, yres-w);

		glVertex2i(xres, yres);
		glVertex2i(xres-w, yres-w);

		glVertex2i(xres, 0);
		glVertex2i(xres-w, w);
		
		glVertex2i(0, 0);
		glVertex2i(w, w);

	glEnd();
	glDisable(GL_BLEND);

}

//|==============|
//| Enables help |
//|==============|
void show_help( int xres, int yres) {

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
	
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(xcent-w, ycent-h);
        glVertex2f(xcent-w, ycent+h);
        glVertex2f(xcent+w, ycent+h);
        glVertex2f(xcent+w, ycent-h);
    glEnd();
    
	r.left = xcent;
	r.bot  = ycent + 250;
	ggprint16(&r, 80, 0xffffffff, "~~~ HELP ~~~");
	
	r.bot = ycent + 230;
    r.center = 50;
	ggprint16(&r, 25, 0xffffffff, " ");
	ggprint16(&r, 25, 0xffffffff, "Ship Placement");
	
	r.left = xcent - 350;
	r.center = 0;
	ggprint16(&r, 25, 0xffffffff, "Place up to 10 ships total:");
	ggprint16(&r, 25, 0xffffffff, "- 1 repair ship (1x1)");
	ggprint16(&r, 25, 0xffffffff, "- 1 planet (3x3)");
	ggprint16(&r, 25, 0xffffffff, "- Up to 8 attack (1x2) or capital (1x3) ships");
	
	r.left = xcent;
	r.center = 50;
	ggprint16(&r, 25, 0xffffffff, " ");
	ggprint16(&r, 25, 0xffffffff, "Attacking Imperial Ships");
	
	r.left = xcent - 350;
	r.center = 0;
	ggprint16(&r, 25, 0xffffffff, "Each attack ship you place gives you 4 1x1 missiles.");
	ggprint16(&r, 25, 0xffffffff, "Each capital ship you place gives you 2 plus-shaped missiles.");
	
	r.left = xcent;
	r.center = 50;
	ggprint16(&r, 25, 0xffffffff, " ");
	ggprint16(&r, 25, 0xffffffff, "Repairing Rebel Ships");
	
	r.left = xcent - 350;
	r.center = 0;
	ggprint16(&r, 25, 0xffffffff, "If your repair ship is still healthy, you may repair 1 unit of any damaged ship.");
	
	
	r.left = xcent;
	r.center = 50;
	ggprint16(&r, 25, 0xffffffff, " ");
	ggprint16(&r, 25, 0xffffffff, "Moving Rebel Ships");
	
	r.left = xcent - 350;
	r.center = 0;
	ggprint16(&r, 25, 0xffffffff, "Once per game, you may move any placed ship. Only ships that are fully healthy");
	ggprint16(&r, 25, 0xffffffff, "may be moved.");
	
	r.left = xcent;
	r.center = 50;
	ggprint16(&r, 25, 0xffffffff, " ");
	ggprint16(&r, 25, 0xffffffff, "Winning and Losing");
	
	r.left = xcent - 350;
	r.center = 0;
	ggprint16(&r, 25, 0xffffffff, "Win by destroying your opponents planet. Lose by running out of");
	ggprint16(&r, 25, 0xffffffff, "missiles or having your own planet destroyed.");
	
	/*
    r.left = xcent;
    r.bot = ycent + 140;
    r.center = 50;

    ggprint16(&r, 50, 0xffffffff, " --- HELP ---");
    ggprint16(&r, 25, 0xffffffff, " cycle 'F2' or press 'F' for Danny's feature");
    ggprint16(&r, 25, 0xffffffff, " press ??? for Cecilio's feature");
    ggprint16(&r, 25, 0xffffffff, " press 'd' for Delaney's feature");
    ggprint16(&r, 25, 0xffffffff, " cycle 'F2' for Taylor's feature");
    ggprint16(&r, 25, 0xffffffff, " press 'a' for Jason's feature (test with x and z)");
    
    ggprint16(&r, 25, 0xffffffff, "");
    
    ggprint16(&r, 25, 0xffffffff, " press 'c' for Credits");
    ggprint16(&r, 25, 0xffffffff, " press 'p' for Pause");
    ggprint16(&r, 25, 0xffffffff, " press 'o' for Game Over");
    ggprint16(&r, 25, 0xffffffff, " press 'F1' for Help");
    ggprint16(&r, 25, 0xffffffff, " press 'i' for Intro");
    ggprint16(&r, 25, 0xffffffff, " press 'Esc' to quit game");
	*/

}

//|====================|
//| Enables log window |
//|====================|
void log_window(int xres, int yres) {

    Rect r;
    int xcent = xres / 2;
    int ycent = yres / 2;
    int w = xres/12.5;
    int h = yres/4;
	glEnable(GL_BLEND);
    glColor4f(0.6, 0.3, 0.6, 0.7);
    glBegin(GL_QUADS);
        glVertex2f(xcent-w, ycent-h);
        glVertex2f(xcent-w, ycent+h);
        glVertex2f(xcent+w, ycent+h);
        glVertex2f(xcent+w, ycent-h);
    glEnd();
    
    r.left = xcent;
    r.bot = ycent+(h - 40);
    r.center = 50;

	ggprint16(&r, 50, 0xfffffff0, "CAPTAIN'S LOG ");
}

//|================|
//| Print log text |
//|================|
void log_print(Queue queueName, int xres, int yres) {

Rect r;
    int xcent = xres / 2;
    int ycent = yres / 2;
    //int w = xres/12.5;
    int h = yres/3;
	
    r.left = xcent;
    r.bot = ycent+(h - 130);
    r.center = 50;
	
	
	ggprint16(&r, 30, 0xfffffff0, "  ");
	ggprint16(&r, 30, 0xfffffff0, queueName.arr[0].c_str());
	ggprint16(&r, 30, 0xfffffff0, "  ");
	ggprint16(&r, 30, 0xfffffff0, queueName.arr[1].c_str());
	ggprint16(&r, 30, 0xfffffff0, "  ");
	ggprint16(&r, 30, 0xfffffff0, queueName.arr[2].c_str());
	ggprint16(&r, 30, 0xfffffff0, "  ");
	ggprint16(&r, 30, 0xfffffff0, queueName.arr[3].c_str());
	ggprint16(&r, 30, 0xfffffff0, "  ");
	ggprint16(&r, 30, 0xfffffff0, queueName.arr[4].c_str());



}

