// Individual source file
// Author: Jason Rodriguez
// Update Date : October 17 2022

#include <iostream> // <--- output/input functions
#include <string>	// <--- string functions
#include <stdio.h>	// <--- output/input functions 
#include <stdlib.h>	
#include <GL/glx.h>	// <--- OpenGl functions and utilities
#include <string>	// <--- String class
#include "fonts.h"	
#include "jrodriguez4.h"


//****************************************************
//---------- Constructors and Destructors -----------*
//****************************************************

// Constructor creates an empty queue of a specified size
template <class T>
Queue<T>::Queue(int s)
{
	queueArray = new T[s];
	queueSize = s;
	front = -1;
	rear = -1;
	queueItems = 0;
}

// Copy Constructor
template <class T>
Queue<T>::Queue(const Queue & obj)
{
	// Allocate the queue array
	queueArray = new T[obj.queueSize];

	// Copy object attibutes
	queueSize = obj.queueSize;
	front = obj.front;
	rear = obj.rear;
	queueItems = obj.queueItems;

	// Copy the other object's queue array
	for (int i = 0; i < obj.queueSize; i++) {
		queueArray[i] = obj.queueArray[i];
	}
}

// Destructor
template <class T>
Queue<T>::~Queue()
{
	delete [] queueArray;
}

//************************************
//---------- INT FUNCTIONS ----------*
//************************************

// allows for toggling of int variables on/off
unsigned int toggle(unsigned int toggle) {

	toggle = toggle ^ 1;
	return toggle;
}

//*************************************
//---------- BOOL FUNCTIONS ----------*
//*************************************

// Checks if queue is Empty
template <class T>
bool Queue<T>::isEmpty() const
{
	bool status;

	if (queueItems)
		status = false;
	else
		status = true;
	
	return status;
}

// Check if queue is Full
template <class T>
bool Queue<T>::isFull() const
{
	bool status;

	if (queueItems < queueSize)
		status = false;
	else
		status = true;

	return status;

}

//*************************************
//---------- VOID FUNCTIONS ----------*
//*************************************

// Clear the queue
template <class T>
void Queue<T>::clear()
{
	front = queueSize - 1;
	rear = queueSize - 1;
	queueItems = 0;
}

// Enqueue function to insert a value at rear
template <class T>
void Queue<T>::enqueue(T item)
{
	if (isFull()) {
		std::cout << "Queue is full" << std::endl;
	}
	else {
		rear = (rear + 1) % queueSize;
		// Insert Item
		queueArray[rear] = item;
		//Update count
		queueItems++;
	}
}

// Dequeue function to remove value at front of queue
template <class T>
void Queue<T>::dequeue(T item)
{
	if (isEmpty())
		std::cout << "Queue is empty" << std::endl;
	else {
		// Move front
		front = (front + 1 ) % queueSize;
		// Get front item
		item = queueArray[front];
		//Update items
		queueItems--;
	}
}
// Print log Queue
//template <class T>
void printText(Queue<std::string> queueName, int xres, int yres) { 

}
// Log event
template <class T>
void logText(Queue<std::string> queueName, std::string event) {
	
    

}
// Build the log window frame
void logFrame(int xres, int yres) {

    	Rect r;
	int xcent = xres / 2;		
	int ycent = yres / 2;		
	int w = xres/12;
	int h = xres/6;
	glEnable(GL_BLEND);
    	glColor3f(0.6, 0.6, 0.6);
    	glColor4f(0.6, 0.6, 0.6, 0.7);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-h);
        	glVertex2f(xcent-w, ycent+h);
        	glVertex2f(xcent+w, ycent+h);
        	glVertex2f(xcent+w, ycent-h);
    	glEnd();
	glDisable(GL_BLEND);
    	r.left = xres/2;
    	r.bot = yres/1.45;
    	r.center = 50;
        
    	ggprint16(&r, 50, 0xffffffff, " ---- Game Log ----");

}

// Border to show feature mode is on
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

// Help screen 
void show_help( int xres, int yres) {

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
    r.bot = ycent + 120;
    r.center = 50;
        
    ggprint16(&r, 50, 0xffffffff, " --- HELP ---");
    ggprint16(&r, 25, 0xffffffff, " press 's' for Danny");
    ggprint16(&r, 25, 0xffffffff, " press 'g' for Cecilio");
    ggprint16(&r, 25, 0xffffffff, " press 'd' for Delaney");
    ggprint16(&r, 25, 0xffffffff, " press 't' for Taylor");
    ggprint16(&r, 25, 0xffffffff, " press 'a' for Jason");
    
    ggprint16(&r, 25, 0xffffffff, "");
    
    ggprint16(&r, 25, 0xffffffff, " press 'c' for Credits");
    ggprint16(&r, 25, 0xffffffff, " press 'p' for Pause");
    ggprint16(&r, 25, 0xffffffff, " press 'o' for Game Over");
    ggprint16(&r, 25, 0xffffffff, " press 'F1' for Help");
    ggprint16(&r, 25, 0xffffffff, " press 'i' for Intro");
    ggprint16(&r, 25, 0xffffffff, " press 'Esc' to quit game");

}

