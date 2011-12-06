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

// model files
//#include "spaceship3ds.h"
//#include "shuttle.h"
//#include "enterprise.h"
#include "falcon.h"
#include "CSCIx229.h"
#include "planets.h"

using namespace kdraw;
using namespace kutils;

int axes=1;       //  Display axes
int move=1;       //  Move light
int th=-45;         //  Azimuth of view angle
int ph=20;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
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

point3 no_translation = {0,0,0};
point4 no_rotation = {0,0,0,0};
point3 no_scale = {1,1,1};

void draw_comet()
{
	Draw_Comet(no_translation, no_rotation, no_scale, 5, 0.5);
}

void draw_model()
{
	Vector3 x(1,0,0);
	Vector3 m(-1,1,0);
	printf("%g\n",m.Angle(&x,true,false));

	//draw_comet();
//	Draw_falcon();
//	Draw_enterprise();
//	draw_spaceship3ds();
//	Draw_shuttle();
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
	double Ex = -2*dim*KUTILS_SIN(th)*KUTILS_COS(ph);
	double Ey = +2*dim        *KUTILS_SIN(ph);
	double Ez = +2*dim*KUTILS_COS(th)*KUTILS_COS(ph);
	gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,KUTILS_COS(ph),0);

	// smooth shading
	glShadeModel(GL_SMOOTH);

	//  Light switch
	if (light)
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
	else
		glDisable(GL_LIGHTING);

	//  Draw scene
	draw_model();				
						   
	//  Draw axes - no lighting from here on
	glColor3d(1,1,1);
	glDisable(GL_LIGHTING);
	if (axes)
   		DrawAxes(1,1,1,1);
   		
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

   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim -= 0.1;
   else if (key == GLUT_KEY_F1)
      distance = (distance==1) ? 5 : 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
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
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
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
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
	glutCreateWindow("Modeler");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	//  Load textures
	for(int i = 0; i < NUM_TEXS; i++)
	{
		falcon_tex[i] = LoadTexBMP(falcon_tex_names[i]);
	}
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}
