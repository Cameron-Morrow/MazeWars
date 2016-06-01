#ifndef GAME_H
#define GAME_H

#include <cstdlib>
#include <GL/glx.h>
#include "defs.h"
#include "ppm.h"
#include <ctime>

const int MAX_BULLETS = 4;


struct Stats {
	double width, height;
	double radius;
	double speed;
	double angle;
	Vec dir;
	Vec spos;
	Vec vel;
	Vec gpos;
	Vec color;
	struct timespec animationCurrent, animationStart;
	double animationSpan;
    Ppmimage* texture;
    GLuint obj_texture;
	const char* image;
	Stats() {
        texture = new Ppmimage;
		angle = 0;
		width = 0;
		height = 0;
		radius = 0;
		speed = 0;
		VecZero(dir);
		VecZero(vel);
		VecZero(spos);
		VecZero(gpos);
		VecZero(color);
	}
	void setImage(const char* fname) {
		image = fname;
	}
	~Stats() {}
};

struct Object {
	Stats stats;
	Object() {}
	~Object() {}
};

struct Particle {
	Stats stats;
	int drawn;
	Vec vel;
	Vec pos;
	Particle() {
		drawn = 0;
	    //stats = (Stats) malloc(sizeof(Stats));
	}
	~Particle() {}
};

typedef struct game_block_def {
	double width, height;
	int rows, columns;
} gblock_info;

typedef struct game_block_type {
	Stats stats;
	int type, assigned;
	game_block_type() {
		type = 0;
		assigned = 0;
	}
	void operator=(game_block_type gb) {
		stats.gpos[0] = gb.stats.gpos[0];
		stats.color[0] = gb.stats.color[0];
		stats.width = gb.stats.width;
		stats.height = gb.stats.height;
	}
	~game_block_type() {}
} gblock;

typedef struct game_camera {
	int xprimary, yprimary;
	int xsecondary, ysecondary;
} gcamera;

#endif
