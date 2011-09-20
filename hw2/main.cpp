/***************************************************
 * main.cpp
 *
 * Andrew Kessel
 * Created 9/9/2011
 *
 * Main program file for CSCI 5229, Homework 2
 *
 ***************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include "kutils.h"
#include "lorenz.h"

#define POINT_SIZE 2

using namespace kutils;

const double HUE_MIN = 0.0;
const double HUE_MAX = 240.0;

/* Defines how the points will be colored */
enum ColorMode
{
	SOLID,
	ORDERED,
	DENSITY,
	GRADIENT
};

// I like to preface my global variables with "_" so that I always know that I'm dealing with a global

point3_t *_points;
double *_distancesSq; // we only need to know relative distances, so bypassing the sqrt is fine
double *_gradients;

int _nPoints = 50000; // # of points in the lorenz generator
double _sigma  = 10;
double _beta  = 2.6666;
double _rho  = 28;
double _x = 0.1;
double _y = 0.1;
double _z = 0.1;
double _dt = 0.001;
bool _regenerate = false; // controls whether the points will be recalculated or not

// variables for making sure the color spectrum spans only from the max to min
// (i.e. to make sure we get the most granular spectrum of colors across the values)
double _minDistSq_all, _maxDistSq_all;
double _minGrad_all, _maxGrad_all;

// viewing variables
int _th=0;       		// Azimuth of view angle
int _ph=0;       		// Elevation of view angle
double _w=10.0;  		// W variable
double _dim=2;   		// Dimension of orthogonal box
ColorMode _mode=DENSITY; // How the points are colored

/* Gets the text to display for the current color mode */
const char* get_color_mode_name()
{
	const char *result;
	switch(_mode)
	{
		case DENSITY:
			result = "Density";
			break;
		case GRADIENT:
			result = "Gradient";
			break;
		case ORDERED:
			result = "Ordered";
			break;
		default: // SOLID
			result = "Solid";
			break;	
	}
	return result;
}

/* returns the hue based on the the angle between the vectors pointing to this point's neighbors */
double calculate_gradient_hue(int i)
{
	double hue = Transform_Scale(_minGrad_all, _maxGrad_all, HUE_MIN, HUE_MAX, _gradients[i]);
	hue = InvertHue(hue); // invert so that red means a tight gradient as opposed to blue
	return hue;
}

/* returns the hue based on the distance of this point to its closest neighbor */
double calculate_density_hue(int i)
{
	double hue = Transform_Scale(_minDistSq_all, _maxDistSq_all, HUE_MIN, HUE_MAX, _distancesSq[i]);
	return hue;
}

/* Sets the glColor state for the i-th point */
void set_glColor3f(int i)
{
	double r,g,b; // RGB values
	
	double hue = 0.0; // default color mode is SOLID, which will display as red
	double sat = 1.0;
	double val = 1.0;
	
	if(_mode == ORDERED)
		hue = 360.0 * i / _nPoints;
	else if(_mode == DENSITY)
		hue = calculate_density_hue(i);
	else if(_mode == GRADIENT)
		hue = calculate_gradient_hue(i);
	
	// transform the HSV color (which is easy to calculate) into RGB (which is easy for OpenGL)
	HSVtoRGB(&r, &g, &b, hue, sat, val);
	
	// set the OpenGl color state
	glColor3f(r,g,b);
}

/*
 *  Display the scene
 */
void display()
{
	//  Clear the image
	glClear(GL_COLOR_BUFFER_BIT);
	//  Reset previous transforms
	glLoadIdentity();
	//  Set view angle
	glRotated(_ph,1,0,0);
	glRotated(_th,0,1,0);

	// generate lorenz points
	if(_regenerate)
	{
		// build up the input struct
		lorenz::lorenz_input_t input;
		input.sigma = _sigma;
		input.beta = _beta;
		input.rho = _rho;
		input.x = _x;
		input.y = _y;
		input.z = _z;
		input.dt = _dt;
		input.nPoints = _nPoints;
		input.points = _points;
		input.distancesSq = _distancesSq;
		input.gradients = _gradients;
		input.minDistSq = &_minDistSq_all;
		input.maxDistSq= &_maxDistSq_all;
		input.minGrad = &_minGrad_all;
		input.maxGrad= &_maxGrad_all;
		
		lorenz::generate_lorenz(input);
		
		_regenerate = false;
	}

	//  Draw points (color will be determined by the mode)
	glPointSize(POINT_SIZE);
	glBegin(GL_POINTS);

	// create the OpenGL points
	for(int i = 0; i < _nPoints; i++)
	{
		point3_t p = _points[i];
		set_glColor3f(i);
		glVertex4d(p.x,p.y,p.z,_w);
	}

	glEnd();
	
	// Draw axes
	DrawAxes();

	// Display parameter values
	glWindowPos2i(5,5);
	Print("View Angle=%d,%d; w=%.2f; Color mode=%s\n",_th,_ph,_w,get_color_mode_name());
	glWindowPos2i(5,25);
	Print("Sigma=%.2f, Beta=%.2f, Rho=%.2f\n", _sigma, _beta, _rho);

	//  Flush and swap
	glFlush();
	glutSwapBuffers();
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
		_th = _ph = 0;
	// Set the color mode to solid
	else if (ch == '1')
		_mode = SOLID;
	// Set the color mode to ordered
	else if(ch == '2')
		_mode = ORDERED;
	// Set the color mode to denisty
	else if(ch == '3')
		_mode = DENSITY;
	// Set the color mode to gradient
	else if(ch == '4')
		_mode = GRADIENT;
	//  Increase w by 0.1
	else if (ch == '+')
		_w += 0.1;
	//  Decrease w by 0.1
	else if (ch == '-')
		_w -= 0.1;
	// increase sigma by 5%
	else if (ch == 'q')
	{
		_sigma *= 1.05;
		_regenerate = true;
	}
	// decrease sigma by 5%
	else if (ch == 'a')
	{
		_sigma *= 0.95;
		_regenerate = true;
	}
	// increase beta by 5%
	else if (ch == 'w')
	{
		_beta *= 1.05;
		_regenerate = true;
	}
	// decrease beta by 5%
	else if (ch == 's')
	{
		_beta *= 0.95;
		_regenerate = true;
	}
	// increase rho by 5%
	else if (ch == 'e')
	{
		_rho *= 1.05;
		_regenerate = true;
	}
	// decrease rho by 5%
	else if (ch == 'd')
	{
		_rho *= 0.95;
		_regenerate = true;
	}
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      _th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      _th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      _ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      _ph -= 5;
   //  Keep angles to +/-360 degrees
   _th %= 360;
   _ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-_dim*w2h,+_dim*w2h, -_dim,+_dim, -_dim*10,+_dim*10);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Main
 */
int main(int argc, char *argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Assgn 2: Andrew Kessel");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   
   // allocate memory for the lorenz points and set flag to regenerate
   printf("creating points...\n");
   int nBytes = _nPoints * sizeof(point3_t);
   _points = (point3_t*)malloc(nBytes);
   _distancesSq = (double*)malloc(nBytes);
   _gradients = (double*)malloc(nBytes);
   _regenerate = true;
   
   //  Pass control to GLUT so it can interact with the user
   printf("entering main glut loop...\n");
   glutMainLoop();
   printf("exiting main glut loop...\n");
   
   // clean up
   printf("cleaning up...\n");
   if(_points != NULL)
   	free(_points);
   if(_distancesSq != NULL)
   	free(_distancesSq);
   if(_gradients != NULL)
   	free(_gradients);
   
   //  Return code
   return 0;
}
