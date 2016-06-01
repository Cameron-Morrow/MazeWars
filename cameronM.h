#ifndef CAMERON_H
#define CAMERON_H

#include <cmath>
#include "joystick.hh"
#include "davidV.h"
#include "fonts/fonts.h"
#include "mtime.h"

void getScreenRes(int x, int y);
void pointPlayer(Game *g, int savex, int savey);
void drawHealth(Player *x);
void drawAmmo(Player x);
void drawDiamondBack(int x);
void drawDiamond(int x);
void drawArtifacts(Player x);
void drawLives(int x, int y);
void drawBackground();
void drawHUD(Player *x);
void monsterMovement(Game *g, int monNum, int startx, int starty);
void monsterGetShot(Game *g, int monNum, int startx, int starty);
void monsterDamagePlayer(Game *g, int monNum, int startx, int starty);
void GameOver();
void Restart(Game *x);
void drawHealthPack(int x, int y, int z, Game *g);
void shadowBox();
void monster(Game *g, int monNum, int startx, int starty);
void monsterMovement(Game *g, int monNum, int startx, int starty);
void monsterGetShot(Game *g, int monNum, int startx, int starty);
void monsterDamagePlayer(Game *g, int monNum, int startx, int starty);
void renderCharacterEnemy(Person personc, Game *g, float w, int keys[] ,GLuint personTexture1c, int i);
void renderPauseBackground();
void renderPauseButtons(int x);
int PAUSE(Game *g, int keys[]);
void specialKey();
void endCredits(Game *g, int keys[]);
void loadEndCreditsTextures();
void loadLivesSprite();
struct HealthPack{
    Vec pos;
    int size;
};

#endif
