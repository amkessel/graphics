#include "planets.h"
#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

unsigned int planet_tex[NUM_PLANET_TEXS];
char *planet_tex_names[] = { (char*)"./texs/tex_sun.bmp",
							 (char*)"./texs/tex_earth.bmp",
							 (char*)"./texs/tex_moon.bmp"};
							 

/* Draw vertex in polar coordinates (taken from example 17) */
static void Vertex(int th,int ph)
{
	double x = KUTILS_SIN(th)*KUTILS_COS(ph);
	double y = KUTILS_COS(th)*KUTILS_COS(ph);
	double z = KUTILS_SIN(ph);
	glNormal3d(x,y,z);
	glTexCoord2d(th/360.0,ph/180.0+0.5);
	glVertex3d(x,y,z);
}

/* Draw planet (modified from example 17) */
void draw_planet(int tex)
{
   int th,ph;

   /*
    *  Draw surface of the planet
    */
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, planet_tex[tex]);
   //  Latitude bands
   glColor3f(1,1,1);
   for (ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+5);
      }
      glEnd();
   }
}							 
							 
void Draw_Sun(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	
	draw_planet(SUN_TEX);
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_Earth(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	glRotated(-90,1,0,0); // the texture needs to be rotated 90 degrees
	
	draw_planet(EARTH_TEX);
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_Moon(point3 earth_trans, point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	glTranslated(earth_trans.x, 0, earth_trans.z);
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	glRotated(-90,1,0,0); // the texture needs to be rotated 90 degrees
	
	draw_planet(MOON_TEX);
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_Comet(point3 translation, point4 rotation, point3 scale)
{
}
