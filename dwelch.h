//delaneys include files 
//include guards still needed

//extern void over();
extern unsigned int manage_over_state(unsigned int o);

#ifndef _GameOver_
#define _GameOver_
class Show_Over{
	public:
		int width, height;
		unsigned char *data;
		~Show_Over();
		Show_Over(const char *fname);
};
#endif

extern int show_dwelch();

extern void showGameOver(int xres, int yres, string gameOver);
