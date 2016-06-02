#ifndef DRAW_F
#define DRAW_F

#include "game_objects.h"
#include "game.h"
#include "jobG.h"
#include <cstring>
#include <ctime>
#include <vector>
#include <stdlib.h>
#include <cmath>

template <typename OType>
void drawOType(OType otype, Game *g);
template <typename OType>
bool checkDistanceStats(OType otype, Game *g, float xcheck, float ycheck);

void setColor(Stats);
void drawSquare(Stats *, int, int, int);
void drawParticle(Particle *, int, int, int);
void drawBullet(Game *g, Bullet *b, float red, float blue, float green);
void drawTextString(float, float, float, float, std::string);
void drawText(float, float, float, float);
void drawPlayer(Player p);
void assign_gblock(gblock &, Stats &, int, int, int);
gblock return_gblock(gblock, int, int, int);
Game init_game(Game, gblock_info);
bool inDrawingDistanceBlock(Game *, gblock);
bool checkDistanceBlock(Game *, gblock, float, float);
void drawBlock(Game *, gblock);
void drawGBlocks(Game *);

void renderCharacter(Player player, Game *g, float w, int keys[], 
	GLuint personTexture1);

void set_gblock_size(double&, double&, double);
void set_gblock_gpos(double&, int, int);
void create_gblock(gblock&, int, int, int);
void begin_game(Game&, gblock_info&);

void drawGameStats(Game *, Stats);
void drawStats(Game *, Stats);
bool checkDistanceStats(Game *, Stats, float, float);
float getDistanceStatsVal(Game *, Stats, int);
float getDistanceStats(Game *, Stats);
float getXYDistValue(float, float);

void init_textures(Game &);

char* getBlockTexture(gblock block);
void renderBlockTexture(gblock block);
//
//
void init_blocks(Game *, gblock_info);

/* block.h */

/*
 * Author: David Vaughan
 * Purpose: 2D Array generator
 * Code: This is the header file for the "BlockConstructor" code.
 * See block.cpp for more information
 */
// Macros and templates //
//
//


/* end block.h */

//***************************************
//***************************************
//
// Templates section
//
//***************************************
//***************************************
//***************************************
//***************************************
//
// Operator= section, exchanging stats
// -- Many of these templates may just be
// copy-pasted to adjust for pointer obj
// -ects, only because I haven't figured
// out how to detect for pointer type
// inside the template functions.
//
//***************************************
//***************************************
template <typename Receiver, typename Source>
void exchangeGpos(Receiver receiver, Source source)
{
	receiver.stats.gpos[0] = source.stats.gpos[0];
	receiver.stats.gpos[1] = source.stats.gpos[1];
	receiver.stats.gpos[2] = source.stats.gpos[2];
}
template <typename Receiver, typename Source>
void exchangeGpos(Receiver *receiver, Source source)
{
	receiver->stats.gpos[0] = source.stats.gpos[0];
	receiver->stats.gpos[1] = source.stats.gpos[1];
	receiver->stats.gpos[2] = source.stats.gpos[2];
}
template <typename Receiver, typename Source>
void exchangeGpos(Receiver *receiver, Source *source)
{
	receiver->stats.gpos[0] = source->stats.gpos[0];
	receiver->stats.gpos[1] = source->stats.gpos[1];
	receiver->stats.gpos[2] = source->stats.gpos[2];
}
template <typename Receiver, typename Source>
void exchangeGpos(Receiver receiver, Source *source)
{
	receiver.stats.gpos[0] = source->stats.gpos[0];
	receiver.stats.gpos[1] = source->stats.gpos[1];
	receiver.stats.gpos[2] = source->stats.gpos[2];
}
//***************************************
//***************************************
//
// Drawing section, including distance
// checkers and some other stuff
//
//***************************************
//***************************************
// Checks the distance for objects via their stats
// will be used in draw function and likely physics
template <typename OType>
bool checkPlayerDistanceOType(OType otype, Game *g, float xcheck, float ycheck)
{
	Player player = g->Player_1;
	Stats stats;
	stats = otype.stats;
	bool indistancex = false, indistancey = false;
	if (abs(player.stats.gpos[0] - stats.gpos[0] +
		    		stats.width) < xcheck) {
		indistancex = true;
	}
	if (abs(player.stats.gpos[1] - stats.gpos[1] +
		    		stats.width) < ycheck) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
	return false;
}

// Another specialization, once again for stats
// and if the objects are pointers
template <typename OType>
bool checkPlayerDistanceOType(OType *otype, Game *g, float xcheck, float ycheck)
{
	Player player = g->Player_1;
	Stats stats;
	stats = otype->stats;
	bool indistancex = false, indistancey = false;
	if (abs(player.stats.gpos[0] - stats.gpos[0] +
		    		stats.width) < xcheck) {
		indistancex = true;
	}
	if (abs(player.stats.gpos[1] - stats.gpos[1] +
		    		stats.width) < ycheck) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
	return false;
}
template <typename C>
void calculateEnemy(C c, Game *g)
{
	if (checkPlayerDistanceOType(c, g, g->g_xres/2, g->g_yres/2)) {
		if (checkPlayerDistanceOType(c, g, 300, 300)) {
			c.pursuit = true;
		}
	}
}

template <typename C>
void getEnemyTexCoords(C &c, int type, float &x1, float &x2,
							float &y1, float &y2)
{
	float yc = type * 160;
	float mode = c.mode * 32;
	y1 = (yc + mode + 32)/640;
	y2 = (yc + mode)/640;
	if (c.stats.animationSpan >= 100) {
		c.stats.animationSpan = 0.0;
		clock_gettime(CLOCK_REALTIME, &c.stats.animationStart);
	} else if (true) {
		if (c.stats.animationSpan < 10.0) {
			x1 = 32/320;
			x2 = 0;
			return;
		} else if (c.stats.animationSpan < 20.0) {
			x1 = 64/320;
			x2 = 32/320;
			return;
		} else if (c.stats.animationSpan < 30.0) {
			x1 = 96/320;
			x2 = 64/320;
			return;
		} else if (c.stats.animationSpan < 40.0) {
			x1 = 128/320;
			x2 = 96/320;
			return;
		} else if (c.stats.animationSpan < 50.0) {
			x1 = 160/320;
			x2 = 128/320;
			return;
		} else if (c.stats.animationSpan < 60.0) {
			x1 = 192/320;
			x2 = 160/320;
			return;
		} else if (c.stats.animationSpan < 70.0) {
			x1 = 224/320;
			x2 = 192/320;
			return;
		} else if (c.stats.animationSpan < 80.0) {
			x1 = 256/320;
			x2 = 224/320;
			return;
		} else if (c.stats.animationSpan < 90.0) {
			x1 = 288/320;
			x2 = 256/320;
			return;
		} else if (c.stats.animationSpan < 100.0) {
			x1 = 1;
			x2 = 288/320;
			return;
		}
	} else {
    		x1 = 32/320;
    		x2 = 0;
		return;
	}
	clock_gettime(CLOCK_REALTIME, &c.stats.animationCurrent);
	c.stats.animationSpan += timeDiff(&c.stats.animationStart,
		&c.stats.animationCurrent);
}

template <typename C>
void renderEnemy(C &c, Game *g)
{
	//if (checkPlayerDistanceOType(c, g, g->g_xres/2, g->g_yres/2)) {
	    int type = c.type;
	    float xdist, ydist;
	    xdist = g->g_xres/2;// + (c.stats.gpos[0] - g->Player_1.stats.gpos[0] -
		    //c.stats.width);
	    ydist = g->g_yres/2;// + (c.stats.gpos[1] - g->Player_1.stats.gpos[1] -
		    //c.stats.width);
	    float size = 32;
	    float cx1, cx2, cy1, cy2;
	    getEnemyTexCoords(c, type, cx1, cx2, cy1, cy2);
	    if (c.stats.vel[0] > 0) {}
	    else { cx1 = cx1 * -1; cx2 = cx2 * -1; }
	    glPushMatrix();
	    glTranslatef(xdist, ydist, 0.0f);
	    glEnable(GL_ALPHA_TEST);
	    glAlphaFunc(GL_GREATER, 0.0f);
	    glBindTexture(GL_TEXTURE_2D, g->enemyTextures);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(cx1, cy1); glVertex2f(-size, -size);
	    	glTexCoord2f(cx1, cy2); glVertex2f(-size, size);
	    	glTexCoord2f(cx2, cy2); glVertex2f(size, size);
	    	glTexCoord2f(cx2, cy1); glVertex2f(size, -size);
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	//}
}



template <typename OType>
bool checkBlockDistanceOType(OType otype, gblock block, float xcheck, float ycheck)
{
	Stats stats = otype.stats;
	bool indistancex = false, indistancey = false;
	if (abs(stats.gpos[0] - block.stats.gpos[0] +
		    		block.stats.width) < xcheck) {
		indistancex = true;
	}
	if (abs(stats.gpos[1] - block.stats.gpos[1] +
		    		block.stats.width) < ycheck) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
	return false;
}

// Another specialization, once again for stats
// and if the objects are pointers
template <typename OType>
bool checkBlockDistanceOType(OType *otype, gblock block, float xcheck, float ycheck)
{
	Stats stats = otype->stats;
	bool indistancex = false, indistancey = false;
	if (abs(otype->stats.gpos[0] - block.stats.gpos[0] +
		    		block.stats.width) < xcheck) {
		indistancex = true;
	}
	if (abs(otype->stats.gpos[1] - block.stats.gpos[1] +
		    		block.stats.width) < ycheck) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
	return false;
}
// Template for objects with stats class - should apply to basically everything
template <typename OType>
void drawOType(OType otype, Game *g)
{
	Player p = g->Player_1;
	Stats stats;
	stats = otype.stats;
	if (checkPlayerDistanceOType(otype, g, g->g_xres/2,g->g_yres/2)) {
		float xdist, ydist;
		xdist = p.stats.spos[0] + (stats.gpos[0] - p.stats.gpos[0] - stats.width);
		ydist = p.stats.spos[1] + (stats.gpos[1] - p.stats.gpos[1] - stats.height);
		otype.draw(xdist, ydist, stats);
	}
}

// Specialization for pointer objects
template <typename OType>
void drawOType(OType *otype, Game *g)
{
	Player p = g->Player_1;
	Stats stats;
	stats = otype->stats;
	if (checkPlayerDistanceOType(otype, g, g->g_xres/2,g->g_yres/2)) {
		float xdist, ydist;
		xdist = p.stats.spos[0] + (stats.gpos[0] - p.stats.gpos[0] - stats.width);
		ydist = p.stats.spos[1] + (stats.gpos[1] - p.stats.gpos[1] - stats.height);
		otype->draw(xdist, ydist, stats);
	}
}


//*****************************
//*****************************
//
// Physics/collision templates
//
//*****************************
//*****************************
template <typename OType>
void check_gblock_collision(OType otype, Game *g, float xcheck, float ycheck)
{
	int nrows = g->game_info.rows;
	int ncols = g->game_info.columns;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			if (g->blocks[i][j].assigned == 1) {
				if (checkBlockDistanceOType(otype,g->blocks[i][j],
							xcheck,ycheck)) {
					otype.stats.gpos[0] += otype.stats.vel[0] * -1;
					otype.stats.gpos[1] += otype.stats.vel[1] * -1;
				}
			}
		}
	}
}

template <typename OType>
void updateObjGposStat(OType &otype)
{
	otype.stats.gpos[0] = otype.stats.gpos[0] + otype.stats.vel[0];
	otype.stats.gpos[1] = otype.stats.gpos[1] + otype.stats.vel[1];
}
template <typename OType>
void updateObjGposStat(OType *otype)
{
	otype->stats.gpos[0] = otype->stats.gpos[0] + otype->stats.vel[0];
	otype->stats.gpos[1] = otype->stats.gpos[1] + otype->stats.vel[1];
}
//
//
//
#endif
