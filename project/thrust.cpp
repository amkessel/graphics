#include "thrust.h"
#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

#define N_PTS	200
#define N_PTS_CREATE  10 // the number of points to create at a time
#define POINT_SIZE	10
#define THRUST_LIFETIME	2	// how many seconds a thrust point should live
#define THRUST_EJECT_SPEED 0.1 // speed of ejected thrust particles, in units/sec

typedef struct thrust_point_s
{
	point3 loc; // where the point ismaps
	point3 vel; // the point's velocity (a vector)
	double birth; // time of point's birth
	bool alive;
} thrust_point;

thrust_point thrust_pts[N_PTS];
int thrust_pointer = 0; // points to where in the thrust array the last thrust_point was created

void Initialize_Thrust()
{
	for(int i = 0; i < N_PTS; i++)
	{
		thrust_pts[i].alive = false;
	}
}

void Draw_Thrust(double time)
{
	
	//GLfloat thrustEmissiveMaterial[] = {1.0, 0.25, 0.0};
	//GLfloat blackEmissiveMaterial[] = {0.0, 0.0, 0.0};

	for(int i = 0; i < N_PTS; i++)
	{
		thrust_point pt = thrust_pts[i];
	
		if(pt.alive)
		{
			double percent = (THRUST_LIFETIME - (time-pt.birth)) / THRUST_LIFETIME;
			
			double r,g,b;
			double hue = 45 - percent * 45; // 0-45 is a red-to-yellow spectrum
			double sat = 1.0;
			double val = 1.0;//percent;
			HSVtoRGB(&r, &g, &b, hue, sat, val);
			//printf("hsv = {%g,%g,%g} rgb = {%g,%g,%g}\n", hue, sat, val, r, g, b);
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDepthMask(0);
			glColor4d(r,g,b,0.5*percent);
			//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, thrustEmissiveMaterial);
			//glColor3d(1,0,0);

			/*
			glPointSize(POINT_SIZE);
			glBegin(GL_POINTS);
			glVertex3d(pt.loc.x, pt.loc.y, pt.loc.z);
			glEnd();
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackEmissiveMaterial);
			*/
			/*
			glBegin(GL_POLYGON);
			glVertex3d(pt.loc.x, pt.loc.y, pt.loc.z);
			glVertex3d(pt.loc.x+0.005, pt.loc.y, pt.loc.z);
			glVertex3d(pt.loc.x, pt.loc.y, pt.loc.z+0.005);
			glEnd();
			*/
			/*
			double factor = 0.01;
			point3 normedVel = pt.vel;
			double len = sqrt(normedVel.x*normedVel.x + normedVel.z*normedVel.z);
			normedVel.x /= len;
			normedVel.z /= len;
			glBegin(GL_POLYGON);
			glVertex3d(pt.loc.x+factor*normedVel.x, pt.loc.y, pt.loc.z+factor*normedVel.z);
			if(normedVel.x > 0 && normedVel.z > 0 || normedVel.x < 0 && normedVel.z < 0)
			{
				glVertex3d(pt.loc.x+factor*normedVel.z, pt.loc.y, pt.loc.z+factor*normedVel.x);
				glVertex3d(pt.loc.x+factor*normedVel.z, pt.loc.y, pt.loc.z+factor*normedVel.x);				
			}
			else
			{
				glVertex3d(pt.loc.x+factor*normedVel.z, pt.loc.y, pt.loc.z+factor*normedVel.x);
				glVertex3d(pt.loc.x-factor*normedVel.z, pt.loc.y, pt.loc.z-factor*normedVel.x);
			}
			*/
			
			double factor = 0.1;
			point3 nv = pt.vel; // normalized velocity vector
			double len = sqrt(nv.x*nv.x + nv.z*nv.z);
			nv.x /= (len/factor);
			nv.y = 0;
			nv.z /= (len/factor);
			point3 tip = {pt.loc.x+nv.x, pt.loc.y+nv.y, pt.loc.z+nv.z};
			point3 side1;
			if(nv.x > 0 && nv.z > 0 || nv.x < 0 && nv.z < 0)
			{
				side1.x = -pt.loc.z;
				side1.y =  pt.loc.y;
				side1.z =  pt.loc.x;
			}
			else
			{
				side1.x = pt.loc.z;
				side1.y = pt.loc.y;
				side1.z = pt.loc.x;
			}
			point3 side2 = {-side1.x, side1.y, -side1.z};
			
			glVertex3d(tip.x, tip.y, tip.z);
			glVertex3d(side2.x, side2.y, side2.z);
			glVertex3d(side1.x, side1.y, side1.z);
			
			glEnd();
			glDepthMask(1);
			glDisable(GL_BLEND);
			glEnable(GL_LIGHTING);
		}
	}
}

// generates a mid-point at a random place between a min and max
double generate_mid(double min, double max)
{
	if(min > max)
	{
		double tmp = min;
		min = max;
		max = tmp;
	}
	int randInt = rand();
	double percent = ((double)randInt)/RAND_MAX;
	return min + percent*(max-min);
}

// generates a point inside a rectangular volume
// simple way to do this w/out complicated math:
// 1. find two points, one at the front surface of the box and one at the back
// 2. compute the line between those points
// 3. find a randomly weighted point along that line
// that should give us a point within the volume
// (we only need to work in the x-z plane since the box will always be level to that plane)
point3 generate_point(thrust_box tbox)
{
	point3 pfront;
	pfront.x = generate_mid(tbox.ful.x, tbox.fur.x);
	pfront.z = generate_mid(tbox.ful.z, tbox.fur.z);
	
	point3 pback;
	pback.x = generate_mid(tbox.bul.x, tbox.bur.x);
	pback.z = generate_mid(tbox.bul.z, tbox.bur.z);
	
	point3 mid;
	mid.x = generate_mid(pfront.x, pback.x);
	mid.z = generate_mid(pfront.z, pback.z);
	
	mid.y = generate_mid(tbox.ful.y, tbox.fll.y);
	return mid;
}

point3 generate_vel(point3 falcon_vel, double falcon_dir)
{
	point3 result;
	
	// find the vector opposite of the direction
	result.x = -THRUST_EJECT_SPEED * KUTILS_COS(falcon_dir);
	result.y = 0;
	result.z = THRUST_EJECT_SPEED * KUTILS_SIN(falcon_dir);
	
	// add the falcon's velocity
	result.x += falcon_vel.x;
	result.y += falcon_vel.y;
	result.z += falcon_vel.z;
	
	return result;
}

void Create_Thrust(thrust_box tbox, double time, point3 falcon_vel, double falcon_dir)
{
	for(int i = 0; i < N_PTS_CREATE; i++)
	{
		thrust_point *pt = &thrust_pts[thrust_pointer++];
	
		//if(!pt->alive)
		//{
			point3 mid = generate_point(tbox);
			pt->loc.x = mid.x;
			pt->loc.y = mid.y;
			pt->loc.z = mid.z;
			//pt->loc.x = generate_coord(thrust_box.ul.x, thrust_box.ur.x);
			//pt->loc.y = generate_coord(thrust_box.ul.y, thrust_box.ll.y);
			//pt->loc.z = generate_coord(thrust_box.ul.z, thrust_box.ur.z);

			point3 vel = generate_vel(falcon_vel, falcon_dir);
			pt->vel.x = vel.x;
			pt->vel.y = vel.y;
			pt->vel.z = vel.z;
	
			pt->alive = true;
			pt->birth = time;
		//}
	
		if(thrust_pointer >= N_PTS)
			thrust_pointer = 0;
	}
}

void Move_Thrust(double time, double last_time)
{
	double delta_t = time - last_time;
	for(int i = 0; i < N_PTS; i++)
	{
		thrust_point *pt = &thrust_pts[i];
		
		if((time - pt->birth) > THRUST_LIFETIME)
		{
			pt->alive = false;
		}
		
		if(pt->alive)
		{
			pt->loc.x += pt->vel.x * delta_t;
			pt->loc.y += pt->vel.y * delta_t;
			pt->loc.z += pt->vel.z * delta_t;
		}
	}
}
