#include "planets.h"
#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

unsigned int planet_tex[NUM_PLANET_TEXS];
char *planet_tex_names[] = { (char*)"./texs/tex_sun.bmp",
							 (char*)"./texs/tex_earth.bmp",
							 (char*)"./texs/tex_moon.bmp",
							 (char*)"./texs/tex_jupiter.bmp"};

double *corona_lens;
bool corona_first_pass = true;
							
static point3 calc_vertex(int th, int ph)
{
	point3 result;
	result.x = KUTILS_SIN(th)*KUTILS_COS(ph);
	result.y = KUTILS_COS(th)*KUTILS_COS(ph);
	result.z = KUTILS_SIN(ph);
	return result;
}

/* Draw vertex in polar coordinates (taken from example 17) */
static void Vertex(int th,int ph, bool reverse_normal)
{
	point3 p = calc_vertex(th,ph);
	if(reverse_normal)
		glNormal3d(-p.x,-p.y,-p.z);
	else
		glNormal3d(p.x,p.y,p.z);
	glTexCoord2d(th/360.0,ph/180.0+0.5);
	glVertex3d(p.x,p.y,p.z);
}

/* Draw planet (modified from example 17) */
void draw_planet(int tex, bool reverse_normal)
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
         Vertex(th,ph,reverse_normal);
         Vertex(th,ph+5,reverse_normal);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
}							 

void draw_planet(int tex)
{
	draw_planet(tex,false);
}
							 
void Draw_Sun(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	
	draw_planet(SUN_TEX, true);
	
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

void Draw_Jupiter(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	glRotated(-90,1,0,0); // the texture needs to be rotated 90 degrees
	
	draw_planet(JUPITER_TEX);
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_Comet(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	
	draw_planet(SUN_TEX); // sun tex will be fine for this
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_CometTail(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	
	
	// reset the old transformation
	glPopMatrix();
}

void Draw_Corona(point3 translation, point4 rotation, point3 scale)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	glRotated(-90,1,0,0); // the texture needs to be rotated 90 degrees
	
	int th,ph;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);

	//  Latitude bands
	for (ph=-90;ph<90;ph+=5)
	{
		double rad = KUTILS_COS(ph);
		double circum = 2*PI*rad;
		double arc = (circum > CORONA_SPACING) ? CORONA_SPACING : circum;
		double angle = 360*arc / circum;
		for (th=0;th<=360+angle;th+=angle)
		{
			point3 p1 = calc_vertex(th,  ph);
			point3 p2 = calc_vertex(th+5,ph);
			point3 p3 = calc_vertex(th+5,ph+5);
			point3 p4 = calc_vertex(th,  ph+5);
			
			double len = CORONA_LENGTH;
			
			glBegin(GL_QUAD_STRIP);
			glColor4f(1,1,0,CORONA_BASE_ALPHA); glVertex3d(    p1.x,     p1.y,     p1.z);
			glColor4f(1,1,0,0);                 glVertex3d(len*p1.x, len*p1.y, len*p1.z);
			glColor4f(1,1,0,CORONA_BASE_ALPHA); glVertex3d(    p2.x,     p2.y,     p2.z);
			glColor4f(1,1,0,0);                 glVertex3d(len*p2.x, len*p2.y, len*p2.z);
			glColor4f(1,1,0,CORONA_BASE_ALPHA); glVertex3d(    p3.x,     p3.y,     p3.z);
			glColor4f(1,1,0,0);                 glVertex3d(len*p3.x, len*p3.y, len*p3.z);
			glColor4f(1,1,0,CORONA_BASE_ALPHA); glVertex3d(    p4.x,     p4.y,     p4.z);
			glColor4f(1,1,0,0);                 glVertex3d(len*p4.x, len*p4.y, len*p4.z);
			glColor4f(1,1,0,CORONA_BASE_ALPHA); glVertex3d(    p1.x,     p1.y,     p1.z);
			glColor4f(1,1,0,0);                 glVertex3d(len*p1.x, len*p1.y, len*p1.z);
			glEnd();
		}
	}
	
	glDepthMask(1);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	// reset the old transformation
	glPopMatrix();
}
