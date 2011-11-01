#include "falcon.h"
#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

// sauce dimensions
#define R_OUTER        1		    // radius of outer saucer circle
#define R_INNER        0.25		    // radius of inner saucer circle
#define H_INNER        R_OUTER/6.75	// height of outer ring above x-z plane
#define H_OUTER        H_INNER/4.0	// height of inner ring above x-z plane

// cockpit dimensions
#define R_CPIT_TIP     0.139
#define R_CPIT_MID     0.25
#define R_CPIT_INN     0.15
#define LEN_CPIT_TIP   0.25
#define LEN_CPIT_MID   0.7
#define CPIT_Y_OFFSET  0

// fork dimensions
#define FORK_WIDTH     0.691
#define FORK_HEIGHT    1.157
#define TINE_WIDTH     0.066

#define HULL_COLOR 1
	
GLfloat thrustEmissiveMaterial[] = {5.0, 5.0, 1.0};
GLfloat blackEmissiveMaterial[] = {0.0, 0.0, 0.0};

unsigned int falcon_tex[NUM_FALCON_TEXS];
char *falcon_tex_names[] = { (char*)"./texs/tex_bottom.bmp",
							 (char*)"./texs/tex_top.bmp",
							 (char*)"./texs/tex_side.bmp",
							 (char*)"./texs/tex_around.bmp",
							 (char*)"./texs/tex_canopy_top.bmp",
							 (char*)"./texs/tex_canopy_front.bmp",
							 (char*)"./texs/tex_fork_top.bmp",
							 (char*)"./texs/tex_fork_bottom.bmp"};

/* Takes a model coordinate and transforms it into a texture coordinate.
   Specific to mapping a 2x2 square centered on the origin to a 1x1 texture */
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
		
		glBindTexture(GL_TEXTURE_2D, falcon_tex[TOP_TEX]);
				
		// top circle
		glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glTexCoord2f(tc(topcenter.z,offset), tc(topcenter.x,offset));
		glVertex3f(topcenter.x, topcenter.y, topcenter.z);
		glTexCoord2f(tc(p1.z,offset),tc(p1.x,offset));	glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(tc(p8.z,offset),tc(p8.x,offset));	glVertex3f(p8.x, p8.y, p8.z);
		glEnd();
		
		// top slanted
		norm = ComputeNormal(p8, p1, p2);
		glBegin(GL_POLYGON);
		glNormal3f(norm.x, norm.y, norm.z);
		glTexCoord2f(tc(p1.z,offset),tc(p1.x,offset));	glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(tc(p2.z,offset),tc(p2.x,offset));	glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f(tc(p7.z,offset),tc(p7.x,offset));	glVertex3f(p7.x, p7.y, p7.z);
		glTexCoord2f(tc(p8.z,offset),tc(p8.x,offset));	glVertex3f(p8.x, p8.y, p8.z);
		glEnd();
		
		// side rim
		glBindTexture(GL_TEXTURE_2D, falcon_tex[AROUND_TEX]);
		// because of the texture, the thrust exists starting at 38% around the circle and ending at 56%
		if(i > 0.38*npts && i < 0.56*npts)
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, thrustEmissiveMaterial);
		
		glBegin(GL_POLYGON);
		glNormal3f(p2.x, p2.y, p2.z);
		glTexCoord2f((double)(i+1)/npts,1);	glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f((double)(i+1)/npts,0);	glVertex3f(p3.x, p3.y, p3.z);
		glNormal3f(p6.x, p6.y, p6.z);
		glTexCoord2f((double)(next_i+1)/npts,0);	glVertex3f(p6.x, p6.y, p6.z);
		glTexCoord2f((double)(next_i+1)/npts,1);	glVertex3f(p7.x, p7.y, p7.z);
		glEnd();
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackEmissiveMaterial);
		glBindTexture(GL_TEXTURE_2D, falcon_tex[BOTTOM_TEX]);
		
		// bottom slanted
		norm = ComputeNormal(p6, p3, p4);
		glBegin(GL_POLYGON);
		glNormal3f(norm.x, norm.y, norm.z);
		glTexCoord2f(tc(p3.z,offset),tc(p3.x,offset));	glVertex3f(p3.x, p3.y, p3.z);
		glTexCoord2f(tc(p4.z,offset),tc(p4.x,offset));	glVertex3f(p4.x, p4.y, p4.z);
		glTexCoord2f(tc(p5.z,offset),tc(p5.x,offset));	glVertex3f(p5.x, p5.y, p5.z);
		glTexCoord2f(tc(p6.z,offset),tc(p6.x,offset));	glVertex3f(p6.x, p6.y, p6.z);
		glEnd();
				
		// bottom circle
		glBegin(GL_POLYGON);
		glNormal3f(0,-1,0);
		glTexCoord2f(tc(btmcenter.z,offset), tc(btmcenter.x,offset));
		glVertex3f(btmcenter.x, btmcenter.y, btmcenter.z);
		glTexCoord2f(tc(-p4.z,offset),tc(-p4.x,offset));	glVertex3f(p4.x, p4.y, p4.z);
		glTexCoord2f(tc(-p5.z,offset),tc(-p5.x,offset));	glVertex3f(p5.x, p5.y, p5.z);
		glEnd();
		
	}
	
	free(outer);
	free(inner);
}

void draw_cockpit()
{
	// R_CPIT_TIP
	// R_CPIT_MID
	// R_CPIT_INN
	// LEN_CPIT_TIP
	// LEN_CPIT_MID
	// CPIT_Y_OFFSET

	// compute the 2D points
	int npts = 20;	
	point2 *tip_pts_cir = (point2*)malloc(npts*sizeof(point2));
	point2 *mid_pts_cir = (point2*)malloc(npts*sizeof(point2));
	point2 *inn_pts_cir = (point2*)malloc(npts*sizeof(point2));
	
	ComputeCirclePoints(R_CPIT_TIP, tip_pts_cir, npts);
	ComputeCirclePoints(R_CPIT_MID, mid_pts_cir, npts);
	ComputeCirclePoints(R_CPIT_INN, inn_pts_cir, npts);
	
	// generate the 3D points
	point3 *tip_pts = (point3*)malloc(npts*sizeof(point3));
	point3 *mid_pts = (point3*)malloc(npts*sizeof(point3));
	point3 *inn_pts = (point3*)malloc(npts*sizeof(point3));
	
	for(int i = 0; i < npts; i++)
	{
		tip_pts[i].x = LEN_CPIT_MID + LEN_CPIT_TIP;
		tip_pts[i].y = tip_pts_cir[i].y;
		tip_pts[i].z = tip_pts_cir[i].x; // points will be rotated to y-z plane
		
		mid_pts[i].x = LEN_CPIT_MID;
		mid_pts[i].y = mid_pts_cir[i].y;
		mid_pts[i].z = mid_pts_cir[i].x; // points will be rotated to y-z plane
		
		inn_pts[i].x = 0;
		inn_pts[i].y = inn_pts_cir[i].y;
		inn_pts[i].z = inn_pts_cir[i].x; // points will be rotated to y-z plane
	}
	
	// draw the mid-cylinder	
	double offset = R_OUTER;
	glBindTexture(GL_TEXTURE_2D, falcon_tex[TOP_TEX]);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= npts/2; i++)
	{
		int idx = (i < npts) ? i : 0; // wrap around
		
		point3 mid = mid_pts[idx];
		point3 inn = inn_pts[idx];

		// compute surface normal		
		point3 midcenter = { mid.x, 0, 0 }; // center of circle for mid points lies on x axis
		point3 norm_tmp = ComputeNormal(inn, mid, midcenter);
		point3 tmp = { norm_tmp.x+mid.x, norm_tmp.y+mid.y, norm_tmp.z+mid.z };
		point3 norm = ComputeNormal(inn, mid, tmp);
		
		glNormal3d(norm.x, norm.y, norm.z);
		glTexCoord2d(tc(inn.z,offset),tc(inn.x,offset));	glVertex3d(inn.x,  inn.y,  inn.z);
		glTexCoord2d(tc(mid.z,offset),tc(mid.x,offset));	glVertex3d(mid.x,  mid.y,  mid.z);
	}
	glEnd();
	
	// draw the tip-cylinder
	glBindTexture(GL_TEXTURE_2D, falcon_tex[CANOPY_TOP_TEX]);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= npts/2-1; i++)
	{
		int idx = (i < npts) ? i : 0; // wrap around
		
		point3 tip = tip_pts[idx];
		point3 mid = mid_pts[idx];

		// compute surface normal		
		point3 midcenter = { mid.x, 0, 0 }; // center of circle for mid points lies on x axis
		point3 norm_tmp = ComputeNormal(tip, mid, midcenter);
		point3 tmp = { norm_tmp.x+mid.x, norm_tmp.y+mid.y, norm_tmp.z+mid.z };
		point3 norm = ComputeNormal(tip, mid, tmp);
		
		// compute texture x coordinates (y will just be zero or one)
		double tex_x = 1.1*i*2.0/npts;
		
		glNormal3d(norm.x, norm.y, norm.z);
		glTexCoord2d(tex_x, 1);	glVertex3d(mid.x,  mid.y,  mid.z);
		glTexCoord2d(tex_x, 0);	glVertex3d(tip.x,  tip.y,  tip.z);
	}
	glEnd();
	
	// draw the tip
	glNormal3d(1,0,0);
	point3 center = { LEN_CPIT_MID + LEN_CPIT_TIP, 0, 0 };
	glBindTexture(GL_TEXTURE_2D, falcon_tex[CANOPY_FRONT_TEX]);
	for(int i = 0; i < npts/2-1; i++)
	{		
		int i_next = (i < npts-1) ? i+1 : 0; // wrap around
		
		point3 p1 = tip_pts[i];
		point3 p2 = tip_pts[i_next];
		
		point2 tex1, tex2;
		tex1.x = TransformToTexCoord(0,             2*R_CPIT_TIP, p1.z);
		tex1.y = TransformToTexCoord(CPIT_Y_OFFSET, 2*R_CPIT_TIP, p1.y);
		tex2.x = TransformToTexCoord(0,             2*R_CPIT_TIP, p2.z);
		tex2.y = TransformToTexCoord(CPIT_Y_OFFSET, 2*R_CPIT_TIP, p2.y);
		
		glBegin(GL_POLYGON);
		glTexCoord2d(0.5,0.5);		glVertex3d(center.x, center.y, center.z);
		glTexCoord2d(tex1.x,tex1.y);	glVertex3d(p1.x,     p1.y,     p1.z);
		glTexCoord2d(tex2.x,tex2.y);	glVertex3d(p2.x,     p2.y,     p2.z);
		glEnd();
	}	
	
	free(tip_pts_cir);
	free(mid_pts_cir);
	free(inn_pts_cir);
	free(tip_pts);
	free(mid_pts);
	free(inn_pts);
}

void draw_forks()
{
	// FORK_WIDTH
	// FORK_HEIGHT
	// TINE_WIDTH
	
	// draw left fork
	 
	point3 offset = { 0.518, H_OUTER, -0.86 };
	
	point3 p1 = { offset.x, offset.y, offset.z };
	point3 p2 = { offset.x, offset.y, offset.z+FORK_WIDTH };
	point3 p3 = { offset.x+FORK_HEIGHT, offset.y, offset.z+FORK_WIDTH };
	point3 p4 = { offset.x+FORK_HEIGHT, offset.y, offset.z+FORK_WIDTH-TINE_WIDTH };
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[FORK_TOP_TEX]); // drawing top
	point3 norm = { 0, 1, 0 };
	
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0,    0);	glVertex3d(p1.x, p1.y, p1.z);
	glTexCoord2d(1,    0);	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2d(1,    1);	glVertex3d(p3.x, p3.y, p3.z);
	glTexCoord2d(0.903,1);	glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[FORK_BOTTOM_TEX]); // draw bottom
	norm.y = -1;
	p1.y -= 2*H_OUTER;
	p2.y -= 2*H_OUTER;
	p3.y -= 2*H_OUTER;
	p4.y -= 2*H_OUTER;
		
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0,    0);	glVertex3d(p1.x, p1.y, p1.z);
	glTexCoord2d(1,    0);	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2d(1,    1);	glVertex3d(p3.x, p3.y, p3.z);
	glTexCoord2d(0.903,1);	glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[SIDE_TEX]);
	norm.x = 0; norm.y = 0; norm.z = 1; // draw inner side
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(1,  0);	glVertex3d(p2.x, p2.y,           p2.z);
	glTexCoord2d(1,  1);	glVertex3d(p2.x, p2.y+2*H_OUTER, p2.z);
	glTexCoord2d(0.5,1);	glVertex3d(p3.x, p3.y+2*H_OUTER, p3.z);
	glTexCoord2d(0.5,0);	glVertex3d(p3.x, p3.y,           p3.z);
	glEnd();
	
	norm.x = 1; norm.y = 0; norm.z = 0; // draw front side
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0.9,1);	glVertex3d(p3.x, p3.y,           p3.z);
	glTexCoord2d(0.9,0);	glVertex3d(p3.x, p3.y+2*H_OUTER, p3.z);
	glTexCoord2d(1,  0);	glVertex3d(p4.x, p4.y+2*H_OUTER, p4.z);
	glTexCoord2d(1,  1);	glVertex3d(p4.x, p4.y,           p4.z);
	glEnd();
	
	point3 p4top = { p4.x, p4.y+2*H_OUTER, p4.z };
	norm = ComputeNormal(p1, p4, p4top); // drawing outer/slanted side takes more logic
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(1,  0);	glVertex3d(p1.x, p1.y,           p1.z);
	glTexCoord2d(1,  1);	glVertex3d(p1.x, p1.y+2*H_OUTER, p1.z);
	glTexCoord2d(0.5,1);	glVertex3d(p4.x, p4.y+2*H_OUTER, p4.z);
	glTexCoord2d(0.5,0);	glVertex3d(p4.x, p4.y,           p4.z);
	glEnd();	
	
	
	// draw right fork
	 
	p1.z *= -1; // mirror the points
	p2.z *= -1;
	p3.z *= -1;
	p4.z *= -1;
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[FORK_BOTTOM_TEX]); // draw bottom
	norm.x = 0; norm.y = -1; norm.z = 0;
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0,    0);	glVertex3d(p1.x, p1.y, p1.z);
	glTexCoord2d(1,    0);	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2d(1,    1);	glVertex3d(p3.x, p3.y, p3.z);
	glTexCoord2d(0.903,1);	glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[FORK_TOP_TEX]); // draw top
	norm.y = 1;

	p1.y += 2*H_OUTER;
	p2.y += 2*H_OUTER;
	p3.y += 2*H_OUTER;
	p4.y += 2*H_OUTER;
		
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0,    0);	glVertex3d(p1.x, p1.y, p1.z);
	glTexCoord2d(1,    0);	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2d(1,    1);	glVertex3d(p3.x, p3.y, p3.z);
	glTexCoord2d(0.903,1);	glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, falcon_tex[SIDE_TEX]);
	norm.x = 0; norm.y = 0; norm.z = -1; // draw inner side
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(1,  0);	glVertex3d(p2.x, p2.y,           p2.z);
	glTexCoord2d(1,  1);	glVertex3d(p2.x, p2.y-2*H_OUTER, p2.z);
	glTexCoord2d(0.5,1);	glVertex3d(p3.x, p3.y-2*H_OUTER, p3.z);
	glTexCoord2d(0.5,0);	glVertex3d(p3.x, p3.y,           p3.z);
	glEnd();
	
	norm.x = 1; norm.y = 0; norm.z = 0; // draw front side
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(0.9,1);	glVertex3d(p3.x, p3.y,           p3.z);
	glTexCoord2d(0.9,0);	glVertex3d(p3.x, p3.y-2*H_OUTER, p3.z);
	glTexCoord2d(1,  0);	glVertex3d(p4.x, p4.y-2*H_OUTER, p4.z);
	glTexCoord2d(1,  1);	glVertex3d(p4.x, p4.y,           p4.z);
	glEnd();
	
	point3 p4bot = { p4.x, p4.y-2*H_OUTER, p4.z };
	norm = ComputeNormal(p1, p4, p4bot); // drawing outer/slanted side takes more logic
	glBegin(GL_POLYGON);
	glNormal3d(norm.x, norm.y, norm.z);
	glTexCoord2d(1,  0);	glVertex3d(p1.x, p1.y,           p1.z);
	glTexCoord2d(1,  1);	glVertex3d(p1.x, p1.y-2*H_OUTER, p1.z);
	glTexCoord2d(0.5,1);	glVertex3d(p4.x, p4.y-2*H_OUTER, p4.z);
	glTexCoord2d(0.5,0);	glVertex3d(p4.x, p4.y,           p4.z);
	glEnd();	
}

void Draw_falcon()
{		
	// set up for textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	
	// set some material properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackEmissiveMaterial);
	
	draw_forks();
	draw_saucer();
	draw_cockpit();
	
	glDisable(GL_TEXTURE_2D);
}

void Draw_falcon(point3 translation, point4 rotation, point3 scale)
{
	Draw_falcon(&translation, &rotation, &scale, 1, 1, 1);
}

void Draw_falcon(point3 *translations, point4 *rotations, point3 *scales,
				 int ntrans, int nrots, int nscales)
{
	//  Save transformation
	glPushMatrix();

	//  Offset
	Transform(translations, rotations, scales, ntrans, nrots, nscales);
	
	// draw the model
	Draw_falcon();
	
	// reset transformation
	glPopMatrix();
}

