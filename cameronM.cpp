/* Name: Cameron Morrow
 * Date: May 04, 2016
 * Last Modified: May 04, 2016
 * Description: My code handles all of the weapon movement if a controller 
 * is not being used.
 * In the mazewars.cpp I modified all of the original asteroids code to remove
 * the engine from the ship and the asteroid field, also changed the window title
 * 
 
 void pointPlayer(Game *g)
{
	//Make the player point at the cursor
	g->gun.pos[0] = g->Player_1.pos[0];
	g->gun.pos[1] = g->Player_1.pos[1];
	float weaponx = g->gun.pos[0];
	float weapony = g->gun.pos[1];

	float nDeg = atan(((yres-savey)-(weapony))/((savex)-(weaponx))) * 180 / PI;

    if (savex > weaponx && (yres - savey) > weapony)
		nDeg += 180;
	if (savex > weaponx && (yres - savey) < weapony)
		nDeg -= 180;
	if (g->gun.angle >= 360.f)
		g->gun.angle -= 360.0f;
	if (g->gun.angle <= 360.0f){
 		if (nDeg > 270)
			nDeg -= 360;
		g->gun.angle = nDeg + 90;
	}
	if (g->gun.angle < 0.0f)
		g->gun.angle += 360.0f;
}
void MouseCrosshairs()
{
	if(keyboard == true){	
  		float ndeg2 = atan(((900 - savey) - (g->Player_1.pos[1]))/((savex)-(g->Player_1.pos[0]))) * 180/PI;
		if(savex < g->Player_1.pos[0] && (900 - savey) > g->Player_1.pos[1]){
			ndeg2 += 180;
		}
		if(savex < g->Player_1.pos[0] && (900 - savey) < g->Player_1.pos[1]){
			ndeg2 += 180;	    
		}
		crosshair.angle = ndeg2;
	}
}
*/
using namespace std;
void drawHealth(Player x)
{
    //Background of health bar
	glColor3ub(255, 0, 0);
	glPushMatrix();
	glTranslatef(1100, 50, 0);
	glBegin(GL_QUADS);
		glVertex2i(-100, -15);
		glVertex2i(-100, 15);
		glVertex2i(100, 15);
		glVertex2i(100, -15);
	glEnd();
	glPopMatrix();

	//remaining health
	int remaining = 100 - x.Current_Health;
	glColor3ub(0, 255, 0);
        glPushMatrix();
        glTranslatef(1100-remaining, 50, 0);
        glBegin(GL_QUADS);
                glVertex2i(-(100 - remaining), -15);
                glVertex2i(-(100 - remaining), 15);
                glVertex2i((100 - remaining), 15);
                glVertex2i((100 - remaining), -15);
        glEnd();
        glPopMatrix();

	Rect r;
	//
	r.bot = 20;
	r.left =1100;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffffff, "Total: %i", x.Max_Health);
	ggprint8b(&r, 16, 0x00ffffff, "Remaining: %i", x.Current_Health);
}
void drawAmmo(Player x){
    //Background of ammo bar
        glColor3ub(255, 0, 0);
        glPushMatrix();
        glTranslatef(1100, 150, 0);
        glBegin(GL_QUADS);
                glVertex2i(-100, -15);
                glVertex2i(-100, 15);
                glVertex2i(100, 15);
                glVertex2i(100, -15);
        glEnd();
        glPopMatrix();

        //remaining health
        int remaining = 100 - x.Current_Ammo;
        glColor3ub(0, 0, 255);
        glPushMatrix();
        glTranslatef(1100-remaining, 150, 0);
        glBegin(GL_QUADS);
                glVertex2i(-(100 - remaining), -15);
                glVertex2i(-(100 - remaining), 15);
                glVertex2i((100 - remaining), 15);
                glVertex2i((100 - remaining), -15);
        glEnd();
        glPopMatrix();

        Rect r;
        //
        r.bot = 120;
        r.left =1100;
        r.center = 0;
        ggprint8b(&r, 16, 0x00ffffff, "Total: %i", x.Max_Ammo);
        ggprint8b(&r, 16, 0x00ffffff, "Remaining: %i", x.Current_Ammo);
}