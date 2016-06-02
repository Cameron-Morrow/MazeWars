// Lab 5
// David Vaughan
// 04/28/2016
//
// My code involves simplifying the render process by introducing draw
// functions. I also generate the random map and am attempting to introduce
// more AI control to the enemies.


#ifndef DRAW_F
#define DRAW_F

#include <GL/glx.h>
#include "game.h"
#include "game_objects.h"
#include "fonts/fonts.h"
#include "jobG.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "davidV.h"


template <typename T>
bool checkVoid(T *t) {
	if (t == NULL) {
		return true;
	} else {
		return false;
	}
}
//
//
// End Macros and Templates //

struct DRules {
	int num_turns;
	int num_halls;
	int MIN_HALL_VER_LENGTH;
	int MAX_HALL_VER_LENGTH;
	int MIN_HALL_HOR_LENGTH;
	int MAX_HALL_HOR_LENGTH;
	int MAXDIST;
	int MINDIST;
};

struct DSpecs {
	int BLOCK_LENGTH;
	int game_time_considered;
	int rows;
	int cols;
	int player_speed;
};

struct DInit {
	int startrow;
	int startcol;
	int endrow;
	int endcol;
};

struct DCounter {
	int counter;
	int finalcount;
};

class BlockConstructor {
	public:
	int tread;
	int direction;
	int col;
	int row;
	BlockConstructor();
	void operator=(BlockConstructor);
	void operator=(BlockConstructor*);
	~BlockConstructor();
};

class Block {
public:
	int maintype;
	int subtype;
	int path;
	BlockConstructor blockc;
	Block *north;
	Block *east;
	Block *south;
	Block *west;
	Block *northeast;
	Block *northwest;
	Block *southeast;
	Block *southwest;
	Block();
	void operator=(Block);
	void operator=(Block*);
	~Block();
};

struct DFork {
	int forkpos[2];
	DFork *prev;
	DFork *next;
	DRules rules;
	int attempted_turns[4];
	void init();
};

class DForkMonitor {
public:
	DRules forkRules;
	DForkMonitor();
	~DForkMonitor();
	// linked-fork functions
	DFork* getNextFork(DFork *fork) {
		if (!checkVoid(fork)) return fork->next;
	}
	DFork* getPrevFork(DFork *fork) {
		if (!checkVoid(fork)) return fork->prev;
	}
	bool linkForks(DFork *fork_src, DFork *fork_trg) {
		if (!checkVoid(fork_src)) {
			fork_src->next = fork_trg;
			fork_trg->prev = fork_src;
			return true;
		} else {
		    return false;
		}
	}
	void traceForward(DFork *fork) {
		if (!checkVoid(fork)) {
			DFork *nextfork = fork->next;
			fork = nextfork;
		}
	}
	void traceBackward(DFork *fork) {
		if (!checkVoid(fork)) {
			DFork *prevfork = fork->prev;
			fork = prevfork;
		}
	}
};


/////

/* array_functions.h */
using namespace std;

void generator(vector<vector<Block> >&, DSpecs, DInit, DRules);

void generateRules(DRules& rules, DSpecs& specs, DInit& init);
void srandByTime(struct timespec& rtime);
void initGamePositions(DInit& init, DSpecs& specs, struct timespec rtime);
void initialize_values(DSpecs specs, DInit init, vector<vector<Block> > &);

/* end array_functions.h*/

/* events.h */

//#include "pathing_functions.h"
//#include "block.h"
//#include "array_functions.h"

bool mayHitWall(int, int, int, DSpecs, DRules);
int calc_hall_length(DSpecs, int, Block &, int, int);
Block DbuildHall(DRules, DSpecs, int, int, int, vector<vector<Block> > &,
				DCounter &);

/* end events.h */
/* pathing_functions.h */

//#include "block.h"

void pathingInit(DRules, DInit, DSpecs, vector<vector<Block> >&);
void buildPath(DRules, DInit, DSpecs, vector<vector<Block> >&);
void initStartBlocks(DRules, DInit, DSpecs, 
                vector<vector<Block> >&, struct timespec rtime);
int DnewDirection(DSpecs specs, DRules rules, Block&);
int DrandomDirection(int);
int DrandomDirection();
void initPath(DRules, DInit, DSpecs, vector<vector<Block> > &);
Block returnBuildBlock(int, int&, int&, vector<vector<Block> > &, int);
//Block returnBuildBlock(int, Block&, vector<vector<Block> > &, int);
//void buildBlock(int, Block&, vector<vector<Block> > &, int);
DFork getNewFork(Block);
void setDisabledTurn(DFork &, Block);
void setDisabledTurn(DFork &, Block, int);
bool checkAttemptedTurn(DFork, int);
void connectForks(DFork &, DFork &);

int parseSurroundingBlocks(DSpecs specs, vector<vector<Block> > &dungeon,
		Block block);
vector<vector<Block> > newParsedMap(DSpecs specs, int,
									vector<vector<Block> > &dungeon);
int parseToBlockTextures(vector<vector<Block> > &, int, int, DSpecs);


void getBlockTexCoords(int, float &, float &, float &, float &);
vector<vector<Block> > newTexturedMap(DSpecs, vector<vector<Block> > &);
/////


void getBlockTexCoords(int type, float &x1, float &x2, float &y1, float &y2) {
	if (type < 16) {
		x1 = (float) (32 + type*32) / 512;
		x2 = x1 - (float) 32/512;
		y1 = (float) 1/4;
		y2 = 0;
	} else {
		x1 = (float) 32/512;
		x2 = 0;
		y1 = (float) 2/4;
		y2 = (float) 1/4;
	}
	/*switch(type) {
		case 0:
			x1 = (float) 8.5/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 3/8;
			y2 = y1 - (float) 1/4;
			break;
		case 1:
			x1 = 1;
			x2 = x1 - (float) 1/13;
			y1 = (float) 1/4;
			y2 = y1 - (float) 1/4;
			break;
		case 2:
			x1 = 1;
			x2 = x1 - (float) 1/13;
			y1 = 1;
			y2 = (float) 3/4;
			break;
		case 3:
			x1 = (float) 11/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 3/4;
			y2 = y1 - (float) 1/4;
			break;
		case 4:
			x1 = 1;
			x2 = x1 - (float) 1/13;
			y1 = (float) 3/4;
			y2 = y1 - (float) 1/4;
			break;
		case 5:
			x1 = (float) 6/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 2/4;
			y2 = y1 - (float) 1/4;
			break;
		case 6:
			x1 = (float) 12/13;
			x2 = x1 - (float) 1/13;
			y1 = 1;
			y2 = (float) 3/4;
			break;
		case 7:
			x1 = (float) 11/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 7/8;
			y2 = y1 - (float) 1/4;
			break;
		case 8:
			x1 = 1;
			x2 = 1 - (float) 1/13;
			y1 = (float) 1/2;
			y2 = (float) 1/4;
			break;
		case 9:
			x1 = (float) 12/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 3/4;
			y2 = y1 - (float) 1/4;
			break;
		case 10:
			x1 = (float) 5/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 1/4;
			y2 = 0;
			break;
		case 11:
			x1 = (float) 11.5/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 3/4;
			y2 = y1 - (float) 1/4;
			break;
		case 12:
			x1 = (float) 12/13;
			x2 = x1 - (float) 1/13;
			y1 = 1;
			y2 = y1 - (float) 1/4;
			break;
		case 13:
			x1 = (float) 12/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 7/8;
			y2 = y1 - (float) 1/4;
			break;
		case 14:
			x1 = (float) 11.5/13;
			x2 = x1 - (float) 1/13;
			y1 = 1;
			y2 = y1 - (float) 1/4;
			break;
		case 15:
			x1 = (float) 6/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 1/4;
			y2 = 0;
			break;
		case 16:
			x1 = (float) 2/13;
			x2 = x1 - (float) 1/13;
			y1 = (float) 2/4;
			y2 = y1 - (float) 1/4;
			break;
			
	}*/
}


void init_textures(Game &game) {
	Ppmimage *blockSpriteSheet;
       	blockSpriteSheet = ppm6GetImage((char*)"images/newtiles.ppm");
	//create opengl texture elements for the blockspritesheet
    Ppmimage *enemySpriteSheet;
    	enemySpriteSheet = ppm6GetImage((char*)"images/enemysheet.ppm");
	glGenTextures(1, &game.blockTexture);
	
	//person
	int w = blockSpriteSheet->width;
	int h = blockSpriteSheet->height;
	unsigned char *blockData = buildAlphaData(blockSpriteSheet);	
	glBindTexture(GL_TEXTURE_2D, game.blockTexture);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, blockData);
	free(blockData);

	glGenTextures(1, &game.enemyTextures);

	w = enemySpriteSheet->width;
	h = enemySpriteSheet->height;
	unsigned char *enemyData = buildAlphaData(enemySpriteSheet);
	glBindTexture(GL_TEXTURE_2D, game.enemyTextures);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, enemyData);
	free(enemyData);
	glBindTexture(GL_TEXTURE_2D, 0);
}
/////

void drawTextString(float x, float y, 
	float offsetx, float offsety, std::string printvalue)
{
	Rect r;
	std::string s;
	char const *c;
	r.center = 0;
	r.bot = x + offsety;
	r.left = y + offsetx;
	s = printvalue;
	c = s.c_str();
	ggprint8b(&r, 0, 0, "%s", c);
}

void drawText(float x, float y, float offsetx, float offsety, float printvalue)
{
	Rect r;
	std::string s;
	char const *c;
	r.center = 0;
	r.bot = x + offsety;
	r.left = y + offsetx;
	s = std::to_string(printvalue);
	c = s.c_str();
	ggprint8b(&r, 0, 0, "%s", c);
}


/*
void drawParticle(Particle *p, int red, int blue, int green)
{
	setColor(*p->stats);
	glPushMatrix();
	glTranslatef(p->stats->pos[0], p->obj->pos[1], p->obj->pos[2]);
	glBegin(GL_QUADS);
		glVertex2i(-p->stats->width, -p->obj->height);
		glVertex2i(-p->stats->width, p->obj->height);
		glVertex2i(p->stats->width, p->obj->height);
		glVertex2i(p->stats->width, -p->obj->height);
	glEnd();
	glPopMatrix();
}
*/


void drawBullet(Game *g, Bullet *b, float red, float blue, float green)
{
	//Stats *bullet = b->stats;
	//setColor(*bullet);
	float xdist, ydist;
	//glColor3f(red, green, blue);
	xdist = 625 + (b->stats.gpos[0] - g->Player_1.stats.gpos[0] - 1.0);
	ydist = 450 + (b->stats.gpos[1] - g->Player_1.stats.gpos[1] - 1.0);
	b->stats.spos[0] = xdist;
	b->stats.spos[1] = ydist;
	//glTranslatef(xdist,ydist, 0);
	glColor3f(red - .2f, blue - 0.2f, green - 0.2f);
	glBegin(GL_POINTS);
	glVertex2f(xdist, ydist);
	glVertex2f(xdist-1.0f, ydist);
	glVertex2f(xdist+1.0f, ydist);
	glVertex2f(xdist, ydist-1.0f);
	glVertex2f(xdist, ydist+1.0f);
	glVertex2f(xdist-1.0f, ydist-1.0f);
	glVertex2f(xdist-1.0f, ydist+1.0f);
	glVertex2f(xdist+1.0f, ydist-1.0f);
	glVertex2f(xdist+1.0f, ydist+1.0f);
	glEnd();

}

// Cameron wrote some player render code w/Gordons
// ship as the temporary player - I've 
// basically put it into it's own draw function
// and added some features, ie the x/y pos text


void drawPlayer(Player p)
{
	glColor3f(p.stats.color[0], p.stats.color[1],
		p.stats.color[2]);
	glPushMatrix();
	glTranslatef(625, 450, p.stats.gpos[2]);
	glRotatef(p.stats.angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(-12.0f, -10.0f);
	glVertex2f(  0.0f,  20.0f);
	glVertex2f(  0.0f,  -6.0f);
	glVertex2f(  0.0f,  -6.0f);
	glVertex2f(  0.0f,  20.0f);
	glVertex2f( 12.0f, -10.0f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	drawText(p.stats.spos[0],p.stats.spos[1],10,10,p.stats.gpos[0]);
	drawText(p.stats.spos[0],p.stats.spos[1],10,-10,p.stats.gpos[1]);
}

void assign_gblock(gblock &block, Stats &stats, int type, int row, int col)
{
	block.type = type;
	if (type < 16) {
	    block.assigned = 1;
	}
	else {
	    block.assigned = 0;
	}
	stats.gpos[0] = row * 50.0;
	stats.gpos[1] = col * 50.0;
	printf("Block[%d][%d] located at x(%f) y(%f)\n",
		row,col,block.stats.gpos[0],block.stats.gpos[1]);
}

gblock return_gblock(gblock block, int type, int row, int col)
{
	block.type = type;
	block.assigned = 1;
	block.stats.gpos[0] = row * 50.0;
	block.stats.gpos[1] = col * 50.0;
	printf("Block[%d][%d] located at x(%f) y(%f)\n",
		row,col,block.stats.gpos[0],block.stats.gpos[1]);
	return block;
}

Game init_game(Game g, gblock_info gbi)
{
	Game game = g;

	game.game_info.rows = gbi.rows;
	game.game_info.columns = gbi.columns;
	game.blocks = new gblock*[gbi.rows];
	for (int i = 0; i < gbi.rows; i++) {
		game.blocks[i] = new gblock[gbi.columns];
	}
	/*
	assign_gblock(game.blocks[5][25],game.blocks[5][25].stats,1,5,25);
	assign_gblock(game.blocks[15][15],game.blocks[15][15].stats,1,15,15);
	assign_gblock(game.blocks[30][30],game.blocks[30][30].stats,1,30,30);
	assign_gblock(game.blocks[0][0],game.blocks[0][0].stats,0,0,10);
	assign_gblock(game.blocks[25][5],game.blocks[25][5].stats,0,25,5);
	assign_gblock(game.blocks[7][21],game.blocks[7][21].stats,0,7,21);
	*/
	game.blocks[5][5] = return_gblock(game.blocks[5][5],1,5,5);
	return game;
}

char* getBlockTexture(gblock block)
{
	switch(block.type)
	{
		case 0:
		return "images/pokecavefloor.ppm";
		break;
		case 1:
		return "images/pokecavewallleft.ppm";
		break;
		default:
		return "images/pokecavefloor.ppm";
		break;
	}
}

GLuint renderBlockTexture(gblock& block)
{
	char* filepath = getBlockTexture(block);
	block.stats.texture = ppm6GetImage(filepath);
	GLuint blockTexture;
	int w = block.stats.width;
	int h = block.stats.height;
	glBindTexture(GL_TEXTURE_2D, blockTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *imageData = buildAlphaData(block.stats.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return blockTexture;
}

void set_gblock_gpos(double& gposition, int num, int bsize)
{
	gposition = (double)num * bsize;
}

void set_gblock_size(gblock& block, double& height, double& width, double size)
{
	block.stats.height = size;
	block.stats.width = size;
	height = size;
	width = size;
}

void create_gblock(gblock& block, int type, int row, int col)
{
	block.type = type;
	block.assigned = 1;
	//block.stats.obj_texture = renderBlockTexture(block);
	set_gblock_gpos(block.stats.gpos[0], row, block.stats.width*2);
	set_gblock_gpos(block.stats.gpos[1], col, block.stats.width*2);
	//printf("Block[%d][%d] located at x(%f) y(%f)\n", row, col, 
	//	block.stats.gpos[0], block.stats.gpos[1]);
}

void begin_game(Game& game, gblock_info& gbi)
{
	game.game_info.rows = gbi.rows;
	game.game_info.columns = gbi.columns;
	game.blocks = new gblock*[gbi.rows];
	for (int i = 0; i < gbi.rows; i++) {
		game.blocks[i] = new gblock[gbi.columns];
	}
	for (int i = 0; i < gbi.rows; i++) {
		for (int j = 0; j < gbi.columns; j++) {
			set_gblock_size(game.blocks[i][j],
				game.blocks[i][j].stats.height,
				game.blocks[i][j].stats.width, gbi.width);
		}
	}
	//
	DSpecs specs;
	DInit init;
	DRules rules;
	specs.BLOCK_LENGTH = gbi.width;
	specs.player_speed = 16;
	specs.game_time_considered = 0;
	generateRules(rules, specs, init);
	Block block;
	vector<Block> dungeonCols(specs.cols, block);
	vector<vector<Block> > dungeon(specs.rows, dungeonCols);
	generator(dungeon, specs, init, rules);
	//
	for (int i = 0; i < gbi.rows; i++) {
		for (int j = 0; j < gbi.columns; j++) {
			create_gblock(game.blocks[i][j], dungeon[i][j].subtype, i, j);
		}
	}
	game.Player_1.stats.gpos[0] = init.startrow*gbi.width;
	game.Player_1.stats.gpos[1] = init.startcol*gbi.width;
}

//
//
// Rendering function section
//
//
//

bool inDrawingDistanceBlock(Game *g, gblock block)
{
	Player player = g->Player_1;
	bool indistancex = false, indistancey = false;
	printf("%f ", block.stats.gpos[0] - player.stats.gpos[0] - 25);
	printf("%f\n", block.stats.gpos[1] - player.stats.gpos[1] - 25);
	if (abs(player.stats.gpos[0] - block.stats.gpos[0] -
		block.stats.width*2) < 625) {
		indistancex = true;
	}
	if (abs(player.stats.gpos[1] - block.stats.gpos[1] -
		block.stats.width*2) < 450) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
}


float getXYDistValue(float x, float y)
{
	return sqrt((x*x)+(y*y));
}
float getDistanceStatsVal(Game *g, Stats stats, int coord)
{
	Player player = g->Player_1;
	float distance = player.stats.gpos[coord] - stats.gpos[coord] 
	+ stats.width; 
	return distance;
}
float getDistanceStats(Game *g, Stats stats)
{
	Player player = g->Player_1;
	float distancex = player.stats.gpos[0] - stats.gpos[0] 
	+ stats.width;
	float distancey = player.stats.gpos[1] - stats.gpos[1] 
	+ stats.width;
	float distance = sqrt((distancex*distancex)+(distancey*distancey));
	return distance;
}

bool checkDistanceBlock(Game *g, gblock block, float xcheck, float ycheck)
{
	Player player = g->Player_1;
	bool indistancex = false, indistancey = false;
	if (abs(player.stats.gpos[0] - block.stats.gpos[0] +
		block.stats.width) < xcheck) {
		indistancex = true;
	}
	if (abs(player.stats.gpos[1] - block.stats.gpos[1] +
		block.stats.width) < ycheck) {
		indistancey = true;
	}
	if (indistancex && indistancey) {
		return true;
	}
	return false;
}

bool checkDistanceStats(Game *g, Stats stats, float xcheck, float ycheck)
{
	Player player = g->Player_1;
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


void drawStats(Game *g, Stats stats)
{
	Player player = g->Player_1;
	float xdist, ydist;
	xdist = g->Player_1.stats.spos[0] +
	(stats.gpos[0] - g->Player_1.stats.gpos[0] - stats.width);
	ydist = g->Player_1.stats.spos[1] +
	(stats.gpos[1] - g->Player_1.stats.gpos[1] - stats.width);
	float size = stats.width;
	glColor3f((int)stats.color[0], (int)stats.color[1],
		(int)stats.color[2]);
	glPushMatrix();
	glTranslatef(xdist, ydist, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-size, -size);
	glVertex2f(-size, size);
	glVertex2f(size, size);
	glVertex2f(size, -size);
	glEnd();
	glPopMatrix();
	drawTextString(ydist,xdist,-25,0,"davidbox");	
}

void drawBlock(Game *g, gblock block)
{
	Player player = g->Player_1;
	float xdist, ydist;
	xdist = g->g_xres/2 + (block.stats.gpos[0] - player.stats.gpos[0] -
		block.stats.width);
	ydist = g->g_yres/2 + (block.stats.gpos[1] - player.stats.gpos[1] -
		block.stats.width);
	float size = block.stats.width;
	float cx1, cx2, cy1, cy2;
	getBlockTexCoords(block.type, cx1, cx2, cy1, cy2);
	glPushMatrix();
	glTranslatef(xdist, ydist, 0.0f);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g->blockTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(cx2, cy1); glVertex2f(-size, -size);
		glTexCoord2f(cx2, cy2); glVertex2f(-size, size);
		glTexCoord2f(cx1, cy2); glVertex2f(size, size);
		glTexCoord2f(cx1, cy1); glVertex2f(size, -size);
		/*glTexCoord2f(cx2, cy2); glVertex2f(-size, -size);
		glTexCoord2d(cx1, cy2); glVertex2f(-size, size);
		glTexCoord2d(cx1, cy1); glVertex2f(size, size);
		glTexCoord2f(cx2, cy1); glVertex2f(size, -size);*/
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void drawGameStats(Game *g, Stats stats)
{
	if (checkDistanceStats(g, stats, g->g_xres/2, g->g_yres/2)) {
		drawStats(g, stats);
	}
}

void drawGBlocks(Game *g)
{
	int nrows = g->game_info.rows;
	int ncols = g->game_info.columns;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			if (g->blocks[i][j].assigned == 1) {
				if (checkDistanceBlock(g,g->blocks[i][j],
					(float)g->g_xres/2,(float)g->g_yres/2)) {
					glColor3f(255.0, 255.0, 255.0);
					drawBlock(g, g->blocks[i][j]);
				}
			}
		}
	}
}


/***************************************************
*
*
*
*
*
*
*
*
****************************************************/
void init_blocks(Game *g, gblock_info gbi)
{
	g->game_info.rows = gbi.rows;
	g->game_info.columns = gbi.columns;
}
/****************************************
Trying to port over my dungeon-generator
this block: array_functions.cpp
*/



using namespace std;

void generateRules(DRules& rules, DSpecs& specs, DInit& init)
{
	/*
	*	@descr: Variables which will hold our generated data
	*	@param rows
	*	the number of rows generated for the 2d array
	*	@param cols
	*	the number of columns generated for the 2d array
	*	@param odd_blocks
	*	set to 0 if an even number of blocks are generated
	*	for the min distance, 1 if odd. 
	*	@param gensize
	*	size of rows/cols generated. literally blocks_to_travel/2
	*	@param blocks_to_travel
	*	this is the possible range of blocks that could be traveled
	*	by the player/object. Although it's essentially a constant
	*	range for each time type, it provides a good base for the
	*	random generation of rows and columns to start from.
	*	@param rows_cols_diff
	*	this is used to provide "dynamic" generation to the columns
	*	ie if blocks_to_travel = 45, rows = 23, then cols generates
	*	off of the range 23-67.
	*	this lets the maps change and become fun
	*/
	struct timespec rtime;
	bool randomizingDone;

	int rows;			
	int cols;
	int rows_cols_diff;
	int gensize;
	int odd_blocks;

	double travel_speed;
	int min_blocks_traveled;
	int max_blocks_traveled;
	int blocks_to_travel;
	
	unsigned int num_halls;
	unsigned int num_turns;
	int total_dist;
	// First, determine the game length type
	int type = specs.game_time_considered;
	/*int min_secs, max_secs;
	switch (type) {
		case 0:
			// 3-6 mins
			min_secs = 180;
			max_secs = 360;
			srandByTime(rtime);
			break;

		case 1:
			// 6-12 mins
			min_secs = 360;
			max_secs = 720;
			srandByTime(rtime);
			break;

		case 2:
			// 12-25 mins
			min_secs = 720;
			max_secs = 1500;
			srandByTime(rtime);
			break;

		case 3:
			// 25-45 mins
			min_secs = 1500;
			max_secs = 2700;
			srandByTime(rtime);
			break;

		default:
			min_secs = 180;
			max_secs = 1500;
			srandByTime(rtime);
			break;
	}
	// Next, determine how far the player travels
	// per second compared to block length/size*/
	travel_speed = specs.player_speed / (double) specs.BLOCK_LENGTH;
	/*min_blocks_traveled = ceil((double) min_secs*travel_speed);
	max_blocks_traveled = ceil((double) max_secs*travel_speed);
	blocks_to_travel = abs(min_blocks_traveled - max_blocks_traveled);
	rules.MINDIST = min_blocks_traveled;
	rules.MAXDIST = max_blocks_traveled * 3;
	// Check if odd or even
	if (blocks_to_travel % 2 == 1) {
		gensize = (blocks_to_travel - 1) / 2;
		odd_blocks = 1;
	} else {
		gensize = blocks_to_travel / 2;
		odd_blocks = 0;
	}
	// Now, generate the number of columns and rows
	// randomly. This makes dynamic maps of various sizes
	//
	srandByTime(rtime);
	rows = rand()%(gensize + 1) + min_blocks_traveled;
	rows_cols_diff = abs(gensize - rows);
	// Redo the seed
	srandByTime(rtime);
	cols = rand()%(gensize + odd_blocks + 1)
				+ min_blocks_traveled;
	specs.rows = rows;
	specs.cols = cols;
	total_dist = rows + cols;
	//
	// Now randomize the initial starting/ending points*/

	// Calculate hallway length min/max from #rows/columns
	specs.rows = 100;
	specs.cols = 100;
	initGamePositions(init, specs, rtime);
	rules.MIN_HALL_VER_LENGTH = (int) 1/travel_speed;
	rules.MAX_HALL_VER_LENGTH = (int) 7/travel_speed;
	rules.MIN_HALL_HOR_LENGTH = (int) 1/travel_speed;
	rules.MAX_HALL_HOR_LENGTH = (int) 7/travel_speed;
	/*while(!randomizingDone) {
		srandByTime(rtime);
		int PATH_LENGTH = rand()%(rules.MAXDIST - rules.MINDIST) +
		    rules.MINDIST;
		int AVG_HALL_LENGTH = PATH_LENGTH / 
		    ((rules.MAX_HALL_VER_LENGTH + rules.MAX_HALL_HOR_LENGTH)/2);
		//num_halls = PATH_LENGTH / AVG_HALL_LENGTH;
		//num_turns = num_halls - 1;
		if ((num_halls * AVG_HALL_LENGTH) > PATH_LENGTH) {
		    continue;
		} else {randomizingDone = true;}
	}*/
	//rules.num_halls = num_halls;
	//rules.num_turns = num_turns;
		printf("rows %d cols %d\n", specs.rows, specs.cols);
	}

	void initialize_values(DSpecs specs, DInit init, vector<vector<Block> > &dungeon)
	{
		dungeon.resize(specs.rows);
		for (int i = 0; i < specs.rows; i++) {
			dungeon.resize(specs.cols);
			for (int j = 0; j < specs.cols; j++) {
				dungeon[i][j].maintype = 0;
				dungeon[i][j].subtype = 16;
				dungeon[i][j].path = 0;
				dungeon[i][j].blockc.tread = 0;
				dungeon[i][j].blockc.direction = 0;
				dungeon[i][j].blockc.col = j;
				dungeon[i][j].blockc.row = i;
			}
		}
	}

	void srandByTime(struct timespec& rtime)
	{
		clock_gettime(CLOCK_REALTIME, &rtime);
		srand(rtime.tv_nsec);
	}

/*
 * @desc
 * this function randomizes the starting points of the 2d array
 * based off of the nanosecond time, and the choice of either the
 * random column or row # generated
 */
 void initGamePositions(DInit& init, DSpecs& specs, struct timespec rtime)
 {
	if (specs.cols % 2 == 0) {
		init.startcol = specs.cols / 2;
		if (specs.rows % 2 == 0) {
			init.startrow = specs.rows / 2;
		} else {
			init.startrow = 1 + specs.rows / 2;
		}
	} else {
		init.startcol = 1 + specs.cols / 2;
		if (specs.rows % 2 == 0) {
			init.startrow = specs.rows / 2;
		} else {
			init.startrow = 1 + specs.rows / 2;
		}
	}
}


/***************
end array_functions.cpp
now block.cpp
*/

/*
 * Author: David Vaughan
 * Purpose: 2D Array generator
 * Code: This is the constructor for the "BlockConstructor", which defines each
 * point in the 2d array and keeps track of their relative position in the array.
 * It will allow block "types" to be assigned to each point in the array, which 
 * keeps organization in place.
 */

 BlockConstructor::BlockConstructor() {
 }

 BlockConstructor::~BlockConstructor() {
 }

 void BlockConstructor::operator=(BlockConstructor blockc) {
 	tread = blockc.tread;
 	direction = blockc.direction;
 	col = blockc.col;
 	row = blockc.row;
 }

 void BlockConstructor::operator=(BlockConstructor *blockc) {
 	tread = blockc->tread;
 	direction = blockc->direction;
 	col = blockc->col;
 	row = blockc->row;
 }

/*
 * operations with class "Block"
 */

 Block::Block() {
 	north = NULL;
 	east = NULL;
 	south = NULL;
 	west = NULL;
 	northeast = NULL;
 	northwest = NULL;
 	southeast = NULL;
 	southwest = NULL;
	maintype = 0;
	subtype = 0;
	path = 0;
}

Block::~Block() {
	if (checkVoid(north)) delete north;
	if (checkVoid(east)) delete east;
	if (checkVoid(south)) delete south;
	if (checkVoid(west)) delete west;
	if (checkVoid(northeast)) delete northeast;
	if (checkVoid(northwest)) delete northwest;
	if (checkVoid(southeast)) delete southeast;
	if (checkVoid(southwest)) delete southwest;
}

void Block::operator=(Block block) {
	if (checkVoid(block.north)) {
		blockc = block.blockc;
		if (checkVoid(block.north)) north = block.north;
		if (checkVoid(block.east)) east = block.east;
		if (checkVoid(block.south)) south = block.south;
		if (checkVoid(block.west)) west = block.west;
		if (checkVoid(block.northeast)) northeast = block.northeast;
		if (checkVoid(block.northwest)) northwest = block.northwest;
		if (checkVoid(block.southeast)) southeast = block.southeast;
		if (checkVoid(block.southwest)) southwest = block.southwest;
	}
	maintype = block.maintype;
	subtype = block.subtype;
	path = block.path;
}

void Block::operator=(Block *block) {
	if (checkVoid(block)) {
		blockc = block->blockc;
		if (checkVoid(block->north)) north = block->north;
		if (checkVoid(block->east)) east = block->east;
		if (checkVoid(block->south)) south = block->south;
		if (checkVoid(block->west)) west = block->west;
		if (checkVoid(block->northeast)) northeast = block->northeast;
		if (checkVoid(block->northwest)) northwest = block->northwest;
		if (checkVoid(block->southeast)) southeast = block->southeast;
		if (checkVoid(block->southwest)) southwest = block->southwest;
	}
	maintype = block->maintype;
	subtype = block->subtype;
	path = block->path;
}

void DFork::init() {
	attempted_turns[0] = 0;
	attempted_turns[1] = 0;
	attempted_turns[2] = 0;
	attempted_turns[3] = 0;	
}



/************
end block.cpp
now events.cpp
*/


bool mayHitWall(int direction, int row, int col, DSpecs specs, DRules rules)
{
	switch(direction) {
		case 0:
		if (row  == 0) return true;
		if (row - rules.MAX_HALL_VER_LENGTH < 0) return true;
		return false;
		case 1:
		if (col == specs.cols - 1) return true;
		if (col + rules.MAX_HALL_HOR_LENGTH > specs.cols - 1) return true;
		return false;
		case 2:
		if (row >= specs.rows - 1) return true;
		if (row + rules.MAX_HALL_VER_LENGTH > specs.rows - 1) return true;
		return false;
		case 3:
		if (col == 0) return true;
		if (col - rules.MAX_HALL_HOR_LENGTH < 0) return true;
		return false;
	}
}

int calc_hall_length(DSpecs specs, int direction,
	Block &current, int max_l, int min_l)
{
	struct timespec rtime;
	int HALL_LENGTH;

	srandByTime(rtime); // get random seed
	HALL_LENGTH = rand()%(max_l - min_l) + min_l;
	switch(direction) {
		case 0:	// if north
		if (current.blockc.row - HALL_LENGTH < 1) {
			calc_hall_length(specs, direction,
				current, max_l, min_l);
			break;
		} else {
			break;
		}
		case 1: // if east
		if (current.blockc.col + HALL_LENGTH > specs.cols - 1) {
			calc_hall_length(specs, direction,
				current, max_l, min_l);
			break;
		} else {
			break;
		}
		case 2: // if south
		if (current.blockc.row + HALL_LENGTH > specs.rows - 1) {
			calc_hall_length(specs, direction,
				current, max_l, min_l);
			break;
		} else {
			break;
		}
		case 3: // if west
		if (current.blockc.col - HALL_LENGTH < 1) {
			calc_hall_length(specs, direction,
				current, max_l, min_l);
			break;
		} else {
			break;
		}	
	}
	//printf("Hall_length = %d\n", HALL_LENGTH);
	return HALL_LENGTH;
}

Block DbuildHall(DRules rules, DSpecs specs,
	int direction, int row, int col,
	vector<vector<Block> > &dungeon, DCounter &counter)
{
	// If block is sitting on the wall, return the current block
	// and essentially cycle back to the event cast in buildPath(...)
	if (!mayHitWall(direction, row,
		col, specs, rules)) {
		DCounter blockcounter;
	struct timespec rtime;
	int max_l;
	int min_l;
	int HALL_LENGTH;
	bool calculating_hlength = true;

		//set hall bounds based on direction
	if (direction == 0 || direction == 2) {
		max_l = rules.MAX_HALL_VER_LENGTH;
		min_l = rules.MIN_HALL_VER_LENGTH; 
	} else {
		max_l = rules.MAX_HALL_HOR_LENGTH;
		min_l = rules.MIN_HALL_HOR_LENGTH;
	}
	Block current;
	Block next;
	next = new Block;
	current.blockc.row = row;
	current.blockc.col = col;
	HALL_LENGTH = calc_hall_length(specs, direction, current, max_l, min_l);

		//Now it's time to start "setting" the pathway
	blockcounter.finalcount = HALL_LENGTH;
	for (int i = 0; i < blockcounter.finalcount; i++) {
		next = returnBuildBlock(1, row, col, dungeon, direction);
			//printf("2) s_row = %d s_col = %d\n",row, col);
		switch(direction) {
			case 0:
			row = row - 1;
			continue;
			case 1:
			col = col + 1;
			continue;
			case 2:
			row = row + 1;
			continue;
			case 3:
			col = col - 1;
			continue;
		}
			//printf("3) s_row = %d s_col = %d\n",row, col);
	}
	Block end;
	end.blockc.row = row;
	end.blockc.col = col;
	end.blockc.direction = direction;

//		monitor.prev_fork_hallway = 1;
//		monitor.prev_fork_turned = 0;

	counter.counter = counter.counter + 1;
	return end;
}
}

void DbuildTurn(DRules rules, DSpecs specs,
	int row, int col, vector<vector<Block> > &dungeon)
{/*
	if (row == specs.rows - 1 || row == 0) {
		if (col == specs.cols - 1 || col = 0) {

		}
	}*/
	}

// this has taken me FOREVER to do
// gaining a lot of good experience, though

/********
end events.cpp
now main.cpp (converting "main" to be titled generator)
*/

void generator(vector<vector<Block> > &dungeon, DSpecs specs,
		DInit init, DRules rules)
{
	initialize_values(specs, init, dungeon);
	pathingInit(rules, init, specs, dungeon);
	//dungeon[init.startrow][init.startcol].maintype = 1;
	//dungeon[init.endrow][init.endcol].maintype = 2;
	for (int i = 0; i < specs.rows; i++) {
		for (int j = 0; j < specs.cols; j++) {
			printf("%d",dungeon[i][j].maintype);
		}
		printf("\n");
	}
	printf("Min/max ver-hall length: %d || %d\n",
		rules.MIN_HALL_VER_LENGTH, rules.MAX_HALL_VER_LENGTH);
	printf("Min/max hor-hall length: %d || %d\n",
		rules.MIN_HALL_HOR_LENGTH, rules.MAX_HALL_HOR_LENGTH);
}

/*******
end main.cpp
now pathing_functions.cpp
*/


void pathingInit(DRules rules, DInit init, DSpecs specs,
	vector<vector<Block> > &dungeon)
{
	struct timespec rtime;
	initStartBlocks(rules, init, specs, dungeon, rtime);
	initPath(rules, init, specs, dungeon);
}

void initStartBlocks(DRules rules, DInit init, DSpecs specs,
	vector<vector<Block> >& dungeon, struct timespec rtime)
{
	// needed for probability check
	double prob;
	// reduce writelength
	Block start;
	start.maintype = 3;
	start.subtype = 999;
	start.path = 1;
	// Set internal constructor to row/col number
	start.blockc.row = init.startrow;
	start.blockc.col = init.startcol;
	// Set initial block direction
	int bpos[2];
	int next_to_wall;
	bpos[0] = init.startrow;
	bpos[1] = init.startcol;

	//
	start.blockc.direction = DrandomDirection();

	dungeon[init.startrow][init.startcol] = start;
	dungeon[init.startrow][init.startcol].blockc = start.blockc;
	//dungeon[init.endrow][init.endcol] = end;
	printf("Pointing %d\n", start.blockc.direction);
	printf("Starting at %d %d\n", start.blockc.row, start.blockc.col);
	//buildBlock(1, dungeon[init.startrow][init.startcol],
	//		dungeon, start.blockc.direction);
}

/*
 *
 *
 *	Block building functions. One returns, one is void
 *
 *
 */

 Block returnBuildBlock(int maintype, int &s_row, int &s_col,
 	vector<vector<Block> > &dungeon, int direction)
 {
 	switch(direction) {
 		case 0:
			//printf("1) s_row = %d s_col = %d\n",s_row, s_col);
 		dungeon[s_row-1][s_col].maintype = maintype;
 		dungeon[s_row-1][s_col].blockc.direction = direction;
 		dungeon[s_row-1][s_col].blockc.row = s_row-1;
 		dungeon[s_row-1][s_col].blockc.col = s_col;
 		dungeon[s_row-1][s_col].path = 1;
 		dungeon[s_row-1][s_col].south = &dungeon[s_row][s_col];
 		dungeon[s_row][s_col].north = &dungeon[s_row-1][s_col];
 		return dungeon[s_row-1][s_col];
			//break;
 		case 1:
			//printf("1) s_row = %d s_col = %d\n",s_row, s_col);
 		dungeon[s_row][s_col+1].maintype = maintype;
 		dungeon[s_row][s_col+1].blockc.direction = direction;
 		dungeon[s_row][s_col+1].blockc.row = s_row;
 		dungeon[s_row][s_col+1].blockc.col = s_col+1;
 		dungeon[s_row][s_col+1].path = 1;
 		dungeon[s_row][s_col+1].west = &dungeon[s_row][s_col];
 		dungeon[s_row][s_col].east = &dungeon[s_row][s_col+1];
 		return dungeon[s_row][s_col+1];
			//break;
 		case 2:
			//printf("1) s_row = %d s_col = %d\n",s_row, s_col);
 		dungeon[s_row + 1][s_col].maintype = maintype;
 		dungeon[s_row + 1][s_col].blockc.direction = direction;
 		dungeon[s_row + 1][s_col].blockc.row = s_row + 1;
 		dungeon[s_row + 1][s_col].blockc.col = s_col;
 		dungeon[s_row + 1][s_col].path = 1;
 		dungeon[s_row + 1][s_col].north = &dungeon[s_row][s_col];
 		dungeon[s_row][s_col].south = &dungeon[s_row + 1][s_col];
 		return dungeon[s_row + 1][s_col];
			//break;
 		case 3:
			//printf("1) s_row = %d s_col = %d\n",s_row, s_col);
 		dungeon[s_row][s_col-1].maintype = maintype;
 		dungeon[s_row][s_col-1].blockc.direction = direction;
 		dungeon[s_row][s_col-1].blockc.row = s_row;
 		dungeon[s_row][s_col-1].blockc.col = s_col-1;
 		dungeon[s_row][s_col-1].path = 1;
 		dungeon[s_row][s_col-1].east = &dungeon[s_row][s_col];
 		dungeon[s_row][s_col].west = &dungeon[s_row][s_col-1];
 		return dungeon[s_row][s_col-1];
			//break;
 	}

 }

 void buildBlock(int maintype, Block &src, 
 	vector<vector<Block> > &dungeon, int direction)
 {
 	int row = src.blockc.row;
 	int col = src.blockc.col;
 	switch(direction) {
 		case 0:
 		dungeon[row - 1][col].maintype = maintype;
 		dungeon[row - 1][col].path = 1;
 		dungeon[row - 1][col].south = &src;
 		dungeon[row - 1][col].blockc.direction = direction;
 		src.north = &dungeon[row - 1][col];
 		break;
 		case 1:
 		dungeon[row][col + 1].maintype = maintype;
 		dungeon[row][col + 1].path = 1;
 		dungeon[row][col + 1].west = &src;
 		dungeon[row][col + 1].blockc.direction = direction;
 		src.east = &dungeon[row][col + 1];
 		break;
 		case 2:
 		dungeon[row + 1][col].maintype = maintype;
 		dungeon[row + 1][col].path = 1;
 		dungeon[row + 1][col].north = &src;
 		dungeon[row + 1][col].blockc.direction = direction;
 		src.south = &dungeon[row + 1][col];
 		break;
 		case 3:
 		dungeon[row][col - 1].maintype = maintype;
 		dungeon[row][col - 1].path = 1;
 		dungeon[row][col - 1].east = &src;
 		dungeon[row][col - 1].blockc.direction = direction;
 		src.west = &dungeon[row][col - 1];
 		break;
 	}
 }

/*
 *
 *	End of block building functions
 *
 *
 */


 void initPath(DRules rules, DInit init, DSpecs specs,
 	vector<vector<Block> > &dungeon)
 {
 	DCounter event_counters[99];
	//DBuildMonitor monitor;
 	Block test = dungeon[init.startrow][init.startcol];

 	Block event = DbuildHall(rules, specs, test.blockc.direction,
 		test.blockc.row, test.blockc.col, dungeon, event_counters[2]);

 	DFork turn = getNewFork(event);
 	for (int i = 0; i < (specs.rows + specs.cols)*sqrt(specs.cols*2); i++) {
 		test.blockc.row = turn.forkpos[0];
 		test.blockc.col = turn.forkpos[1];
 		test.blockc.direction = DrandomDirection(test.blockc.direction);
 		event = DbuildHall(rules, specs, test.blockc.direction, test.blockc.row,
 			test.blockc.col, dungeon, event_counters[2]);
 		turn = getNewFork(event);
 	}
 	int tolerance = 6;

 	Block block;
 	vector<Block> dungeonCols(specs.cols, block);
 	vector<vector<Block> > newdungeon(specs.rows, dungeonCols);

 	for (int i = 0; i < 100; i++) {
 		newdungeon = newParsedMap(specs, tolerance, dungeon);
 		dungeon = newdungeon;
 	}
 	newdungeon = newTexturedMap(specs, dungeon);
 	dungeon = newdungeon;
 }

 void buildPath(DRules rules, DSpecs specs,
 	DCounter *&event_counters, vector<vector<Block> > &dungeon)
 {
	//getEvent(event_counters[], 
 }

 void startEvent(int event, DRules rules, DSpecs specs,
 	vector<vector<Block> > &dungeon, DCounter *&event_counters)
 {

 }

 int getEvent(DRules rules, DInit init, DSpecs specs, Block current)
 {
 	int row = current.blockc.row;
 	int col = current.blockc.col;

 }

 int DrandomDirection(int prevdirection)
 {
 	struct timespec rtime;
 	int direction;
 	while(true) {
 		srandByTime(rtime);
 		double probability = (rand()%100);
 		if (probability < 25) {
 			direction = 0;
 		} else if (probability < 50) {
 			direction = 1;
 		} else if (probability < 75) {
 			direction = 2;
 		} else {
 			direction = 3;
 		}
 		if (direction == prevdirection) continue;
 		break;
 	}
 	return direction;
 }

 int DrandomDirection()
 {
 	struct timespec rtime;
 	srandByTime(rtime);
 	double probability = (rand()%100);
 	if (probability < 25) {
 		return 0;
 	} else if (probability < 50) {
 		return 1;
 	} else if (probability < 75) {
 		return 2;
 	} else {
 		return 3;
 	}
 }

 int DnewDirection(DSpecs specs, DRules rules, Block &block)
 {
 	int skip_direction = (block.blockc.direction + 2) % 4;
 	int used_directions[4];
 	used_directions[skip_direction] = 1;
 	int directions[3];
 	int i = 0;
 	for (int j = 0; j < 4; j++) {
 		if (used_directions[j] == 1) continue;
 		used_directions[j] = 1;
 		directions[i] = j;
 		i++;
 		break;

 	}
 	struct timespec rtime;
 	srandByTime(rtime);
 	double probability = rand()%RAND_MAX;
 	if (probability < 33) { return directions[0]; }
 	else if (probability < 66) { return directions[1]; }
 	else { return directions[2]; }
 }

/*
 *
 *	Utility functions for block building
 */

 DFork getNewFork(Block block)
 {
 	DFork fork;
 	fork.init();
 	switch(block.blockc.direction) {
 		case 0:
 		fork.attempted_turns[2] = 1;
 		break;
 		case 1:
 		fork.attempted_turns[3] = 1;
 		break;
 		case 2:
 		fork.attempted_turns[0] = 1;
 		break;
 		case 3:
 		fork.attempted_turns[1] = 1;
 		break;
 	}
 	fork.forkpos[0] = block.blockc.row;
 	fork.forkpos[1] = block.blockc.col;

 	return fork;
 }

 void setDisabledTurn(DFork &fork, Block block)
 {
 	switch(block.blockc.direction) {
 		case 0:
 		fork.attempted_turns[2] = 1;
 		break;
 		case 1:
 		fork.attempted_turns[3] = 1;
 		break;
 		case 2:
 		fork.attempted_turns[0] = 1;
 		break;
 		case 3:
 		fork.attempted_turns[1] = 1;
 		break;
 	}
 	fork.forkpos[0] = block.blockc.row;
 	fork.forkpos[1] = block.blockc.col;
 }

 void setDisabledTurn(DFork &fork, Block block, int direction)
 {
 	switch(direction) {
 		case 0:
 		fork.attempted_turns[0] = 1;
 		break;
 		case 1:
 		fork.attempted_turns[1] = 1;
 		break;
 		case 2:
 		fork.attempted_turns[2] = 1;
 		break;
 		case 3:
 		fork.attempted_turns[3] = 1;
 		break;
 	}
 	fork.forkpos[0] = block.blockc.row;
 	fork.forkpos[1] = block.blockc.col;
 }

 bool checkAttemptedTurn(DFork fork, int direction)
 {
 	switch(direction) {
 		case 0:
 		if (fork.attempted_turns[2] == 1) return true;
 		return false;
 		case 1:
 		if (fork.attempted_turns[3] == 1) return true;
 		return false;
 		case 2:
 		if (fork.attempted_turns[0] == 1) return true;
 		return false;
 		case 3:
 		if (fork.attempted_turns[1] == 1) return true;
 		return false;
 	}
 }

 void connectForks(DFork &prev, DFork &next)
 {
 	prev.next = &next;
 	next.prev = &prev;
 }
/** Parsing the map **/

int parseSurroundingBlocks(DSpecs specs, vector<vector<Block> > &dungeon,
	Block block)
{
	int count = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			int block_x = block.blockc.col+i;
			int block_y = block.blockc.row+j;
			if (i == 0 && j == 0) { continue; }
			else if (block_x < 0 || block_y < 0 ||
					block_x > specs.cols - 1 || block_y > specs.rows - 1) {
					count = count + 1;
			} else if (dungeon[block_y][block_x].maintype == 1) {
				count = count + 1;
			}
		}
	}
	return count;
}

vector<vector<Block> > newParsedMap(DSpecs specs, int tolerance,
	vector<vector<Block> > &dungeon)
{
	Block block;
	struct timespec rtime;
	vector<Block> dungeonCols(specs.cols, block);
	vector<vector<Block> > newdungeon(specs.rows, dungeonCols);
	for (int i = 0; i < specs.rows - 1; i++) {
		for (int j = 0; j < specs.cols - 1; j++) {
			//printf("parsedmap\n");
			int nbs = parseSurroundingBlocks(specs, dungeon, dungeon[i][j]);
			if (dungeon[i][j].maintype == 1) {
				if (nbs < tolerance) {
					newdungeon[i][j].maintype = 0;
				} else {
					newdungeon[i][j].maintype = 1;
				}
			} else if (nbs > tolerance + 2) {
					newdungeon[i][j].maintype = 1;
			} else {
					newdungeon[i][j].maintype = 0;
			}
			if (nbs == tolerance) {
				srandByTime(rtime);
				int prob = rand()%100;
				if (prob < 0.5) {
					newdungeon[i][j].maintype = 1;
				} else {
					newdungeon[i][j].maintype = 0;
				}
			}	
		}
	}
	/*for (int i = 0; i < specs.rows - 1; i++) {
		for (int j = 0; j < specs.cols - 1; j++) {
			int textype = parseToBlockTextures(dungeon, i, j, specs);
			newdungeon[i][j].subtype = textype;
		}
	}*/
	return newdungeon;
}

vector<vector<Block> > newTexturedMap(DSpecs specs, vector<vector<Block> > &dungeon)
{
	Block block;
	vector<Block> dungeonCols(specs.cols, block);
	vector<vector<Block> > newdungeon(specs.rows, dungeonCols);
	for (int i = 0; i < specs.rows - 1; i++) {
		for (int j = 0; j < specs.cols - 1; j++) {
			int textype = parseToBlockTextures(dungeon, i, j, specs);
			newdungeon[i][j].subtype = textype;
		}
	}
	return newdungeon;
}

int parseToTreasureBlocks(DSpecs specs, vector<vector<Block> > &dungeon,
	Block block)
{
	int count = 0;
}

int parseToBlockTextures(vector<vector<Block> > &dungeon,
	int row, int col, DSpecs specs)
{
	int type = 0;
	if (dungeon[row][col].maintype == 1) return 16;
	else {
		if (row == 0) {
			if (col == 0) {
				if (dungeon[row+1][col].maintype == 0) type += 4;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				return type;
			} else if (col < specs.cols - 1) {
				if (dungeon[row][col-1].maintype == 0) type += 8;
				if (dungeon[row+1][col].maintype == 0) type += 4;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				return type;
			} else {
				if (dungeon[row][col-1].maintype == 0) type += 8;
				if (dungeon[row+1][col].maintype == 0) type += 4;
				return type;
			}
			return type;
		} else if (row < specs.rows - 1) {
			if (col == 0) {
				if (dungeon[row-1][col].maintype == 0) type += 1;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				if (dungeon[row+1][col].maintype == 0) type += 4;
				return type;
			} else if (col < specs.cols - 1) {
				if (dungeon[row-1][col].maintype == 0) type += 1;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				if (dungeon[row+1][col].maintype == 0) type += 4;
				if (dungeon[row][col-1].maintype == 0) type += 8;
				return type;
			} else {
				if (dungeon[row-1][col].maintype == 0) type += 1;
				if (dungeon[row+1][col].maintype == 0) type += 4;
				if (dungeon[row][col-1].maintype == 0) type += 8;
				return type;
			}
			return type;
		} else {
			if (col == 0) {
				if (dungeon[row-1][col].maintype == 0) type += 1;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				return type;
			} else if (col < specs.cols - 1) {
				if (dungeon[row][col-1].maintype == 0) type += 8;
				if (dungeon[row-1][col].maintype == 0) type +=1;
				if (dungeon[row][col+1].maintype == 0) type += 2;
				return type;
			} else {
				if (dungeon[row][col-1].maintype == 0) type += 8;
				if (dungeon[row-1][col].maintype == 0) type += 1;
				return type;
			}
			return type;
		}
	}
	return type;
}

//
//
//
//
#endif
