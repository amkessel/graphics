/***************************************************
 * kutils.h
 *
 * Andrew Kessel (hence the "k" before "utils" :)
 * Created 9/9/2011
 *
 * General utility definitions/implementations (in one package for easy portability)
 * specifically for CSCI 5229.
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

/*********************************************************************
 *********************************************************************
 * DECLARATIONS
 *********************************************************************
 *********************************************************************/
 
namespace kutils
{	
	/*******************************
	 * Classes and structs
	 *******************************/

	typedef struct point2_s
	{
		double x,y;
	} point2_t;

	typedef struct point3_s
	{
		double x,y,z;
	} point3_t;

	typedef struct point4_s
	{
		double x,y,z,w;
	} point4_t;
	
	/* Class that represents a 3d vector */
	class Vector3
	{
		private:
			double x, y, z;
		
		public:
			/* Constructor, given the x,y,z components of the vector */
			Vector3(double x, double y, double z);
			/* Constructor, given a point that represents the x,y,z components of the vector */
			Vector3(point3_t point);
			/* Constructor, given two points such that the vector will point from p1 to p2 */
			Vector3(point3_t p1, point3_t p2);
			
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
	
	/* Calculates on the target number line the value based on a source number line  
	   e.g. if val = 5 on a scale of 0 to 100, then val = 0.5 on a scale of 0 to 10 */
	double Transform_Scale(double minSrc, double maxSrc, double minTgt, double maxTgt, double val);

	/* Converts RGB to HSV color */
	void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v );
	
	/* Converts HSV to RGB color */
	void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v );
	
	/* Inverts the hue value (e.g. a hue of 90 will be inverted to 270) */
	double InvertHue(double hue);
	
	/* Computes the points on a circle of radius r */
	void ComputeCirclePoints(double r, point2_t *points, int n);
	
	/* Computes the points on an arc spanning an angle of 'degrees' */
	void ComputeArchPoints(double r, double degrees, point2_t *points, int n);
	
	/* Draw vertex in polar coordinates */
	void Vertex(double theta, double phi);
	
	/* Finds the cartesian coordinates of a point on a circle */
	void ConvertCircleToCartesianCoord(double theta, double r, double *x, double *y);
}

/*********************************************************************
 *********************************************************************
 * IMPLEMENTATIONS
 *********************************************************************
 *********************************************************************/
 
void kutils::ConvertCircleToCartesianCoord(double theta, double r, double *x, double *y)
{
	*x = r * KUTILS_COS(theta);
	*y = r * KUTILS_SIN(theta);
}
 
void kutils::Vertex(double theta, double phi)
{
   glColor3f(KUTILS_COS(theta)*KUTILS_COS(theta) , KUTILS_SIN(phi)*KUTILS_SIN(phi) , KUTILS_SIN(theta)*KUTILS_SIN(theta));
   glVertex3d(KUTILS_SIN(theta)*KUTILS_COS(phi) , KUTILS_SIN(phi) , KUTILS_COS(theta)*KUTILS_COS(phi));
}
 
void kutils::ComputeArchPoints(double r, double degrees, point2_t *points, int n)
{
	// compute the points using the parametric equations
	// x = a + r*cos(t)
	// y = b + r*sin(t)
	// where (a,b) is the origin of the circle, (0,0) for our purposes

	double dt = degrees / (n-1);
	
	for(int i = 0; i < n; i++)
	{
		points[i].x = r * KUTILS_COS(dt * i);
		points[i].y = r * KUTILS_SIN(dt * i);
	}
}

void kutils::ComputeCirclePoints(double r, kutils::point2_t *points, int n)
{
	if(n < 3)
	{
		throw "kutils ComputeCirclePoints: Not enough points to draw circle";
	}
	
	kutils::ComputeArchPoints(r, 360, points, n-1);
	
	// make the last point the same as the first
	points[n-1].x = points[0].x;
	points[n-1].y = points[0].y;
}
 
kutils::Vector3::Vector3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

kutils::Vector3::Vector3(point3_t point)
{
	this->x = point.x;
	this->y = point.y;
	this->z = point.z;
}

kutils::Vector3::Vector3(point3_t p1, point3_t p2)
{
	this->x = p2.x - p1.x;
	this->y = p2.y - p1.y;
	this->z = p2.z - p1.z;
}

double kutils::Vector3::Get_x() { return this->x; }
double kutils::Vector3::Get_y() { return this->y; }
double kutils::Vector3::Get_z() { return this->z; }

double kutils::Vector3::Dot_Product(Vector3 *other)
{
	return this->x*other->x + this->y*other->y + this->z*other->z;
}

double kutils::Vector3::Angle(Vector3 *other, bool degrees, bool normalized)
{
	double dot = this->Dot_Product(other);

	double factor = dot;
	if(!normalized)
		factor /= (this->Length() * other->Length());
		
	double angle = acos(factor);
	
	if(degrees)
		angle *= (180.0/PI);
	
	return angle;
}

double kutils::Vector3::Length()
{
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

void kutils::Vector3::Normalize()
{
	double length = this->Length();
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

/* This algorithm taken from http://www.cs.rit.edu/~ncs/color/t_convert.html */
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
// if s == 0, then h = -1 (undefined)
void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v )
{
	double min, max, delta;
	min = fmin( fmin(r, g), b );
	max = fmax( fmax(r, g), b );
	*v = max;				// v
	delta = max - min;
	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}

/* This algorithm taken from http://www.cs.rit.edu/~ncs/color/t_convert.html */
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
// if s == 0, then h = -1 (undefined)
void kutils::HSVtoRGB( double *r, double *g, double *b, double h, double s, double v )
{
	int i;
	double f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

double kutils::InvertHue(double hue)
{
	hue += 180.0;
	
	if(hue >= 360.0)
		hue -= 360.0;
		
	return hue;
}

double kutils::Transform_Scale(double minSrc, double maxSrc, double minTgt, double maxTgt, double val)
{
	return minTgt + (maxTgt - minTgt) * (val - minSrc) / (maxSrc - minSrc);
}

#endif
