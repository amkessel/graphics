#include "planets.h"
#include "kutils.h"
#include "kdraw.h"
#include "hw3_objects.h"

using namespace kutils;
using namespace kdraw;

point3 no_translation = {0,0,0};
point4 no_rotation = {0,0,0,0};
point3 no_scale = {1,1,1};
	
GLfloat sunEmissiveMaterial[] = {1, 1, 0.5};
GLfloat blckEmissiveMaterial[] = {0.0, 0.0, 0.0};

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
static void Vertex(int th,int ph, bool reverse_normal, bool tex)
{
	point3 p = calc_vertex(th,ph);
	if(reverse_normal)
		glNormal3d(-p.x,-p.y,-p.z);
	else
		glNormal3d(p.x,p.y,p.z);
	if(tex)
		glTexCoord2d(th/360.0,ph/180.0+0.5);
	glVertex3d(p.x,p.y,p.z);
}

/* Draw planet (modified from example 17) */
void draw_planet(int tex, bool reverse_normal, float thmin, float thmax, float r, float g, float b, float a)
{
	int th,ph;
	
	bool use_texture = (tex >= 0);

	/*
	*  Draw surface of the planet
	*/
	//  Set texture
	if(use_texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, planet_tex[tex]);
	}
	//  Latitude bands
	glColor4f(r,g,b,a);
	for (ph=-90;ph<90;ph+=5)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=thmin;th<=thmax;th+=5)
		{
			Vertex(th,ph,reverse_normal,use_texture);
			Vertex(th,ph+5,reverse_normal,use_texture);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void draw_planet(int tex, bool reverse_normal)
{
	draw_planet(tex, reverse_normal, 0,360, 1,1,1,1);
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
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunEmissiveMaterial);
	draw_planet(SUN_TEX, true);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blckEmissiveMaterial);
	
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

void Draw_Comet(point3 translation, point4 rotation, point3 scale, double tail_len, double coma_alpha)
{
	// save the current transformation
	glPushMatrix();
	
	// perform our own transformation
	Transform(translation, rotation, scale);
	
	// make sure lighting and all is set correctly
	glDepthMask(1);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glColor3f(0.5,0.5,0.5);
	Draw_Cow(no_translation, no_rotation, no_scale);
	
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);
	
	// draw the coma and tail
	
	double r = 1;
	double g = 1;
	double b = 0.7;
	double coma_rad = 1.3;
	double tip_size = 0.5;
	
	glPushMatrix();
	glScaled(coma_rad,coma_rad,coma_rad);
	draw_planet(-1,false, 0, 180,r,g,b,coma_alpha); // no texture for the coma
	glPopMatrix();
	
	int n = 360/5; // match with # segments in planetary sphere 
	point2 pts[n];
	ComputeCirclePoints(coma_rad,pts,n);
	
	for(int i = 1; i < n; i++)
	{
		glBegin(GL_POLYGON);
		glColor4d(r,g,b,0);
		glVertex3d(-tail_len, tip_size*pts[i-1].x, tip_size*pts[i-1].y);
		glVertex3d(-tail_len, tip_size*pts[i].x,   tip_size*pts[i].y  );
		glColor4d(r,g,b,coma_alpha);
		glVertex3d(0.0, pts[i].x,   pts[i].y  );
		glVertex3d(0.0, pts[i-1].x, pts[i-1].y);
		glEnd();
	}
	
	glDepthMask(1);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	
	// reset the old transformation
	glPopMatrix();
}

void Calculate_Comet_Params(double x, double z, double orbit_rad, double *coma_alpha, double *tail_len, double *rot_angle)
{
	double distSq = x*x + z*z;
	double orbitSq = orbit_rad * orbit_rad;
	
	*tail_len = COMET_TAIL_LEN * orbitSq / distSq; // tail gets longer as comet gets closer to sun
	*coma_alpha = 2 / distSq; // alpha gets higher as comet gets closer to sun
	
	// find the angle, clock-wise from x-axis
	int quadrant;
	if(x >  0 && z >= 0) quadrant = 1;
	if(x >= 0 && z <  0) quadrant = 2;
	if(x <  0 && z <= 0) quadrant = 3;
	if(x <= 0 && z >  0) quadrant = 4;
	
	Vector3 comet_vec (x,0,z);
	Vector3 axis_vec (1,0,0);
	
	double angle = 180+comet_vec.Angle(&axis_vec,true,false);
	
	if     (quadrant == 1 || quadrant == 4) *rot_angle = angle;
	else if(quadrant == 2 || quadrant == 3) *rot_angle = 360 - angle;
	
	*rot_angle *= -1;
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunEmissiveMaterial);

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
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blckEmissiveMaterial);

	// reset the old transformation
	glPopMatrix();
}
