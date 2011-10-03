#ifndef _HW3_OBJECTS_
#define _HW3_OBJECTS_


#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

void Draw_H(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from three modified cubes
	point3 trans_left = {-0.4, 0, 0};
	point3 scale_left = {0.2, 1.5, 0.2};
	point4 rot_left = {0, 0, 0, 0};
	
	point3 trans_mid = {0,0,0};
	point3 scale_mid = {0.6, 0.2, 0.2};
	point4 rot_mid = {0, 0, 0, 0};
	
	point3 trans_right = {0.4, 0, 0};
	point3 scale_right = {0.2, 1.5, 0.2};
	point4 rot_right = {0, 0, 0, 0};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_mid, rot_mid, scale_mid);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_O(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from arches and cubes
	point3 trans_top = {0, 0.25, 0};
	point3 scale_top = {1, 1, 0.2};
	point4 rot_top = {0, 0, 0, 0};
	
	point3 trans_bottom = {0, -0.25, 0};
	point3 scale_bottom = {1, 1, 0.2};
	point4 rot_bottom = {1, 0, 0, 180};
	
	point3 trans_left = {-0.4, 0, 0};
	point3 scale_left = {0.2, 0.5, 0.2};
	point4 rot_left = {0, 0, 0, 0};
	
	point3 trans_right = {0.4, 0, 0};
	point3 scale_right = {0.2, 0.5, 0.2};
	point4 rot_right = {0, 0, 0, 0};
	
	double r_outer = 0.5;
	double r_inner = 0.3;
	double degrees = 180;
	int segs = 20;
	
	Arch(r_outer, r_inner, degrees, segs, trans_top, rot_top, scale_top, false);
	Arch(r_outer, r_inner, degrees, segs, trans_bottom, rot_bottom, scale_bottom, false);
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_L(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from two modified cubes
	point3 trans_left = {-0.4, 0, 0};
	point3 scale_left = {0.2, 1.5, 0.2};
	point4 rot_left = {0,0,0,0};
	
	point3 trans_bottom = {0, -0.65, 0};
	point3 scale_bottom = {1, 0.2, 0.2};
	point4 rot_bottom = {0,0,0,0};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_bottom, rot_bottom, scale_bottom);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_Y(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from three modified cubes
	point3 trans_left = {-0.25, 0.35, 0};
	point3 scale_left = {0.87, 0.2, 0.2};
	point4 rot_left = {0, 0, 1, -56.31};
	
	point3 trans_mid = {0, -0.375, 0};
	point3 scale_mid = {0.2, 0.75, 0.2};
	point4 rot_mid = {0, 0, 0, 0};
	
	point3 trans_right = {0.25, 0.35, 0};
	point3 scale_right = {0.87, 0.2, 0.2};
	point4 rot_right = {0, 0, 1, 56.31};
	
	Cube(trans_left, rot_left, scale_left);
	Cube(trans_mid, rot_mid, scale_mid);
	Cube(trans_right, rot_right, scale_right);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_C(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from arches and cubes
	point3 trans_top = {0, 0.25, 0};
	point3 scale_top = {1, 1, 0.2};
	point4 rot_top = {0, 0, 1, 25};
	
	point3 trans_bottom = {0, -0.25, 0};
	point3 scale_bottom = {1, 1, 0.2};
	point4 rot_bottom1 = {1, 0, 0, 180};
	point4 rot_bottom2 = {0, 0, 1, 25};
	
	point3 trans_bottoms[1] = { trans_bottom };
	point3 scale_bottoms[1] = { scale_bottom };
	point4 rot_bottoms[2] = { rot_bottom1, rot_bottom2 };
	
	point3 trans_left = {-0.4, 0, 0};
	point3 scale_left = {0.2, 0.5, 0.2};
	point4 rot_left = {0, 0, 0, 0};
	
	double r_outer = 0.5;
	double r_inner = 0.3;
	double degrees = 155;
	int segs = 20;
	
	Arch(r_outer, r_inner, degrees, segs, trans_top, rot_top, scale_top, true);
	Arch(r_outer, r_inner, degrees, segs, trans_bottoms, rot_bottoms, scale_bottoms, 1, 2, 1, true);
	Cube(trans_left, rot_left, scale_left);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_W(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build it up from four modified cubes
	
	point3 trans_far_left = {-0.375, 0, 0};
	point3 scale_far_left = {1.52, 0.2, 0.2};
	point4 rot_far_left = {0, 0, 1, -80.54};
	
	point3 trans_mid_left = {-0.125, -0.375, 0};
	point3 scale_mid_left = {0.79, 0.2, 0.2};
	point4 rot_mid_left = {0, 0, 1, 71.57};
	
	point3 trans_mid_right = {0.125, -0.375, 0};
	point3 scale_mid_right = {0.79, 0.2, 0.2};
	point4 rot_mid_right = {0, 0, 1, -71.57};
	
	point3 trans_far_right = {0.375, 0, 0};
	point3 scale_far_right = {1.52, 0.2, 0.2};
	point4 rot_far_right = {0, 0, 1, 80.54};
	
	point3 trans_mid_tip = {0, -0.07, 0};
	point3 scale_mid_tip = {0.2, 0.2, 0.2};
	point4 rot_mid_tip = {0,0,1,90};
	
	point3 trans_left_tip = {-0.25, -0.68, 0};
	point3 scale_left_tip = {0.2, 0.2, 0.2};
	point4 rot_left_tip = {0,0,1,90};
	
	point3 trans_right_tip = {0.25, -0.68, 0};
	point3 scale_right_tip = {0.2, 0.2, 0.2};
	point4 rot_right_tip = {0,0,1,90};
	
	Cube(trans_far_left, rot_far_left, scale_far_left);
	Cube(trans_mid_left, rot_mid_left, scale_mid_left);
	Cube(trans_mid_right, rot_mid_right, scale_mid_right);
	Cube(trans_far_right, rot_far_right, scale_far_right);
	Cube(trans_mid_tip, rot_mid_tip, scale_mid_tip);
	Cube(trans_left_tip, rot_left_tip, scale_left_tip);
	Cube(trans_right_tip, rot_right_tip, scale_right_tip);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_ExclamationPoint(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	// build from a cone and sphere
	
	int segs = 20;
	
	point3 trans_cone = {0, 0.75, 0};
	point3 scale_cone = {0.5, 0.5, 1};
	point4 rot_cone = {1, 0, 0, 90};
	
	point3 trans_sphere = {0, -0.5, 0};
	point3 scale_sphere = {0.2, 0.2, 0.2};
	
	Cone(segs, trans_cone, rot_cone, scale_cone);
	Sphere(segs, trans_sphere, scale_sphere);
	
	// Undo transformations
	glPopMatrix();
}

void Draw_Cow(point3 translation, point4 rotation, point3 scale)
{
	// Save transformation
	glPushMatrix();
	
	// Offset
	Transform(translation, rotation, scale);
	
	point3 trans_body = {0,0,0};
	point3 scale_body = {1, 0.5, 0.5};
	point4 rot_body = {0,0,0,0};
	
	point3 trans_head = {0.6, 0.4, 0};
	point3 scale_head = {0.3, 0.3, 0.5};
	point4 rot_head = {0,1,0,90};
	
	point3 trans_leg = {0.45, -0.4, 0.175};
	point3 scale_leg = {0.1, 0.3, 0.1};
	point4 rot_leg = {0,0,0,0};
	
	point3 trans_horn = {0.5, 0.75, 0};
	point3 scale_horn = {1, 1, 0.1};
	point4 rot_horn1 = {0, 1, 0, 90};
	point4 rot_horn2 = {0,0,1,180};
	
	Cube(trans_body, rot_body, scale_body);
	Cube(trans_head, rot_head, scale_head);
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.x = -0.45;
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.z = -0.175;
	Cube(trans_leg, rot_leg, scale_leg);
	trans_leg.x = 0.45;
	Cube(trans_leg, rot_leg, scale_leg);
	
	double r_outer = 0.4;
	double r_inner = 0.3;
	double degrees = 180;
	int segs = 5;
	point3 trans_horns[1] = { trans_horn };
	point3 scale_horns[1] = { scale_horn };
	point4 rot_horns[2] = { rot_horn1, rot_horn2 };
	
	Arch(r_outer, r_inner, degrees, segs, trans_horns, rot_horns, scale_horns, 1, 2, 1, true);
	
	// Undo transformations
	glPopMatrix();
}

#endif
