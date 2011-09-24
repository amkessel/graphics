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
#include "hw3_objects.h"

using namespace kutils;
using namespace kdraw;		  

enum ProjectionMode
{
	ORTHO = 0,
	PERSP = 1
};

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
bool axes = true; //  Display axes
double circling_cows_angle = 0.0; // rotation of cows
ProjectionMode mode = PERSP;
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world


static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode == PERSP) // being explicit here
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
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
	//  Perspective - set eye position
	if (mode == PERSP)
	{
		double Ex = -2*dim*KUTILS_SIN(th)*KUTILS_COS(ph);
		double Ey = +2*dim*KUTILS_SIN(ph);
		double Ez = +2*dim*KUTILS_COS(th)*KUTILS_COS(ph);
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,KUTILS_COS(ph),0);
	}
	//  Orthogonal - set world orientation
	else
	{
		glRotatef(ph,1,0,0);
		glRotatef(th,0,1,0);
	}
	//  Set view angle
//	glRotatef(ph,1,0,0);
//	glRotatef(th,0,1,0);
   
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
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP)
		dim += 0.1;
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
		dim -= 0.1;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Update projection
	Project();
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
	else if (ch == 'm')
//		(int)mode ^= 1; // concise way to toggle between ORTHO (=0) and PERSP (=1)
		mode = (mode == PERSP) ? ORTHO : PERSP;

	//  Update projection
	Project();
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
	Project();
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
   glutCreateWindow("Assgn 4: Andrew Kessel");
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
