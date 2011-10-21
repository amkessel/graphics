#include "kutils.h"

using namespace kutils;

double kutils::TransformToTexCoord(double offset, double scale, double val)
{
	// move the value so that its reference point is zeroed
	// scale it down to the size of the texture map
	// 
	return (val-offset) * (1.0/scale) + 0.5;
}

//point2 kutils::CircleToTexCoord(double x_offset, double y_offset, double r, point2 p)
//{
//	p.x = (p.x-x_offset) * (0.5/r) + 0.5;
//	p.y = (p.y-y_offset) * (0.5/r) + 0.5;
//	return p;
//}

point2 kutils::Normalize(point2 p)
{
	double length = sqrt(p.x*p.x + p.y*p.y);
	p.x /= length;
	p.y /= length;
	return p;
}
							   
Vector3 kutils::AverageVectors(Vector3 a, Vector3 b)
{
	Vector3 avg ( (a.Get_x()+b.Get_x())/2, (a.Get_y()+b.Get_y())/2, (a.Get_z()+b.Get_z())/2 );
	return avg;
}
	
Vector3 kutils::AverageVectors(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{
	Vector3 avg1 = AverageVectors(a, b);
	Vector3 avg2 = AverageVectors(c, d);
	Vector3 avg = AverageVectors(avg1, avg2);
	return avg;
}


void kutils::ComputeSineWavePoints(double amp, double omega, double phi, double zmax, double dx,
								   point3 *points, point3 *normals, int npts)
{
	if(npts % 2 != 0)
		throw "kutils::ComputeSineWavePoints: npts must be divisible by 2";
		
	double x = 0;
	double zmin = 0;
	int pitch = npts/2;
	double ynorm = 1;
	for(int i = 0; i < npts/2; i++)
	{
		double y = amp * sin(omega * x + phi);
		double xnorm = -cos(omega * x + phi);
		point3 p1 = {x, y, zmin};
		point3 p2 = {x, y, zmax};
		point3 norm = {xnorm, ynorm, 0};
		points[i] = p1;
		points[i+pitch] = p2;
		normals[i] = norm;
		normals[i+pitch] = norm;
		
		x += dx;
	}
}
 
void kutils::ConvertCircleToCartesianCoord(double theta, double r, double *x, double *y)
{
	*x = r * KUTILS_COS(theta);
	*y = r * KUTILS_SIN(theta);
}

void kutils::ComputeArchPoints(double r, double degrees, point2 *points, int n)
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

void kutils::ComputeCirclePoints(double r, kutils::point2 *points, int n)
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

kutils::Vector3::Vector3(point3 point)
{
	this->x = point.x;
	this->y = point.y;
	this->z = point.z;
}

kutils::Vector3::Vector3(point3 p1, point3 p2)
{
	this->x = p2.x - p1.x;
	this->y = p2.y - p1.y;
	this->z = p2.z - p1.z;
}

double kutils::Vector3::Get_x() { return this->x; }
double kutils::Vector3::Get_y() { return this->y; }
double kutils::Vector3::Get_z() { return this->z; }

double kutils::Vector3::Dot_Product(Vector3 *that)
{
	return this->x*that->Get_x() + this->y*that->Get_y() + this->z*that->Get_z();
}

kutils::Vector3 kutils::CrossProduct(Vector3 *a, Vector3 *b, bool normalize)
{
	double x = a->Get_y()*b->Get_z() - a->Get_z()*b->Get_y();
	double y = a->Get_z()*b->Get_x() - a->Get_x()*b->Get_z();
	double z = a->Get_x()*b->Get_y() - a->Get_y()*b->Get_x();
	
	Vector3 result = Vector3(x,y,z);
	
	if(normalize)
	{
		result.Normalize();
	}
	
	return result;
}

point3 kutils::ComputeNormal(point3 first, point3 center, point3 second)
{
	Vector3 *a = new Vector3(center, first);
	Vector3 *b = new Vector3(center, second);
	
	Vector3 normal = kutils::CrossProduct(a, b, true);
	kutils::point3 result = {normal.Get_x(), normal.Get_y(), normal.Get_z()};
	
	delete a;
	delete b;
	
	return result;
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
