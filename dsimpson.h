extern unsigned int manage_state(unsigned int s);
extern void PauseScreen(int xres, int yres);
extern int LaunchMissile(int x, int y, int cent[], int qsize, int missileType);//, int grid2[16][16]);
extern void FeatureBorder(int xres, int yres);
extern void ExplosionAnimation(int x, int y, int width, int m);
extern void make_particle(int x, int y, int width);
extern void explosion_physics();
