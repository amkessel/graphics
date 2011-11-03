#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "kutils.h"
#include "planets.h"
#include "sheet.h"

using namespace kutils;

void Draw_sheet(point3 pts[SHEET_PTS][SHEET_PTS], point3 norms[SHEET_PTS][SHEET_PTS], bool sheet, bool grid)
{
	for(int j = 1; j < SHEET_PTS; j++)
	{
		for(int i = 1; i < SHEET_PTS; i++)
		{
			point3 p_i_j     = pts[i][j];
			point3 p_im1_j   = pts[i-1][j];
			point3 p_im1_jm1 = pts[i-1][j-1];
			point3 p_i_jm1   = pts[i][j-1];
					
			point3 n_i_j     = norms[i][j];
			point3 n_im1_j   = norms[i-1][j];
			point3 n_im1_jm1 = norms[i-1][j-1];
			point3 n_i_jm1   = norms[i][j-1];
						
			if(sheet)
			{
				// draw the sheet
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1,-1);
				glColor3d(SHEET_COLOR,SHEET_COLOR,SHEET_COLOR);
				glBegin(GL_POLYGON);
				
				glNormal3d(n_i_j.x, n_i_j.x, n_i_j.z);
				glVertex3d(p_i_j.x, p_i_j.y, p_i_j.z);
				
				glNormal3d(n_i_jm1.x, n_i_jm1.x, n_i_jm1.z);
				glVertex3d(p_i_jm1.x, p_i_jm1.y, p_i_jm1.z);
				
				glNormal3d(n_im1_jm1.x, n_im1_jm1.x, n_im1_jm1.z);
				glVertex3d(p_im1_jm1.x, p_im1_jm1.y, p_im1_jm1.z);
				
				glNormal3d(n_im1_j.x, n_im1_j.x, n_im1_j.z);
				glVertex3d(p_im1_j.x, p_im1_j.y, p_im1_j.z);

				glEnd();
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
			else
				glPolygonOffset(0,0);
			
			if(grid)
			{
				// draw the grid
				double color = sheet ? GRID_COLOR : 1-GRID_COLOR;
				
				/*
				// DRAW NORMALS ON SHEET FOR DEBUGGING
				if( (i == SHEET_PTS/2 || i == SHEET_PTS/2-1) && (j == SHEET_PTS/2 || j == SHEET_PTS/2-1))
				{
					glColor3d(1,0,0);
					glBegin(GL_LINES);
					glVertex3d(p_i_j.x, p_i_j.y, p_i_j.z);
					glVertex3d(p_i_j.x + n_i_j.x*0.1, p_i_j.y + n_i_j.y*0.1, p_i_j.z + n_i_j.z*0.1);
					glEnd();
				}
				*/
				
				glColor3d(color,color,color);
				glBegin(GL_LINE_STRIP);
				glVertex3d(p_i_j.x,     p_i_j.y,     p_i_j.z);
				glVertex3d(p_i_jm1.x,   p_i_jm1.y,   p_i_jm1.z);
				glVertex3d(p_im1_jm1.x, p_im1_jm1.y, p_im1_jm1.z);
				glVertex3d(p_im1_j.x,   p_im1_j.y,   p_im1_j.z);
				glEnd();
			}
		}
	}
}

void Calculate_sheet_points(point3 pts[SHEET_PTS][SHEET_PTS], point3 falcon_pos)
{
	// find the closest point rounded to the smallest multiple of SHEET_DIST
	point3 center_pos = { falcon_pos.x - fmod(falcon_pos.x,SHEET_DIST),
						  0,
						  falcon_pos.z - fmod(falcon_pos.z,SHEET_DIST) };
						  
	// find the starting point at the lowest x/z
	point3 start_pos = { center_pos.x - SHEET_DIST*SHEET_PTS/2,
						 0,
						 center_pos.z - SHEET_DIST*SHEET_PTS/2 };
						 
	// finally, fill in the points
	for(int j = 0; j < SHEET_PTS; j++)
	{
		for(int i = 0; i < SHEET_PTS; i++)
		{
			pts[i][j].x = start_pos.x + SHEET_DIST * i;
			pts[i][j].z = start_pos.z + SHEET_DIST * j;
		}
	}
}

void Calculate_sheet_normals(point3 pts[SHEET_PTS][SHEET_PTS], point3 norms[SHEET_PTS][SHEET_PTS])
{
	for(int j = 1; j < SHEET_PTS-1; j++)
	{
		for(int i = 1; i < SHEET_PTS-1; i++)
		{
			point3 p_ip1_j   = pts[i+1][j];
			point3 p_im1_j   = pts[i-1][j];
			point3 p_i_jp1   = pts[i][j+1];
			point3 p_i_jm1   = pts[i][j-1];
			
			Vector3 vec1(p_i_jm1, p_i_jp1);
			Vector3 vec2(p_im1_j, p_ip1_j);
			
			Vector3 norm = CrossProduct(&vec1, &vec2, true);
			norms[i][j].x = norm.Get_x();
			norms[i][j].y = norm.Get_y();
			norms[i][j].z = norm.Get_z();
			
			if(i == 1 && j == 1)
			{
				norms[0][0].x = norm.Get_x();
				norms[0][0].y = norm.Get_y();
				norms[0][0].z = norm.Get_z();
			}
			else if(i == 1)
			{
				norms[0][j].x = norm.Get_x();
				norms[0][j].y = norm.Get_y();
				norms[0][j].z = norm.Get_z();			
			}
			else if(j == 1)
			{
				norms[i][0].x = norm.Get_x();
				norms[i][0].y = norm.Get_y();
				norms[i][0].z = norm.Get_z();
			}
			
			if(i == SHEET_PTS-2 && j == SHEET_PTS-2)
			{
				norms[SHEET_PTS-1][SHEET_PTS-1].x = norm.Get_x();
				norms[SHEET_PTS-1][SHEET_PTS-1].y = norm.Get_y();
				norms[SHEET_PTS-1][SHEET_PTS-1].z = norm.Get_z();
			}
			else if(i == SHEET_PTS-2)
			{
				norms[SHEET_PTS-1][j].x = norm.Get_x();
				norms[SHEET_PTS-1][j].y = norm.Get_y();
				norms[SHEET_PTS-1][j].z = norm.Get_z();
			}
			else if(j == SHEET_PTS-2)
			{
				norms[i][SHEET_PTS-1].x = norm.Get_x();
				norms[i][SHEET_PTS-1].y = norm.Get_y();
				norms[i][SHEET_PTS-1].z = norm.Get_z();
			}
		}
	}
}

double calculate_gravity(point3 pt, point3 body, double mass, double radius)
{
	double dist = sqrt( (pt.x-body.x)*(pt.x-body.x) + (pt.z-body.z)*(pt.z-body.z) );
	
	double potential = 0; // default to zero
	
//	if(dist < radius * GRAV_CUTOFF)
//	{
		double width = 1/radius;
		double depth = mass;
	
		potential = depth / exp(width*dist*dist);
//	}
	
	return potential;
}

void Calculate_sheet_heights(point3 pts[SHEET_PTS][SHEET_PTS], point3 sun, point3 earth, point3 moon)
{
	moon.x += earth.x;
	moon.z += earth.z;

	for(int j = 0; j < SHEET_PTS; j++)
	{
		for(int i = 0; i < SHEET_PTS; i++)
		{
			point3 pt = { pts[i][j].x, 0, pts[i][j].z };
			
			double sun_grav   = calculate_gravity(pt, sun,   SUN_MASS, SUN_RAD);
			double earth_grav = calculate_gravity(pt, earth, EARTH_MASS, EARTH_RAD);
			double moon_grav  = calculate_gravity(pt, moon, MOON_MASS, MOON_RAD);
			
			pts[i][j].y = 0 - sun_grav - earth_grav - moon_grav;
			pts[i][j].y *= GRAV_FACTOR;
		}
	}
	
}
