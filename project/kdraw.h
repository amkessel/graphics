/***************************************************
 * kdraw.h
 *
 * Andrew Kessel (hence the "k" before "draw" :)
 * Created 9/21/2011
 *
 * General definitions for accessing OpenGL functionality, specifically for CSCI 5229.
 *
 ***************************************************/
 
#ifndef _KDRAW_H_
#define _KDRAW_H_
 
#include <GL/glut.h>
#include "kutils.h"

using namespace kutils;

namespace kdraw
{
	/* Convenience routine to output raster text
	   Use VARARGS to make this more flexible */
	void Print(const char* format , ...);
	
	/* Taken from the CSCIx229 library provided by Dr. Schreuder */
	void Project(double fov,double asp,double dim);
	
	/* TRANSFORMATIONS */
	
	/* Draws the xyz axes on the display */
	void DrawAxes(float r, float g, float b, float len);
	
	/* Performs the offset translations and scales */	
	void Transform(point3 *translations, point3 *scales, int ntrans, int nscales);

	/* Performs the offset translations, rotations and scales */	
	void Transform(point3 *translations, point4 *rotations, point3 *scales,
				   int ntrans, int nrots, int nscales);
				
	/* Performs the offset translations, rotations and scales */	
	void Transform(point3 translation, point4 rotation, point3 scale);
				
	/* Performs the offset translations and scales */	
	void Transform(point3 translation, point3 scale);
	
	/* 2D OBJECTS */
	
	void Circle(double r, int segs, point3 color,
				point3 *translations, point4 *rotations, point3 *scales,
				int ntrans, int nrots, int nscales);
	void Circle(double r, int segs, point3 color,
				point3 translation, point4 rotation, point3 scale);
	void Circle(double r, int segs, point3 color);
	
	void Triangle(point3 p1, point3 p2, point3 p3, point3 color,
				  point3 *translations, point4 *rotations, point3 *scales,
				  int ntrans, int nrots, int nscales);
	void Triangle(point3 p1, point3 p2, point3 p3, point3 color,
				  point3 translation, point4 rotation, point3 scale);
	void Triangle(point3 p1, point3 p2, point3 p3, point3 color);
	
	void Quad(point3 p1, point3 p2, point3 p3, point3 p4, point3 normal, point3 color);
	
	void QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch);
	void QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch,
			   point3 translation, point4 rotation, point3 scale);
	void QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch,
			   point3 *translations, point4 *rotations, point3 *scales,
			   int ntrans, int nrots, int nscales);
	
	/* 3D OBJECTS */
	
	/* Draws a 1x1x1 cube centered on (0,0,0) then transformed by the given transformations */		
	void Cube(point3 *translations, point4 *rotations, point3 *scales,
			  int ntrans, int nrots, int nscales);

	/* Draws a 1x1x1 cube centered on (0,0,0) then transformed by the given transformations */
	void Cube(point3 translation, point4 rotation, point3 scale);

	/* Draws an arch of depth 1 in the z-axis and thickness in the radial direction given by
	   r_outer and r_inner and spanning an arc given by degrees. The center point of the arch
	   is located at (0,0,0). It is then transformed by the given transformations. The granularity
	   of the line-approximation to a circle is given by segs. */
	void Arch(double r_outer, double r_inner, double degrees, int segs,
			  point3 *translations, point4 *rotations, point3 *scales,
			  int ntrans, int nrots, int nscales, bool endcaps);

	/* Draws an arch of depth 1 in the z-axis and thickness in the radial direction given by
	   r_outer and r_inner and spanning an arc given by degrees. The center point of the arch
	   is located at (0,0,0). It is then transformed by the given transformations. The granularity
	   of the line-approximation to a circle is given by segs. */
	void Arch(double r_outer, double r_inner, double degrees, int segs,
			  point3 translation, point4 rotation, point3 scale,
			  bool endcaps);

	/* Draws a cone of diameter 1 and height 1 then transformed by the given transformations */
	void Cone(int segs, point3 *translations, point4 *rotations, point3 *scales,
			  int ntrans, int nrots, int nscales);

	/* Draws a cone of diameter 1 and height 1 then transformed by the given transformations */
	void Cone(int segs, point3 translation, point4 rotation, point3 scale);
	
	/* Draw point in polar coordinates */
	void SpherePoint(double theta, double phi);
	
	/* Draws a unit sphere that is then transformed by the given transformations */
	void Sphere(int segs, point3 *translations, point3 *scales, int ntrans, int nscales);
	
	/* Draws a unit sphere that is then transformed by the given transformations */
	void Sphere(int segs, point3 translation, point3 scale);
}

#endif
