/***************************************************
 * main.cpp
 *
 * Andrew Kessel
 * CSCI 5229 Final Project
 *
 * Controls:
 * ---------
 * Up arrow:     thrust
 * Back arrow:   flip 180 degrees (actually implemented as thrust backwards right now)
 * Left/Right    arrows: yaw left/right
 * Page Up/Down: zoom in/out
 * Space:        apply inertial braking
 * Enter:        hyperjump forward
 * z:            toggle spacetime sheet mode (grid/sheet/none)
 * m:            pause planet movement
 * p:            pointer to sun
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

// planetary constants
#define SUN_HEIGHT     0.2
#define EARTH_HEIGHT   0.1
#define MOON_HEIGHT    0.02
#define JUPITER_HEIGHT 0.12
#define COMET_HEIGHT   0.1
#define FALCON_HEIGHT  0.1

#define ORBIT_RAD_EARTH   3
#define ORBIT_RAD_MOON    0.75
#define ORBIT_RAD_JUPITER 6
#define ORBIT_RAD_COMET   7

#define ORBIT_OFFSET_COMET 5
#define ORBIT_SQUISH_COMET 0.5 // factor by which the comet's orbit will be squished in the z direction

#define ORBIT_SPEED_EARTH   10  // degrees/second
#define ORBIT_SPEED_MOON    20   // degrees/second
#define ORBIT_SPEED_JUPITER 5 // degrees/second
#define ORBIT_SPEED_COMET   5  // degrees/second

#define ROTATE_SPEED_EARTH   70    // degrees/second
#define ROTATE_SPEED_JUPITER 20 // degrees/second

// animation constants
#define JUMP_DURATION 1 // seconds
#define JUMP_DIST	  1 // how far to jump ahead
#define FLIP_DURATION 1 // seconds
#define EYE_ROT_DURATION 0.5 // seconds

// control constants
#define SPEED_LIMIT	2 // units per second (note that the sun's radius is 1 unit)
#define SPEED_INC	0.01 // how much to increase the speed by per key hit
#define BRAKE_INC	0.9 // percentage of speed to reduce to per key hit
#define TURN_RATE	100 // the degrees per second that the falcon can turn

#define GRAV_STRENGTH 0.05 // how much effect gravity has on the velocity

// viewing constants
#define FOLLOW_DIST     0.2  // how far behind the spaceship the eye should be
#define VIEW_DIST_AHEAD 1    // how far ahead of the spaceship the focal point is
#define EYE_HEIGHT_INC  0.01
#define SKYBOX_DIM		18
#define FALCON_SCALE    0.01

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

enum anim_type
{
	NO_ANIM,
	JUMP,
	FLIP,
	ROLL,
	EYE_ROT
};

int fov=35;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=2.0;   //  Size of world
// Light values
int inc       =  10;  // Ball increment
int emission  =   0;  // Emission intensity (%)
int ambient   =  0;  // Ambient intensity (%)
int diffuse   = 50;  // Diffuse intensity (%)
int specular  = 100;  // Specular intensity (%)
int shininess =   6;  // Shininess (power of two)
float shinyvec[1] = {64};    // Shininess (value)
unsigned int stars_tex;
unsigned int cockpit_tex;
unsigned int instr_tex;

// game state
bool pause_planets = false;
bool gravity_on = true;
// falcon state
point3 falcon_pos = {-2,FALCON_HEIGHT,-2};
//point3 falcon_pos = {-10,FALCON_HEIGHT,-10};
point3 falcon_vel = {0.0,0.0,0.0};
double falcon_dir = 270; // angle from the x-axis that the falcon is pointing
double eye_height = 0.05;
bool thrust_on = false;
// state of planetary system
double orbit_angle_earth = 0.0;
double rotate_angle_earth = 0.0;
double orbit_angle_moon = 0.0;
double rotate_angle_moon = 0.0;
double orbit_angle_jupiter = 0.0;
double rotate_angle_jupiter = 0.0;
double orbit_angle_comet = 270.0;
point3 sun_pos = {0,0,0};
point3 earth_pos = {0,0,0};
point3 moon_pos = {0,0,0};
point3 jupiter_pos = {0,0,0};
point3 comet_pos = {0,0,0};
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
bool show_pointer = false;
point3 *ref_eye_pos = &falcon_pos;
bool cockpit_view = false;
bool top_down_view = false;
// animation parameters
bool lock_controls = false;
anim_type animation = NO_ANIM;
double anim_start_time = 0.0;
double flip_start_time = 0.0;
double roll_start_time = 0.0;
point3 falcon_jump_start_pos;
point3 jump_eye_pos;
double falcon_scale_factor = 1; // stretches the falcon during a jump
double falcon_flip_angle = 0.0;
double falcon_roll_angle = 0.0;
double falcon_eye_angle = 0.0;
bool was_in_cockpit_view = false;
bool was_paused = false;

point3 pts[SHEET_PTS][SHEET_PTS];
point3 norms[SHEET_PTS][SHEET_PTS];

extern point3 no_translation;
extern point4 no_rotation;
extern point3 no_scale;
	
GLfloat starsEmissiveMaterial[] = {1, 1, 1};
extern GLfloat blackEmissiveMaterial[];

/******************************
 * HELPER FUNCTIONS
 ******************************/
 
void rotate_about_point(point2 target, point2 reference, double angle, point2 *result)
{
	// first translate the point so that it's rotating about the origin
	target.x -= reference.x;
	target.y -= reference.y;
	
	// perform the math to rotate the point
	double new_x = target.x * KUTILS_COS(angle) - target.y * KUTILS_SIN(angle);
	double new_y = target.x * KUTILS_SIN(angle) + target.y * KUTILS_COS(angle);
	
	// translate it back to the reference point
	result->x = new_x + reference.x;
	result->y = new_y + reference.y;
}

long double GetTime()
{
	//  Elapsed time in seconds
	return glutGet(GLUT_ELAPSED_TIME)/1000.0;
}

double angle_from_horiz(double x, double y, double z)
{
	double len = sqrt(x*x + y*y + z*z);
	double angle = KUTILS_ASIN(y/len);
	return angle;
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

/*********************************
 * DRAWING FUNCTIONS
 *********************************/

/*
 *  Draw the cockpit as an overlay
 *  Must be called last
 */
void Cockpit()
{
	//  Save transform attributes (Matrix Mode and Enabled Modes)
	glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
	//  Save projection matrix and set unit transform
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-asp,+asp,-1,1,-1,1);
	//  Save model view matrix and set to indentity
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
		
	glDisable(GL_LIGHTING);
	
	// draw speedometer
	point2 base_rght = { 0.00573576,-0.581808};
	point2 base_left = {-0.00573576,-0.598192};
	point2 tip = {-0.122873,-0.676036};
	point2 ref = {0,-0.59};
	point2 new_base_rght;
	point2 new_base_left;
	point2 new_tip;
	
	double max_angle = -250;
	double vel = sqrt(falcon_vel.x*falcon_vel.x + falcon_vel.z*falcon_vel.z);
	double rotate = max_angle * vel / SPEED_LIMIT;

	rotate_about_point(base_rght, ref, rotate, &new_base_rght);
	rotate_about_point(base_left, ref, rotate, &new_base_left);
	rotate_about_point(tip, ref, rotate, &new_tip);
	
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(new_base_rght.x, new_base_rght.y);
	glVertex2f(new_base_left.x, new_base_left.y);
	glVertex2f(new_tip.x,       new_tip.y);
	glEnd();
	
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, instr_tex);	
	glBegin(GL_POLYGON);
	glTexCoord2d(0,     0);    glVertex2f(-0.5,   -1.1);
	glTexCoord2d(0,     0.5);  glVertex2f(-0.5,   -0.6);
	glTexCoord2d(0.3125,0.75); glVertex2f(-0.1875,-0.35);
	glTexCoord2d(0.6875,0.75); glVertex2f( 0.1875,-0.35);
	glTexCoord2d(1,     0.5);  glVertex2f( 0.5,   -0.6);
	glTexCoord2d(1,     0);    glVertex2f( 0.5,   -1.1);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, cockpit_tex);	
	glBegin(GL_POLYGON);
	glTexCoord2d(0,     0);    glVertex2f(-0.55,   -1);
	glTexCoord2d(0,     0.5);  glVertex2f(-0.55,   -0.58);
	glTexCoord2d(0.3125,0.75); glVertex2f(-0.1875,-0.3);
	glTexCoord2d(0.6875,0.75); glVertex2f( 0.1875,-0.3);
	glTexCoord2d(1,     0.5);  glVertex2f( 0.55,   -0.58);
	glTexCoord2d(1,     0);    glVertex2f( 0.55,   -1);
	glEnd();
	
	glBegin(GL_QUADS);
	// center bar
	glTexCoord2d(0,0); glVertex2f( 0.5,-0.1);
	glTexCoord2d(1,0); glVertex2f( 0.5,-0.16);
	glTexCoord2d(1,5); glVertex2f(-0.5,-0.16);
	glTexCoord2d(0,5); glVertex2f(-0.5,-0.1);
	// right vertical bar
	glTexCoord2d(0,0); glVertex2f(0.94,-0.58);
	glTexCoord2d(1,0); glVertex2f(1,   -0.58);
	glTexCoord2d(1,5); glVertex2f(1,   -1);
	glTexCoord2d(0,5); glVertex2f(0.94,-1);
	// right diagonal bar
	glTexCoord2d(0,0); glVertex2f(0.5, -0.1);
	glTexCoord2d(1,0); glVertex2f(1,   -0.58);
	glTexCoord2d(1,5); glVertex2f(0.94,-0.58);
	glTexCoord2d(0,5); glVertex2f(0.5, -0.16);
	// right inner upward diagonal bar
	glTexCoord2d(0,0); glVertex2f(0.43,-0.1);
	glTexCoord2d(1,0); glVertex2f(1.3,1);
	glTexCoord2d(1,15); glVertex2f(1.37,1);
	glTexCoord2d(0,15); glVertex2f(0.5,-0.1);
	// right outer upward diagonal bar
	glTexCoord2d(0,0); glVertex2f(1,-0.58);
	glTexCoord2d(1,0); glVertex2f(5,1);
	glTexCoord2d(1,35); glVertex2f(5.1, 1);
	glTexCoord2d(0,35); glVertex2f(1,-0.63);
	// left vertical bar
	glTexCoord2d(0,0); glVertex2f(-0.94,-0.58);
	glTexCoord2d(1,0); glVertex2f(-1,   -0.58);
	glTexCoord2d(1,5); glVertex2f(-1,   -1);
	glTexCoord2d(0,5); glVertex2f(-0.94,-1);
	// left diagonal bar
	glTexCoord2d(0,0); glVertex2f(-0.5, -0.1);
	glTexCoord2d(1,0); glVertex2f(-1,   -0.58);
	glTexCoord2d(1,5); glVertex2f(-0.94,-0.58);
	glTexCoord2d(0,5); glVertex2f(-0.5, -0.16);
	// left inner upward diagonal bar
	glTexCoord2d(0,0);  glVertex2f(-0.43,-0.1);
	glTexCoord2d(1,0);  glVertex2f(-1.3,1);
	glTexCoord2d(1,15); glVertex2f(-1.37,1);
	glTexCoord2d(0,15); glVertex2f(-0.5,-0.1);
	// left outer upward diagonal bar
	glTexCoord2d(0,0);  glVertex2f(-1,-0.58);
	glTexCoord2d(1,0);  glVertex2f(-5,1);
	glTexCoord2d(1,35); glVertex2f(-5.1, 1);
	glTexCoord2d(0,35); glVertex2f(-1,-0.63);
	glEnd();
	
	glEnable(GL_LIGHTING);
	
	//  Reset model view matrix
	glPopMatrix();
	//  Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//  Pop transform attributes (Matrix Mode and Enabled Modes)
	glPopAttrib();
}

static void Sky(double D, point3 trans)
{
	glPushMatrix();
	Transform(trans, no_rotation, no_scale);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, starsEmissiveMaterial);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);

	//  Sides
	glBindTexture(GL_TEXTURE_2D,stars_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(3,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(3,3); glVertex3f(+D,+D,-D);
	glTexCoord2f(0,3); glVertex3f(-D,+D,-D);

	glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(3,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(3,3); glVertex3f(+D,+D,+D);
	glTexCoord2f(0,3); glVertex3f(+D,+D,-D);

	glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(3,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(3,3); glVertex3f(-D,+D,+D);
	glTexCoord2f(0,3); glVertex3f(+D,+D,+D);

	glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(3,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(3,3); glVertex3f(-D,+D,-D);
	glTexCoord2f(0,3); glVertex3f(-D,+D,+D);
	glEnd();

	//  Top and bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(+D,+D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,+D,+D);
	glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

	glTexCoord2f(1,1); glVertex3f(-D,-D,+D);
	glTexCoord2f(0,1); glVertex3f(+D,-D,+D);
	glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackEmissiveMaterial);

	glPopMatrix();
}

void draw_pointer()
{
	glPushMatrix();
	Transform(falcon_pos, no_rotation, no_scale);

	point3 dir = falcon_pos;
	double lenSq = sqrt(falcon_pos.x*falcon_pos.x + falcon_pos.y*falcon_pos.y + falcon_pos.z*falcon_pos.z);
	dir.x /= lenSq;
	dir.y /= lenSq;
	dir.z /= lenSq;

	glColor3d(1,0,0);
	glBegin(GL_LINES);
	glVertex3d(-dir.x, -dir.y, -dir.z);
	glVertex3d(0,0,0);
	glEnd();
	
	glPopMatrix();
}

void draw_falcon(point3 *trans)
{
	// draw the falcon
	point3 falcon_trans = {falcon_pos.x, falcon_pos.y, falcon_pos.z};
	*trans = falcon_trans;
	point4 falcon_base_rot = {0, 1, 0, falcon_dir};
	point4 falcon_flip_rot = {0, 0, 1, falcon_flip_angle}; // only flip about the falcon's z-axis
	point4 falcon_roll_rot = {1, 0, 0, falcon_roll_angle}; // only roll about the falcon's x-axis
	point4 falcon_rots[] = {falcon_base_rot, falcon_flip_rot, falcon_roll_rot};
	point3 falcon_scale = {falcon_scale_factor*FALCON_SCALE, FALCON_SCALE, FALCON_SCALE};
	if(!cockpit_view)
		Draw_falcon(&falcon_trans, falcon_rots, &falcon_scale, 1,3,1, thrust_on, &tbox);
	// the thrust rectangle is in falcon coordinates, so transform it to world coordinates
	tbox.ful = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.ful);
	tbox.fll = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.fll);
	tbox.flr = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.flr);
	tbox.fur = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.fur);
	tbox.bul = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.bul);
	tbox.bll = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.bll);
	tbox.blr = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.blr);
	tbox.bur = ManualTransformAboutY(falcon_trans, falcon_rots[0], falcon_scale, tbox.bur);
}

void draw_planets(point3 *earth_trans, point3 *moon_trans, point3 *jupiter_trans)
{
	// draw the earth
	earth_pos.x = -ORBIT_RAD_EARTH*KUTILS_COS(orbit_angle_earth);
	earth_pos.y = EARTH_HEIGHT;
	earth_pos.z = ORBIT_RAD_EARTH*KUTILS_SIN(orbit_angle_earth);
	*earth_trans = earth_pos;
	point4 earth_rots = {0,1,0,rotate_angle_earth};
	point3 earth_scale = {EARTH_RAD,EARTH_RAD,EARTH_RAD};
	Draw_Earth(*earth_trans, earth_rots, earth_scale);
	
	// draw the moon
	moon_pos.x = -ORBIT_RAD_MOON*KUTILS_COS(orbit_angle_moon);
	moon_pos.y = MOON_HEIGHT;
	moon_pos.z = ORBIT_RAD_MOON*KUTILS_SIN(orbit_angle_moon);
	*moon_trans = moon_pos;
	point4 moon_rots = {0,1,0,orbit_angle_moon};
	point3 moon_scale = {MOON_RAD,MOON_RAD,MOON_RAD};
	Draw_Moon(*earth_trans, *moon_trans, moon_rots, moon_scale);
	
	// draw jupiter
	jupiter_pos.x = -ORBIT_RAD_JUPITER*KUTILS_COS(orbit_angle_jupiter);
	jupiter_pos.y = JUPITER_HEIGHT;
	jupiter_pos.z = ORBIT_RAD_JUPITER*KUTILS_SIN(orbit_angle_jupiter);
	*jupiter_trans = jupiter_pos;
	point4 jupiter_rots = {0,1,0,rotate_angle_jupiter};
	point3 jupiter_scale = {JUPITER_RAD,JUPITER_RAD,JUPITER_RAD};
	Draw_Jupiter(*jupiter_trans, jupiter_rots, jupiter_scale);
}

void draw_scene()
{
	point3 falcon_trans, earth_trans, moon_trans, jupiter_trans;
	
	draw_falcon(&falcon_trans);
	
	// draw pointer to sun
	if(show_pointer)
		draw_pointer();
	
	// draw the skybox, centered on the falcon
	point3 eye_pos = {Ex,Ey,Ez};
	Sky(SKYBOX_DIM, eye_pos);
	
	// draw the sun
	sun_pos.y = SUN_HEIGHT;
	point3 sun_trans = sun_pos;
	point4 sun_rots = no_rotation;
	point3 sun_scale = {SUN_RAD,SUN_RAD,SUN_RAD};
	Draw_Sun(sun_trans, sun_rots, sun_scale);	
	
	draw_planets(&earth_trans, &moon_trans, &jupiter_trans);
	
	// draw the sheet
	Calculate_sheet_points(pts, falcon_pos);
	Calculate_sheet_heights(pts, sun_trans, earth_trans, moon_trans, jupiter_trans);
	Calculate_sheet_normals(pts, norms);
	Draw_sheet(pts, norms, show_sheet, show_grid);
	
	// TRANSPARENT OBJECTS AFTER THIS

	// draw the comet
	double coma_alpha, tail_len, rot_angle;
	comet_pos.x = -ORBIT_RAD_COMET*KUTILS_COS(orbit_angle_comet) + ORBIT_OFFSET_COMET;
	comet_pos.y = COMET_HEIGHT;
	comet_pos.z = ORBIT_SQUISH_COMET*ORBIT_RAD_COMET*KUTILS_SIN(orbit_angle_comet);
	Calculate_Comet_Params(comet_pos.x, comet_pos.z, ORBIT_RAD_COMET, &coma_alpha, &tail_len, &rot_angle);
	point3 comet_trans = comet_pos;
	point4 comet_rots = {0,1,0, rot_angle};
	point3 comet_scale = {COMET_RAD,COMET_RAD,COMET_RAD};
	Draw_Comet(comet_trans, comet_rots, comet_scale, tail_len, coma_alpha);
	
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
		double scale = cockpit_view ? 0.25 : 0.015;
		point3 orb_trans = falcon_trans;
		point3 orb_scale = {scale,scale,scale};
		//point3 orb_scale = {orb_alpha*ORB_BASE_RAD, orb_alpha*ORB_BASE_RAD, orb_alpha*ORB_BASE_RAD};
		Draw_Braking_Orb(orb_trans, orb_scale, orb_alpha);
	}
	
	// draw the sun's corona
	Draw_Corona(sun_trans, sun_rots, sun_scale);
	
	if(cockpit_view)
	{
		Cockpit();
	}
}

void set_lighting()
{
	//  Translate intensity to color vectors
	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position
	float Position[] = {0,0,0};
	//  Draw light position as ball (still no lighting here)
	//glColor3f(1,1,1);
	//point3 sphere_trans = {Position[0],Position[1],Position[2]};
	//point3 sphere_scale = {0.001, 0.001, 0.001};
	//Sphere(20, sphere_trans, sphere_scale);
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

/****************************
 * COLLISION DETECTION
 ****************************/

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

/**************************
 * GAME FUNCTIONALITY
 **************************/
 
void update_velocity()
{
	if(gravity_on)
	{
		point3 weighted_norm = find_weighted_norm();
	
		falcon_vel.x += GRAV_STRENGTH*weighted_norm.x;
		falcon_vel.z += GRAV_STRENGTH*weighted_norm.z;
	}
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

/****************************
 * ANIMATIONS
 ****************************/

void start_anim(anim_type type)
{
	anim_start_time = GetTime();
	falcon_jump_start_pos = falcon_pos;
	if(type == JUMP)
	{
		jump_eye_pos = falcon_jump_start_pos;
		ref_eye_pos = &jump_eye_pos;
	}
	was_in_cockpit_view = cockpit_view;
	was_paused = pause_planets;
	cockpit_view = false;
	lock_controls = true;
	animation = type;
}

void end_anim()
{
	falcon_scale_factor = 1;
	falcon_flip_angle = 0.0;
	falcon_roll_angle = 0.0;
	falcon_eye_angle = 0.0;
	ref_eye_pos = &falcon_pos;
	lock_controls = false;
	cockpit_view = was_in_cockpit_view;
	pause_planets = was_paused;
	animation = NO_ANIM;
}

void animate_jump()
{
	double delta_t = GetTime() - anim_start_time;
	
	// stopping case
	if (delta_t >= JUMP_DURATION)
	{
		falcon_vel.x = 0;
		falcon_vel.y = 0;
		falcon_vel.z = 0;
		
		end_anim();
	}
	else
	{
		double dist = JUMP_DIST * delta_t / JUMP_DURATION;
		
		falcon_pos.x = falcon_jump_start_pos.x + dist * KUTILS_COS(falcon_dir);
		falcon_pos.z = falcon_jump_start_pos.z - dist * KUTILS_SIN(falcon_dir);
		
		falcon_scale_factor = (delta_t <= JUMP_DURATION/2.0)
							? (dist / FALCON_SCALE)
							: ((JUMP_DIST - dist) / FALCON_SCALE);
							
		double jump_fraction = 0.8;
		if(delta_t >= JUMP_DURATION * jump_fraction)
		{
			double delta_t_eye = delta_t - JUMP_DURATION * jump_fraction;
			double eye_dist = 4*JUMP_DIST * delta_t_eye / (JUMP_DURATION * jump_fraction);
			jump_eye_pos.x = falcon_jump_start_pos.x + eye_dist * KUTILS_COS(falcon_dir);
			jump_eye_pos.z = falcon_jump_start_pos.z - eye_dist * KUTILS_SIN(falcon_dir);
		}
	}
}

void animate_flip()
{
	double delta_t = GetTime() - anim_start_time;
	
	// stopping case
	if (delta_t >= FLIP_DURATION)
	{
		if(was_in_cockpit_view)
		{
			end_anim();
			falcon_dir += 180;
			falcon_dir = fmod(falcon_dir,360.0);
		}
		else
			start_anim(EYE_ROT);
	}
	else
	{
		double angle = 180 * delta_t / FLIP_DURATION;
		falcon_flip_angle = angle;	
		falcon_roll_angle = angle;		
	}
}

void animate_eye_rotation()
{
	double delta_t = GetTime() - anim_start_time;
	
	// stopping case
	if (delta_t >= EYE_ROT_DURATION)
	{
		falcon_dir += 180;
		falcon_dir = fmod(falcon_dir,360.0);
		
		end_anim();
	}
	else
	{
		double angle = 180 * delta_t / EYE_ROT_DURATION;
		falcon_eye_angle = angle;
	}
}

/***********************************
 * CAMERA FUNCTIONS
 ***********************************/

double calc_adjustment_height(point3 pos, point3 body_pos, double body_rad, double buffer)
{
	double adjust = 0;
	
	double sqrt2 = 1.414;
	double delta_x = body_pos.x - pos.x;
	double delta_z = body_pos.z - pos.z;
	double xz_dist = sqrt(delta_x*delta_x + delta_z*delta_z);
	
	if(xz_dist < sqrt2 * (body_rad+buffer))
	{
		adjust = (sqrt2 * body_rad) - xz_dist + buffer;
	}
	
	return adjust;
}

double adjust_eye_height(double x, double y, double z)
{
	point3 pos = {x, y, z};
	double buffer = 0.2;
	
	double adjust = calc_adjustment_height(pos, sun_pos, SUN_RAD, buffer);	
	if(adjust > 0)
		return adjust + y;
		
	adjust = calc_adjustment_height(pos, earth_pos, EARTH_RAD, buffer);	
	if(adjust > 0)
		return adjust + y;
		
	adjust = calc_adjustment_height(pos, moon_pos, MOON_RAD, buffer);
	if(adjust > 0)
		return adjust + y;
		
	adjust = calc_adjustment_height(pos, jupiter_pos, JUPITER_RAD, buffer);
	if(adjust > 0)
		return adjust + y;
		
	return y;
}

void set_eye_position()
{
	double dist = -1 * (FOLLOW_DIST+1.5*eye_height);

	double Ux = 0;
	double Uy = 1;
	double Uz = 0;

	if(cockpit_view)
	{
		Ex = falcon_pos.x;
		Ey = falcon_pos.y;
		Ez = falcon_pos.z;

		Cx = falcon_pos.x + VIEW_DIST_AHEAD * KUTILS_COS(falcon_dir);
		Cy = falcon_pos.y;
		Cz = falcon_pos.z - VIEW_DIST_AHEAD * KUTILS_SIN(falcon_dir);
	}
	else if(top_down_view)
	{
		Ex = falcon_pos.x;
		Ey = falcon_pos.y + 7*eye_height;
		Ez = falcon_pos.z;
	
		Cx = falcon_pos.x - 0.01;
		Cy = falcon_pos.y;
		Cz = falcon_pos.z;
	}
	else
	{
		Ex = ref_eye_pos->x + dist * KUTILS_COS(falcon_dir);
		Ez = ref_eye_pos->z - dist * KUTILS_SIN(falcon_dir);
		Ey = adjust_eye_height(Ex, ref_eye_pos->y + eye_height, Ez);
		
		Cx = ref_eye_pos->x + VIEW_DIST_AHEAD * KUTILS_COS(falcon_dir);
		Cy = ref_eye_pos->y;
		Cz = ref_eye_pos->z - VIEW_DIST_AHEAD * KUTILS_SIN(falcon_dir);
	}

	// falcon_eye_angle is the angle from falcon_dir that the camera is pointing
	if(falcon_eye_angle != 0)
	{
		point2 target = {Ex, Ez};
		point2 reference = {ref_eye_pos->x, ref_eye_pos->z};
		point2 result;
		rotate_about_point(target, reference, falcon_eye_angle, &result);
		Ex = result.x;
		Ez = result.y;

		target.x = Cx;
		target.y = Cz;
		rotate_about_point(target, reference, falcon_eye_angle, &result);
		Cx = result.x;
		Cz = result.y;
	}

	gluLookAt(Ex,Ey,Ez, Cx,Cy,Cz, Ux,Uy,Uz);	
}

/******************************
 * OPENGL FUNCTIONS
 ******************************/

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
	
	// Display game state
	if(animation == NO_ANIM)
	{
		if(!gravity_on)
		{
			glWindowPos2i(5,5);
			Print("Gravity off\n");
		}
		if(pause_planets)
		{
			glWindowPos2i(5,25);
			Print("Planet motion paused\n");
		}		
	}
	
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
	double delta_t = time - last_time;

	if(!pause_planets)
	{
		// earth orbit
		//orbit_angle_earth = fmod(90*time*ORBIT_SPEED_EARTH,360.0);
		orbit_angle_earth += delta_t * ORBIT_SPEED_EARTH;
		orbit_angle_earth = fmod(orbit_angle_earth,360.0);

		// earth rotation
		//rotate_angle_earth = fmod(90*time*ROTATE_SPEED_EARTH,360.0);
		rotate_angle_earth += delta_t * ROTATE_SPEED_EARTH;
		rotate_angle_earth = fmod(rotate_angle_earth,360.0);

		// moon orbit
		//orbit_angle_moon = fmod(90*time*ORBIT_SPEED_MOON,360.0);
		orbit_angle_moon += delta_t * ORBIT_SPEED_MOON;
		orbit_angle_moon = fmod(orbit_angle_moon,360.0);		
		
		// jupiter orbit
		//orbit_angle_jupiter = fmod(90*time*ORBIT_SPEED_JUPITER,360.0);
		orbit_angle_jupiter += delta_t * ORBIT_SPEED_JUPITER;
		orbit_angle_jupiter = fmod(orbit_angle_jupiter,360.0);

		// jupiter rotation
		//rotate_angle_jupiter = fmod(90*time*ROTATE_SPEED_JUPITER,360.0);
		rotate_angle_jupiter += delta_t * ROTATE_SPEED_JUPITER;
		rotate_angle_jupiter = fmod(rotate_angle_jupiter,360.0);
		
		// comet orbit
		//orbit_angle_comet = fmod(90*time*ORBIT_SPEED_COMET,360.0);
		orbit_angle_comet += delta_t * ORBIT_SPEED_COMET;
		orbit_angle_comet = fmod(orbit_angle_comet,360.0);
	}
	
	// update falcon's velocity
	update_velocity();
	
	// move the falcon
	move_falcon(time);
	
	// turn the falcon
	turn_falcon(time);

	// set whether we need to increase or decrease the orb alpha
	if(increase_orb_alpha)
	{
		orb_alpha += ORB_ALPHA_INC;
		if(orb_alpha > ORB_ALPHA_MAX) orb_alpha = ORB_ALPHA_MAX;
	}
	else
	{
		orb_alpha -= ORB_ALPHA_INC;
		if(orb_alpha < 0) orb_alpha = 0;
	} 

	if(animation == JUMP)
		animate_jump();
	else if (animation == FLIP)
		animate_flip();
	else if (animation == EYE_ROT)
		animate_eye_rotation();
	
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
	if (key == GLUT_KEY_RIGHT && !lock_controls)
	{
		//falcon_dir -= 5;
		falcon_turn = RIGHT_TURN;
	}
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT && !lock_controls)
	{
		//falcon_dir += 5;
		falcon_turn = LEFT_TURN;
	}
	//  Up arrow key
	else if (key == GLUT_KEY_UP && !lock_controls)
	{
		increase_velocity(SPEED_INC);
		thrust_on = true;
	}
	//  Down arrow key
	else if (key == GLUT_KEY_DOWN && !lock_controls)
	{
		start_anim(FLIP);
	}
	//  PageUp key - increase view elevation
	else if (key == GLUT_KEY_PAGE_DOWN && !cockpit_view)
		eye_height += EYE_HEIGHT_INC;
	//  PageDown key - decrease view elevation
	else if (key == GLUT_KEY_PAGE_UP && !cockpit_view)
		if(eye_height > 0)
			eye_height -= EYE_HEIGHT_INC;

	//  Update projection
	Project(fov,asp,dim);

	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

void keyup(unsigned char ch,int x,int y)
{
	// Space bar
	if (ch == 32)
	{
		increase_orb_alpha = false;
	}
	else if (ch == 'p' || ch == 'P')
	{
		show_pointer = false;
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
	else if (ch == 13 && !lock_controls)
	{
		start_anim(JUMP);
	}
	//  Toggle orbiting planets
	else if (ch == 'm' || ch == 'M')
		pause_planets = !pause_planets;
	//  Change field of view angle
	else if (ch == '-' && ch>1)
		fov--;
	else if (ch == '+' && ch<179)
		fov++;
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
	else if (ch=='c' || ch =='C')
		cockpit_view = !cockpit_view;
	else if (ch=='t' || ch=='T')
		top_down_view = !top_down_view;
	else if (ch=='g' || ch=='G')
		gravity_on = !gravity_on;
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
	else if (ch == 'p' || ch == 'P')
		show_pointer = true;

	//  Translate shininess power to value (-1 => 0)
	shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
	//  Reproject
	Project(fov,asp,dim);
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
	glutCreateWindow("Andrew Kessel - HW 10 - Final Project");
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
	cockpit_tex = LoadTexBMP((char*)"./texs/tex_cockpit.bmp");
	instr_tex = LoadTexBMP((char*)"./texs/tex_instrument.bmp");
	// perform some initializations
	Initialize_Thrust();
	last_time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	//  Initialize texture map
	//InitMap();
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}
