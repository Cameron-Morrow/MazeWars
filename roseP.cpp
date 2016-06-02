#ifndef ROSEP_CPP
#define ROSEP_CPP

// Name: Rose Phannavong
// Written: 27 April 2016
// Modified: 1 June 2016
// Description: The code will be implementing sound/music for the game.
//              Press key R will display bubbles around the player.
//              Rendering a maze that is 24 by 24, and doing maze
//              collision for it. Convert PPMs and PNGs. 

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <GL/glx.h>
#include <stdlib.h>
#include </usr/include/AL/alut.h>
#include <cmath>
#include "game_objects.h"
#include "roseP.h"

ALuint alSource[20];
ALuint alBuffer[20];

static float Volume = 1.0f;
void getVolume(float V)
{
		Volume = V;
}

void init_sounds()
{
    //Check and clear.
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: alutInit()\n");
    }
    alGetError();

    //Setup the listener, used forward and up vectors.
    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

}

void release_sounds()
{
    //First we must delete the source.
    alDeleteSources(1, &alSource[0]);
    alDeleteSources(1, &alSource[1]);
    alDeleteSources(1, &alSource[2]);

    //Second we must delete the buffer.
    alDeleteBuffers(1, &alBuffer[0]);
    alDeleteBuffers(1, &alBuffer[1]);
    alDeleteBuffers(1, &alBuffer[2]);

    //Get active context and device for active context.
    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);

    //Disable and release context.
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);

    //Close device.
    alcCloseDevice(Device);
}

void load_sounds()
{
    //Bullet sound.
    alBuffer[0] = alutCreateBufferFromFile("./sound/bang.wav");
    alGenSources(1, &alSource[0]);
    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);

    //Death sound.
    alBuffer[1] = alutCreateBufferFromFile("./sound/male_death.wav");
    alGenSources(1, &alSource[1]);
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);

    //Walk sound.
    alBuffer[2] = alutCreateBufferFromFile("./sound/walk.wav");
    alGenSources(1, &alSource[2]);
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    
    //Death sound.
    alBuffer[3] = alutCreateBufferFromFile("./sound/3107.wav");
    alGenSources(1, &alSource[3]);
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);

    //Background Pause music.
    alBuffer[4] = alutCreateBufferFromFile("./sound/bensound-epic.wav");
    alGenSources(1, &alSource[4]);
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);

    //Falling boulders sound.
    alBuffer[5] = alutCreateBufferFromFile("./sound/Rock_Slide-000.wav");
    alGenSources(1, &alSource[5]);
    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);
    
    //Background music.
    alBuffer[6] = alutCreateBufferFromFile("./sound/parabolix_cavern.wav");
    alGenSources(1, &alSource[6]);
    alSourcei(alSource[6], AL_BUFFER, alBuffer[6]);
    
    //Cameron's Piano.
    alBuffer[11] = alutCreateBufferFromFile("./sound/c.wav");
    alGenSources(1, &alSource[11]);
    alSourcei(alSource[11], AL_BUFFER, alBuffer[11]);
    alBuffer[12] = alutCreateBufferFromFile("./sound/d.wav");
    alGenSources(1, &alSource[12]);
    alSourcei(alSource[12], AL_BUFFER, alBuffer[12]);
    alBuffer[13] = alutCreateBufferFromFile("./sound/e.wav");
    alGenSources(1, &alSource[13]);
    alSourcei(alSource[13], AL_BUFFER, alBuffer[13]);
    alBuffer[14] = alutCreateBufferFromFile("./sound/f.wav");
    alGenSources(1, &alSource[14]);
    alSourcei(alSource[14], AL_BUFFER, alBuffer[14]);
    alBuffer[15] = alutCreateBufferFromFile("./sound/g.wav");
    alGenSources(1, &alSource[15]);
    alSourcei(alSource[15], AL_BUFFER, alBuffer[15]);
    alBuffer[16] = alutCreateBufferFromFile("./sound/a.wav");
    alGenSources(1, &alSource[16]);
    alSourcei(alSource[16], AL_BUFFER, alBuffer[16]);
    alBuffer[17] = alutCreateBufferFromFile("./sound/b.wav");
    alGenSources(1, &alSource[17]);
    alSourcei(alSource[17], AL_BUFFER, alBuffer[17]);
    alBuffer[18] = alutCreateBufferFromFile("./sound/c2.wav");
    alGenSources(1, &alSource[18]);
    alSourcei(alSource[18], AL_BUFFER, alBuffer[18]);
    alBuffer[19] = alutCreateBufferFromFile("./sound/credits.wav");
    alGenSources(1, &alSource[19]);
    alSourcei(alSource[19], AL_BUFFER, alBuffer[19]);
}

void play_sounds(int soundOption)
{
    //Set volume and pitch.
    alSourcef(alSource[soundOption], AL_GAIN, Volume);
    alSourcef(alSource[soundOption], AL_PITCH, 1.0f);
    alSourcei(alSource[soundOption], AL_LOOPING, AL_FALSE);

    //No looping for sound.
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: Setting Source\n");
    }
    alSourcePlay(alSource[soundOption]);
}

void play_sounds(int soundOption, int loop)
{
    //Set volume and pitch.
    alSourcef(alSource[soundOption], AL_GAIN, Volume);
    alSourcef(alSource[soundOption], AL_PITCH, 1.0f);
    alSourcei(alSource[soundOption], AL_LOOPING, AL_TRUE);

    //No looping for sound.
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: Setting Source\n");
    }
    alSourcePlay(alSource[soundOption]);
}

void bubblez(int radius, int x, int y, float red, float green, float blue)
{
    //Get bubbles to appear.
    static int num = 0;
    std::cout << "circle num: " << num++ << std::endl;
    glColor3f(red, green, blue);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POINTS);

    //Randomly get a quadrant.
    int quadrant = rand() % 4 + 1;
    switch (quadrant) {
        case(1):
            x += rand() % 150;
            y += rand() % 150;
            break;
        case(2):
            x -= rand() % 150;
            y += rand() % 150;
            break;
        case(3):
            x -= rand() % 150;
            y -= rand() % 150;
            break;
        case(4):
            x += rand() % 150;
            y -= rand() % 150;
            break;
    }
    for (float i = 0.0; i < 360.0; i += 1.0) {
         glVertex2f(radius * cos(i) + x, radius * sin(i) + y);
    }
 
    glEnd();
    //glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
}

void pressR(Game *g)
{
    //Special keystroke for bubblez.
    std::cout << "R pressed" << std::endl;
    srand(NULL);
    int x = g->Player_1.stats.spos[0];
    int y = g->Player_1.stats.spos[1];

    int numBubbles = 10;
    for (int i = 0; i < numBubbles; i++) {
	 int radius = rand() % 20 + 10;
	 float red = rand() * 10;
	 float green = rand() * 15;
	 float blue = rand() * 20;
         bubblez(radius, x, y, red, green, blue);
    }
}

void render_maze(Game *g, GLuint mazeTexture, Ppmimage *mazeImage) 
{
    //Rendering maze[24][24].
    int w = mazeImage->width*4;
    int h = mazeImage->height*4;

    Player player = g->Player_1;
    float xdist, ydist;
    xdist = -1000 - player.stats.gpos[0];// + w);
    ydist = 280 - player.stats.gpos[1]; // h);
    //std::cout << "xdist: " << xdist << std::endl;
    //std::cout << "ydist: " << ydist << std::endl;
    
    glBindTexture(GL_TEXTURE_2D, mazeTexture);
    glPushMatrix();
    glTranslatef(xdist, ydist, 0);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, h);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(w, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);

    glEnd();
    glPopMatrix();
}

int maze[24][24] = {
                {1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
                {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                {1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1},
                {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
                {1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
                {1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
                {1,0,1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,1,0,1,1,1,0,1},
                {1,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0,1,1,0,1,0,0,0,1},
                {1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1},
                {1,0,1,0,0,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,0,0,1},
                {1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1},
                {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,1},
                {1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1},
                {1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,1},
                {1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1},
                {1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,0,1},
                {1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1},
                {1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1},
                {1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1},
                {1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,0,1},
                {1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1},
                {1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1},
                {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1}};

void maze_collision(Game *g)
{
    //Maze collision for the maze[24][24].

    //for (int row = 0; row < 24; row++) {
    //    for (int col = 0; col < 24; col++) {
    //        if (maze[row][col] == 1) {
    //std::cout << g->Player_1.sats.gpos[1] << std::endl;
    int wallY = fabs(floor((280 - g->Player_1.stats.gpos[1]-80)/128 - 2));
    int wallX = fabs(floor(-1000 - g->Player_1.stats.gpos[0]-100)/128 - 4);
    int x = g->Player_1.stats.gpos[0]+26;
    int y = g->Player_1.stats.gpos[1]+164;
    //std::cout << x << std::endl;
    //std::cout << y << std::endl;

    //std::cout << (int)((float)x/128.0)+12 << std::endl;
    //std::cout << (int)(((float)y/128.0) - .4) << std::endl;
    int cX = ((float)x/128.0)+12;
    int cY = ((float)y/128.0 - .4);
    //x is from 520 down to -something
    if (cY < 24 && cY < 24 && maze[cX][cY] == 0) {
        //std::cout << "y: " << y << std::endl;        
    }

}

//Convert the PPMs and PNGs.
void convertPNGtoPPM() {
    std::cout << "converting" << std::endl;
    system("mogrify -format ppm images/*.png");
    system("mogrify -format ppm parallax/*.png");
}

void convertPPMtoPNG() {
    system("mogrify -format png images/*.png");
    system("mogrify -format png parallax/*.ppm");
    removePPMImages();
}

void removePPMImages() {
    system("rm images/*.ppm");
    system("rm parallax/*.ppm");
}

#endif
