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

#include <GL/glut.h>
#include <math.h>

/***********************************
 ***********************************
 * MACROS
 ***********************************
 ***********************************/
 
#define KUTIL_MIN2(A,B) (A < B) ? A : B
#define KUTIL_MAX2(A,B) (A >= B) ? A : B

#define PI 3.14159

/***********************************
 ***********************************
 * DECLARATIONS
 ***********************************
 ***********************************/
 
namespace kutils
{
	typedef struct double_s
	{
		double x,y,z;
	} double_t;
	
	typedef struct triple_s
	{
		double x,y,z;
	} triple_t;

	typedef struct point2_s
	{
		double_t;
	} point2_t;

	typedef struct point3_s
	{
		triple_t;
	} point3_t;

	/* Convenience routine to output raster text
	   Use VARARGS to make this more flexible */
	void Print(const char* format , ...);
	
	/* Calculates on the target number line the value based on a source number line  
	   e.g. if val = 5 on a scale of 0 to 100, then val = 0.5 on a scale of 0 to 10 */
	double Transform_Scale(double minSrc, double maxSrc, double minTgt, double maxTgt, double val);

	/* Converts RGB to HSV color */
	void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v );
	
	/* Converts HSV to RGB color */
	void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v );
	
	/* Inverts the hue value (e.g. a hue of 90 will be inverted to 270) */
	double InvertHue(double hue);
	
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
	 * OpenGL drawing functions
	 *******************************/

	/* Draws the xyz axes on the display */
	void DrawAxes(float r, float g, float b, float len);
	
	/* Draws a 3D rectangle of dimensions (l,w,h), translated from the origin,
	   scaled and rotated (th) radians */
	void DrawRect3d(double l, double w, double h,
					triple_t translation, triple_t scale, triple_t rotation, double th);
}

/***********************************
 ***********************************
 * IMPLEMENTATIONS
 ***********************************
 ***********************************/
 
void DrawRect3d(double l, double w, double h, triple_t color
				triple_t translation, triple_t scale, triple_t rotation, double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(translation.x, translation.y, translation.z);
   glRotated(th, rotation.x, rotation.y, rotation.z);
   glScaled(scale.x, scale.y, scale.z);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
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

void kutils::DrawAxes(float r=1, float g=1, float b=1, float len=1)
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

#define KUTILS_LEN 8192  // Maximum length of text string
void kutils::Print(const char* format , ...)
{
   char    buf[KUTILS_LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,KUTILS_LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

#endif
