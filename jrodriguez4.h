// Jason's Include file 
// Update Date : November 2  2022
#include <iostream>
#include <string>

using namespace std;

//
//QUEUE FOR STRINGS ( size of 5 )
//

class Queue {

	private:
		int front, rear;
	public:
		string arr[5];
	Queue() {
		front = -1;
		rear = -1;
		for ( int i=0; i<5; i++) {
			arr[i] = " ---- ";
		}
	}
	bool isEmpty();
	bool isFull();
	void enqueue(string action);
	void dequeue();
	int size();
	void showQueue();
};

//
//FUNCTIONS
//
void log_print(Queue queueName, int xres, int yres); // shows log window 
void log_window(int xres, int yres); // shows log window 
void feature_border(int xres, int yres); // enables feature mode
void show_help(int xres, int yres); // shows help window
unsigned int toggle(unsigned int toggle); // used to toggle variables on/off
