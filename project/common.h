#ifndef _COMMON_H_
#define _COMMON_H_

#include "kutils.h"

using namespace kutils;

// gives the area defined by the thrust on the back of the falcon
// front is defined as towards the front of the falcon
// left and right are defined when looking the direction the falcon is facing
typedef struct thrust_box_s
{
	point3 ful; // front upper left
	point3 fur; // front upper right
	point3 flr; // front lower right
	point3 fll; // front lower left
	point3 bul; // back  upper left
	point3 bur; // back  upper right
	point3 blr; // back  lower right
	point3 bll; // back  lower left
} thrust_box;

#endif
