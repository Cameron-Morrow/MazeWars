#include <ctime>
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];
typedef int Explosive;
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
struct Weapon{
    Vec dir;
    Vec pos;
    Vec vel;
    float angle;
    Weapon(){
	VecZero(dir);
        pos[0] = (Flt)(40);
        pos[1] = (Flt)(40);
        pos[2] = 0.0f;
        VecZero(vel);
        angle = 0.0;
    }
};

struct Bullet {
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
	void operator=(Bullet x) {
		pos[0] = x.pos[0];
		pos[1] = x.pos[1];
		vel[0] = x.vel[1];
		vel[1] = x.vel[1];
		color[0] = x.color[0];
		color[1] = x.color[1];
		color[2] = x.color[2];
		time = x.time;
	}
};
