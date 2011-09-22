/***************************************************
 * kdraw.h
 *
 * Andrew Kessel (hence the "k" before "draw" :)
 * Created 9/21/2011
 *
 * General definitions/implementations (in one package for easy portability)
 * for accessing OpenGL functionality, specifically for CSCI 5229.
 *
 ***************************************************/
 
#ifndef _KDRAW_H_
#define _KDRAW_H_
 
#include <GL/glut.h>
#include "kutils.h"

/*********************************************************************
 *********************************************************************
 * DECLARATIONS
 *********************************************************************
 *********************************************************************/
using namespace kutils;

namespace kdraw
{
	/* Convenience routine to output raster text
	   Use VARARGS to make this more flexible */
	void Print(const char* format , ...);
	
	/* Draws the xyz axes on the display */
	void DrawAxes(float r, float g, float b, float len);
	
	/* Performs the offset translations and scales */	
	void Transform(point3_t *translations, point3_t *scales, int ntrans, int nscales);

	/* Performs the offset translations, rotations and scales */	
	void Transform(point3_t *translations, point4_t *rotations, point3_t *scales,
				   int ntrans, int nrots, int nscales);
				
	/* Performs the offset translations, rotations and scales */	
	void Transform(point3_t translation, point4_t rotation, point3_t scale);
				
	/* Performs the offset translations and scales */	
	void Transform(point3_t translation, point3_t scale);
	
	/* Draws a 1x1x1 cube centered on (0,0,0) then transformed by the given transformations */		
	void Cube(point3_t *translations, point4_t *rotations, point3_t *scales,
			  int ntrans, int nrots, int nscales);

	/* Draws a 1x1x1 cube centered on (0,0,0) then transformed by the given transformations */
	void Cube(point3_t translation, point4_t rotation, point3_t scale);

	/* Draws an arch of depth 1 in the z-axis and thickness in the radial direction given by
	   r_outer and r_inner and spanning an arc given by degrees. The center point of the arch
	   is located at (0,0,0). It is then transformed by the given transformations. The granularity
	   of the line-approximation to a circle is given by segs. */
	void Arch(double r_outer, double r_inner, double degrees, int segs,
			  point3_t *translations, point4_t *rotations, point3_t *scales,
			  int ntrans, int nrots, int nscales, bool endcaps);

	/* Draws an arch of depth 1 in the z-axis and thickness in the radial direction given by
	   r_outer and r_inner and spanning an arc given by degrees. The center point of the arch
	   is located at (0,0,0). It is then transformed by the given transformations. The granularity
	   of the line-approximation to a circle is given by segs. */
	void Arch(double r_outer, double r_inner, double degrees, int segs,
			  point3_t translation, point4_t rotation, point3_t scale,
			  bool endcaps);

	/* Draws a cone of diameter 1 and height 1 then transformed by the given transformations */
	void Cone(int segs, point3_t *translations, point4_t *rotations, point3_t *scales,
			  int ntrans, int nrots, int nscales);

	/* Draws a cone of diameter 1 and height 1 then transformed by the given transformations */
	void Cone(int segs, point3_t translation, point4_t rotation, point3_t scale);
	
	/* Draws a unit sphere that is then transformed by the given transformations */
	void Sphere(int segs, point3_t *translations, point3_t *scales, int ntrans, int nscales);
	
	/* Draws a unit sphere that is then transformed by the given transformations */
	void Sphere(int segs, point3_t translation, point3_t scale);
}

/*********************************************************************
 *********************************************************************
 * IMPLEMENTATIONS
 *********************************************************************
 *********************************************************************/

void kdraw::Cube(point3_t *translations, point4_t *rotations, point3_t *scales,
		  		 int ntrans, int nrots, int nscales)
{
   //  Save transformation
   glPushMatrix();
   
   //  Offset
   Transform(translations, rotations, scales, ntrans, nrots, nscales);
   
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-0.5, -0.5, 0.5);
   glVertex3f(+0.5, -0.5, 0.5);
   glVertex3f(+0.5, +0.5, 0.5);
   glVertex3f(-0.5, +0.5, 0.5);
   //  Back
   glColor3f(1,0,0);
   glVertex3f(+0.5, -0.5, -0.5);
   glVertex3f(-0.5, -0.5, -0.5);
   glVertex3f(-0.5, +0.5, -0.5);
   glVertex3f(+0.5, +0.5, -0.5);
   //  Right
   glColor3f(0,1,0);
   glVertex3f(+0.5, -0.5, +0.5);
   glVertex3f(+0.5, -0.5, -0.5);
   glVertex3f(+0.5, +0.5, -0.5);
   glVertex3f(+0.5, +0.5, +0.5);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-0.5, -0.5, -0.5);
   glVertex3f(-0.5, -0.5, +0.5);
   glVertex3f(-0.5, +0.5, +0.5);
   glVertex3f(-0.5, +0.5, -0.5);
   //  Top
   glColor3f(0,0,1);
   glVertex3f(-0.5, +0.5, +0.5);
   glVertex3f(+0.5, +0.5, +0.5);
   glVertex3f(+0.5, +0.5, -0.5);
   glVertex3f(-0.5, +0.5, -0.5);
   //  Bottom
   glColor3f(0,0,1);
   glVertex3f(-0.5, -0.5, -0.5);
   glVertex3f(+0.5, -0.5, -0.5);
   glVertex3f(+0.5, -0.5, +0.5);
   glVertex3f(-0.5, -0.5, +0.5);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

void kdraw::Cube(point3_t translation, point4_t rotation, point3_t scale)
{
	point3_t translations[1] = { translation };
	point4_t rotations[1] = { rotation };
	point3_t scales[1] = { scale };
	
	Cube(translations, rotations, scales, 1, 1, 1);
}

void kdraw::Arch(double r_outer, double r_inner, double degrees, int segs,
		  point3_t *translations, point4_t *rotations, point3_t *scales,
		  int ntrans, int nrots, int nscales, bool endcaps)
{
	// compute the points for the arch
	
	int n = segs + 1; // the number of points is 1 plus the number of segments
	int nBytes = n * sizeof(point2_t);
	point2_t *inner = (point2_t *)malloc(nBytes);
	point2_t *outer = (point2_t *)malloc(nBytes);
	
	ComputeArchPoints(r_inner, degrees, inner, n);	
	ComputeArchPoints(r_outer, degrees, outer, n);
	
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// draw the front face
	glColor3f(1,0,0);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		glVertex3d(inner[i].x, inner[i].y, 0.5);
		glVertex3d(outer[i].x, outer[i].y, 0.5);
	}
	glEnd();
	
	// draw the back face
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		glVertex3d(outer[i].x, outer[i].y, -0.5);
		glVertex3d(inner[i].x, inner[i].y, -0.5);
	}
	glEnd();
	
	// draw the outer rim
	glColor3f(0,1,0);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		glVertex3d(outer[i].x, outer[i].y, 0.5);
		glVertex3d(outer[i].x, outer[i].y, -0.5);
	}
	glEnd();
	
	// draw the inner rim
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		glVertex3d(inner[i].x, inner[i].y, -0.5);
		glVertex3d(inner[i].x, inner[i].y, 0.5);
	}
	glEnd();
	
	//draw the end caps
	if(endcaps)
	{
		glColor3f(0,0,1);
		glBegin(GL_QUADS);
		glVertex3d(outer[0].x, outer[0].y, 0.5);
		glVertex3d(outer[0].x, outer[0].y, -0.5);
		glVertex3d(inner[0].x, inner[0].y, -0.5);
		glVertex3d(inner[0].x, inner[0].y, 0.5);
		
		glVertex3d(outer[n-1].x, outer[n-1].y, 0.5);
		glVertex3d(outer[n-1].x, outer[n-1].y, -0.5);
		glVertex3d(inner[n-1].x, inner[n-1].y, -0.5);
		glVertex3d(inner[n-1].x, inner[n-1].y, 0.5);
		glEnd();
	}
	
	// Undo transformations
	glPopMatrix();
	
	// cleanup
	free(inner);
	free(outer);
}

void kdraw::Arch(double r_outer, double r_inner, double degrees, int segs,
		  point3_t translation, point4_t rotation, point3_t scale,
		  bool endcaps)
{
	point3_t translations[1] = { translation };
	point4_t rotations[1] = { rotation };
	point3_t scales[1] = { scale };
	
	Arch(r_outer, r_inner, degrees, segs, translations, rotations, scales, 1, 1, 1, endcaps);
}
 
void kdraw::Cone(int segs, point3_t *translations, point4_t *rotations, point3_t *scales,
		  int ntrans, int nrots, int nscales)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// get the points around the base of the cone (diameter = 1, radius = 0.5)
	int nBytes = (segs + 1)*sizeof(point2_t);
	point2_t *points = (point2_t *)malloc(nBytes);
	
	ComputeCirclePoints(0.5, points, segs+1);
	
	// draw the flat face
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);
	for(int i = 0; i < segs+1; i++)
		glVertex3d(points[i].x, points[i].y, 0);
	glEnd();
	
	// draw the sides
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 1); // the tip of the cone
	for(int i = 0; i < segs+1; i++) // the base of the cone
	{
		double hue = 360 * ((double)i)/(segs+1);
		double sat = 1;
		double val = 1;
		double r, g, b;
		HSVtoRGB(&r, &g, &b, hue, sat, val);
		glColor3f(r,g,b);
		glVertex3d(points[i].x, points[i].y, 0);
	}
	glEnd();
	
	// Undo transformations
	glPopMatrix();
}

void kdraw::Cone(int segs, point3_t translation, point4_t rotation, point3_t scale)
{
	point3_t translations[1] = { translation };
	point4_t rotations[1] = { rotation };
	point3_t scales[1] = { scale };
	
	Cone(segs, translations, rotations, scales, 1, 1, 1);
}

void kdraw::Sphere(int segs, point3_t *translations, point3_t *scales, int ntrans, int nscales)
{
	const int d=5;
	int th,ph;
	segs = 1;

	//  Save transformation
	glPushMatrix();
	
	//  Offset and scale
	Transform(translations, scales, ntrans, nscales);

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

void kdraw::Sphere(int segs, point3_t translation, point3_t scale)
{
	point3_t translations[1] = { translation };
	point3_t scales[1] = { scale };
	Sphere(segs, translations, scales, 1, 1);
}
	 
void kdraw::Transform(point3_t *translations, point4_t *rotations, point3_t *scales,
	  int ntrans, int nrots, int nscales)
{
	for(int i = 0; i < ntrans; i++)
		glTranslated(translations[i].x, translations[i].y, translations[i].z);
	for(int i = 0; i < nrots; i++)
		glRotated(rotations[i].w, rotations[i].x, rotations[i].y, rotations[i].z);
	for(int i = 0; i < nscales; i++)
		glScaled(scales[i].x, scales[i].y, scales[i].z);
}

void kdraw::Transform(point3_t *translations, point3_t *scales, int ntrans, int nscales)
{
	point4_t rotation = {0,0,0,0};
	point4_t rotations[1] = { rotation };
	Transform(translations, rotations, scales, ntrans, 1, nscales);
}

void kdraw::Transform(point3_t translation, point4_t rotation, point3_t scale)
{
	point3_t translations[1] = { translation };
	point4_t rotations[1] = { rotation };
	point3_t scales[1] = { scale };
	
	kdraw::Transform(translations, rotations, scales, 1, 1, 1);
}

void kdraw::Transform(point3_t translation, point3_t scale)
{
	point4_t rotation = {0,0,0,0};	
	kdraw::Transform(translation, rotation, scale);
}

void kdraw::DrawAxes(float r=1, float g=1, float b=1, float len=1)
{
	//  Draw axes in white
	glColor3f(r,g,b);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(len,0,0);
	glVertex3d(0,0,0);
	glVertex3d(0,len,0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,len);
	glEnd();
	//  Label axes
	glRasterPos3d(len,0,0);
	Print("X");
	glRasterPos3d(0,len,0);
	Print("Y");
	glRasterPos3d(0,0,len);
	Print("Z");
}

#define KDRAW_LEN 8192  // Maximum length of text string
void kdraw::Print(const char* format , ...)
{
   char    buf[KDRAW_LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,KDRAW_LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
 
#endif
