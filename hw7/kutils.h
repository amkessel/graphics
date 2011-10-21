/***************************************************
 * kutils.h
 *
 * Andrew Kessel (hence the "k" before "utils" :)
 * Created 9/9/2011
 *
 * General utility definitions specifically for CSCI 5229.
 *
 ***************************************************/

#ifndef _KUTILS_H_
#define _KUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

/*********************************************************************
 *********************************************************************
 * MACROS
 *********************************************************************
 *********************************************************************/
 
#define PI 3.1415927

#define KUTILS_MIN2(A,B) (A < B) ? A : B
#define KUTILS_MAX2(A,B) (A >= B) ? A : B

#define KUTILS_COS(x) (cos((x)*PI/180))
#define KUTILS_SIN(x) (sin((x)*PI/180))

namespace kutils
{	
	/*******************************
	 * Classes and structs
	 *******************************/

	typedef struct point2_s
	{
		double x,y;
	} point2;

	typedef struct point3_s
	{
		double x,y,z;
	} point3;

	typedef struct point4_s
	{
		double x,y,z,w;
	} point4;
	
	/* Class that represents a 3d vector */
	class Vector3
	{
		private:
			double x, y, z;
		
		public:
			/* Constructor, given the x,y,z components of the vector */
			Vector3(double x, double y, double z);
			/* Constructor, given a point that represents the x,y,z components of the vector */
			Vector3(point3 point);
			/* Constructor, given two points such that the vector will point from p1 to p2 */
			Vector3(point3 p1, point3 p2);
			
			/* Accessor method for x */
			double Get_x();
			/* Accessor method for y */
			double Get_y();
			/* Accessor method for z */
			double Get_z();
			
			/* Gets the length of the vector */
			double Length();
			
			/* Finds the dot product of this vector with another */
			double Dot_Product(Vector3 *other);
			
			/* Finds the angle of this vector with another
			   degrees: should the answer be in degrees (or radians)?
			   normalized: are both vectors normalized? (better perf if they are) */
			double Angle(Vector3 *other, bool degrees, bool normalized);
			
			/* Normalizes this vector */
			void Normalize();
	};
	
	/*******************************
	 * Generic functions
	 *******************************/
			
	/* Finds the cross product of two vectors */
	Vector3 CrossProduct(Vector3 *a, Vector3 *b, bool normalize);
	
	/* Calculates on the target number line the value based on a source number line  
	   e.g. if val = 5 on a scale of 0 to 100, then val = 0.5 on a scale of 0 to 10 */
	double Transform_Scale(double minSrc, double maxSrc, double minTgt, double maxTgt, double val);

	/* Converts RGB to HSV color */
	void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v );
	
	/* Converts HSV to RGB color */
	void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v );
	
	/* Inverts the hue value (e.g. a hue of 90 will be inverted to 270) */
	double InvertHue(double hue);
	
	/* Computes the points on a circle of radius r (points are ordered counter-clockwise on x-y plane) */
	void ComputeCirclePoints(double r, point2 *points, int n);
	
	/* Computes the points on an arc spanning an angle of 'degrees' (points are ordered counter-clockwise on x-y plane) */
	void ComputeArchPoints(double r, double degrees, point2 *points, int n);
	
	/* Finds the cartesian coordinates of a point on a circle */
	void ConvertCircleToCartesianCoord(double theta, double r, double *x, double *y);
	
	/* Finds the normal between two vectors defined by (center,first) and (center,second) */
	point3 ComputeNormal(point3 first, point3 center, point3 second);
	
	/* Generate points for a 3D sine wave (y is "up" and the wave will travel in the x direction) */
	void ComputeSineWavePoints(double amp, double omega, double phi, double zmax, double dx,
							   point3 *points, point3 *normals, int npts);
							   
	/* Finds the average vector between two vectors */
	Vector3 AverageVectors(Vector3 a, Vector3 b);
	
	/* Finds the average vector between four vectors */
	Vector3 AverageVectors(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	
	/* Normalize a 2D vector represented by a point2 */
	point2 Normalize(point2 p);
	
	double TransformToTexCoord(double offset, double scale, double val);
}

#endif
