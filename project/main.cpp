/***************************************************
 * main.cpp
 *
 * Andrew Kessel
 * Created 10/6/2011
 *
 * Main file for homework 5 (lighting)
 *
 ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "kdraw.h"
#include "kutils.h"
#include "CSCIx229.h"

// model files
#include "common.h"
#include "falcon.h"
#include "sheet.h"
#include "planets.h"
#include "thrust.h"

using namespace kdraw;
using namespace kutils;

// game constants
#define SUN_HEIGHT    0.2
#define EARTH_HEIGHT  0.1
#define MOON_HEIGHT   0.02
#define FALCON_HEIGHT 0.1

#define ORBIT_RAD_EARTH 3
#define ORBIT_RAD_MOON  0.75

#define ORBIT_SPEED_EARTH 0.05
#define ORBIT_SPEED_MOON  0.5

// control constants
#define SPEED_LIMIT	2 // units per second (note that the sun's radius is 1 unit)
#define SPEED_INC	0.01 // how much to increase the speed by per key hit
#define BRAKE_INC	0.9 // percentage of speed to reduce to per key hit
#define JUMP_DIST	1 // how far to jump ahead
#define TURN_RATE	100 // the degrees per second that the falcon can turn

#define GRAV_STRENGTH 0.05 // how much effect gravity has on the velocity

// viewing constants
#define FOLLOW_DIST     0.2  // how far behind the spaceship the eye should be
#define VIEW_DIST_AHEAD 1    // how far ahead of the spaceship the focal point is
#define EYE_HEIGHT_INC  0.01
#define SKYBOX_DIM		18

// inertial braking orb constants
#define ORB_ALPHA_INC	0.01
#define ORB_ALPHA_MAX	0.25

enum turn_dir
{
	NO_TURN,
	RIGHT_TURN,
	LEFT_TURN
};
enum turn_dir falcon_turn;

int axes=0;       //  Display axes
//int th=-45;         //  Azimuth of view angle
//int ph=20;         //  Elevation of view angle
int fov=35;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=2.0;   //  Size of world
// Light values
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 50;  // Diffuse intensity (%)
int specular  = 100;  // Specular intensity (%)
int shininess =   6;  // Shininess (power of two)
float shinyvec[1] = {64};    // Shininess (value)
int zh        =  0;  // Light azimuth
float ylight  =   2;  // Elevation of light
unsigned int stars_tex;

// game state
// falcon state
//point3 falcon_pos = {-2,FALCON_HEIGHT,-2};
point3 falcon_pos = {-10,FALCON_HEIGHT,-10};
point3 falcon_vel = {0.0,0.0,0.0};
double falcon_dir = 270; // angle from the x-axis that the falcon is pointing
double eye_height = 0.05;
bool thrust_on = false;
// state of planetary system
double orbit_angle_earth = 0.0;
double rotate_angle_earth = 0.0;
double orbit_angle_moon = 0.0;
double rotate_angle_moon = 0.0;
point3 sun_pos = {0,0,0};
point3 earth_pos = {0,0,0};
point3 moon_pos = {0,0,0};
bool orbit_planets = true;
// thrust definitions
thrust_box tbox = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
// state of spacetime sheet
bool show_grid = true;
bool show_sheet = true;
double time = 0.0; // the current time
double last_time = 0.0; // the last last
double orb_alpha = 0.0; // the transparency of the inertial braking orb
bool increase_orb_alpha = false; // whether to increase or decrease the orb's alpha value
// viewing parameters
double Ex, Ey, Ez; // location of the camera
double Cx, Cy, Cz; // where the camera is looking

point3 no_translation = {0,0,0};
point4 no_rotation = {0,0,0,0};
point3 no_scale = {1,1,1};

point3 pts[SHEET_PTS][SHEET_PTS];
point3 norms[SHEET_PTS][SHEET_PTS];

long double GetTime()
{
	//  Elapsed time in seconds
	return glutGet(GLUT_ELAPSED_TIME)/1000.0;
}

static void Sky(double D, point3 trans)
{
	glPushMatrix();
	Transform(trans, no_rotation, no_scale);
/*
   //  Sides
   glBegin(GL_QUADS);
   glColor3f(1,0,0);
   glVertex3f(-D,-D,-D);
   glVertex3f(+D,-D,-D);
   glVertex3f(+D,+D,-D);
   glVertex3f(-D,+D,-D);

   glColor3f(0,0,1);
   glVertex3f(+D,-D,-D);
   glVertex3f(+D,-D,+D);
   glVertex3f(+D,+D,+D);
   glVertex3f(+D,+D,-D);

   glColor3f(1,0,0);
   glVertex3f(+D,-D,+D);
   glVertex3f(-D,-D,+D);
   glVertex3f(-D,+D,+D);
   glVertex3f(+D,+D,+D);

   glColor3f(0,0,1);
   glVertex3f(-D,-D,+D);
   glVertex3f(-D,-D,-D);
   glVertex3f(-D,+D,-D);
   glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBegin(GL_QUADS);
   glColor3f(0,1,0);
   glVertex3f(+D,+D,-D);
   glVertex3f(+D,+D,+D);
   glVertex3f(-D,+D,+D);
   glVertex3f(-D,+D,-D);

   glColor3f(0,1,0);
   glVertex3f(-D,-D,+D);
   glVertex3f(+D,-D,+D);
   glVertex3f(+D,-D,-D);
   glVertex3f(-D,-D,-D);
   glEnd();
   
*/
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,stars_tex);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

double angle_from_horiz(double x, double y, double z)
{
	double len = sqrt(x*x + y*y + z*z);
	double angle = KUTILS_ASIN(y/len);
	return angle;
}

void draw_scene()
{
	// draw the falcon
	point3 falcon_trans = {falcon_pos.x, falcon_pos.y, falcon_pos.z};
	point4 falcon_rot = {0, 1, 0, falcon_dir};
	point3 falcon_scale = {0.01, 0.01, 0.01};
	Draw_falcon(falcon_trans, falcon_rot, falcon_scale, thrust_on, &tbox);
	
	// draw the skybox, centered on the falcon
	Sky(SKYBOX_DIM, falcon_trans);
	
	// the rectangle is in falcon coordinates, so transform it to world coordinates
	tbox.ful = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.ful);
	tbox.fll = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.fll);
	tbox.flr = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.flr);
	tbox.fur = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.fur);
	tbox.bul = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.bul);
	tbox.bll = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.bll);
	tbox.blr = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.blr);
	tbox.bur = ManualTransformAboutY(falcon_trans, falcon_rot, falcon_scale, tbox.bur);
	
	// draw the sun
	sun_pos.y = SUN_HEIGHT;
	point3 sun_trans = sun_pos;
	point4 sun_rots = no_rotation;
	point3 sun_scale = {SUN_RAD,SUN_RAD,SUN_RAD};
	Draw_Sun(sun_trans, sun_rots, sun_scale);	
	
	// draw the earth
	earth_pos.x = -ORBIT_RAD_EARTH*KUTILS_COS(orbit_angle_earth);
	earth_pos.y = EARTH_HEIGHT;
	earth_pos.z = ORBIT_RAD_EARTH*KUTILS_SIN(orbit_angle_earth);
	point3 earth_trans = earth_pos;
	point4 earth_rots = {0,1,0,rotate_angle_earth};
	point3 earth_scale = {EARTH_RAD,EARTH_RAD,EARTH_RAD};
	Draw_Earth(earth_trans, earth_rots, earth_scale);
	
	// draw the moon
	moon_pos.x = -ORBIT_RAD_MOON*KUTILS_COS(orbit_angle_moon);
	moon_pos.y = MOON_HEIGHT;
	moon_pos.z = ORBIT_RAD_MOON*KUTILS_SIN(orbit_angle_moon);
	point3 moon_trans = moon_pos;
	point4 moon_rots = {0,1,0,orbit_angle_moon};
	point3 moon_scale = {MOON_RAD,MOON_RAD,MOON_RAD};
	Draw_Moon(earth_trans, moon_trans, moon_rots, moon_scale);
	
	// draw the sheet
	Calculate_sheet_points(pts, falcon_pos);
	Calculate_sheet_heights(pts, sun_trans, earth_trans, moon_trans);
	Calculate_sheet_normals(pts, norms);
	Draw_sheet(pts, norms, show_sheet, show_grid);
	
	// TRANSPARENT OBJECTS AFTER THIS
	
	// draw the thrust
	Draw_Thrust(time);
	// create new thrust points if needed
	if(thrust_on)
		Create_Thrust(tbox, time, falcon_vel, falcon_dir);
	// move the points making up the thrust
	Move_Thrust(time, last_time);
	
	// draw inertial braking orb
	if(orb_alpha > 0)
	{
		point3 orb_trans = falcon_trans;
		point3 orb_scale = {0.015,0.015,0.015};
		//point3 orb_scale = {orb_alpha*ORB_BASE_RAD, orb_alpha*ORB_BASE_RAD, orb_alpha*ORB_BASE_RAD};
		Draw_Braking_Orb(orb_trans, orb_scale, orb_alpha);
	}
}

void set_lighting()
{
	//  Translate intensity to color vectors
	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position
	float Position[]  = {distance*KUTILS_COS(zh),ylight,distance*KUTILS_SIN(zh),1.0};
	//  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);
	point3 sphere_trans = {Position[0],Position[1],Position[2]};
	point3 sphere_scale = {0.1, 0.1, 0.1};
	Sphere(20, sphere_trans, sphere_scale);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING);
	//  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,true);
	//  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);
}

void collision_detect(point3 pos, point3 body_pos, double body_rad, bool buffer, double *result)
{
	*result = pos.y;
	double rad_sq = body_rad * body_rad;
	double dist_sq = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z + buffer;
	
	if(dist_sq <= rad_sq) // check if inside the body
	{
		double delta_x = body_pos.x - pos.x;
		double delta_z = body_pos.z - pos.z;
		double xz_dist = sqrt(delta_x*delta_x + delta_z*delta_z);
		double relative_height = rad_sq - xz_dist*xz_dist;
		double actual_height = body_pos.y - pos.y;
		double height_adjust = relative_height - fabs(actual_height);
		if(actual_height < 0)
			*result -= height_adjust;
		else
			*result += height_adjust;
	}
}

double adjustment_height(point3 pos, point3 body_pos, double body_rad, double buffer)
{
	double adjust = 0;
	
	double sqrt2 = 1.414;
	double delta_x = body_pos.x - pos.x;
	double delta_z = body_pos.z - pos.z;
	double xz_dist = sqrt(delta_x*delta_x + delta_z*delta_z);
	
	if(xz_dist < sqrt2 * body_rad)
	{
		adjust = (sqrt2 * body_rad) - xz_dist + buffer;
	}
	
	return adjust;
}

double adjust_eye_height(double x, double y, double z)
{
	/*
	point3 pos = {x, y, z};
	double buffer = 1;
	
	double adjust = adjustment_height(pos, sun_pos, SUN_RAD, buffer);	
	if(adjust > 0)
		return adjust + y;
		
	adjust = adjustment_height(pos, earth_pos, EARTH_RAD, buffer);	
	if(adjust > 0)
		return adjust + y;
		
	adjust = adjustment_height(pos, moon_pos, MOON_RAD, buffer);
	if(adjust > 0)
		return adjust + y;
		
	return y;
	*/
	return y;
}

void set_eye_position()
{

	double dist = -1 * (FOLLOW_DIST+1.5*eye_height);

	Ex = falcon_pos.x + dist * KUTILS_COS(falcon_dir);
	Ez = falcon_pos.z - dist * KUTILS_SIN(falcon_dir);
	Ey = adjust_eye_height(Ex, falcon_pos.y + eye_height, Ez);
	
	Cx = falcon_pos.x + VIEW_DIST_AHEAD * KUTILS_COS(falcon_dir);
	Cy = falcon_pos.y;
	Cz = falcon_pos.z - VIEW_DIST_AHEAD * KUTILS_SIN(falcon_dir);
/*
	// TOP DOWN VIEW FOR DEBUGGING
	double Ex = falcon_pos.x;
	double Ey = falcon_pos.y + 7*eye_height;
	double Ez = falcon_pos.z;
	
	double Cx = falcon_pos.x - 0.01;
	double Cy = falcon_pos.y;
	double Cz = falcon_pos.z;
*/
	gluLookAt(Ex,Ey,Ez , Cx,Cy,Cz, 0,1,0);	
}

point3 find_weighted_norm()
{
	// The falcon is always between points SHEET_PTS/2-1 and SHEET_PTS/2 in the x and z direction
	point3 n1 = norms[SHEET_PTS/2-1][SHEET_PTS/2-1];
	point3 n2 = norms[SHEET_PTS/2  ][SHEET_PTS/2-1];
	point3 n3 = norms[SHEET_PTS/2  ][SHEET_PTS/2  ];
	point3 n4 = norms[SHEET_PTS/2-1][SHEET_PTS/2  ];

	point3 pf = falcon_pos;
	
	// just find the average for now
	point3 result = { (n1.x+n2.x+n3.x+n4.x)/4,
					  0,						// we don't need the y component
					  (n1.z+n2.z+n3.z+n4.z)/4 };

	return result;
}

void update_velocity()
{
	point3 weighted_norm = find_weighted_norm();
	
	falcon_vel.x += GRAV_STRENGTH*weighted_norm.x;
	falcon_vel.z += GRAV_STRENGTH*weighted_norm.z;
}

void move_falcon(double time) // time is in sec
{
	double delta_t = time-last_time;

	falcon_pos.x += delta_t * falcon_vel.x;
	falcon_pos.z += delta_t * falcon_vel.z;
}

void turn_falcon(double time)
{	
	if(falcon_turn != NO_TURN)
	{
		int dir = (falcon_turn == RIGHT_TURN) ? -1 : 1;
		double delta_t = time - last_time;
		double turn_dist = dir * delta_t * TURN_RATE;
		falcon_dir += turn_dist;
		
		falcon_dir += 360;
		falcon_dir = fmod(falcon_dir,360.0);
	}
}

void increase_velocity(double amt)
{
	double vel = sqrt(falcon_vel.x*falcon_vel.x + falcon_vel.z*falcon_vel.z);

	if(vel + amt <= SPEED_LIMIT)
	{
		falcon_vel.x += amt * KUTILS_COS(falcon_dir);
		falcon_vel.z -= amt * KUTILS_SIN(falcon_dir);
	}
}

void brake(double percent)
{
	falcon_vel.x *= percent;
	falcon_vel.z *= percent;
}

void jump(double dist)
{
	falcon_pos.x += dist * KUTILS_COS(falcon_dir);
	falcon_pos.z -= dist * KUTILS_SIN(falcon_dir);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);

	//  Undo previous transformations
	glLoadIdentity();
	
	set_eye_position();

	// smooth shading
	glShadeModel(GL_SMOOTH);
	
	set_lighting();

	draw_scene();		
						   
	//  Draw axes - no lighting from here on
	glColor3d(1,1,1);
	glDisable(GL_LIGHTING);
	if (axes)
   		DrawAxes(1,1,1,1);
/*   		
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,"Perpective",light?"On":"Off");
     
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Distance=%d Elevation=%.1f",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
*/
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
	// set the times
	last_time = time;
	time = GetTime();

	// light orbit
	zh = fmod(90*time,360.0);

	if(orbit_planets)
	{
		// earth orbit
		orbit_angle_earth = fmod(90*time*ORBIT_SPEED_EARTH,360.0);

		// earth rotation
		rotate_angle_earth = fmod(90*time,360.0);

		// moon orbit
		orbit_angle_moon = fmod(90*time*ORBIT_SPEED_MOON,360.0);
	}
	// update falcon's velocity
	update_velocity();

	// move the falcon
	move_falcon(time);
	
	// turn the falcon
	turn_falcon(time);

	// set whether we need to increase or decrease the orb alpha
	if(increase_orb_alpha) orb_alpha += ORB_ALPHA_INC;
	else orb_alpha -= ORB_ALPHA_INC;
	
	if(orb_alpha < 0) orb_alpha = 0;
	else if(orb_alpha > ORB_ALPHA_MAX) orb_alpha = ORB_ALPHA_MAX;
	
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

void specialup(int key, int x, int y)
{
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT)
	{
		falcon_turn = NO_TURN;
	}
	else if(key == GLUT_KEY_UP)
		thrust_on = false;
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
   {
      //falcon_dir -= 5;
      falcon_turn = RIGHT_TURN;
   }
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
   {
      //falcon_dir += 5;
      falcon_turn = LEFT_TURN;
   }
   //  Up arrow key
   else if (key == GLUT_KEY_UP)
   {
      increase_velocity(SPEED_INC);
      thrust_on = true;
   }
   //  Down arrow key
   else if (key == GLUT_KEY_DOWN)
   {
      increase_velocity(-SPEED_INC);
   }
   //  PageUp key - increase view elevation
   else if (key == GLUT_KEY_PAGE_DOWN)
      eye_height += EYE_HEIGHT_INC;
   //  PageDown key - decrease view elevation
   else if (key == GLUT_KEY_PAGE_UP)
      eye_height -= EYE_HEIGHT_INC;
   else if (key == GLUT_KEY_F1)
      distance = (distance==1) ? 5 : 1;
      
   //  Keep angles to +/-360 degrees
   //falcon_dir %= 360;
   
   //  Update projection
   Project(fov,asp,dim);
   
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void keyup(unsigned char ch,int x,int y)
{
	if (ch == 32)
	{
		increase_orb_alpha = false;
	}
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   // Space bar: brake
   else if (ch == 32)
   {
      brake(BRAKE_INC);
      increase_orb_alpha = true;
   }  
   // Enter: jump
   else if (ch == 13)
      jump(JUMP_DIST);
   //  Reset view angle
   //else if (ch == '0')
   //   th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle orbiting planets
   else if (ch == 'm' || ch == 'M')
      orbit_planets = !orbit_planets;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   else if (ch=='z')
   {
   		if(show_sheet && show_grid)
   			show_sheet = false;
   		else if(!show_sheet && show_grid)
   		{
   			show_sheet = true;
   			show_grid = false;
   		}
   		else if(show_sheet && !show_grid)
   			show_sheet = false;
   		else
   			show_sheet = show_grid = true;
   }
      
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   //glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("Andrew Kessel - HW 7 - Textures");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialup);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyup);
	glutIdleFunc(idle);
	//  Load textures
	for(int i = 0; i < NUM_FALCON_TEXS; i++)
	{
		falcon_tex[i] = LoadTexBMP(falcon_tex_names[i]);
	}
	for(int i = 0; i < NUM_PLANET_TEXS; i++)
	{
		planet_tex[i] = LoadTexBMP(planet_tex_names[i]);
	}
	stars_tex = LoadTexBMP((char*)"./texs/tex_stars.bmp");
	// perform some initializations
	Initialize_Thrust();
	last_time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}
