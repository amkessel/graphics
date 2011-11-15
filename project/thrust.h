#ifndef _THRUST_H_
#define _THRUST_H_

#include "kutils.h"
#include "common.h"

using namespace kutils;

void Initialize_Thrust();

void Draw_Thrust(double time);

void Create_Thrust(thrust_box tbox, double time, point3 falcon_vel, double falcon_dir);

void Move_Thrust(double time, double last_time);

#endif
