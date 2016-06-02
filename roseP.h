#ifndef ROSEP_H
#define ROSEP_H
#include </usr/include/AL/alut.h>
// Name: Rose Phannavong

void init_sounds();
void release_sounds();
void load_sounds();
void play_sounds(int soundOption);
void play_sounds(int soundOption, int loop);
void bubblez(int radius, float red, float green, float blue);
void pressR(Game *g);
void render_maze(Game *g, GLuint mazeTexture, Ppmimage *mazeImage);
void maze_collision(Game *g);
void getVolume(float V);
void convertPNGtoPPM();
void convertPPMtoPNG();
void removePPMImages();

#endif
