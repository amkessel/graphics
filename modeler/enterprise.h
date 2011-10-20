#ifndef _ENTERPRISE_H_
#define _ENTERPRISE_H_

#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

/***********************************
 * DECLARATIONS
 ***********************************/

#define SHULL_TEX 0
#define HULL_TEX 1
unsigned int enterprise_tex[2]; // textures

void Draw_enterprise();

/***********************************
 * IMPLEMENTATIONS
 ***********************************/

// sauce dimensions
#define R_OUTER 1		// radius of outer saucer circle
#define R_INNER 0.25		// radius of inner saucer circle
#define H_INNER R_OUTER/6.75	// height of outer ring above x-z plane
#define H_OUTER H_INNER/4.0	// height of inner ring above x-z plane

// corridor dimensions
#define C_INNER_LEN R_OUTER		// length of inner corridor
#define C_OUTER_LEN R_OUTER/4.0		// length of outer corridor

#define HULL_COLOR 0.9
 
double tc(double c, double offset)
{
	return (c + offset) / (offset * 2);
}

void draw_saucer()
{
	int segs = 60;
	int npts = segs+1;
	
	point2 *outer = (point2*)malloc(npts*sizeof(point2));
	point2 *inner = (point2*)malloc(npts*sizeof(point2));
	
	ComputeCirclePoints(R_OUTER, outer, npts);
	ComputeCirclePoints(R_INNER, inner, npts);
	
	point3 topcenter = {0,  H_INNER, 0};
	point3 btmcenter = {0, -H_INNER, 0};
	
	double offset = R_OUTER;
	
	for(int i = 0; i < npts; i++)
	{
		point3 p1 = { inner[i].x,  H_INNER, inner[i].y };
		point3 p2 = { outer[i].x,  H_OUTER, outer[i].y };
		point3 p3 = { outer[i].x, -H_OUTER, outer[i].y };
		point3 p4 = { inner[i].x, -H_INNER, inner[i].y };
		
		int next_i = (i == npts-1 ? 0 : i+1); // wrap around
		point3 p5 = { inner[next_i].x, -H_INNER, inner[next_i].y };
		point3 p6 = { outer[next_i].x, -H_OUTER, outer[next_i].y };
		point3 p7 = { outer[next_i].x,  H_OUTER, outer[next_i].y };
		point3 p8 = { inner[next_i].x,  H_INNER, inner[next_i].y };
		
		// draw polygons
		glColor3f(HULL_COLOR,HULL_COLOR,HULL_COLOR);
		point3 norm;
		
		glBindTexture(GL_TEXTURE_2D, enterprise_tex[SHULL_TEX]);
				
		// top circle
		glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glTexCoord2f(tc(topcenter.x,offset), tc(topcenter.z,offset));
		glVertex3f(topcenter.x, topcenter.y, topcenter.z);
		glTexCoord2f(tc(p1.x,offset),tc(p1.z,offset));	glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(tc(p8.x,offset),tc(p8.z,offset));	glVertex3f(p8.x, p8.y, p8.z);
		glEnd();
		
		// top slanted
		norm = ComputeNormal(p8, p1, p2);
		glBegin(GL_POLYGON);
		glNormal3f(norm.x, norm.y, norm.z);
		glTexCoord2f(tc(p1.x,offset),tc(p1.z,offset));	glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(tc(p2.x,offset),tc(p2.z,offset));	glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f(tc(p7.x,offset),tc(p7.z,offset));	glVertex3f(p7.x, p7.y, p7.z);
		glTexCoord2f(tc(p8.x,offset),tc(p8.z,offset));	glVertex3f(p8.x, p8.y, p8.z);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, enterprise_tex[HULL_TEX]);
		
		// side rim
		glBegin(GL_POLYGON);
		glNormal3f(p2.x, p2.y, p2.z);
		glTexCoord2f(0,0.1);	glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f(0,0);	glVertex3f(p3.x, p3.y, p3.z);
		glNormal3f(p6.x, p6.y, p6.z);
		glTexCoord2f(0.1,0);	glVertex3f(p6.x, p6.y, p6.z);
		glTexCoord2f(0.1,0.1);	glVertex3f(p7.x, p7.y, p7.z);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, enterprise_tex[SHULL_TEX]);
		
		// bottom slanted
		norm = ComputeNormal(p6, p3, p4);
		glBegin(GL_POLYGON);
		glNormal3f(norm.x, norm.y, norm.z);
		glTexCoord2f(tc(p3.x,offset),tc(p3.z,offset));	glVertex3f(p3.x, p3.y, p3.z);
		glTexCoord2f(tc(p4.x,offset),tc(p4.z,offset));	glVertex3f(p4.x, p4.y, p4.z);
		glTexCoord2f(tc(p5.x,offset),tc(p5.z,offset));	glVertex3f(p5.x, p5.y, p5.z);
		glTexCoord2f(tc(p6.x,offset),tc(p6.z,offset));	glVertex3f(p6.x, p6.y, p6.z);
		glEnd();
				
		// bottom circle
		glBegin(GL_POLYGON);
		glNormal3f(0,-1,0);
		glTexCoord2f(tc(btmcenter.x,offset), tc(btmcenter.z,offset));
		glVertex3f(btmcenter.x, btmcenter.y, btmcenter.z);
		glTexCoord2f(tc(-p4.x,offset),tc(-p4.z,offset));	glVertex3f(p4.x, p4.y, p4.z);
		glTexCoord2f(tc(-p5.x,offset),tc(-p5.z,offset));	glVertex3f(p5.x, p5.y, p5.z);
		glEnd();
		
	}
	
	free(outer);
	free(inner);
}

void draw_cockpit()
{
	double r = H_INNER;
	
	int npts = 20;	
	point2 *points = (point2*)malloc(npts*sizeof(point2));
	
	ComputeCirclePoints(r, points, npts);
	
	point3 *inner = (point3*)malloc(npts*sizeof(point3));
	point3 *middle = (point3*)malloc(npts*sizeof(point3));
	point3 *outer = (point3*)malloc(npts*sizeof(point3));
	
	// these are some numbers for computing the shifting of the middle circle
	double x_ratio = 0.5;
	double z_ratio = 0.8660254; // -sqrt(3)/2;
	double trans_x = x_ratio * C_INNER_LEN;
	double trans_z = z_ratio * C_INNER_LEN;
	
	point3 innercenter =  { 0, 0, 0 };
	point3 middlecenter = { trans_x,             0, trans_z };
	point3 outercenter =  { trans_x+C_OUTER_LEN, 0, trans_z };
	
	// set up the points
	for(int i = 0; i < npts; i++)
	{
		point2 p = points[i];
		
		inner[i].x = p.x;
		inner[i].y = p.y;
		inner[i].z = 0;
	
		// middle points need to be rotated 60 degrees around the y axis
		// and then translated out		
		double new_x =  x_ratio*p.x;
		double new_z = -z_ratio*p.x;
		middle[i].x = middlecenter.x+new_x;
		middle[i].y = p.y;
		middle[i].z = middlecenter.z+new_z;
				
		// outer points need to be rotated 90 degrees around the y axis
		// and then translated out		
		double shift_x = 1;
		outer[i].x = outercenter.x;
		outer[i].y = p.y;
		outer[i].z = middle[i].z;
	}
	
	// draw the corridor	
	glBindTexture(GL_TEXTURE_2D, enterprise_tex[HULL_TEX]);
	double tex_horiz_factor = 1;
	double tex_vert_factor = 1;
	for(int i = 0; i < npts; i++)
	{
		int i_next = (i == npts-1) ? 0 : i+1; // wrap around
				
		// from inner to middle circle
		
		point3 p1 = inner[i];
		point3 p2 = middle[i];
		point3 p3 = middle[i_next];
		point3 p4 = inner[i_next];
		
		point3 norm_i  = { p2.x-middlecenter.x, p2.y-middlecenter.y, p2.z-middlecenter.z };
		point3 norm_in = { p3.x-middlecenter.x, p3.y-middlecenter.y, p3.z-middlecenter.z };
		
		glBegin(GL_POLYGON);
		glNormal3d(norm_i.x, norm_i.y, norm_i.z);
		glTexCoord2d(0,0);			glVertex3d(p1.x, p1.y, p1.z);
		glTexCoord2d(5.0,5.0);		glVertex3d(p2.x, p2.y, p2.z);
		glNormal3d(norm_in.x, norm_in.y, norm_in.z);
		glTexCoord2d(5.0,5.0);		glVertex3d(p3.x, p3.y, p3.z);
		glTexCoord2d(0,i*5.0/npts);	glVertex3d(p4.x, p4.y, p4.z);
		glEnd();
		
		// from middle to outer circle
		
		p1 = middle[i];
		p2 = outer[i];
		p3 = outer[i_next];
		p4 = middle[i_next];
		
		norm_i  = { p2.x-outercenter.x, p2.y-outercenter.y, p2.z-outercenter.z };
		norm_in = { p3.x-outercenter.x, p3.y-outercenter.y, p3.z-outercenter.z };
		
		glBegin(GL_POLYGON);
		glNormal3d(norm_i.x, norm_i.y, norm_i.z);
		glTexCoord2d(i*1.0/npts,0);		glVertex3d(p1.x, p1.y, p1.z);
		glTexCoord2d(i*1.0/npts,1.0);	glVertex3d(p2.x, p2.y, p2.z);
		glNormal3d(norm_in.x, norm_in.y, norm_in.z);
		glTexCoord2d(i*1.0/npts,1);		glVertex3d(p3.x, p3.y, p3.z);
		glTexCoord2d(i*1.0/npts,1.0);	glVertex3d(p4.x, p4.y, p4.z);
		glEnd();
	}
	
	free(points);
	free(inner);
	free(outer);
	free(middle);
	
	// create the inner corridor
/*	glPushMatrix();
	glRotated(30,0,1,0);
	
	glBindTexture(GL_TEXTURE_2D, enterprise_tex[HULL_TEX]);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= npts; i++)
	{
		int idx = (i == npts) ? 0 : i; // wrap around
		
		point2 p = points[idx];
	
	
		
//		double max = r*2;
//		double angle = asin(p.y/r);
//		double shift = max*sin(angle/2);

		double max = 1.732050808*r;
		double shift = -(0.29*max)*p.x/r;
		
		glNormal3f(p.x, p.y, 0); // normal is radial
		glTexCoord2d((double)i/npts,0);	glVertex3d(p.x, p.y, 0);
		glTexCoord2d((double)i/npts,1);	glVertex3d(p.x, p.y, C_IN_LEN+shift);
	}
	glEnd();
	glPopMatrix();

	// create the outer corridor
	glPushMatrix();
	glTranslated(0.50*R_OUTER,0,0.87*R_OUTER);
	glRotated(90,0,1,0);
	
	glBindTexture(GL_TEXTURE_2D, enterprise_tex[HULL_TEX]);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= npts; i++)
	{
		int idx = (i == npts) ? 0 : i; // wrap around
		
		point2 p = points[idx];

		double max = r*2;
		double shift = -(0.57*max)*p.x/r;
		
		glNormal3f(p.x, p.y, 0); // normal is radial
		glTexCoord2d((double)i/npts,0);	glVertex3d(p.x, p.y, -shift);
		glTexCoord2d((double)i/npts,1);	glVertex3d(p.x, p.y, C_OUT_LEN);
	}
	glEnd();	
	glPopMatrix();
	*/
}

void Draw_enterprise()
{
	draw_saucer();
	draw_cockpit();
}


#endif
