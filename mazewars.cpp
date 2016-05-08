#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
//#include "Weapon.h"
//#include "Power_up.h"
//#include "Player.h"
#include "joystick.hh"
#include "game.h"
#include "matthewG.h"
#include "roseP.h"
#include "jobG.h"
#include <unistd.h> //required for controller
#include "davidV.h"
#include "defs.h"
#include "person.h"
#include "fonts/fonts.h"
#include "cameronM.h"
#include "cameronM.cpp"
using namespace std;

//defined types
/*
   typedef float Flt;
   typedef float Vec[3];
   typedef Flt	Matrix[4][4];
   typedef int Explosive;
   */

//macros
/*
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
(c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
*/

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
//unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) 
{
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) 
{
		memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

int xres=1250, yres=900;

/*struct Bullet {
  Vec pos;
  Vec vel;
  float color[3];
  struct timespec time;
  struct Bullet *prev;
  struct Bullet *next;
  Bullet() {
  prev = NULL;
  next = NULL;
  }
  void operator=(Bullet x){
  pos[0] = x.pos[0];
  pos[1] = x.pos[1];
  vel[0] = x.vel[0];
  vel[1] = x.vel[1];
  color[0] = x.color[0];
  color[1] = x.color[1];
  color[2] = x.color[2];
  time = x.time;
  }
  };

  const int MAX_BULLETS = 4;

  struct Game {
  Player Player_1;
  Weapon gun;
  Bullet *barr;
  int nbullets;
  int score = 0;
  struct timespec bulletTimer;
  Game() {
  barr = new Bullet[MAX_BULLETS];
  nbullets = 0;
  }
  };
  */

/*struct Person {
  Vec dir;
  Vec pos;
  Vec lastpos;
  Vec vel;
  float angle;
  float color[3];
  Person() {
  VecZero(dir);
  pos[0] = (Flt)(xres/2);
  pos[1] = (Flt)(yres/2);
  pos[2] = 0.0f;
  VecZero(vel);
  angle = 0.0;
  color[0] = 1.0;
  color[1] = 1.0;
  color[2] = 1.0;
  }
  };*/
int keys[65536];
int joy[65536];
int axis[65536];
int people = 0;
Ppmimage *personImage = NULL;
GLuint personTexture;
GLuint silhouetteTexture;
Person person;

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e, Game *game);
void pointPlayer(Game *g, int savex, int savey);
//void getJoystickEvent(JoystickEvent event);
int check_keys(XEvent *e);
void init(Game *g);
void init_sounds(void);
void physics(Game *game);
void render(Game *game);

int main(void)
{
		logOpen();
		initXWindows();
		init_opengl();
		Game game;
		gblock_info gbi;
		gbi.width = 50;
		gbi.height = 50;
		gbi.rows = 50;
		gbi.columns = 50;
		begin_game(game, gbi);
		Joystick joystick;
		init(&game);
		srand(time(NULL));
		clock_gettime(CLOCK_REALTIME, &timePause);
		clock_gettime(CLOCK_REALTIME, &timeStart);
		int done=0;
		while (!done) {
				while (XPending(dpy)) {
						XEvent e;
						XNextEvent(dpy, &e);
						check_resize(&e);
						check_mouse(&e, &game);
						done = check_keys(&e);
				}

				//Joystick events need to be checked outside of XPending(dpy)
				JoystickEvent event;
				if (joystick.sample(&event)) {
						if (event.isButton() || event.isAxis()){
								getJoystickEvent(event, joy, axis);
						}
				}
				clock_gettime(CLOCK_REALTIME, &timeCurrent);
				timeSpan = timeDiff(&timeStart, &timeCurrent);
				timeCopy(&timeStart, &timeCurrent);
				physicsCountdown += timeSpan;
				while (physicsCountdown >= physicsRate) {
						physics(&game);
						physicsCountdown -= physicsRate;
				}
				render(&game);
				glXSwapBuffers(dpy, win);
		}
		cleanupXWindows();
		cleanup_fonts();
		logClose();
		return 0;
}

void cleanupXWindows(void)
{
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
}

void set_title(void)
{
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Maze Wars");
}

void setup_screen_res(const int w, const int h)
{
		xres = w;
		yres = h;
		getScreenRes(w , h);
}

void initXWindows(void)
{
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(xres, yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
				std::cout << "\n\tcannot connect to X server" << std::endl;
				exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
				std::cout << "\n\tno appropriate visual found\n" << std::endl;
				exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
						vi->depth, InputOutput, vi->visual,
						CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
		//window has been resized.
		setup_screen_res(width, height);
		//
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, xres, 0, yres, -1, 1);
		set_title();
}

void init_opengl(void)
{
		//OpenGL initialization
		glViewport(0, 0, xres, yres);
		//Initialize matrices
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		//This sets 2D mode (no perspective)
		glOrtho(0, xres, 0, yres, -1, 1);
		//
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glDisable(GL_CULL_FACE);
		//
		//Clear the screen to black
		glClearColor(0.0, 0.0, 0.0, 1.0);
		//Do this to allow fonts
		glEnable(GL_TEXTURE_2D);
		initialize_fonts();
		job_opengl(personImage, personTexture, silhouetteTexture);
}

void check_resize(XEvent *e)
{
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

void init(Game *g) 
{
		clock_gettime(CLOCK_REALTIME, &g->bulletTimer);
		memset(keys, 0, 65536);
}

void normalize(Vec v) 
{
		Flt len = v[0]*v[0] + v[1]*v[1];
		if (len == 0.0f) {
				v[0] = 1.0f;
				v[1] = 0.0f;
				return;
		}
		len = 1.0f / sqrt(len);
		v[0] *= len;
		v[1] *= len;
}

void check_mouse(XEvent *e, Game *g)
{
		//Did the mouse move?
		//Was a mouse button clicked?
		static int savex = 0;
		static int savey = 0;
		//
		if (e->type == ButtonRelease) {
				return;
		}
		if (e->type == ButtonPress) {
				if (e->xbutton.button==1) {
						//Left button is down
				}
				if (e->xbutton.button==3) {
						//Right button is down
				}
		}
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
				//Mouse moved
				savex = e->xbutton.x;
				savey = e->xbutton.y;
		}
		//point player at mouse
		pointPlayer(g, savex, savey);
}

void pointPlayer(Game *g, int savex, int savey)
{
	//Make the player point at the cursor
	g->gun.pos[0] = g->Player_1.stats.gpos[0];
	g->gun.pos[1] = g->Player_1.stats.gpos[1];
	float weaponx = g->gun.pos[0];
	float weapony = g->gun.pos[1];

	float nDeg = atan(((yres-savey)-(weapony))/\
					((savex)-(weaponx))) * 180 / PI;

	if (savex > weaponx && (yres - savey) > weapony)
			nDeg += 180;
	if (savex > weaponx && (yres - savey) < weapony)
			nDeg -= 180;

	if (g->gun.angle > 360.f)
			g->gun.angle = 360.0f;
	if (g->gun.angle <= 360.0f) {
			if (nDeg > 270)
					nDeg -= 360;
			g->gun.angle = nDeg + 90;
	}
	if (g->gun.angle < 0.0f)
			g->gun.angle += 360.0f;
}

/*void getJoystickEvent(JoystickEvent event)
{
		int number = event.number;
		if (event.type == 0x01) {
				if (joy[number] == 0)
						joy[number] = 1;
				else
						joy[number] = 0;
		}

		if (event.type == 0x02) {
				float value = event.value;
				//std::cout << value << std::endl;
				if (fabs(value) / (float) event.MAX_AXES_VALUE < .2)
						axis[number] = 0;
				else
						axis[number] = value;
		}
}*/

int check_keys(XEvent *e)
{
		//keyboard input?
		int quit;
		static int shift=0;
		int key = XLookupKeysym(&e->xkey, 0);
		//
		//This code maintains an array of key status values.
		if (e->type == KeyRelease) {
				keys[key]=0;
				if (key == XK_Shift_L || key == XK_Shift_R)
						shift=0;
				return 0;
		}
		if (e->type == KeyPress) {
				keys[key]=1;
				if (key == XK_Shift_L || key == XK_Shift_R) {
						shift=1;
						return 0;
				}
		} else {
				return 0;
		}
		if (shift){}
		job_keys(key, quit, person, people);
		if(quit == 1) 
				return 1;
		else
				return 0;
}

/*void movement(Game *g) {
  Flt rad = ((g->Player_1.angle+90.0f) / 360.0f) * PI * 2.0f;

  Flt xdir = cos(rad);
  Flt ydir = sin(rad);

  g->Player_1.vel[0] = xdir*2.0f;
  g->Player_1.vel[1] = ydir*2.0f;
  } */


void physics(Game *g)
{
		//Update Player_1 position
		g->Player_1.stats.gpos[0] += g->Player_1.stats.vel[0];
		g->Player_1.stats.gpos[1] += g->Player_1.stats.vel[1];
		g->gun.pos[0] += g->gun.vel[0];
		g->gun.pos[1] += g->gun.vel[1];
		//Check for collision with window edges
		if (g->Player_1.pos[0] < 0.0f) {
				g->Player_1.pos[0] += (float)xres;
		}
		else if (g->Player_1.pos[0] > (float)xres) {
				g->Player_1.pos[0] -= (float)xres;
		}
		else if (g->Player_1.pos[1] < 0.0f) {
				g->Player_1.pos[1] += (float)yres;
		}
		else if (g->Player_1.pos[1] > (float)yres) {
				g->Player_1.pos[1] -= (float)yres;
		}
		//
		//
		//Update bullet positions
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		for (int i=0; i<g->nbullets; i++) {
				Bullet *b = &g->barr[i];
				//How long has bullet been alive?
				double ts = timeDiff(&b->time, &bt);
				if (ts > 0.5f) {
						//Delete bullet here.
						for(int k = i; k < g->nbullets-1; k++){
								g->barr[k] = g->barr[k+1];

						}
						g->nbullets--;
				}
				//move the bullet
				b->stats.gpos[0] += 6*b->vel[0];
				b->stats.gpos[1] += 6*b->vel[1];
				//Check for collision with window edges
				/*if (b->pos[0] < 0.0f) {
				  b->pos[0] += (float)xres;
				  }
				  else if (b->pos[0] > (float)xres) {
				  b->pos[0] -= (float)xres;
				  }
				  else if (b->pos[1] < 0.0f) {
				  b->pos[1] += (float)yres;
				  }
				  else if (b->pos[1] > (float)yres) {
				  b->pos[1] -= (float)yres;
				  }*/
		}
		//---------------------------------------------------
		//check keys pressed now
		if (keys[XK_a] && (g->Player_1.Current_Health > 0)) {
				g->Player_1.angle += 4.0f;
				if (g->Player_1.angle >= 360.0f)
						g->Player_1.angle -= 360.0f;
		}
		if (keys[XK_d] && (g->Player_1.Current_Health > 0)) {
				g->Player_1.angle -= 4.0f;
				if (g->Player_1.angle < 0.0f)
						g->Player_1.angle += 360.0f;
		}
		if (keys[XK_w] && (g->Player_1.Current_Health > 0)) {
				//convert Player_1 angle to radians
				Flt rad = ((g->Player_1.angle+90.0f) / 360.0f) * PI * 2.0f;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				g->Player_1.stats.vel[0] = xdir;
				g->Player_1.stats.vel[1] = ydir;
				//	g->gun.pos[0] = xdir;
				//	g->gun.pos[1] = ydir;
		} else {
				g->Player_1.stats.vel[0] = 0;
				g->Player_1.stats.vel[1] = 0;
		}
		if (keys[XK_s] && (g->Player_1.Current_Health > 0)) {
				//convert Player_1 angle to radians
				Flt rad = ((g->Player_1.angle+90.0f) / 360.0f) * PI * 2.0f;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				g->Player_1.stats.vel[0] = -1 * xdir;
				g->Player_1.stats.vel[1] = -1 * ydir;
		}
		if (axis[0] || axis[1] ||  axis[4] || axis[5]) {	
				checkController(axis, g);
		}

		if ((keys[XK_space] || joy[0]) && (g->Player_1.Current_Ammo > 0) && 
			(g->Player_1.Current_Health > 0)) {
				//a little time between each bullet
				struct timespec bt;
				clock_gettime(CLOCK_REALTIME, &bt);
				double ts = timeDiff(&g->bulletTimer, &bt);
				if (ts > 0.1 && g->nbullets < MAX_BULLETS) {
						timeCopy(&g->bulletTimer, &bt);
						//shoot a bullet...
						Bullet *b = &g->barr[g->nbullets];
						timeCopy(&b->time, &bt);
						b->stats.gpos[0] = g->gun.pos[0];
						b->stats.gpos[1] = g->gun.pos[1];
						b->vel[0] = g->gun.vel[0];
						b->vel[1] = g->gun.vel[1];
						//convert Player_1 angle to radians
						Flt rad = ((g->gun.angle+90.0f) / 360.0f) * PI * 2.0f;
						//convert angle to a vector
						Flt xdir = cos(rad);
						Flt ydir = sin(rad);
						b->stats.gpos[0] += xdir*20.0f;
						b->stats.gpos[1] += ydir*20.0f;
						b->vel[0] += xdir*6.0f + rnd()*0.1f;
						b->vel[1] += ydir*6.0f + rnd()*0.1f;
						b->stats.color[0] = 1.0f;
						b->stats.color[1] = 1.0f;
						b->stats.color[2] = 1.0f;
						g->nbullets++;
						g->Player_1.Current_Ammo--;
				}
		}
		if(keys[XK_F7]){
				if(g->Player_1.Current_Health > 0)
						g->Player_1.Current_Health -= 5;
		}
		if(keys[XK_F6]){
				Restart(&g->Player_1);
		}
}

void render(Game *g)
{
		glClear(GL_COLOR_BUFFER_BIT);

		drawGBlocks(g);
		//Draw the Player_1
		if(g->Player_1.Current_Health > 0)
				drawOType(g->Player_1, g);

		if (axis[3] || axis[4])
			renderCrosshair(axis, g, false);
		if (joy[4] || keys[XK_b]) 
			renderShield(g);
		
		//Draw the bullets
		for (int i=0; i<g->nbullets; i++) {
				Bullet *b = &g->barr[i];
				if (b != NULL) {
						drawOType(b, g);
				}
		}
		drawHealth(g->Player_1);
		drawAmmo(g->Player_1);
		if(g->Player_1.Current_Health == 0)
				GameOver();
		drawHealthPack(500, 400, 0);
		drawHealthPack(100, 800, 0);
		if (people) {
				glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
				glPushMatrix();
				glTranslatef(person.pos[0], person.pos[1], person.pos[2]);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.0f);
				glBindTexture(GL_TEXTURE_2D, personTexture);
				glBegin(GL_QUADS);
				float w = 325;
				glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, w);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,-w);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_ALPHA_TEST);
				glPopMatrix();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
}
