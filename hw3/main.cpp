/*
 *  Object Examples
 *  'm' to switch modes (objects)
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 */
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
double zh=0;      //  Rotation of teapot
int axes=1;       //  Display axes
int mode=0;       //  What to display

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

using namespace kutils;
using namespace kdraw;

/*
 *  Draw vertex in polar coordinates
 */
 /*
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
*/

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
 /*
static void sphere1(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph++)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}
*/

/*
 *  Draw a sphere (version 2)
 *     at (x,y,z)
 *     radius (r)
 */
/*
static void sphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}
*/		  

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
	point3_t trans_left = {-0.25, 0.5, 0};
	point3_t scale_left = {0.707, 0.2, 0.2};
	point4_t rot_left = {0, 0, 1, -45};
	
	point3_t trans_mid = {0, -0.15, 0};
	point3_t scale_mid = {0.2, 0.8, 0.2};
	point4_t rot_mid = {0, 0, 0, 0};
	
	point3_t trans_right = {0.25, 0.5, 0};
	point3_t scale_right = {0.707, 0.2, 0.2};
	point4_t rot_right = {0, 0, 1, 45};
	
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
	
	Arch(r_outer, r_inner, degrees, segs, trans_top, rot_top, scale_top, false);
	Arch(r_outer, r_inner, degrees, segs, trans_bottoms, rot_bottoms, scale_bottoms, 1, 2, 1, false);
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
	
	// Undo transformations
	glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
//   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   //  Decide what to draw
   point3_t translation = {0, 0, 0};
   point4_t rotation = {1, 0, 0, 0};
   point3_t scale = {1, 1, 1};
//   Cone(1.0, 20, translation, scale, rotation);
//	Arch(1.0, 0.75, PI/4, 5, translation, scale, rotation, false);
//	Draw_H(translation, rotation, scale);
//	Draw_O(translation, rotation, scale);
//	Draw_L(translation, rotation, scale);
//	Draw_Y(translation, rotation, scale);
	Draw_C(translation, rotation, scale);
/*   switch (mode)
   {
      //  Draw cubes
      case 0:
         cube(0,0,0 , 0.3,0.3,0.3 , 0);
         cube(1,0,0 , 0.2,0.2,0.2 , 45);
         cube(0,1,0 , 0.4,0.4,0.2 , 90);
         break;
      //  Draw spheres
      case 1:
         sphere1(0,0,0 , 0.4);
         sphere1(1,0,0 , 0.2);
         sphere2(0,1,0 , 0.2);
         break;
      //  Line airplane
      case 2:
         PolyPlane(GL_LINE_LOOP , 0,0,0);
         break;
      //  Polygon airplane
      case 3:
         PolyPlane(GL_POLYGON , 0,0,0);
         break;
      //  Three flat airplanes
      case 4:
         FlatPlane( 0.0, 0.0, 0.0);
         FlatPlane(-0.5, 0.5,-0.5);
         FlatPlane(-0.5,-0.5,-0.5);
         break;
      // Three solid airplanes
      case 5:
         SolidPlane( 0, 0, 0 , 1,0,0 , 0, 1,0);
         SolidPlane(-1, 1, 0 ,-1,0,0 , 0,-1,0);
         SolidPlane(-1,-1, 0 ,-1,0,0 , 0, 1,0);
         break;
      // Mix of objects
      case 6:
         //  Cube
         cube(-1,0,0 , 0.3,0.3,0.3 , 3*zh);
         //  Ball
         sphere1(0,0,0 , 0.3);
         //  Solid Airplane
         SolidPlane(Cos(zh),Sin(zh), 0 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
         //  Utah Teapot
         glPushMatrix();
         glTranslatef(0,0,-1);
         glRotatef(zh,0,1,0);
         glColor3f(Cos(zh)*Cos(zh),0,Sin(zh)*Sin(zh));
         glutSolidTeapot(0.5);
         glPopMatrix();
         break;
   }
   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   */
   DrawAxes(1,1,1,1.5);
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
   //  Switch display mode
   else if (ch == 'm')
      mode = (mode+1)%7;
   else if (ch == 'M')
      mode = (mode+6)%7;
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
   zh = fmod(90*t,360);
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
   glutCreateWindow("Objects");
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
