#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

void kdraw::Cube(point3 *translations, point4 *rotations, point3 *scales,
		  		 int ntrans, int nrots, int nscales)
{
	//  Save transformation
	glPushMatrix();

	//  Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);

	//  Cube
	glBegin(GL_QUADS);
	//  Front
//	glColor3f(1,0,0);
	glNormal3f(0, 0, 1);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(+0.5, -0.5, 0.5);
	glVertex3f(+0.5, +0.5, 0.5);
	glVertex3f(-0.5, +0.5, 0.5);
	//  Back
//	glColor3f(1,0,0);
	glNormal3f(0, 0, -1);
	glVertex3f(+0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, +0.5, -0.5);
	glVertex3f(+0.5, +0.5, -0.5);
	//  Right
//	glColor3f(0,1,0);
	glNormal3f(1, 0, 0);
	glVertex3f(+0.5, -0.5, +0.5);
	glVertex3f(+0.5, -0.5, -0.5);
	glVertex3f(+0.5, +0.5, -0.5);
	glVertex3f(+0.5, +0.5, +0.5);
	//  Left
//	glColor3f(0,1,0);
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, +0.5);
	glVertex3f(-0.5, +0.5, +0.5);
	glVertex3f(-0.5, +0.5, -0.5);
	//  Top
//	glColor3f(0,0,1);
	glNormal3f(0, 1, 0);
	glVertex3f(-0.5, +0.5, +0.5);
	glVertex3f(+0.5, +0.5, +0.5);
	glVertex3f(+0.5, +0.5, -0.5);
	glVertex3f(-0.5, +0.5, -0.5);
	//  Bottom
//	glColor3f(0,0,1);
	glNormal3f(0, -1, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(+0.5, -0.5, -0.5);
	glVertex3f(+0.5, -0.5, +0.5);
	glVertex3f(-0.5, -0.5, +0.5);
	//  End
	glEnd();
	//  Undo transofrmations
	glPopMatrix();
}

void kdraw::Cube(point3 translation, point4 rotation, point3 scale)
{
	point3 translations[1] = { translation };
	point4 rotations[1] = { rotation };
	point3 scales[1] = { scale };
	
	Cube(translations, rotations, scales, 1, 1, 1);
}

void kdraw::Arch(double r_outer, double r_inner, double degrees, int segs,
		  point3 *translations, point4 *rotations, point3 *scales,
		  int ntrans, int nrots, int nscales, bool endcaps)
{
	// compute the points for the arch
	
	int n = segs + 1; // the number of points is 1 plus the number of segments
	int nBytes = n * sizeof(point2);
	point2 *inner = (point2 *)malloc(nBytes);
	point2 *outer = (point2 *)malloc(nBytes);
	
	ComputeArchPoints(r_inner, degrees, inner, n);	
	ComputeArchPoints(r_outer, degrees, outer, n);
	
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// draw the front face
//	glColor3f(1,0,0);
	// the normal points in the same direction for all quads on this face, which is +z for the front
	glNormal3d(0,0,1);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		glVertex3d(inner[i].x, inner[i].y, 0.5);
		glVertex3d(outer[i].x, outer[i].y, 0.5);
	}
	glEnd();
	
	// draw the back face
	glBegin(GL_QUAD_STRIP);
	// the normal points in the same direction for all quads on this face, which is -z for the back
	glNormal3d(0,0,-1);

	for(int i = 0; i < n; i++)
	{
		glVertex3d(outer[i].x, outer[i].y, -0.5);
		glVertex3d(inner[i].x, inner[i].y, -0.5);
	}
	glEnd();
	
	// draw the outer rim
//	glColor3f(0,1,0);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		if(i < n-1)
		{
			point3 i_front   = { outer[i].x,   outer[i].y,   0.5 };
			point3 i_back    = { outer[i].x,   outer[i].y,  -0.5 };
			point3 ip1_front = { outer[i+1].x, outer[i+1].y, 0.5 };
			
			point3 normal = ComputeNormal(i_back, i_front, ip1_front);
			glNormal3d(normal.x, normal.y, normal.z);
		}
		glVertex3d(outer[i].x, outer[i].y, 0.5);
		glVertex3d(outer[i].x, outer[i].y, -0.5);
	}
	glEnd();
	
	// draw the inner rim
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < n; i++)
	{
		if(i < n-1)
		{		
			point3 i_front   = { outer[i].x,   outer[i].y,   0.5 };
			point3 i_back    = { outer[i].x,   outer[i].y,  -0.5 };
			point3 ip1_front = { outer[i+1].x, outer[i+1].y, 0.5 };
			
			point3 normal = ComputeNormal(ip1_front, i_front, i_back);
			glNormal3d(normal.x, normal.y, normal.z);
		}
		glVertex3d(inner[i].x, inner[i].y, -0.5);
		glVertex3d(inner[i].x, inner[i].y, 0.5);
	}
	glEnd();
	
	//draw the end caps
	if(endcaps)
	{
		glBegin(GL_QUADS);
		
//		glColor3f(0,0,1);
		point3 outer_top1 = { outer[0].x,   outer[0].y,  0.5 };
		point3 inner_top1 = { inner[0].x,   inner[0].y,  0.5 };
		point3 inner_bot1 = { inner[0].x,   inner[0].y, -0.5 };	
		point3 normal = ComputeNormal(inner_bot1, inner_top1, outer_top1);
		glNormal3d(normal.x, normal.y, normal.z);
			
		glVertex3d(outer[0].x, outer[0].y, 0.5);
		glVertex3d(outer[0].x, outer[0].y, -0.5);
		glVertex3d(inner[0].x, inner[0].y, -0.5);
		glVertex3d(inner[0].x, inner[0].y, 0.5);
		
		point3 inner_bot2 = { inner[n-1].x, inner[n-1].y, -0.5 };
		point3 inner_top2 = { inner[n-1].x, inner[n-1].y,  0.5 };
		point3 outer_top2 = { outer[n-1].x, outer[n-1].y,  0.5 };	
		normal = ComputeNormal(outer_top2, inner_top2, inner_bot2);
		glNormal3d(normal.x, normal.y, normal.z);
		
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
		  point3 translation, point4 rotation, point3 scale,
		  bool endcaps)
{
	point3 translations[1] = { translation };
	point4 rotations[1] = { rotation };
	point3 scales[1] = { scale };
	
	Arch(r_outer, r_inner, degrees, segs, translations, rotations, scales, 1, 1, 1, endcaps);
}
 
void kdraw::Cone(int segs, point3 *translations, point4 *rotations, point3 *scales,
		  int ntrans, int nrots, int nscales)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// get the points around the base of the cone (diameter = 1, radius = 0.5)
	int nBytes = (segs + 1)*sizeof(point2);
	point2 *points = (point2 *)malloc(nBytes);
	
//	point3 color = {1,1,1};
//	point3 translation = {0,0,0};
//	point4 rotation = {1, 0, 0, 180};
//	point3 scale = {1, 1, 1};
//	Circle(0.5, segs, color, translation, rotation, scale);
	
	ComputeCirclePoints(0.5, points, segs+1);
	
	// draw the flat face
	glColor3f(1,1,1);
	glNormal3f(0,0,-1);
	glBegin(GL_POLYGON);
	for(int i = 0; i < segs+1; i++)
		glVertex3d(points[i].x, points[i].y, 0);
	glEnd();
	
	// draw the sides
	point3 tip = {0, 0, 1};
//	glBegin(GL_TRIANGLES);
	for(int i = 0; i < segs; i++) // the base of the cone
	{
		point3 base1 = {points[i].x,   points[i].y,   0};
		point3 base2 = {points[i+1].x, points[i+1].y, 0};
	
		point3 normal = ComputeNormal(base1, tip, base2);
		
		glBegin(GL_POLYGON);
		glNormal3d(normal.x, normal.y, normal.z);
		glVertex3d(base1.x,  base1.y,  base1.z);
		glVertex3d(tip.x,	 tip.y,	   tip.z);
		glVertex3d(base2.x,  base2.y,  base1.z);
		glEnd();
	}
//	glEnd();
	
	// Undo transformations
	glPopMatrix();
}

void kdraw::Cone(int segs, point3 translation, point4 rotation, point3 scale)
{
	point3 translations[1] = { translation };
	point4 rotations[1] = { rotation };
	point3 scales[1] = { scale };
	
	Cone(segs, translations, rotations, scales, 1, 1, 1);
}

void kdraw::SpherePoint(double theta, double phi)
{
	double x = KUTILS_SIN(theta)*KUTILS_COS(phi);
	double y = KUTILS_COS(theta)*KUTILS_COS(phi);
	double z = KUTILS_SIN(phi);

//	glColor3f(KUTILS_COS(theta)*KUTILS_COS(theta) , KUTILS_SIN(phi)*KUTILS_SIN(phi) , KUTILS_SIN(theta)*KUTILS_SIN(theta));
//	glVertex3d(KUTILS_SIN(theta)*KUTILS_COS(phi) , KUTILS_SIN(phi) , KUTILS_COS(theta)*KUTILS_COS(phi));
	
	glNormal3d(x, y, z);
	glVertex3d(x, y, z);
}

void kdraw::Sphere(int segs, point3 *translations, point3 *scales, int ntrans, int nscales)
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
	SpherePoint(0,-90);
	for (th=0;th<=360;th+=d)
	{
		SpherePoint(th,d-90);
	}
	glEnd();

	//  Latitude bands
	for (ph=d-90;ph<=90-2*d;ph++)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=d)
		{
			SpherePoint(th,ph);
			SpherePoint(th,ph+d);
		}
		glEnd();
	}

	//  North pole cap
	glBegin(GL_TRIANGLE_FAN);
	SpherePoint(0,90);
	for (th=0;th<=360;th+=d)
	{
		SpherePoint(th,90-d);
	}
	glEnd();

	//  Undo transformations
	glPopMatrix();
}

void kdraw::Sphere(int segs, point3 translation, point3 scale)
{
	point3 translations[1] = { translation };
	point3 scales[1] = { scale };
	Sphere(segs, translations, scales, 1, 1);
}

