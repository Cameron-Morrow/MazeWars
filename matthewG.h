#include "game_objects.h"
#include "joystick.hh"
#include "person.h"
#include <string>
//author: Matthew Gaitan
void getJoystickEvent(JoystickEvent event, int joy[], int axis[]);
void movement(Game *g);
void checkController(int axis[], Game *g, int &controller);
void checkKeys(int keys[], Game *g);
void renderCrosshair(int axis[], Game *g, bool keyboard);
void renderShield(Game *g);
Ppmimage* characterSelection(std::string characterSelected);
void mousePosition(int x, int y);
void renderCharacter(Person person, Game *g, float w, int keys[], int axis[], GLuint personTexture1); 
void fps();
int renderTitleScreen(GLuint introTextures[], Ppmimage *introImages[], 
	int &enterPressed, int &downPressed, int &upPressed, int keys[], 
	int axis[], int &aButton);
void resolution(int width, int height);
void graveKeyPress(int keys[]);