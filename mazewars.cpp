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
#include "joystick.hh"
#include "game.h"
#include "cameronM.h"
#include "matthewG.h"
#include "roseP.h"
#include "jobG.h"
#include "davidV.h"
#include "defs.h"
#include "person.h"
#include "fonts/fonts.h"
#include </usr/include/AL/alut.h>
#include "mtime.h"

using namespace std;

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
static float VOLUME = 1.0f;
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
struct timespec timeT1;
struct timespec timeT2;
struct timespec timeT3;
struct timespec timeT4;
struct timespec timeT5;
struct timespec timeT6;
double physicsCountdown=0.0;
double timeSpan=0.0;
double timeSpanT1=0.0;
double timeSpanT2=0.0;
double timeSpanT3=0.0;
double timeSpanT4=0.0;
double timeSpanT5=0.0;
double timeSpanT6=0.0;

int xres=1250, yres=900;
int keys[65536];
int joy[65536];
int axis[65536];
GLuint texture[10];
int people = 0;
int titleScreen = 1;
int parallax = 1;
bool winCondition = false;
bool Pause = false;
Ppmimage *testImage = NULL;
GLuint testTexture;
GLuint silhouetteTexture;
bool QUIT = 0;

bool showtest = 0;
Ppmimage *personImage1 = NULL;
Ppmimage *personImage1c = NULL;
GLuint personTexture1;
GLuint personTexture1c;
Ppmimage *introImages[10] = {NULL};
GLuint introTextures[10];
Ppmimage *winImages[10] = {NULL};
GLuint winTextures[10];
Ppmimage *mazeImage = {NULL};
GLuint mazeTexture;
// GLuint titleTexture; //introTexture[0]
// GLuint boulderTexture; //introTexture[1]
// GLuint logoTexture; //introTexture[2]
// GLuint enterTexture; //introTexture[3]
// GLuint optionsTexture; //introTexture[4]

Person person;
Person personc;
int enterPressed = 0;
int downPressed = 0;
int upPressed = 0;

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e);
void init(Game *g);
void physics(Game *game);
void render(Game *game);

int main(int argc, char *argv[])
{
	logOpen();
	initXWindows();
	init_opengl();
	init_sounds();
	load_sounds();
	Game game;
	gblock_info gbi;
	gbi.width = 32;
	gbi.height = 32;
	gbi.rows = 100;
	gbi.columns = 100;
	begin_game(game, gbi);
	init_textures(game);
	Joystick joystick;
	init(&game);
	printf("%f\n",game.Player_1.stats.color[0]);
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
		if (argc == 1) {
			if (titleScreen) {
				if (keys[XK_Return]) {
					enterPressed = keys[XK_Return];
				}
				if (keys[XK_Down]) {
					downPressed ^= keys[XK_Down];
					upPressed = 0;
					enterPressed = 0;
				}
				if (keys[XK_Up]) {
					upPressed ^= keys[XK_Up];
					downPressed = 0;
					enterPressed = 0;
				}
				titleScreen = renderTitleScreen(introTextures, introImages, 
								enterPressed, downPressed, upPressed, keys);
			} else if (Pause) {
	
			QUIT = PAUSE(&game, keys);
			if (QUIT)
				return(0);

		} else if (winCondition) {
				//test
					endCredits(&game, keys);
			} 
			else {
				glClearColor(0.8, 0.8, 0.8, 1.0);
				render(&game);
			}
		}  
		else if (winCondition) {
			//test
				endCredits(&game, keys);
		}  
		else if (Pause) {
	
			QUIT = PAUSE(&game, keys);
			if (QUIT)
				return(0);

		} else {
		    glClearColor(0.8,0.8,0.8,1.0);
		    render(&game);
		}
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	logClose();
	release_sounds();
	convertPPMtoPNG();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	const char image[15] = {'i','m','a','g','e','s','/','H','e','a','d','.','p','n','g'};
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Maze Wars");
	XSetIconName(dpy, win, image);
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
	resolution(xres, yres);
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
	resolution(width, height);

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
	glDisable(GL_BLEND);
	//
	//Clear the screen to black
	glClearColor(0.8, 0.8, 0.8, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	//glGenTextures(2, texture);
	initialize_fonts();
	
	//personImage1 = ppm6GetImage((char*)"images/red.ppm");
	//personImage2 = ppm6GetImage((char*)"images/sprite.ppm");
	//string characterSelected = "red";
	//personImage1 = characterSelection(characterSelected);
	//job_opengl(personImage1, personTexture1);
	//job_opengl(personImage2, personTexture2);

	convertPNGtoPPM();

	string characterSelected = "red";
	personImage1 = characterSelection(characterSelected);
	personImage1c = ppm6GetImage((char*)"images/enemy.ppm");
	
	testImage = ppm6GetImage((char*)"images/ZombieStand.ppm");
	
	
	introImages[0] = ppm6GetImage((char*)"images/titleBackground.ppm");
	introImages[1] = ppm6GetImage((char*)"images/boulder.ppm");
	introImages[2] = ppm6GetImage((char*)"images/logo.ppm");
	introImages[3] = ppm6GetImage((char*)"images/enterBold.ppm");
	introImages[4] = ppm6GetImage((char*)"images/optionsBold.ppm");
	introImages[5] = ppm6GetImage((char*)"images/Arrow.ppm");
	introImages[6] = ppm6GetImage((char*)"images/sign.ppm");
	introImages[7] = ppm6GetImage((char*)"images/Arrow.ppm");
	
	//mazeImage = ppm6GetImage((char*)"images/maze1.ppm");

	glGenTextures(1, &testTexture);
	glGenTextures(1, &personTexture1);
	glGenTextures(1, &personTexture1c);
	glGenTextures(1, &introTextures[0]); //titleTexture
	glGenTextures(1, &introTextures[1]); //boulderTexture
	glGenTextures(1, &introTextures[2]); //logoTexture
	glGenTextures(1, &introTextures[3]); //enterTexture
	glGenTextures(1, &introTextures[4]); //optionsTexture
	glGenTextures(1, &introTextures[5]); //ArrowTexture
	glGenTextures(1, &introTextures[6]); //signTexture

	glGenTextures(1, &mazeTexture);

	
	/****testing the zombie sprite********************/
	float w = testImage->width;
	float h = testImage->height;
	
	glBindTexture(GL_TEXTURE_2D, testTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
							   testImage->data);

	//glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	unsigned char *testData = buildAlphaData(testImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, testData);
	free(testData);
	/*************************************************/
	
	//Character Texture
	w = personImage1->width;
	h = personImage1->height;

	glBindTexture(GL_TEXTURE_2D, personTexture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	unsigned char *personData = buildAlphaData(personImage1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, personData);

	free(personData);
	
	//Characterc Texture
	w = personImage1c->width;
	h = personImage1c->height;

	glBindTexture(GL_TEXTURE_2D, personTexture1c);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	unsigned char *personDatac = buildAlphaData(personImage1c);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, personDatac);

	free(personDatac);

	//Title Screen Background Texture
	w = introImages[0]->width;
	h = introImages[0]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *titleBackgroundData = buildAlphaData(introImages[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, titleBackgroundData);
	free(titleBackgroundData);

	//boulder falling texture
	w = introImages[1]->width;
	h = introImages[1]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *boulderData = buildAlphaData(introImages[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, boulderData);
	free(boulderData);

	//MazeWars logo texture
	w = introImages[2]->width;
	h = introImages[2]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *logoData = buildAlphaData(introImages[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, logoData);
	free(logoData);

	//enter maze texture
	w = introImages[3]->width;
	h = introImages[3]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *enterData = buildAlphaData(introImages[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, enterData);
	free(enterData);

	//options texture
	w = introImages[4]->width;
	h = introImages[4]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *optionsData = buildAlphaData(introImages[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, optionsData);
	free(optionsData);

	//Arrow Texture
	w = introImages[5]->width;
	h = introImages[5]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *arrowData = buildAlphaData(introImages[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, arrowData);
	free(arrowData);
	
	//Sign texture
	w = introImages[6]->width;
	h = introImages[6]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *signData = buildAlphaData(introImages[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, signData);
	free(signData);

	//arrow 2 texture
	w = introImages[7]->width;
	h = introImages[7]->height;
	glBindTexture(GL_TEXTURE_2D, introTextures[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *arrow2Data = buildAlphaData(introImages[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, arrow2Data);
	free(arrow2Data);

	//maze texture
	/*w = mazeImage->width;
	h = mazeImage->height;
	glBindTexture(GL_TEXTURE_2D, mazeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *mazeData = buildAlphaData(mazeImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, mazeData);
	free(mazeData);	*/


	//loadEndCreditsTextures();
	loadLivesSprite();
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
		//reshape_window(xce.width, xce.height);
		XResizeWindow(dpy, win, xres, yres);
	}
}

void init(Game *g) 
{
	clock_gettime(CLOCK_REALTIME, &g->bulletTimer);
	memset(keys, 0, 65536);
	g->Player_1.stats.color[0] = 255;
	g->Player_1.stats.spos[0] = 625;
	g->Player_1.stats.spos[1] = 450;
	
	g->mon[0].stats.spos[0] = 500;
	g->mon[0].stats.spos[1] = 500;
	g->mon[0].spawnPos[0] = 500;
	g->mon[0].spawnPos[1] = 500;
	
	g->mon[1].stats.spos[0] = 600;
	g->mon[1].stats.spos[1] = 600;
	g->mon[1].spawnPos[0] = 600;
	g->mon[1].spawnPos[1] = 600;
	
	g->mon[2].stats.spos[0] = 700;
	g->mon[2].stats.spos[1] = 400;
	g->mon[2].spawnPos[0] = 700;
	g->mon[2].spawnPos[1] = 400;
	
	g->mon[3].stats.spos[0] = 400;
	g->mon[3].stats.spos[1] = 300;
	g->mon[3].spawnPos[0] = 400;
	g->mon[3].spawnPos[1] = 300;
	
	g->mon[4].stats.spos[0] = 300;
	g->mon[4].stats.spos[1] = 200;
	g->mon[4].spawnPos[0] = 300;
	g->mon[4].spawnPos[1] = 200;
	printf("%f\n",g->Player_1.stats.color[0]);
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
	if (quit == 1) 
	return 1;
	else
	return 0;
}

void physics(Game *g)
{
	check_gblock_collision(g->Player_1, g, 0.1, 0.1);
	updateObjGposStat(g->Player_1);

	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	for (int i = 0; i < g->nbullets; i++) {
		Bullet *b = &g->barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 0.5f) {
			//Delete bullet here.
			for (int k = i; k < g->nbullets-1; k++) {
				g->barr[k] = g->barr[k+1];
			}
			g->nbullets--;
		}
		//move the bullet
		b->stats.gpos[0] += 6*b->stats.vel[0];
		b->stats.gpos[1] += 6*b->stats.vel[1];
	}
	if (keys[XK_z]) {
		//g->mon[6] = new Monster;
		g->mon[6].type = 0;
		g->mon[6].mode = 1;
		g->mon[6].stats.gpos[0] = g->Player_1.stats.gpos[0] + 25;
		g->mon[6].stats.gpos[1] = g->Player_1.stats.gpos[1] + 25;
	}
	if (keys[XK_p]) {
		timeSpanT6 = timeDiff(&timeT6, &timeCurrent);
                if(timeSpanT6 > 0.2){
                        clock_gettime(CLOCK_REALTIME, &timeT6);
			Pause = !Pause;
		}
	}
	if (keys[XK_a] && !g->Player_1.gameOver && !Pause && !winCondition) {
		g->Player_1.stats.angle += 4.0f;
		if (g->Player_1.stats.angle >= 360.0f)
		g->Player_1.stats.angle -= 360.0f;

		person.pos[0] =	g->Player_1.stats.spos[0];
		person.pos[1] = g->Player_1.stats.spos[1];
	}
	if (keys[XK_l]) {
		VOLUME += 0.01f;
		if (VOLUME >= 1.0f)
			VOLUME = 1.0f;
	}
	if (keys[XK_k]) {
		VOLUME -= 0.01f;
		if (VOLUME <= 0.0f)
			VOLUME = 0.0f;
	}
	if (keys[XK_d] && !g->Player_1.gameOver && !Pause && !winCondition) {
		g->Player_1.stats.angle -= 4.0f;
		if (g->Player_1.stats.angle < 0.0f)
		g->Player_1.stats.angle += 360.0f;
	}
	if (keys[XK_w] && !g->Player_1.gameOver && !Pause && !winCondition) {
		//convert Player_1.stats.angle to radians
		Flt rad = ((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		g->Player_1.stats.vel[0] = xdir;
		g->Player_1.stats.vel[1] = ydir;
		timeSpanT5 = timeDiff(&timeT5, &timeCurrent);
        if (timeSpanT5 > 0.3) {
			clock_gettime(CLOCK_REALTIME, &timeT5);
			play_sounds(2);
		}
	} else {
		g->Player_1.stats.vel[0] = 0;
		g->Player_1.stats.vel[1] = 0;
	}
	if (keys[XK_s] && !g->Player_1.gameOver && !Pause && !winCondition) {
		//convert Player_1.stats.angle to radians
		Flt rad = ((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		g->Player_1.stats.vel[0] = -1 * xdir;
		g->Player_1.stats.vel[1] = -1 * ydir;
		timeSpanT5 = timeDiff(&timeT5, &timeCurrent);
		if (timeSpanT5 > 0.3) {
			clock_gettime(CLOCK_REALTIME, &timeT5);
			play_sounds(2);
		}
	}
	if (axis[0] || axis[1] ||  axis[4] || axis[5]) {	
		checkController(axis, g);
	}

	if ((keys[XK_space] || joy[0]) && g->Player_1.Current_Ammo > 0 && 
		  g->Player_1.Current_Health > 0 && !Pause && !winCondition) {
		g->Player_1.stats.angle = g->gun.stats.angle;
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g->bulletTimer, &bt);
		if (ts > 0.1 && g->nbullets < MAX_BULLETS) {
			 play_sounds(0);
			timeCopy(&g->bulletTimer, &bt);
			//shoot a bullet...
			Bullet *b = &g->barr[g->nbullets];
			timeCopy(&b->time, &bt);
			b->stats.gpos[0] = g->gun.stats.gpos[0];
			b->stats.gpos[1] = g->gun.stats.gpos[1];
			b->stats.vel[0] = g->gun.stats.vel[0];
			b->stats.vel[1] = g->gun.stats.vel[1];
			//convert Player_1.stats.angle to radians
			Flt rad = ((g->gun.stats.angle+90.0f) / 360.0f) * PI * 2.0f;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			b->stats.gpos[0] += xdir*20.0f;
			b->stats.gpos[1] += ydir*20.0f;
			b->stats.vel[0] += xdir*6.0f + rnd()*0.1f;
			b->stats.vel[1] += ydir*6.0f + rnd()*0.1f;
			b->stats.color[0] = 1.0f;
			b->stats.color[1] = 1.0f;
			b->stats.color[2] = 1.0f;
			g->nbullets++;
			g->Player_1.Current_Ammo--;
		}
	}
	if (keys[XK_F5] && !Pause && !winCondition) {
		if (g->Player_1.Current_Health > 0)
			play_sounds(4);
		else 
			return;
	}

	if (keys[XK_F7] && !g->Player_1.gameOver && !Pause) {
		if (g->Player_1.Current_Health > 0)
			g->Player_1.Current_Health -= 5;
	}
	if (keys[XK_F6] && !Pause) {
		Restart(g);
	}
	if (keys[XK_F8] && !Pause) {
		timeSpanT1 = timeDiff(&timeT1, &timeCurrent);
		if (timeSpanT1 > 0.2) {
			clock_gettime(CLOCK_REALTIME, &timeT1);
			g->Player_1.artifact[0] = !g->Player_1.artifact[0];
		}
	}
	if(keys[XK_F9] && !Pause) {
		timeSpanT2 = timeDiff(&timeT2, &timeCurrent);
                if (timeSpanT2 > 0.2) {
                        clock_gettime(CLOCK_REALTIME, &timeT2);
                        g->Player_1.artifact[1] = !g->Player_1.artifact[1];
                }
	}
	if (keys[XK_F10] && !Pause) {
		timeSpanT3 = timeDiff(&timeT3, &timeCurrent);
                if (timeSpanT3 > 0.2) {
                        clock_gettime(CLOCK_REALTIME, &timeT3);
                        g->Player_1.artifact[2] = !g->Player_1.artifact[2];
			g->mon[0].pursuit = !g->mon[0].pursuit;
                }

	}
	if (keys[XK_F12] && !Pause) {
		timeSpanT4 = timeDiff(&timeT4, &timeCurrent);
                if (timeSpanT4 > 0.2){
                        clock_gettime(CLOCK_REALTIME, &timeT4);
			g->Player_1.lives += 1;
			g->Player_1.lives = g->Player_1.lives % 5;
		}
	}
	if (g->Player_1.artifact[0] && g->Player_1.artifact[1] && g->Player_1.artifact[2] && !g->Player_1.gameOver) {
		if (g->Player_1.Current_Health > 0)
		    winCondition = true;
	    
	}
}

//struct timespec animationStart, animationCurrent;
//double animationSpan=0.0;
static int a = 0;
void render(Game *g)
{
	
	//~ if(showtest){
		//~ renderEnemy(g, testImage->width, testTexture);
	//~ }
	
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	//if(animationSpan > 60/1000.0f) {	
	//	clock_gettime(CLOCK_REALTIME, &animationStart);
	//}
	drawGBlocks(g);
	//Draw the Player_1
	//if(g->Player_1.Current_Health > 0 && !g->Player_1.gameOver)
	//	drawOType(g->Player_1, g);

	play_sounds(6);
	//render_maze(g, mazeTexture, mazeImage);

		
	if (axis[3] || axis[4])
		renderCrosshair(axis, g, false);
	if (joy[4] || keys[XK_b]) 
		renderShield(g);	
	
	if (g->Player_1.Current_Health == 0) {
		g->Player_1.lives--;
		play_sounds(1);
		//Play death groan upon player death
		if (g->Player_1.lives == 0) {
			play_sounds(3);
		}
		g->Player_1.Current_Health = g->Player_1.Max_Health;
	}
	if (g->Player_1.lives == 0) {
		GameOver();
		g->Player_1.gameOver = true;
	}
	//drawHealthPack(500, 400, 0, g);
	//drawHealthPack(100, 800, 0, g);
	float w = personImage1->width/4;

	renderEnemy(g->mon[6], g);

	if (g->Player_1.gameOver == false)
		//renderCharacter(person, g, w, keys, personTexture1); 
		renderCharacter(g->Player_1, g, w, keys, personTexture1);

	for (int i=0; i<g->nbullets; i++) {
		Bullet *b = &g->barr[i];
		if (b != NULL)
			drawBullet(g, b, 0.0, 0.0, 0.0);	
	}
	for (int i = 0; i < 5; i++) {
		if (keys[XK_w] && g->mon[i].alive && !Pause && !g->Player_1.gameOver) {
			g->mon[i].gameMove(1);
		}
		else if (keys[XK_s] && g->mon[i].alive) {
			g->mon[i].gameMove(0);
		}
		if (g->mon[i].alive && !Pause) {
			renderCharacterEnemy(personc, g, w, keys, personTexture1c, i); 
			personc.pos[0] = g->mon[i].stats.spos[0];
			personc.pos[1] = g->mon[i].stats.spos[1];
			g->mon[i].move();
			g->mon[i].draw();
			monster(g, i, 1,2);
		}
	}
	//shadowBox();
	drawHUD(&g->Player_1);

	graveKeyPress(keys);
	//Keystroke for R
	if (keys[XK_r]) {
                 pressR(g);
         }
	getVolume(VOLUME);
}
