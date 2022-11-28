//delaneys include files 
//include guards still needed

//extern void over();
extern unsigned int manage_over_state(unsigned int o);
extern void showGameOver(int xres, int yres, GLuint overTexture);
extern void showTeir(int xres, int yres, GLuint xTexture);
extern void FeatureBox(int xres, int yres);

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
