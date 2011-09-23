/***************************************************
 * main.cpp
 *
 * Andrew Kessel
 * Created 9/19/2011
 *
 * Main program file for CSCI 5229, Homework 3
 *
 ***************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include "kutils.h"
#include "kdraw.h"

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
bool axes = true; //  Display axes
double circling_cows_angle = 0.0; // rotation of cows

using namespace kutils;
using namespace kdraw;		  

void Draw_H(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from three modified cubes
	point3_t trans_left = {-0.4, 0, 0};
	point3_t scale_left = {0.2, 1.5, 0.2};
	point4_t rot_left = {0, 0, 0, 0};
	
	point3_t trans_mid = {0,0,0};
	point3_t scale_mid = {0.6, 0.2, 0.2};
	point4_t rot_mid = {0, 0, 0, 0};
	
	point3_t trans_right = {0.4, 0, 0};
	point3_t scale_right = {0.2, 1.5, 0.2};
	point4_t rot_right = {0, 0, 0, 0};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_mid, rot_mid, scale_mid);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_O(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from arches and cubes
	point3_t trans_top = {0, 0.25, 0};
	point3_t scale_top = {1, 1, 0.2};
	point4_t rot_top = {0, 0, 0, 0};
	
	point3_t trans_bottom = {0, -0.25, 0};
	point3_t scale_bottom = {1, 1, 0.2};
	point4_t rot_bottom = {1, 0, 0, 180};
	
	point3_t trans_left = {-0.4, 0, 0};
	point3_t scale_left = {0.2, 0.5, 0.2};
	point4_t rot_left = {0, 0, 0, 0};
	
	point3_t trans_right = {0.4, 0, 0};
	point3_t scale_right = {0.2, 0.5, 0.2};
	point4_t rot_right = {0, 0, 0, 0};
	
	double r_outer = 0.5;
	double r_inner = 0.3;
	double degrees = 180;
	int segs = 20;
	
	Arch(r_outer, r_inner, degrees, segs, trans_top, rot_top, scale_top, false);
	Arch(r_outer, r_inner, degrees, segs, trans_bottom, rot_bottom, scale_bottom, false);
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_L(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from two modified cubes
	point3_t trans_left = {-0.4, 0, 0};
	point3_t scale_left = {0.2, 1.5, 0.2};
	point4_t rot_left = {0,0,0,0};
	
	point3_t trans_bottom = {0, -0.65, 0};
	point3_t scale_bottom = {1, 0.2, 0.2};
	point4_t rot_bottom = {0,0,0,0};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_bottom, rot_bottom, scale_bottom);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_Y(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from three modified cubes
	point3_t trans_left = {-0.25, 0.35, 0};
	point3_t scale_left = {0.87, 0.2, 0.2};
	point4_t rot_left = {0, 0, 1, -56.31};
	
	point3_t trans_mid = {0, -0.375, 0};
	point3_t scale_mid = {0.2, 0.75, 0.2};
	point4_t rot_mid = {0, 0, 0, 0};
	
	point3_t trans_right = {0.25, 0.35, 0};
	point3_t scale_right = {0.87, 0.2, 0.2};
	point4_t rot_right = {0, 0, 1, 56.31};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_mid, rot_mid, scale_mid);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_C(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from arches and cubes
	point3_t trans_top = {0, 0.25, 0};
	point3_t scale_top = {1, 1, 0.2};
	point4_t rot_top = {0, 0, 1, 25};
	
	point3_t trans_bottom = {0, -0.25, 0};
	point3_t scale_bottom = {1, 1, 0.2};
	point4_t rot_bottom1 = {1, 0, 0, 180};
	point4_t rot_bottom2 = {0, 0, 1, 25};
	
	point3_t trans_bottoms[1] = { trans_bottom };
	point3_t scale_bottoms[1] = { scale_bottom };
	point4_t rot_bottoms[2] = { rot_bottom1, rot_bottom2 };
	
	point3_t trans_left = {-0.4, 0, 0};
	point3_t scale_left = {0.2, 0.5, 0.2};
	point4_t rot_left = {0, 0, 0, 0};
	
	double r_outer = 0.5;
	double r_inner = 0.3;
	double degrees = 155;
	int segs = 20;
	
	Arch(r_outer, r_inner, degrees, segs, trans_top, rot_top, scale_top, true);
	Arch(r_outer, r_inner, degrees, segs, trans_bottoms, rot_bottoms, scale_bottoms, 1, 2, 1, true);
	Cube(trans_left, rot_left, scale_left);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_W(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from four modified cubes
	
	point3_t trans_far_left = {-0.375, 0, 0};
	point3_t scale_far_left = {1.52, 0.2, 0.2};
	point4_t rot_far_left = {0, 0, 1, -80.54};
	
	point3_t trans_mid_left = {-0.125, -0.375, 0};
	point3_t scale_mid_left = {0.79, 0.2, 0.2};
	point4_t rot_mid_left = {0, 0, 1, 71.57};
	
	point3_t trans_mid_right = {0.125, -0.375, 0};
	point3_t scale_mid_right = {0.79, 0.2, 0.2};
	point4_t rot_mid_right = {0, 0, 1, -71.57};
	
	point3_t trans_far_right = {0.375, 0, 0};
	point3_t scale_far_right = {1.52, 0.2, 0.2};
	point4_t rot_far_right = {0, 0, 1, 80.54};
	
	point3_t trans_mid_tip = {0, -0.07, 0};
	point3_t scale_mid_tip = {0.2, 0.2, 0.2};
	point4_t rot_mid_tip = {0,0,1,90};
	
	point3_t trans_left_tip = {-0.25, -0.68, 0};
	point3_t scale_left_tip = {0.2, 0.2, 0.2};
	point4_t rot_left_tip = {0,0,1,90};
	
	point3_t trans_right_tip = {0.25, -0.68, 0};
	point3_t scale_right_tip = {0.2, 0.2, 0.2};
	point4_t rot_right_tip = {0,0,1,90};
	
	Cube(trans_far_left, rot_far_left, scale_far_left);
	Cube(trans_mid_left, rot_mid_left, scale_mid_left);
	Cube(trans_mid_right, rot_mid_right, scale_mid_right);
	Cube(trans_far_right, rot_far_right, scale_far_right);
	Cube(trans_mid_tip, rot_mid_tip, scale_mid_tip);
	Cube(trans_left_tip, rot_left_tip, scale_left_tip);
	Cube(trans_right_tip, rot_right_tip, scale_right_tip);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_ExclamationPoint(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build from a cone and sphere
	
	int segs = 20;
	
	point3_t trans_cone = {0, 0.75, 0};
	point3_t scale_cone = {0.5, 0.5, 1};
	point4_t rot_cone = {1, 0, 0, 90};
	
	point3_t trans_sphere = {0, -0.5, 0};
	point3_t scale_sphere = {0.2, 0.2, 0.2};
	
	Cone(segs, trans_cone, rot_cone, scale_cone);
	Sphere(segs, trans_sphere, scale_sphere);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_Cow(point3_t translation, point4_t rotation, point3_t scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	point3_t trans_body = {0,0,0};
	point3_t scale_body = {1, 0.5, 0.5};
	point4_t rot_body = {0,0,0,0};
	
	point3_t trans_head = {0.6, 0.4, 0};
	point3_t scale_head = {0.3, 0.3, 0.5};
	point4_t rot_head = {0,1,0,90};
	
	point3_t trans_leg = {0.45, -0.4, 0.175};
	point3_t scale_leg = {0.1, 0.3, 0.1};
	point4_t rot_leg = {0,0,0,0};
	
	point3_t trans_horn = {0.5, 0.75, 0};
	point3_t scale_horn = {1, 1, 0.1};
	point4_t rot_horn1 = {0, 1, 0, 90};
	point4_t rot_horn2 = {0,0,1,180};
	
	Cube(trans_body, rot_body, scale_body);
	Cube(trans_head, rot_head, scale_head);
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.x = -0.45;
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.z = -0.175;
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.x = 0.45;
	Cube(trans_leg, rot_leg, scale_leg);
	
	double r_outer = 0.4;
	double r_inner = 0.3;
	double degrees = 180;
	int segs = 5;
	point3_t trans_horns[1] = { trans_horn };
	point3_t scale_horns[1] = { scale_horn };
	point4_t rot_horns[2] = { rot_horn1, rot_horn2 };
	
	Arch(r_outer, r_inner, degrees, segs, trans_horns, rot_horns, scale_horns, 1, 2, 1, true);
	
	// Undo transformations
	glPopMatrix();
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
	//  Set view angle
	glRotatef(ph,1,0,0);
	glRotatef(th,0,1,0);
   
	// transformation objects
   	point3_t translation = {0, 0, 0};
	point4_t rotation = {0, 0, 0, 0};
	point3_t scale = {0.4, 0.5, 0.5};
	
	if(axes)
		DrawAxes(1,1,1,1.5);
	
	// draw "HOLY"
	translation.x = -2;
	Draw_H(translation, rotation, scale);
	translation.x = -1.5;
	Draw_O(translation, rotation, scale);
	translation.x = -1;
	Draw_L(translation, rotation, scale);
	translation.x = -0.5;
	Draw_Y(translation, rotation, scale);

	// draw "COW!"
	translation.x = 0.5;
	Draw_C(translation, rotation, scale);
	translation.x = 1;
	Draw_O(translation, rotation, scale);
	translation.x = 1.5;
	Draw_W(translation, rotation, scale);
	translation.x = 2;
	Draw_ExclamationPoint(translation, rotation, scale);
		
	// draw the center cow
	translation.x = 0;
	rotation.y = 1;
	rotation.w = -90;	
	Draw_Cow(translation, rotation, scale);
	
	// draw the orbiting cows
	translation.y = 1;
	
	double x, z;
	double angle = fmod(circling_cows_angle,360);
	ConvertCircleToCartesianCoord(angle, 1, &x, &z);	
	translation.x = x;
	translation.z = z;
	rotation.z = 1;
	rotation.w = -circling_cows_angle;
	scale.x = 0.25;
	scale.y = 0.25;
	scale.z = 0.25;
	Draw_Cow(translation, rotation, scale);
	
	angle = fmod(circling_cows_angle+90,360);
	ConvertCircleToCartesianCoord(angle, 1, &x, &z);
	translation.x = x;
	translation.z = z;
	rotation.y = 1;
	rotation.z = 1;
	rotation.w = circling_cows_angle;
	scale.x = 0.15;
	scale.y = 0.15;
	scale.z = 0.15;
	Draw_Cow(translation, rotation, scale);
	
	angle = fmod(circling_cows_angle+215,360);
	ConvertCircleToCartesianCoord(angle, 1, &x, &z);
	translation.x = x;
	translation.z = z;
	rotation.x = 1;
	rotation.y = 1;
	rotation.z = 1;
	rotation.w = 2 * circling_cows_angle;
	scale.x = 0.75;
	scale.y = 0.75;
	scale.z = 0.75;
	Draw_Cow(translation, rotation, scale);
	
	//  Five pixels from the lower left corner of the window
	glWindowPos2i(5,5);
	//  Print the text string
	Print("Angle=%d,%d",th,ph);
	
	//  Render the scene
	glFlush();
	
	//  Make the rendered scene visible
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
      
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   const double dim=2.5;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	circling_cows_angle = fmod(90*t,360);
	glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Assgn 3: Andrew Kessel");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
