#include "kutils.h"
#include "kdraw.h"	

using namespace kutils;
using namespace kdraw;

void kdraw::Quad(point3 p1, point3 p2, point3 p3, point3 p4, point3 normal, point3 color)
{
	glColor3f(color.x, color.y, color.z);
	
	glBegin(GL_POLYGON);
	//glNormal3d(normal.x, normal.y, normal.z);
	glVertex3d(p1.x, p1.y, p1.z);
	glVertex3d(p2.x, p2.y, p2.z);
	glVertex3d(p3.x, p3.y, p3.z);
	glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
}

void kdraw::QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch)
{
	point3 translation = {0,0,0};
	point4 rotation = {0,0,0,0};
	point3 scale = {1,1,1};
	QuadSheet(points, normals, color, npoints, pitch, translation, rotation, scale);
}

void kdraw::QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch,
				  point3 translation, point4 rotation, point3 scale)
{
	QuadSheet(points, normals, color, npoints, pitch, &translation, &rotation, &scale, 1, 1, 1);
}
 
void kdraw::QuadSheet(point3 *points, point3 *normals, point3 color, int npoints, int pitch,
				  point3 *translations, point4 *rotations, point3 *scales,
				  int ntrans, int nrots, int nscales)
{
	if(npoints < 2*pitch)
		throw "kdraw::Sheet: Not enough points relative to pitch";
	if(npoints % pitch != 0)
		throw "kdraw::Sheet: npoints is not divisible by pitch";

	//  Save transformation
	glPushMatrix();

	//  Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	glColor3f(color.x, color.y, color.z);
	
	int rows = npoints/pitch;
	int cols = pitch;
	
	for(int j = 0; j < rows-1; j++)
	{		
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < cols; i++)
		{
			int idx = j*pitch + i;
			int idx_jp1 = (j+1)*pitch + i;
			
			//glNormal3d(normals[idx].x, normals[idx].y, normals[idx].z);
			glVertex3d(points[idx].x, points[idx].y, points[idx].z);
			
			//glNormal3d(normals[idx_jp1].x, normals[idx_jp1].y, normals[idx_jp1].z);
			glVertex3d(points[idx_jp1].x, points[idx_jp1].y, points[idx_jp1].z);
		}			
		glEnd();
	}	
	
	//  Undo transofrmations
	glPopMatrix();	
}
 
void kdraw::Circle(double r, int segs, point3 color,
				   point3 *translations, point4 *rotations, point3 *scales,
		  		   int ntrans, int nrots, int nscales)
{
	//  Save transformation
	glPushMatrix();

	//  Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// get the points around the circle (diameter = 1, radius = 0.5)
	int nBytes = (segs + 1)*sizeof(point2);
	point2 *points = (point2 *)malloc(nBytes);
	
	ComputeCirclePoints(0.5, points, segs+1);
	
	glColor3f(color.x, color.y, color.z);
	//glNormal3f(0,0,1);
	glBegin(GL_POLYGON);
	for(int i = 0; i < segs+1; i++)
		glVertex3d(points[i].x, points[i].y, 0);
	glEnd();
	
	//  Undo transofrmations
	glPopMatrix();
	
	free(points);	
}

void kdraw::Circle(double r, int segs, point3 color,
				   point3 translation, point4 rotation, point3 scale)
{
	Circle(r, segs, color, &translation, &rotation, &scale, 1, 1, 1);
}

void kdraw::Circle(double r, int segs, point3 color)
{
	point3 translation = {0,0,0};
	point4 rotation = {0,0,0,0};
	point3 scale = {1,1,1};
	Circle(r, segs, color, translation, rotation, scale);
}

void kdraw::Triangle(point3 p1, point3 p2, point3 p3, point3 color,
					 point3 *translations, point4 *rotations, point3 *scales,
		  			 int ntrans, int nrots, int nscales)
{
	//  Save transformation
	glPushMatrix();

	//  Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	point3 normal = ComputeNormal(p1, p2, p3);
	
	glColor3f(color.x, color.y, color.z);
	//glNormal3f(normal.x, normal.y, normal.z);
	glBegin(GL_POLYGON);
	glVertex3d(p1.x, p1.y, p1.z);
	glVertex3d(p2.x, p2.y, p2.z);
	glVertex3d(p3.x, p3.y, p3.z);
	glEnd();
	
	//  Undo transofrmations
	glPopMatrix();
}

void kdraw::Triangle(point3 p1, point3 p2, point3 p3, point3 color,
					 point3 translation, point4 rotation, point3 scale)
{
	Triangle(p1, p2, p3, color, &translation, &rotation, &scale, 1, 1, 1);
}

void kdraw::Triangle(point3 p1, point3 p2, point3 p3, point3 color)
{
	point3 translation = {0,0,0};
	point4 rotation = {0,0,0,0};
	point3 scale = {1,1,1};
	Triangle(p1, p2, p3, color, translation, rotation, scale);
}
