// Name: Matthew Gaitan
// Date: April 27, 2016
// Last Modified: May 11, 2016
// Description: My code handles all of the character movement if a controller 
// is being used as well as the code for the intro animation and character
// selection.

#include "game_objects.h"
#include "joystick.hh"
#include "person.h"
#include "mtime.h"
#include "matthewG.h"
#include "fonts/fonts.h"
#include <cmath>
#include <GL/glx.h>
#include <iostream>
#include <string>
#include "roseP.h"

using std::cout;
using std::endl;

typedef struct crosshair {
	float angle;
} Crosshair;

Crosshair crosshair;

int xwidth, yheight;

void getJoystickEvent(JoystickEvent event, int joy[], int axis[]) 
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
		if (fabs(value) / (float) event.MAX_AXES_VALUE < .2)
			axis[number] = 0;
		else
			axis[number] = value;
	}
}

void movement(Game *g) 
{
	Flt rad = ((g->Player_1.stats.angle+90.0f) / 360.0f) * M_PI * 2.0f;

	Flt xdir = cos(rad);
	Flt ydir = sin(rad);	

	g->Player_1.stats.vel[0] = xdir*2.0f;
	g->Player_1.stats.vel[1] = ydir*2.0f;
}

//new checkController now with no deadzones and easy to read angles
void checkController(int axis[], Game *g) 
{ 
	if (axis[1] || axis[0]) {
		float angle = atan2(axis[1], -axis[0]) / M_PI*180 + 180;
		//cout << angle << endl;
		if (angle >= 337.5 || angle <= 22.5 ) 
			g->Player_1.stats.angle = 270;	

		else if (angle > 22.5 && angle <= 67.5) 
			g->Player_1.stats.angle = -45;

		else if (angle > 67.5 && angle <= 112.5) 
			g->Player_1.stats.angle = 0;

		else if (angle > 112.5 && angle <= 157.5) 
			g->Player_1.stats.angle = 45;

		else if (angle > 157.5 && angle <= 202.5) 
			g->Player_1.stats.angle = 90;

		else if (angle > 202.5 && angle <= 247.5) 
			g->Player_1.stats.angle = 135;

		else if (angle > 247.5 && angle <= 292.5) 
			g->Player_1.stats.angle = 180;

		else if (angle > 292.5 && angle <=337.5) 
			g->Player_1.stats.angle = 225;

		movement(g);
	}
}

void renderCrosshair(int axis[], Game *g, bool keyboard) 
{
	int radius = 40;
	int playerX = g->Player_1.stats.spos[0]+1;
	int playerY = g->Player_1.stats.spos[1]+2;

	//Draw Crosshair for aiming
	if (keyboard == false) 
		crosshair.angle = atan2(axis[4], -axis[3]) / M_PI*180 + 180;
	else {
		if (axis[XK_q]) {
			crosshair.angle += .1f;
		}
		if (axis[XK_e]) {
			crosshair.angle -= .1f;
		}
	}	

	float xComponent = radius * cos(crosshair.angle*M_PI/180);
	float yComponent = radius * sin(crosshair.angle*M_PI/180);

	glColor3f(.3f, 1.0f, .3f);

	//create a 30px tall & wide crosshair centered on edge of circle
	glBegin(GL_LINES);	
	glVertex2i(xComponent + playerX, playerY + yComponent - 15);
	glVertex2i(xComponent + playerX, playerY + yComponent + 15);
	glVertex2i(xComponent + playerX - 15, yComponent + playerY);
	glVertex2i(xComponent + playerX + 15, yComponent + playerY);
	glEnd();
}

void renderShield(Game *g) 
{
	int radius = 40;
	int playerX = g->Player_1.stats.spos[0]+1;
	int playerY = g->Player_1.stats.spos[1]+2;

	glColor3f(0.0f, 0.0f, 1.0f);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POINTS);
	for (float a = 0.0; a <= 2*M_PI; a+=0.001) 
		glVertex2f(radius * cos(a) + playerX, radius * sin(a) + playerY);

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
}

//montage name-?.png name-??.png -tile 16x16 -geometry 64x64+0+0 -background 
//transparent name.png

Ppmimage* characterSelection(std::string characterColor) 
{
	if (characterColor.compare("red") == 0) {
		return ppm6GetImage((char*)"images/red.ppm");
		std::cout << "Red selected" << std::endl;
	}
	else {
		return ppm6GetImage((char*)"images/blue.ppm");
		std::cout << "Blue selected" << std::endl;
	}
}

struct timespec animationCurrent, animationStart;
double animationSpan = 0.0;
void renderCharacter(Person person, Game *g, float w, int keys[], 
	GLuint personTexture1)
{
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glTranslatef(person.pos[0], person.pos[1], person.pos[2]);	
	glRotatef(g->Player_1.stats.angle, 0, 0, 1.0f);
	glBindTexture(GL_TEXTURE_2D, personTexture1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glBegin(GL_QUADS);	
	if (animationSpan >= 90) {
		animationSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &animationStart);
	}

	if ((keys[XK_w] || keys[XK_s]) && animationSpan < 22) {
		glTexCoord2f(0.5f, 0.0f); glVertex2f(-w, w);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(w, w);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(w, -w);
		glTexCoord2f(0.5f, 1.0f); glVertex2f(-w, -w);
	}
	else
	{
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, w);
		glTexCoord2f(0.5f, 0.0f); glVertex2f( w, w);
		glTexCoord2f(0.5f, 1.0f); glVertex2f( w, -w);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,-w);
	}
	
	clock_gettime(CLOCK_REALTIME, &animationCurrent);
	animationSpan += timeDiff(&animationStart, &animationCurrent);
	
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void resolution(int width, int height)
{
	xwidth = width;
	yheight = height;
}
int mousex;
int mousey;
void mousePosition(int x, int y)
{
	mousex = x;
	mousey = y;
}

int scroll = yheight+yheight;
struct timespec bouldersCurrent, bouldersStart;
struct timespec totCurrent, totStart;
struct timespec logoCurrent, logoStart;
struct timespec enterCurrent, enterStart;
struct timespec optionsCurrent, optionsStart;
struct timespec characterCurrent, characterStart;
struct timespec arrowCurrent, arrowStart;
struct timespec arrowCurrent2, arrowStart2;
struct timespec keyPressCurrent, keyPressStart;
double bouldersSpan = 0.0;
double logoSpan = 0.0;
double enterSpan = 0.0;
double optionsSpan = 0.0;
double characterSpan = 0.0;
double arrowSpan = 0.0;
double arrowSpan2 = 0.0;
double keyPressSpan = 0.0;
int fallingBouldersTimer = 1;
double scale[3] = {1.0f, 1.0f, 0};
double scalePos[3] = {0, 0, 0};
int pos[3] = {0, yheight, 0};
int posLogo[3] = {0, -50, 0};
int posEnter[3] = {xwidth/2, 0, 0};
int posOptions[3] = {xwidth/2+60, 0, 0};
int arrow[3] = {0, 0, 0};
int arrow2[3] = {0, 0, 0};
int character[3] = {0, 0, 0};
int sign[3] = {0, 0, 0};
int optionsFlag = 0;
int fpsActivator = 0;

int renderTitleScreen(GLuint introTextures[], Ppmimage *introImages[], 
	int enterPressed, int downPressed, int upPressed, int keys[])
{
	glClear(GL_COLOR_BUFFER_BIT);

	//set background to black to give cinematic feel
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	//Background
	float w = introImages[0]->width;
	float h = introImages[0]->height;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, introTextures[0]);
	glTranslatef(-scale[0]*625 +625, -scale[1]*354+354, 0);
	glScalef(scale[0], scale[1], scale[2]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, h + 96);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h + 96);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, 96);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 96);

	glEnd();
	glPopMatrix();
	//MazeWars Logo
	if (scroll <= -1800) {
		w = introImages[2]->width;
		h = introImages[2]->height;

		glPushMatrix();
		glTranslatef(posLogo[0], posLogo[1], posLogo[2]);

		glBindTexture(GL_TEXTURE_2D, introTextures[2]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(235, 900);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(w + 235, 900);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(w + 235, h + 294);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(235, h + 294);

		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);

		//Enter Maze
		w = 315 + introImages[3]->width/2;
		h = introImages[3]->height/2;
		glPushMatrix();
		glTranslatef(posEnter[0], posEnter[1], posEnter[2]);

		glBindTexture(GL_TEXTURE_2D, introTextures[3]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(315 + xwidth/2, yheight/2);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(xwidth/2 + w, yheight/2);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(xwidth/2 + w, yheight/2 - h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(315 + xwidth/2, yheight/2 - h);

		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();

		//Options
		w = 275 + introImages[4]->width/2;
		h = -60 - introImages[4]->height/2;
		glPushMatrix();
		glTranslatef(posOptions[0], posOptions[1], posOptions[2]);

		glBindTexture(GL_TEXTURE_2D, introTextures[4]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(xwidth/2 + 275, yheight/2 - 60);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(xwidth/2 + w, yheight/2 - 60);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(xwidth/2 + w, yheight/2 + h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(xwidth/2 + 275, yheight/2 + h);

		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();

		//Arrow
		w = 250 + introImages[5]->width/1.5;
		h = -5 + introImages[5]->height/1.5;
		
		glPushMatrix();
		glTranslatef(arrow[0], arrow[1], arrow[2]);
		glBindTexture(GL_TEXTURE_2D, introTextures[5]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(xwidth/2 + 250, yheight/2 + 5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(xwidth/2 + w, yheight/2 + 5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(xwidth/2 + w, yheight/2 - h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(xwidth/2 + 250, yheight/2 - h);
		
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();

		//Arrow movement
		if (downPressed && !optionsFlag) {
			if (arrow[0] == 0) {
				arrow[0] = 20;
				arrow[1] = -60;
			}
			//cout << "down" << endl;
		}
		else if (upPressed && !optionsFlag) {
			if (arrow[0] == 20) {
				arrow[0] = 0;
				arrow[1] = 0;
			}
			//cout << "up" << endl;
		}

		//Sign
		w = introImages[6]->width/1.4;
		h = introImages[6]->height/1.4;

		glPushMatrix();
		glTranslatef(sign[0], sign[1], sign[2]);
		glBindTexture(GL_TEXTURE_2D, introTextures[6]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(160, h*2);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(160+w, h*2);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(160+w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(160, h);
		
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}

	//Falling Boulders
	if (fallingBouldersTimer == 1) {
		clock_gettime(CLOCK_REALTIME, &totStart);
		fallingBouldersTimer = 0;
	}
	if (bouldersSpan > 6) {
		bouldersSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &bouldersStart);
	}
	if (logoSpan > .8) {
		logoSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &logoStart);
	}
	if (enterSpan > 1) {
		enterSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &enterStart);
	}
	if (optionsSpan > 1) {
		optionsSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &optionsStart);
	}
	if (arrowSpan > 1) {
		arrowSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &arrowStart);
	}

	w = 50 + introImages[1]->width;
	h = introImages[1]->height;
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);

	glBindTexture(GL_TEXTURE_2D, introTextures[1]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(-50, yheight + h);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, yheight + h);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, yheight);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-50, yheight);

	glEnd();
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	clock_gettime(CLOCK_REALTIME, &bouldersCurrent);
	bouldersSpan += timeDiff(&bouldersStart, &bouldersCurrent);

	if (scroll  > -yheight*3 && bouldersSpan > 6) {
		scroll -=100;
		pos[1] = scroll;
	}

	if (scroll == -1700) {
		scroll = -1800;
		clock_gettime(CLOCK_REALTIME, &totCurrent);
	}


	clock_gettime(CLOCK_REALTIME, &logoCurrent);
	clock_gettime(CLOCK_REALTIME, &enterCurrent);
	clock_gettime(CLOCK_REALTIME, &optionsCurrent);
	clock_gettime(CLOCK_REALTIME, &arrowCurrent);
	if (enterPressed) {
		//cout << "enter pressed" << endl;
		logoSpan += timeDiff(&logoStart, &logoCurrent);
		enterSpan += timeDiff(&enterStart, &enterCurrent);
		optionsSpan += timeDiff(&optionsStart, &optionsCurrent);
		arrowSpan += timeDiff(&arrowStart, &arrowCurrent);

		if (posLogo[1] < 6000 && logoSpan > .8) {
				posLogo[1] += 25;
		}

		if (fabs(posEnter[0]) < xwidth && enterSpan > 1) {
			posEnter[0] += 25;
		}

		if (posOptions[0] < xwidth && optionsSpan > 1) {
			posOptions[0] += 25;
		}

		if (arrow[0] < xwidth && arrowSpan > 1) {
			arrow[0] += 25;
		}

		if (!arrow[1]) {
			if (posLogo[1] > 800) {
				scale[0] += .00375; scalePos[0] -= .00375;
				scale[1] += .00375; scalePos[1] -= .00375;
				//cout << scale[1] << endl;
			}
			
			if (posLogo[1] == 1800) {
				return 0;
			}
		}

		else if (arrow[1]) {
			optionsFlag = 1;
			//enterPressed = 0;
			if (sign[1] > -yheight-250) {
				sign[1]--;
				//cout << sign[1] << endl;
			}
		}

	}

	if (optionsFlag) {
		if (sign[1] < -1130) {
			Rect r;
			r.bot = yheight - 160;
			r.left = 310;
			r.center = 0;
			ggprint16(&r, 0, 0xffffff, "Back");

			//arrow 2
			w = introImages[7]->width/3;
			h = introImages[7]->height/3;

			glPushMatrix();
			glTranslatef(arrow2[0],arrow2[1],0);
			glBindTexture(GL_TEXTURE_2D, introTextures[7]);

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f); glVertex2f(275, yheight - 132);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(275 + w, yheight - 132);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(275 + w, yheight - h - 132);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(275, yheight - h - 132);
			
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glPopMatrix();

			//if ()
		}
	}

	graveKeyPress(keys);

	return 1;
}

struct timespec fpsCurrent, fpsPrevious;
int fpsSpan;
int frame = 0;
double currentfps;
void fps() {
	if (frame == 0) {
		clock_gettime(CLOCK_REALTIME, &fpsPrevious);
	}
	frame++;
	clock_gettime(CLOCK_REALTIME, &fpsCurrent);

	fpsSpan = timeDiff(&fpsPrevious, &fpsCurrent) * 1000; //time in miliseconds

	if (fpsSpan > 1000) {
		//cout << "FPS: " << frame / (fpsSpan / 1000.0f) << endl;
		currentfps = frame / (fpsSpan / 1000.0f);
		//cout << "FPS: " << currentfps << endl;
		timeCopy(&fpsPrevious, &fpsCurrent);
		frame = 0;
	}

	Rect r;
	r.bot = yheight - 40;
	r.left = 20;
	r.center = 0;
	ggprint16(&r, 0, 0xffffff, "FPS: %.2lf", currentfps);
}

int tmp = 0;

void graveKeyPress(int keys[]) {
	
	if (keys[XK_grave]) {
		fpsActivator ^= keys[XK_grave];
	}

	if (fpsActivator) {
		fps();
	}
}