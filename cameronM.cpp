#ifndef CAMERON_CPP
#define CAMERON_CPP
/* Name: Cameron Morrow
* Date: May 31, 2016
* Last Modified: May 31, 2016
* Description: My code handles all of the weapon movement if a controller 
* is not being used. 
* In the mazewars.cpp I modified all of the original asteroids code to remove
* the engine from the ship and the asteroid field, changed the window title
* I am drawing and handling the health bar and "HUD" 
* I Draw and move the enemies array and deal with the pause menu and the 
* winning condition ending credits scene
* my one compile warning is an unused parameter because i havent displayed the
* final score yet
*/
#define PI 3.14159268
#include "cameronM.h"
#include "roseP.h"
#include <iostream>

int *res;
struct timespec timeC1;
struct timespec timeCurrentC;
static double timespanC1 = 0.0;
struct timespec timeC2;
struct timespec timeCurrentC2;
struct timespec timem[5];
struct timespec timeCurrentm[5];
static double timespanm[5];
struct timespec timeCurrentqr;
struct timespec timeTq;
static double timeSpanTq = 0.0;

struct timespec timeTr;
static double timeSpanTr = 0.0;

using namespace std;

void getScreenRes(int x, int y)
{
	res = new int[2];
	res[0] = x;
	res[1] = y;
}
void pointPlayer(Game *g, int savex, int savey)
{
	//Make the player's gun point at the cursor
	exchangeGpos(&g->gun, &g->Player_1);
	float weaponx = g->Player_1.stats.spos[0];
	float weapony = g->Player_1.stats.spos[1];
	float nDeg = atan(((res[1]-savey)-(weapony))/\
		((savex)-(weaponx))) * 180 / 3.1415926;
	if (savex > weaponx && (res[1] - savey) > weapony)
		nDeg += 180;
	if (savex > weaponx && (res[1] - savey) < weapony)
		nDeg -= 180;
	if (g->gun.stats.angle > 360.f) {
		g->gun.stats.angle = 360.0f;
	}
	if (g->gun.stats.angle <= 360.0f) {
		if (nDeg > 270) {
			nDeg -= 360;
		}
		g->gun.stats.angle = nDeg + 90;
	}
	if (g->gun.stats.angle < 0.0f) {
		g->gun.stats.angle += 360.0f;
	}
}
void drawHUD(Player *x)
{
	drawBackground();
	drawHealth(x);
	drawAmmo(*x);
	drawArtifacts(*x);
	switch (x->lives) {
		case 4:
			drawLives(10, 0);
		case 3:
			drawLives(50, 0);
		case 2:
			drawLives(10, -30);
		case 1:
			drawLives(50, -30);
	}
}
void drawBackground()
{
	glPushMatrix();
	glTranslatef(res[0], 0, 0);
/*
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(0, 0);
		glColor3ub(84, 84, 84);
		glVertex2i(-350, 0);
		glColor3ub(48, 48, 48);
		glVertex2i(-350, 120);
		glColor3ub(54, 54, 54);
		glVertex2i(-320, 150);
		glColor3ub(84, 84, 84);
		glVertex2i(0, 150);
	glEnd();

	glColor3ub(54, 54, 54);
	glBegin(GL_POLYGON);
		glVertex2i(-340, 10);
		glVertex2i(-350, 0);
		glVertex2i(-350, 120);
		glVertex2i(-340, 115);
	glEnd();

	glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(-340, 115);
		glVertex2i(-350, 120);
		glVertex2i(-320, 150);
		glVertex2i(-315, 140);
	glEnd();

	glColor3ub(152, 152, 152);
	glBegin(GL_POLYGON);
		glVertex2i(-315, 140);
		glVertex2i(-320, 150);
		glVertex2i(0, 150);
		glVertex2i(-15, 140);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(-15, 140);
		glVertex2i(0, 150);
		glVertex2i(0, 0);
		glVertex2i(-15,10);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i(-350, 0);
		glVertex2i(-340, 10);
		glVertex2i(-15, 10);
	glEnd();*/
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(0, 0);
		glColor3ub(84, 84, 84);
		glVertex2i(-150, 0);
		glColor3ub(48, 48, 48);
		glVertex2i(-150, 920);
		glColor3ub(54, 54, 54);
		glVertex2i(-120, 950);
		glColor3ub(84, 84, 84);
		glVertex2i(0, 950);
	glEnd();

	/*glColor3ub(54, 54, 54);
	glBegin(GL_POLYGON);
		glVertex2i(-140, 910);
		glVertex2i(-150, 0);
		glVertex2i(-150, 920);
		glVertex2i(-140, 115);
	glEnd();*/
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(-1100, 0);
		glColor3ub(84, 84, 84);
		glVertex2i(-1250, 0);
		glColor3ub(48, 48, 48);
		glVertex2i(-1250, 920);
		glColor3ub(54, 54, 54);
		glVertex2i(-1220, 950);
		glColor3ub(84, 84, 84);
		glVertex2i(-1100, 950);
	glEnd();

	glColor3ub(54, 54, 54);
	/*glBegin(GL_POLYGON);
		glVertex2i(-1240, 910);
		glVertex2i(-1250, 0);
		glVertex2i(-1250, 920);
		glVertex2i(-1240, 115);
	glEnd();*/
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(0, 0);
		glColor3ub(84, 84, 84);
		glVertex2i(-1250, 0);
		glColor3ub(48, 48, 48);
		glVertex2i(-1250, 120);
		glColor3ub(54, 54, 54);
		glVertex2i(-1220, 150);
		glColor3ub(84, 84, 84);
		glVertex2i(0, 150);
	glEnd();

	glColor3ub(54, 54, 54);
	glBegin(GL_POLYGON);
		glVertex2i(-1240, 10);
		glVertex2i(-1250, 0);
		glVertex2i(-1250, 150);
		glVertex2i(-1240, 145);
	glEnd();

	/*glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(-1240, 115);
		glVertex2i(-1250, 120);
		glVertex2i(-1220, 150);
		glVertex2i(-1215, 140);
	glEnd();*/

	glColor3ub(152, 152, 152);
	glBegin(GL_POLYGON);
		glVertex2i(-1240, 140);
		glVertex2i(-1250, 150);
		glVertex2i(0, 150);
		glVertex2i(-15, 140);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(-15, 140);
		glVertex2i(0, 150);
		glVertex2i(0, 0);
		glVertex2i(-15,10);
	glEnd();

	/*glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i(-1250, 0);
		glVertex2i(-1240, 10);
		glVertex2i(-15, 10);
	glEnd();*/

	// now the top bar
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(0, 800);
		glColor3ub(84, 84, 84);
		glVertex2i(-1250, 800);
		glColor3ub(48, 48, 48);
		glVertex2i(-1250, 920);
		glColor3ub(54, 54, 54);
		glVertex2i(-1220, 950);
		glColor3ub(84, 84, 84);
		glVertex2i(0, 950);
	glEnd();

	glColor3ub(54, 54, 54);
	glBegin(GL_POLYGON);
		glVertex2i(-1240, 810);
		glVertex2i(-1250, 800);
		glVertex2i(-1250, 920);
		glVertex2i(-1240, 915);
	glEnd();

	glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(-1240, 915);
		glVertex2i(-1250, 920);
		glVertex2i(-1220, 950);
		glVertex2i(-1215, 940);
	glEnd();

	glColor3ub(152, 152, 152);
	glBegin(GL_POLYGON);
		glVertex2i(-1215, 940);
		glVertex2i(-1220, 950);
		glVertex2i(0, 950);
		glVertex2i(-15, 940);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(-15, 940);
		glVertex2i(0, 950);
		glVertex2i(0, 800);
		glVertex2i(-15,810);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(0, 800);
		glVertex2i(-1250, 800);
		glVertex2i(-1240, 810);
		glVertex2i(-15, 810);
	glEnd();


/*
	glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(-140, 115);
		glVertex2i(-150, 120);
		glVertex2i(-120, 150);
		glVertex2i(-115, 140);
	glEnd();

	glColor3ub(152, 152, 152);
	glBegin(GL_POLYGON);
		glVertex2i(-115, 140);
		glVertex2i(-120, 150);
		glVertex2i(0, 150);
		glVertex2i(-15, 140);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(-15, 140);
		glVertex2i(0, 150);
		glVertex2i(0, 0);
		glVertex2i(-15,10);
	glEnd();

	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i(-150, 0);
		glVertex2i(-140, 10);
		glVertex2i(-15, 10);
	glEnd();
*/


	glPopMatrix();	
}
void drawDiamondBack(int x)
{
	glPushMatrix();
	glTranslatef(res[0], 0, 0);

	glColor3ub(17, 17, 17);
	glBegin(GL_POLYGON);
		glVertex2i(-315+x, 40);
		glVertex2i(-330+x, 40);
		glVertex2i(-310+x, 60);
		glVertex2i(-310+x, 45);
	glEnd();

	glColor3ub(36, 36, 36);
	glBegin(GL_POLYGON);
		glVertex2i(-310+x, 45);
		glVertex2i(-310+x, 60);
		glVertex2i(-290+x, 40);
		glVertex2i(-305+x, 40);
	glEnd();

	glColor3ub(142, 142, 142);
	glBegin(GL_POLYGON);
		glVertex2i(-305+x, 40);
		glVertex2i(-290+x, 40);
		glVertex2i(-310+x, 20);
		glVertex2i(-310+x, 35);
	glEnd();

	glColor3ub(62, 62, 62);
	glBegin(GL_POLYGON);
		glVertex2i(-310+x, 35);
		glVertex2i(-310+x, 20);
		glVertex2i(-330+x, 40);
		glVertex2i(-315+x, 40);
	glEnd();

	glPopMatrix();
}
void drawDiamond(int x)
{
    	glPushMatrix();
	glTranslatef(res[0], 0, 0);

	glColor3ub(180, 226, 175);
	glBegin(GL_POLYGON);
		glVertex2i(-320+x, 40);
		glVertex2i(-330+x, 40);
		glVertex2i(-310+x, 60);
		glVertex2i(-310+x, 50);
	glEnd();

	glColor3ub(40, 126, 35);
	glBegin(GL_POLYGON);
		glVertex2i(-310+x, 50);
		glVertex2i(-310+x, 60);
		glVertex2i(-290+x, 40);
		glVertex2i(-300+x, 40);
	glEnd();

	glColor3ub(30, 90, 40);
	glBegin(GL_POLYGON);
		glVertex2i(-300+x, 40);
		glVertex2i(-290+x, 40);
		glVertex2i(-310+x, 20);
		glVertex2i(-310+x, 30);
	glEnd();

	glColor3ub(67, 153, 80);
	glBegin(GL_POLYGON);
		glVertex2i(-310+x, 30);
		glVertex2i(-310+x, 20);
		glVertex2i(-330+x, 40);
		glVertex2i(-320+x, 40);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3ub(140, 236, 135);
		glVertex2i(-320+x, 40);
		glColor3ub(50, 175, 40);
		glVertex2i(-310+x, 50);
		glColor3ub(140, 236, 135);
		glVertex2i(-300+x, 40);
		glColor3ub(50, 175, 40);
		glVertex2i(-310+x, 30);
	glEnd();
	
	glPopMatrix();
}
void drawArtifacts(Player x)
{
	if (x.artifact[0]) {
		drawDiamond(80);
	} else {
		drawDiamondBack(80);
	}
	if (x.artifact[1]) {
		drawDiamond(160);
	} else {
		drawDiamondBack(160);
	} 
	if (x.artifact[2]) {
		drawDiamond(240);
	} else {
		drawDiamondBack(240);
	}
	Rect r;
	r.bot = 35;
	r.left = res[0] - 320;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffffff, "Artifacts:");
}
void drawHealth(Player *x)
{
	if (x->Current_Health < 0)
		x->Current_Health = 0;

	double remaining = 100-((x->Current_Health/x->Max_Health)*100);

	//Background of health bar
	glPushMatrix();
	glTranslatef(res[0]-150, 85, 0);
	glColor3ub(100, 100, 100);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();

	int test = 10-x->Current_Health;
	
	if (x->lives > 0) {
		
		glColor3ub(164, 20, 20);
		glPushMatrix();
		glTranslatef((res[0]-150)-remaining, 85, 0);
		
		glBegin(GL_POLYGON);
			if (100-remaining > 10) {
				glVertex2i(-(90 - remaining), -15);
				glVertex2i(-(100 - remaining), -5);
				glVertex2i(-(100 - remaining), 5);
				glVertex2i(-(90 - remaining), 15);
				glVertex2i((90 - remaining), 15);
				glVertex2i((100 - remaining), 5);
				glVertex2i((100 - remaining), -5);
				glVertex2i((90 - remaining), -15);
			} else {
				glVertex2i(-(90 - remaining + test), -15 + (test));
				glVertex2i(-(100 - remaining), -5);
				glVertex2i(-(100 - remaining), 5);
				glVertex2i(-(90 - remaining + test), 15 - (test));
				glVertex2i((90 - remaining + test), 15 - (test));
				glVertex2i((100 - remaining), 5);
				glVertex2i((100 - remaining), -5);
				glVertex2i((90 - remaining + test), -15 + (test));
			}
		glEnd();
		
		glPopMatrix();
	}
	Rect r;
	r.bot = 80;
	r.left = res[0] - 240;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffffff, "Health:");
}
void drawAmmo(Player x)
{
	double remaining = 100-((x.Current_Ammo/x.Max_Ammo)*100);

	//Background of ammo bar
	glColor3ub(100, 100, 100);
	glPushMatrix();
	glTranslatef(res[0]-150, 120, 0);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();

	int test = 10-x.Current_Ammo;
	if (100-remaining > 0) {
		glColor3ub(30, 20, 150);
		glPushMatrix();
		glTranslatef((res[0]-150)-remaining, 120, 0);

		glBegin(GL_POLYGON);
			if (100-remaining > 10) {
				glVertex2i(-(90 - remaining), -15);
				glVertex2i(-(100 - remaining), -5);
				glVertex2i(-(100 - remaining), 5);
				glVertex2i(-(90 - remaining), 15);
				glVertex2i((90 - remaining), 15);
				glVertex2i((100 - remaining), 5);
				glVertex2i((100 - remaining), -5);
				glVertex2i((90 - remaining), -15);
			} else {
				glVertex2i(-(90 - remaining + test), -15 + (test));
				glVertex2i(-(100 - remaining), -5);
				glVertex2i(-(100 - remaining), 5);
				glVertex2i(-(90 - remaining + test), 15 - (test));
				glVertex2i((90 - remaining + test), 15 - (test));
				glVertex2i((100 - remaining), 5);
				glVertex2i((100 - remaining), -5);
				glVertex2i((90 - remaining + test), -15 + (test));
			}
		glEnd();
		
		glPopMatrix();
	}
	Rect r;
	r.bot = 115;
	r.left = res[0] - 240;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffffff, "Ammo:");
}

Ppmimage *livesImage = {NULL};
GLuint livesTexture;
void loadLivesSprite()
{
	livesImage = ppm6GetImage((char*)"images/Head.ppm");
	glGenTextures(1, &livesTexture); 
	
	float w, h;
	//lives texture
	w = livesImage->width;
	h = livesImage->height;
	glBindTexture(GL_TEXTURE_2D, livesTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *livesData = buildAlphaData(livesImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, livesData);
	free(livesData);
	
	
}
void drawLives(int x, int y)
{
	glColor3ub(255, 255, 255);

	float w = livesImage->width;
	float h = livesImage->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, livesTexture);
	glTranslatef(res[0]-10, 60, 0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(-330+x, 60+y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-300+x, 60+y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-300+x, 30+y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-330+x, 30+y);        

	glEnd();
	glPopMatrix();
}
void GameOver()
{
	Rect r;
	r.bot = res[0]/2;
	r.left = res[1]/2;
	r.center = 0;
	ggprint8b(&r, 160, 0x00ff0000, "GAME OVER \n F6 TO RESTART");
}
void Restart(Game *x)
{
	x->Player_1.Current_Health = 100;
	x->Player_1.Current_Ammo = 100;
	x->Player_1.lives = 4;
	x->Player_1.stats.spos[0] = res[0]/2;
	x->Player_1.stats.spos[1] = res[1]/2;
	x->Player_1.stats.gpos[0] = res[0]/2;
	x->Player_1.stats.gpos[1] = res[1]/2;
	x->Player_1.gameOver = false;
	VecZero(x->Player_1.stats.dir);

	for (int i = 0; i<5; i++) {
		x->mon[i].stats.spos[0] = x->mon[i].spawnPos[0];
		x->mon[i].stats.spos[1] = x->mon[i].spawnPos[1];
	}
}
void drawHealthPack(int x, int y, int z, Game *g)
{
	x += cos(\
	PI + (((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f));
	y += sin(\
	PI + (((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f));

	glColor3ub(0, 255, 0);
	glPushMatrix();
	glTranslatef(x, y, z);

	glBegin(GL_QUADS);
		glVertex2i(-20, -8);
		glVertex2i(-20, 8);
		glVertex2i(20, 8);
		glVertex2i(20, -8);
	glEnd();

	glPopMatrix();

	Rect r;
	r.bot = y-5;
	r.left = x-17;
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "Health");
}
void shadowBox()
{
	int x = (res[0]-1250)/2;
	//topLeft
	glBegin(GL_POLYGON);		
		glColor3f(0, 0, 0);
		glVertex2f(0, res[1]);
		glVertex2f((res[0]/2), res[1]);
		glVertex2f((res[0]/2), res[1]-75);
		glVertex2f((res[0]/2)-150-x, res[1]-75);
		glVertex2f((res[0]/2)-400-x, (res[1]/2)+100);
		glVertex2f((res[0]/2)-400-x, (res[1]/2));
		glVertex2f(0, (res[1]/2));
	glEnd();
	//bottomLeft
	glBegin(GL_POLYGON);	
		glColor3f(0, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(0, (res[1]/2));
		glVertex2f((res[0]/2)-400-x, (res[1]/2));
		glVertex2f((res[0]/2)-400-x, (res[1]/2)-100);
		glVertex2f((res[0]/2)-150-x, 75);
		glVertex2f(res[0]/2, 75);
		glVertex2f(res[0]/2, 0);
	glEnd();		
	//topRight	
	glBegin(GL_POLYGON);		
		glColor3f(0, 0, 0);		
		glVertex2f(res[0], res[1]);		
		glVertex2f((res[0]/2), res[1]);		
		glVertex2f((res[0]/2), res[1]-75);		
		glVertex2f((res[0]/2)+150+x, res[1]-75);		
		glVertex2f((res[0]/2)+400+x, (res[1]/2)+100);		
		glVertex2f((res[0]/2)+400+x, (res[1]/2));	
		glVertex2f(res[0], (res[1]/2));
	glEnd();
	//bottomRight	
	glBegin(GL_POLYGON);	
		glColor4f(0, 0, 0, .5);	
		glVertex2f(res[0], 0);	
		glVertex2f(res[0], (res[1]/2));
		glVertex2f((res[0]/2)+400+x, (res[1]/2));	
		glVertex2f((res[0]/2)+400+x, (res[1]/2)-100);	
		glVertex2f((res[0]/2)+150+x, 75);	
		glVertex2f(res[0]/2, 75);	
		glVertex2f(res[0]/2, 0);
	glEnd();glPushMatrix();
}
void monster(Game *g, int monNum, int startx, int starty)
{
	monsterMovement(g, monNum, startx, starty);
	monsterGetShot(g, monNum, startx, starty);
	monsterDamagePlayer(g, monNum, startx, starty);
}
void monsterMovement(Game *g, int monNum, int startx, int starty)
{
	//this calculates the enemys position in gamespace
	g->mon[monNum].gvel[0] = cos(\
	PI + (((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f));
	g->mon[monNum].gvel[1] = sin(\
	PI + (((g->Player_1.stats.angle+90.0f) / 360.0f) * PI * 2.0f));

	if (((g->Player_1.stats.spos[0] <= g->mon[monNum].stats.spos[0]+250) \
	&& (g->Player_1.stats.spos[0] > g->mon[monNum].stats.spos[0]-250)) \
	&& ((g->Player_1.stats.spos[1] <= g->mon[monNum].stats.spos[1]+250) \
	&& (g->Player_1.stats.spos[1] > g->mon[monNum].stats.spos[1]-250))) {
		g->mon[monNum].pursuit = true;
	} else {
		g->mon[monNum].pursuit = false;
	}
	startx++;
	starty++;
	//this is the enemys default movement pattern if not in pursuit mode
	if (!g->mon[monNum].pursuit || g->Player_1.gameOver) {
		clock_gettime(CLOCK_REALTIME, &timeCurrentC);
		timespanC1 = timeDiff(&timeC1, &timeCurrentC);
		if (timespanC1 > 2) {
			g->mon[monNum].stats.vel[0] = 1;
			g->mon[monNum].stats.vel[1] = 0;
			g->mon[monNum].stats.angle = 270;
			if (timespanC1 > 4)
				clock_gettime(CLOCK_REALTIME, &timeC1);
		} else {
			g->mon[monNum].stats.vel[0] = -1;
			g->mon[monNum].stats.vel[1] = 0;
			g->mon[monNum].stats.angle = 90;
		}
	} else {
		//this is the enemys pursuit movement pattern
		g->mon[monNum].stats.angle = 0;
		
		if (g->mon[monNum].stats.spos[0] < g->Player_1.stats.spos[0])
			g->mon[monNum].stats.vel[0] = 0.5;
		else
			g->mon[monNum].stats.vel[0] = -0.5;

		if (g->mon[monNum].stats.spos[1] < g->Player_1.stats.spos[1])
			g->mon[monNum].stats.vel[1] = 0.5;
		else
			g->mon[monNum].stats.vel[1] = -0.5;
	}
}
void monsterGetShot(Game *g, int monNum, int startx, int starty)
{
	//this checks to see if the enemy has been shot and adjusts health 
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (((g->barr[i].stats.spos[0] >= g->mon[monNum].stats.spos[0]-12) &&\
		(g->barr[i].stats.spos[0] <= g->mon[monNum].stats.spos[0]+12)) &&\
		((g->barr[i].stats.spos[1] >= g->mon[monNum].stats.spos[1]-12) &&\
		(g->barr[i].stats.spos[1] <= g->mon[monNum].stats.spos[1]+12))) {
			g->mon[monNum].health -= 20;
			g->Player_1.score+=50;
		} 
	}
	if (g->mon[monNum].health <= 0) {
		g->mon[monNum].health = 0;
		g->mon[monNum].alive = false;
		g->Player_1.kills++;
		g->Player_1.score+=500;
	}
	startx++;
	starty++;
}
void monsterDamagePlayer(Game *g, int monNum, int startx, int starty)
{
	clock_gettime(CLOCK_REALTIME, &timeCurrentm[monNum]);
	timespanm[monNum] = timeDiff(&timem[monNum], &timeCurrentm[monNum]);
	if (((g->Player_1.stats.spos[0] >= g->mon[monNum].stats.spos[0]-12) &&\
	(g->Player_1.stats.spos[0] <= g->mon[monNum].stats.spos[0]+12)) &&\
	((g->Player_1.stats.spos[1] >= g->mon[monNum].stats.spos[1]-12) &&\
	(g->Player_1.stats.spos[1] <= g->mon[monNum].stats.spos[1]+12))) {	
		if (timespanm[monNum] > 0.5) {
			clock_gettime(CLOCK_REALTIME, &timem[monNum]);
			g->Player_1.Current_Health -= 5;
		}
	}
	startx++;
	starty++;
}

struct timespec animationCurrentc, animationStartc;
double animationSpanc = 0.0;

void renderCharacterEnemy(Person personc, Game *g, float w, int keys[], 
GLuint personTexture1c, int i)
{
	keys[XK_g]++;
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glTranslatef(personc.pos[0], personc.pos[1], personc.pos[2]);	
	glRotatef(g->mon[i].stats.angle, 0, 0, 1.0f);
	glBindTexture(GL_TEXTURE_2D, personTexture1c);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glBegin(GL_QUADS);	
		if (animationSpanc >= 90) {
			animationSpanc = 0.0;
			clock_gettime(CLOCK_REALTIME, &animationStartc);
		}

		if (animationSpanc < 12.5) {
			glTexCoord2f(0.66f, 0.0f); glVertex2f(-w, w);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(w, w);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(w, -w);
			glTexCoord2f(0.66f, 1.0f); glVertex2f(-w, -w);
		} else if (animationSpanc < 45) {
			glTexCoord2f(0.33f, 0.0f); glVertex2f(-w, w);
			glTexCoord2f(0.66f, 0.0f); glVertex2f( w, w);
			glTexCoord2f(0.66f, 1.0f); glVertex2f( w, -w);
			glTexCoord2f(0.33f, 1.0f); glVertex2f(-w,-w);
		} else if (animationSpanc < 67.5) {
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, w);
			glTexCoord2f(0.33f, 0.0f); glVertex2f( w, w);
			glTexCoord2f(0.33f, 1.0f); glVertex2f( w, -w);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,-w);
		} else {
			glTexCoord2f(0.33f, 0.0f); glVertex2f(-w, w);
			glTexCoord2f(0.66f, 0.0f); glVertex2f( w, w);
			glTexCoord2f(0.66f, 1.0f); glVertex2f( w, -w);
			glTexCoord2f(0.33f, 1.0f); glVertex2f(-w,-w);
		}

		clock_gettime(CLOCK_REALTIME, &animationCurrentc);
		animationSpanc += timeDiff(&animationStartc, &animationCurrentc);
	glEnd();
	
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}
int PAUSE(Game *g, int keys[])
{
	static int kchkr = 0;
	clock_gettime(CLOCK_REALTIME, &timeCurrentqr);
	if (keys[XK_Up]) {
		timeSpanTq = timeDiff(&timeTq, &timeCurrentqr);
		if (timeSpanTq > 0.2) {
			clock_gettime(CLOCK_REALTIME, &timeTq);
			kchkr--;
		}
	}
	if (keys[XK_Down]) {
		timeSpanTr = timeDiff(&timeTr, &timeCurrentqr);
		if (timeSpanTr > 0.2) {
			clock_gettime(CLOCK_REALTIME, &timeTr);
			kchkr++;
		}
	}
	
	if (keys[XK_a]) {
		play_sounds(11);
	}
	if (keys[XK_s]) {
		play_sounds(12);
	}
	if (keys[XK_d]) {
		play_sounds(13);
	}
	if (keys[XK_f]) {
		play_sounds(14);
	}
	if (keys[XK_g]) {
		play_sounds(15);
	}
	if (keys[XK_h]) {
		play_sounds(16);
	}
	if (keys[XK_j]) {
		play_sounds(17);
	}
	if (keys[XK_k]) {
		play_sounds(18);
	}
	
	kchkr = kchkr%4;
	if (kchkr < 0)
		kchkr = 3;
	renderPauseBackground();
	renderPauseButtons(kchkr);
	kchkr = kchkr%4;
	if (keys[XK_Return] && kchkr == 2) {
		Restart(g);
	}
	if (keys[XK_Return] && kchkr == 3) {
		return 1;
	}
	return 0;
}
void renderPauseButtons(int x)
{
	glPushMatrix();
	glTranslatef((res[0]/2), (res[1]/2) + 50 - 15, 0);
	//change color based on selected or not
	glColor3ub(160, 160, 160);
	
	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();
	
	glPopMatrix();
	Rect q;
	q.bot = res[1]/2 - 20;
	q.left = res[0]/2 + 50 - 15;
	q.center = 0;
	ggprint8b(&q, 48, 0x00FFFFFF, "MAIN MENU");
	///////////////////////////////////////////
	glPushMatrix();
	glTranslatef((res[0]/2), (res[1]/2) + 100 - 15, 0);
	glColor3ub(100, 100, 100);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();
	Rect r;
	r.bot = res[1]/2 + 75;
	r.left = res[0]/2 - 40;
	r.center = 0;
	ggprint16(&r, 48, 0x00000000, "PAUSED");

	///////////////////////////////////////////
	glPushMatrix();
	glTranslatef((res[0]/2), (res[1]/2)- 15, 0);
	
	//change color based on selected or not
	if (x == 1)
		glColor3ub(160, 160, 160);
	else
		glColor3ub(100, 100, 100);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();
	Rect s;
	s.bot = res[1]/2 - 20;
	s.left = res[0]/2 - 15;
	s.center = 0;
	ggprint8b(&s, 48, 0x00000000, "Save");
	///////////////////////////////////////////
	glPushMatrix();
	glTranslatef((res[0]/2), (res[1]/2) - 50 - 15, 0);
	
	//change color based on selected or not
	if (x == 2)
		glColor3ub(160, 160, 160);
	else
		glColor3ub(100, 100, 100);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();
	Rect t;
	t.bot = res[1]/2 - 70;
	t.left = res[0]/2 - 20;
	t.center = 0;
	ggprint8b(&t, 48 , 0x00000000, "Restart");
	//////////////////////////////////////////////
	glPushMatrix();
	glTranslatef((res[0]/2), (res[1]/2) - 100 - 15, 0);
	
	//change color based on selected or not
	if (x == 3)
		glColor3ub(160, 160, 160);
	else
		glColor3ub(100, 100, 100);

	glBegin(GL_POLYGON);
		glVertex2i(-90, -15);
		glVertex2i(-100, -5);

		glVertex2i(-100, 5);
		glVertex2i(-90, 15);

		glVertex2i(90, 15);
		glVertex2i(100, 5);

		glVertex2i(100, -5);
		glVertex2i(90, -15);
	glEnd();

	glPopMatrix();
	Rect u;
	u.bot = res[1]/2 - 120;
	u.left = res[0]/2 - 15;
	u.center = 0;
	ggprint8b(&u, 48, 0x00000000, "Exit");
}
void renderPauseBackground()
{	
	glPushMatrix();
	glTranslatef((res[0]/2), res[1]/2, 0);

	//Background
	glColor3ub(47, 47, 47);
	glBegin(GL_POLYGON);
		glColor3ub(47, 47, 47);
		glVertex2i(175, -150);
		glColor3ub(47, 47, 47);
		glVertex2i(150, -175);
		glColor3ub(84, 84, 84);
		glVertex2i(-150, -175);
		glColor3ub(84, 84, 84);
		glVertex2i(-175, -150);
		glColor3ub(48, 48, 48);
		glVertex2i(-175, 120);
		glColor3ub(54, 54, 54);
		glVertex2i(-145, 150);
		glColor3ub(84, 84, 84);
		glVertex2i(150, 150);
		glColor3ub(84, 84, 84);
		glVertex2i(175, 125);
	glEnd();

	//left hand border
	glColor3ub(54, 54, 54);
	glBegin(GL_POLYGON);
		glVertex2i(-165, 10-150);
		glVertex2i(-175, 0-150);
		glVertex2i(-175, 120);
		glVertex2i(-165, 115);
	glEnd();

	//top left corner border
	glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(-165, 115);
		glVertex2i(-175, 120);
		glVertex2i(-145, 150);
		glVertex2i(-140, 140);
	glEnd();

	//top border
	glColor3ub(152, 152, 152);
	glBegin(GL_POLYGON);
		glVertex2i(-140, 140);
		glVertex2i(-145, 150);
		glVertex2i(175-25, 150);
		glVertex2i(160-25, 140);
	glEnd();

	//TOP RIGHT HAND BORDER
	glColor3ub(179, 179, 179);
	glBegin(GL_POLYGON);
		glVertex2i(+160, 115);
		glVertex2i(+175, 125);
		glVertex2i(+150, 150);
		glVertex2i(+135, 140);
	glEnd();

	//right hand border
	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(160, 140-25);
		glVertex2i(175, 150-25);
		glVertex2i(175, 0-150);
		glVertex2i(160,10-150);
	glEnd();

	//BOTTOM RIGHT HAND BORDER
	glColor3ub(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2i(165-5, -115-25);
		glVertex2i(175, -120-25-5);
		glVertex2i(145, -150-25);
		glVertex2i(140, -140-25);
	glEnd();

	//Bottom Border
	glColor3ub(12, 12, 12);
	glBegin(GL_POLYGON);
		glVertex2i(175-25, 0-175);
		glVertex2i(-175+25, 0-175);
		glVertex2i(-165+25, 10-175);
		glVertex2i(165-25, 10-175);
	glEnd();

	//BOTTOM LEFT HAND BORDER
	glColor3ub(63, 63, 63);
	glBegin(GL_POLYGON);
		glVertex2i(-165, -115-25);
		glVertex2i(-175, -120-25-5);
		glVertex2i(-145-5, -150-25);
		glVertex2i(-140, -140-25);
	glEnd();

	glPopMatrix();
}

Ppmimage *CreditsImages[15] = {NULL};
GLuint CreditsTextures[15];

void loadEndCreditsTextures()
{
	CreditsImages[0] = ppm6GetImage((char*)"parallax/cloud.ppm");
	CreditsImages[1] = ppm6GetImage((char*)"parallax/mount.ppm");
	CreditsImages[2] = ppm6GetImage((char*)"parallax/trees.ppm");
	CreditsImages[3] = ppm6GetImage((char*)"parallax/cliff.ppm");
	CreditsImages[4] = ppm6GetImage((char*)"parallax/grass.ppm");
	CreditsImages[5] = ppm6GetImage((char*)"parallax/trees2.ppm");
	CreditsImages[11] = ppm6GetImage((char*)"parallax/Dude.ppm");
	CreditsImages[12] = ppm6GetImage((char*)"images/THE_END.ppm");
	CreditsImages[6] = ppm6GetImage((char*)"images/CAM.ppm");
	CreditsImages[7] = ppm6GetImage((char*)"images/DAVID.ppm");	
	CreditsImages[8] = ppm6GetImage((char*)"images/JOB.ppm");
	CreditsImages[9] = ppm6GetImage((char*)"images/MATT.ppm");
	CreditsImages[10] = ppm6GetImage((char*)"images/ROSE.ppm");
	CreditsImages[13] = ppm6GetImage((char*)"images/Exit.ppm");
	
	glGenTextures(1, &CreditsTextures[0]); //CloudsTexture
	glGenTextures(1, &CreditsTextures[1]); //MountainsTexture
	glGenTextures(1, &CreditsTextures[2]); //TreesTexture
	glGenTextures(1, &CreditsTextures[3]); //CliffTexture
	glGenTextures(1, &CreditsTextures[4]); //GrassTexture
	glGenTextures(1, &CreditsTextures[5]); //Trees2Texture
	glGenTextures(1, &CreditsTextures[11]); //DudeTexture
	glGenTextures(1, &CreditsTextures[12]); //THE_ENDTexture
	glGenTextures(1, &CreditsTextures[6]); //CAM
	glGenTextures(1, &CreditsTextures[7]); //DAVE
	glGenTextures(1, &CreditsTextures[8]); //ME
	glGenTextures(1, &CreditsTextures[9]); //MATT
	glGenTextures(1, &CreditsTextures[10]); //flower
	glGenTextures(1, &CreditsTextures[13]); //Exit

	float h, w;
	
	//clouds Texture
	w = CreditsImages[0]->width;
	h = CreditsImages[0]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *cloudData = buildAlphaData(CreditsImages[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, cloudData);
	free(cloudData);
	
	//mountains texture
	w = CreditsImages[1]->width;
	h = CreditsImages[1]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *mountainData = buildAlphaData(CreditsImages[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, mountainData);
	free(mountainData);

	//trees background texture
	w = CreditsImages[2]->width;
	h = CreditsImages[2]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *treesData = buildAlphaData(CreditsImages[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, treesData);
	free(treesData);

	//cliff texture
	w = CreditsImages[3]->width;
	h = CreditsImages[3]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *cliffData = buildAlphaData(CreditsImages[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, cliffData);
	free(cliffData);

	//grass texture
	w = CreditsImages[4]->width;
	h = CreditsImages[4]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *grassData = buildAlphaData(CreditsImages[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, grassData);
	free(grassData);

	//trees foreground Texture
	w = CreditsImages[5]->width;
	h = CreditsImages[5]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *trees2Data = buildAlphaData(CreditsImages[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, trees2Data);
	free(trees2Data);	

	//Cameron	
	w = CreditsImages[6]->width;
	h = CreditsImages[6]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *camData = buildAlphaData(CreditsImages[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, camData);
	free(camData);	

	//David
	w = CreditsImages[7]->width;
	h = CreditsImages[7]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *davData = buildAlphaData(CreditsImages[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, davData);
	free(davData);	

	//Me
	w = CreditsImages[8]->width;
	h = CreditsImages[8]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *jobData = buildAlphaData(CreditsImages[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, jobData);
	free(jobData);

	w = CreditsImages[9]->width;
	h = CreditsImages[9]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *matData = buildAlphaData(CreditsImages[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, matData);
	free(matData);		

	//flower
	w = CreditsImages[10]->width;
	h = CreditsImages[10]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *rosData = buildAlphaData(CreditsImages[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, rosData);
	free(rosData);	

	//Dude Texture
	w = CreditsImages[11]->width;
	h = CreditsImages[11]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[11]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *dudeData = buildAlphaData(CreditsImages[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, dudeData);
	free(dudeData);	
	
	
	w = CreditsImages[12]->width;
	h = CreditsImages[12]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[12]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *ENDData = buildAlphaData(CreditsImages[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ENDData);
	free(ENDData);
	
	
	w = CreditsImages[13]->width;
	h = CreditsImages[13]->height;
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[13]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *ExitData = buildAlphaData(CreditsImages[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ExitData);
	free(ExitData);
}

struct timespec CreditsTime;
struct timespec CreditsCurrent;
static double CreditsSpan = 0.0;

void endCredits(Game *g, int keys[])
{
	static int creditsMusic = 1;
	if (creditsMusic) {
		play_sounds(19, 1);
		creditsMusic--;
	}
	Rect u;
	u.bot = res[1]/2;
	u.left = res[0]/2;
	u.center = 0;
	ggprint8b(&u, 48, 0x00FFFFFF, "Your Score: %d", g->Player_1.score);
	static float jmpspd = 0;
	static int jmp = 0;
	static float mov = 0, mov2 = 0;
	static bool ending = false;
	if (keys[XK_a] && mov < 0 && !ending)
		mov += 1;
	if (keys[XK_d] && mov > -1271 && !ending)
		mov -= 1;
	if (keys[XK_space] && !jmp && jmpspd == 0) {
		jmp = 1;
	}
	if (jmp) {
		jmpspd += 5;
	} else if (jmpspd > 0) {
		jmpspd-=5;
	}
	if (jmpspd >= 150) {
		jmpspd = 150;
		jmp = 0;
	}
	if (mov <= -1270) {
		ending = true;
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	//set background to black to give cinematic feel
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/////////////////////////////////////////////////////////////////////
	float w = CreditsImages[0]->width;
	float h = CreditsImages[0]->height;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[0]);
	glTranslatef(mov, 0, 0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	
	w = CreditsImages[1]->width;
	h = CreditsImages[1]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[1]);
	glTranslatef(mov*0.5, 0, 0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();

	w = CreditsImages[2]->width;
	h = CreditsImages[2]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[2]);
	glTranslatef(mov*3, 0, 0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);	

	glEnd();
	glPopMatrix();

	w = CreditsImages[2]->width;
        h = CreditsImages[2]->height;
        
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, CreditsTextures[2]);
        glTranslatef(mov*3+7000, 0, 0);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);        

        glEnd();
        glPopMatrix();

	
		
	w = CreditsImages[13]->width/5;
	h = CreditsImages[13]->height/5;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[13]);
	glTranslatef(mov*5, 65, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();


	/***********DUDE********************/
	w = CreditsImages[11]->width;
	h = CreditsImages[11]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[11]);
	if (ending) {
	    mov2+=2;
	}
	std::cout << mov << std::endl;
	glTranslatef(mov2, jmpspd, 0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
		
		clock_gettime(CLOCK_REALTIME, &CreditsCurrent);
		CreditsSpan += timeDiff(&CreditsTime, &CreditsCurrent);
		
		if (CreditsSpan >= 100) {
			CreditsSpan = 0.0;
			clock_gettime(CLOCK_REALTIME, &CreditsTime);
		}
		if (CreditsSpan < 10 && ((keys[XK_d] || keys[XK_a]) || ending)) {
			glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.1666f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.1666f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		} else if (CreditsSpan < 40 && ((keys[XK_d] || keys[XK_a]) || ending)) {
			glTexCoord2f(0.1666f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.333f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.333f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.1666f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		} else if (CreditsSpan < 60 && ((keys[XK_d] || keys[XK_a]) || ending)) {
			glTexCoord2f(0.3333f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.50f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.50f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.3333f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		} else if (CreditsSpan < 80 && ((keys[XK_d] || keys[XK_a]) || ending)) {
			glTexCoord2f(0.50f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.666f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.666f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.50f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		} else if ( ((keys[XK_d] || keys[XK_a]) || ending)) {
			glTexCoord2f(0.666f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.8333f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(0.8333f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.666f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		} else {
			glTexCoord2f(0.8333f, 0.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - h + h/2 + h/4);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex2f(res[0]/2 + ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
			glTexCoord2f(0.8333f, 1.0f); 
			glVertex2f(res[0]/2 - ((w/2)*0.1666), res[1]/2 - 2*h + h/2 + h/4);
		}

	glEnd();
	glPopMatrix();
	/*********************************/
	/////////////////////////////////////////////////////////////////////
	w = CreditsImages[3]->width;
	h = CreditsImages[3]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[3]);
	glTranslatef(mov*5, 0, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
        w = CreditsImages[3]->width;
        h = CreditsImages[3]->height;
        
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, CreditsTextures[3]);
        glTranslatef(mov*5+7000-20, 0, 0);
        glScalef(1, 1, 1);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);
        
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

        glEnd();
        glPopMatrix();
        /////////////////////////////////////////////////////////////////

	w = CreditsImages[4]->width;
	h = CreditsImages[4]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[4]);
	glTranslatef(mov*10, 0, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	
        w = CreditsImages[4]->width;
        h = CreditsImages[4]->height;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, CreditsTextures[4]);
        glTranslatef(mov*10+7000, 0, 0);
        glScalef(1, 1, 1);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

        glEnd();
        glPopMatrix();
        /////////////////////////////////////////////////////////////////
	

	//Cam
	w = CreditsImages[6]->width;
	h = CreditsImages[6]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[6]);
	glTranslatef(3*400+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	//David
	w = CreditsImages[7]->width;
	h = CreditsImages[7]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[7]);
	glTranslatef(3*1200+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	//me
	w = CreditsImages[8]->width;
	h = CreditsImages[8]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[8]);
	glTranslatef(3*2000+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	//Matt
	w = CreditsImages[9]->width;
	h = CreditsImages[9]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[9]);
	glTranslatef(3*2800+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////
	//flower
	w = CreditsImages[10]->width;
	h = CreditsImages[10]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[10]);
	glTranslatef(3*3600+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);

	glEnd();
	glPopMatrix();
	
	static int mvScore = -res[1] + 100;
	if (ending)
		mvScore++;

	u.bot = res[1]/2 + mvScore - 10;
	u.left = res[0]/2 - 150;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Your Score: %d", g->Player_1.score);
	
	u.bot = res[1]/2 + mvScore - 210;
	u.left = res[0]/2 - 150;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Monsters Killed: %d", g->Player_1.kills);
	
	u.bot = res[1]/2 + mvScore - 410;
	u.left = res[0]/2 - 150;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Remaining Lives: %d", g->Player_1.lives);

	u.bot = res[1]/2 + mvScore - 610;
	u.left = res[0]/2 - 100;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Artifact 1 :  %d", g->Player_1.artifact[0]);
	
	u.bot = res[1]/2 + mvScore - 810;
	u.left = res[0]/2 - 100;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Artifact 2 :  %d", g->Player_1.artifact[1]);
	
	u.bot = res[1]/2 + mvScore - 1010;
	u.left = res[0]/2 - 100;
	u.center = 0;
	ggprint40(&u, 48, 0x00FFFFFF, "Artifact 3 :  %d", g->Player_1.artifact[2]);
		/////////////////////////////////////////////////////////////////////
	//THE END
	w = CreditsImages[10]->width;
	h = CreditsImages[10]->height;
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, CreditsTextures[12]);
	glTranslatef(3*4360+mov*10, 150, 0);
	glScalef(1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0 , h/2+ res[1]/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h/2+ res[1]/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, res[1]/2 - h/2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, res[1]/2 - h/2);
	
	glEnd();
	glPopMatrix();

}
#endif
