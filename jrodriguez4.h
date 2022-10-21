// Jason's Include file 
// Update Date : October 17 2022

//*************************************
//---------- Queue Template ----------*
//*************************************

template <class T>
class Queue {

	private:
		T *queueArray; // Points to the queue array
		int queueSize; // The queue size
		int front; // The queue front 
		int rear; // The queue rear
		int queueItems; // Number of Items in the queue
	public:
		// Constructor
		Queue(int);

		// Copy Constructor
		Queue(const Queue &);

		// Destructor
		~Queue();

		// Operations of the Queue
		void enqueue(T);
		void dequeue(T);
		bool isEmpty() const;
		bool isFull() const;
		void clear();
};

//*************************************
//---------- VOID FUNCTIONS ----------*
//*************************************

void logFrame(int xres, int yres);  // Build the frame for the log window
void feature_border(int xres, int yres); // enables feature mode aka log window
void show_help(int xres, int yres); // shows help window

//*************************************
//---------- INT FUNCTIONS -----------*
//*************************************

unsigned int toggle(unsigned int toggle); // used to toggle variables on/off
